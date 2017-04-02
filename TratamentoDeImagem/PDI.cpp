#include "stdafx.h"
#include "PDI.h"

Mat PDI::escalaCinza(Mat imagemColorida) {
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

Mat PDI::negativoLinear(Mat imagemBase) {
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

Mat PDI::canal(Mat imagemColorida, int canal) {
	Mat aux = imagemColorida.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			int cinza = pixel[canal];

			pixel[2] = cinza;
			pixel[1] = cinza;
			pixel[0] = cinza;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat PDI::logaritmo(Mat imagemBase) {
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

Mat PDI::potencia(Mat imagemBase, float gama) {
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

int PDI::intensidadeMaxima(Mat imagemBase) {
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

Mat PDI::alargamento(Mat imagemBase) {
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

int PDI::intensidadeMinima(Mat imagemBase) {
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

Mat PDI::limiarizacao(Mat imagemBase, int limiar) {
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

Mat PDI::janelamento(Mat imagemBase, int li, int ls) {
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

int** PDI::kernel(int tamanho) {
	int **kernelt = new int*[tamanho];
	kernelt[0] = new int[tamanho * tamanho];
	for (int i = 1; i < tamanho; i++) {
		kernelt[i] = kernelt[i - 1] + tamanho;
	}

	for (int i = 0; i < tamanho; i++) {
		for (int j = 0; j < tamanho; j++) {
			kernelt[i][j] = 1;
		}
	}

	return kernelt;
}

int** PDI::kernelPonderado() {
	//soma 256
	int tamanho = 5;
	int **kernelt = new int*[tamanho];
	kernelt[0] = new int[tamanho * tamanho];
	for (int i = 1; i < tamanho; i++) {
		kernelt[i] = kernelt[i - 1] + tamanho;
	}

	kernelt[0][0] = 1;
	kernelt[0][1] = 4;
	kernelt[0][2] = 6;
	kernelt[0][3] = 4;
	kernelt[0][4] = 1;

	kernelt[1][0] = 4;
	kernelt[1][1] = 16;
	kernelt[1][2] = 24;
	kernelt[1][3] = 16;
	kernelt[1][4] = 4;

	kernelt[2][0] = 6;
	kernelt[2][1] = 24;
	kernelt[2][2] = 36;
	kernelt[2][3] = 24;
	kernelt[2][4] = 6;

	kernelt[3][0] = 4;
	kernelt[3][1] = 16;
	kernelt[3][2] = 24;
	kernelt[3][3] = 16;
	kernelt[3][4] = 4;

	kernelt[4][0] = 1;
	kernelt[4][1] = 4;
	kernelt[4][2] = 6;
	kernelt[4][3] = 4;
	kernelt[4][4] = 1;

	return kernelt;
}

Mat PDI::suavizacaoMedia(Mat imagemBase, int** kernel, int tamanhoKernel, int somaKernel) {
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
			soma = soma / somaKernel;
			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			pixel[2] = soma;
			pixel[1] = soma;
			pixel[0] = soma;
			aux.at<Vec3b>(x, y) = pixel;
		}
	}
	return aux;
}

Mat PDI::suavizacaoMediana(Mat imagemBase, int tamanhoKernel) {
	Mat aux = imagemBase.clone();
	int desl = tamanhoKernel / 2;
	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			int *vet = new int[tamanhoKernel * tamanhoKernel];
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					Vec3b pixel = imagemBase.at<Vec3b>(i, j);
					vet[(i - x + desl) * tamanhoKernel + (j - y + desl)] = pixel[2];
				}
			}
			qsort(vet, tamanhoKernel * tamanhoKernel, sizeof(int), compare);
			int mediana = vet[(tamanhoKernel * tamanhoKernel) / 2];
			Vec3b pixel = imagemBase.at<Vec3b>(x, y);
			pixel[2] = mediana;
			pixel[1] = mediana;
			pixel[0] = mediana;
			aux.at<Vec3b>(x, y) = pixel;
		}
	}
	return aux;
}

int PDI::compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}
