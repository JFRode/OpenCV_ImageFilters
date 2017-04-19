#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <math.h>
#include "PDI.h"

using namespace cv;

Mat erosao(Mat imagem, int tamanhoKernel);
Mat SomaDeImagens(Mat imagemA, Mat imagemB);
Mat SubtracaoDeImagens(Mat imagemA, Mat imagemB);
Mat Laplaciano(Mat imagemBase, int** kernel, int tamanhoKernel);
Mat SharpeningSimplificado(Mat img, Mat kernel);
Mat escalaCinza(Mat imagemColorida);
Mat negativoLinear(Mat imagemBase);
Mat logaritmo(Mat imagemBase);
int intensidadeMaxima(Mat imagemBase);
Mat potencia(Mat imagemBase, float gama);
int intensidadeMinima(Mat imagemBase);
Mat alargamento(Mat imagemBase);
Mat limiarizacao(Mat imagemBase, int limiar);
Mat janelamento(Mat imagemBase, int li, int ls);

int elementoEstruturante[3][3] = {
	0, 1, 0,
	1, 1, 1,
	0, 1, 0
};

const int valorDeNormalizacao = 128;

int main()
{
	int tamanhoKernel = 3;
	int **kernel = new int*[tamanhoKernel];
	kernel[0] = new int[tamanhoKernel * tamanhoKernel];
	for (int i = 1; i < tamanhoKernel; i++) {
		kernel[i] = kernel[i - 1] + tamanhoKernel;
	}

	kernel[0][0] = -1;
	kernel[0][1] = -1;
	kernel[0][2] = -1;

	kernel[1][0] = -1;
	kernel[1][1] = 8;
	kernel[1][2] = -1;

	kernel[2][0] = -1;
	kernel[2][1] = -1;
	kernel[2][2] = -1;
	
	Mat imagemLuaOriginal = imread("C:\\Users\\user\\Desktop\\Trabalho M1\\LuaMascara\\LuaAntes.png");
	
	//	Primeiro metodo
	Mat imagemLuaLaplaciano = Laplaciano(imagemLuaOriginal, kernel, tamanhoKernel);
	imshow("Trabalho M1 | Imagem Original", imagemLuaOriginal);
	imshow("Trabalho M1 | Método 1 | Laplaciano", imagemLuaLaplaciano);
	imshow("Trabalho M1 | Método 1 | Somado", SomaDeImagens(imagemLuaOriginal, imagemLuaLaplaciano));

	//	Segundo metodo
	Mat imagemLuaSuavizada = PDI::suavizacaoMediana(imagemLuaOriginal, 9);
	imshow("Trabalho M1 | Método 2 | Suavizado", imagemLuaSuavizada);
	imshow("Trabalho M1 | Método 2 | Subtraido", SubtracaoDeImagens(imagemLuaOriginal, imagemLuaSuavizada));
	imshow("Trabalho M1 | Método 2 | Somado", SomaDeImagens(imagemLuaOriginal, SubtracaoDeImagens(imagemLuaOriginal, imagemLuaSuavizada)));

	waitKey(0);
	return 0;
}

Mat erosao(Mat imagem, int tamanhoKernel) {
	Mat aux = imagem.clone();
	int desl = tamanhoKernel / 2;
	bool erodir = false;

	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					Vec3b pixel = imagem.at<Vec3b>(i, j);
					if (pixel[0] == 0 && elementoEstruturante[i - x + desl][j - y + desl] == 1) {
						erodir = true;
						break;
					}
				}
			}
			if (erodir) {
				aux.at<Vec3b>(x, y) = 0;
				erodir = false;
			}
		}
	}
	return aux;
}

Mat SomaDeImagens(Mat imagemA, Mat imagemB) {
	int azul;
	int verde;
	int vermelho;
	Mat aux = imagemA.clone();
	int somaAzul;
	int somaVerde;
	int somaVermelho;
	
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixelImagemA = imagemA.at<Vec3b>(x, y);
			Vec3b pixelImagemB = imagemB.at<Vec3b>(x, y);
			Vec3b pixel;

			somaVermelho = pixelImagemA[2] + pixelImagemB[2] - valorDeNormalizacao;
			if (somaVermelho < 0)
				vermelho = 0;
			else if(somaVermelho > 255)
				vermelho = 255;
			else 
				vermelho = somaVermelho;

			somaVerde = pixelImagemA[1] + pixelImagemB[1] - valorDeNormalizacao;
			if (somaVerde < 0)
				verde = 0;
			else if (somaVerde > 255)
				verde = 255;
			else
				verde = somaVerde;

			somaAzul = pixelImagemA[0] + pixelImagemB[0] - valorDeNormalizacao;
			if (somaAzul < 0)
				azul = 0;
			else if (somaAzul > 255)
				azul = 255;
			else
				azul = somaAzul;

			pixel[2] = vermelho;
			pixel[1] = verde;
			pixel[0] = azul;
			aux.at<Vec3b>(x, y) = pixel;
		}
	}
	return aux;
}

Mat SubtracaoDeImagens(Mat imagemA, Mat imagemB) {
	int azul;
	int verde;
	int vermelho;
	Mat aux = imagemA.clone();
	int somaAzul;
	int somaVerde;
	int somaVermelho;

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixelImagemA = imagemA.at<Vec3b>(x, y);
			Vec3b pixelImagemB = imagemB.at<Vec3b>(x, y);
			Vec3b pixel;

			somaVermelho = pixelImagemA[2] - pixelImagemB[2] + valorDeNormalizacao;
			if (somaVermelho < 0)
				vermelho = 0;
			else if (somaVermelho > 255)
				vermelho = 255;
			else
				vermelho = somaVermelho;

			somaVerde = pixelImagemA[1] - pixelImagemB[1] + valorDeNormalizacao;
			if (somaVerde < 0)
				verde = 0;
			else if (somaVerde > 255)
				verde = 255;
			else
				verde = somaVerde;

			somaAzul = pixelImagemA[0] - pixelImagemB[0] + valorDeNormalizacao;
			if (somaAzul < 0)
				azul = 0;
			else if (somaAzul > 255)
				azul = 255;
			else
				azul = somaAzul;

			pixel[2] = vermelho;
			pixel[1] = verde;
			pixel[0] = azul;
			aux.at<Vec3b>(x, y) = pixel;
		}
	}
	return aux;
}

Mat Laplaciano(Mat imagemBase, int** kernel, int tamanhoKernel) {
	Mat aux = imagemBase.clone();
	int desl = tamanhoKernel / 2;
	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			float soma = 0;
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					Vec3b pixel = imagemBase.at<Vec3b>(i, j);
					soma += pixel[2] * kernel[i - x + desl][j - y + desl];
				}
			}

			soma = soma + valorDeNormalizacao;
			if (soma > 255)
				soma = 255;
			else if (soma < 0)
				soma = 0;

			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			pixel[2] = soma;
			pixel[1] = soma;
			pixel[0] = soma;
			aux.at<Vec3b>(x, y) = pixel;
		}
	}
	return aux;
}

Mat SharpeningSimplificado(Mat img, Mat kernel) {
	//	Este foi desenvolvido seguindo dicas de foruns na internet
	Mat lapraciano;
	Mat posProcessamento;

	filter2D(img, lapraciano, CV_32F, kernel);
	img.convertTo(img, CV_32F);
	posProcessamento = img - lapraciano;

	posProcessamento.convertTo(posProcessamento, CV_8U);
	lapraciano.convertTo(lapraciano, CV_8U);

	return posProcessamento = lapraciano + posProcessamento;
}

Mat escalaCinza(Mat imagemColorida) {
	Mat aux = imagemColorida.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			int cinza = pixel[2] * 0.21 + pixel[1] * 0.72 + pixel[0] * 0.07;

			pixel[2] = cinza;
			pixel[1] = cinza;
			pixel[0] = cinza;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat negativoLinear(Mat imagemBase) {
	Mat aux = imagemBase.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			int negativo = 255 - pixel[2];

			pixel[2] = negativo;
			pixel[1] = negativo;
			pixel[0] = negativo;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat logaritmo(Mat imagemBase) {
	Mat aux = imagemBase.clone();

	float c = 255 / (log(intensidadeMaxima(imagemBase)));

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			int pLog = c * log(1 + pixel[2]);

			pixel[2] = pLog;
			pixel[1] = pLog;
			pixel[0] = pLog;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat potencia(Mat imagemBase, float gama) {
	Mat aux = imagemBase.clone();

	float c = 255 / pow(intensidadeMaxima(imagemBase), gama);

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			int pExp = c * pow(pixel[2], gama);

			pixel[2] = pExp;
			pixel[1] = pExp;
			pixel[0] = pExp;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

int intensidadeMaxima(Mat imagemBase) {
	int maximo = 0;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			if (pixel[2] > maximo) {
				maximo = pixel[2];
			}
		}
	}

	return maximo;
}

Mat alargamento(Mat imagemBase) {
	Mat aux = imagemBase.clone();

	int minI = intensidadeMinima(imagemBase);
	int maxI = intensidadeMaxima(imagemBase);
	float escala = maxI - minI;

	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			int novoPixel = (pixel[2] - minI) * (255 / escala);

			pixel[2] = novoPixel;
			pixel[1] = novoPixel;
			pixel[0] = novoPixel;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

int intensidadeMinima(Mat imagemBase) {
	int minimo = 255;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			if (pixel[2] < minimo) {
				minimo = pixel[2];
			}
		}
	}

	return minimo;
}

Mat limiarizacao(Mat imagemBase, int limiar) {
	Mat aux = imagemBase.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			int cPixel;

			if (pixel[2] <= limiar)
				cPixel = 0;
			else
				cPixel = 255;

			pixel[2] = cPixel;
			pixel[1] = cPixel;
			pixel[0] = cPixel;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat janelamento(Mat imagemBase, int li, int ls) {
	Mat aux = imagemBase.clone();

	float escala = ls - li;

	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			int novoPixel = pixel[2];

			if (pixel[2] <= li)
				novoPixel = 0;
			else if (pixel[2] >= ls)
				novoPixel = 255;
			else
				novoPixel = (pixel[2] - li) * (255 / escala);

			pixel[2] = novoPixel;
			pixel[1] = novoPixel;
			pixel[0] = novoPixel;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}
