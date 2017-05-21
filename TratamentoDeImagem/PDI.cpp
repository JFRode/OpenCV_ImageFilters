#include "stdafx.h"
#include "PDI.h"

Mat PDI::limiarizacaoGlobalSimples(Mat imagem, float limiar) {
	Mat aux = escalaCinza(imagem);
	int tamanhoG1 = 0;
	int tamanhoG2 = 0;
	float g1[255] = {0};
	float g2[255] = {0};

	float * histograma = gerarHistograma(aux);
	float limiarAntigo = limiar;

	for (int i = 0; i < 255; i++) {
		if (histograma[i] <= limiar) {
			g1[tamanhoG1] = histograma[i];
			tamanhoG1++;
		}
		else {
			g2[tamanhoG2] = histograma[i];
			tamanhoG2++;
		}
	}

	do {
		float m1 = 0;
		float m2 = 0;
		float totalM1 = 0;
		float totalM2 = 0;

		for (int i = 1; i <= tamanhoG1; i++) {
			m1 += (i * g1[i]);
			totalM1 += g1[i];
		}
		if (m1 > 0) {
			m1 = m1 / totalM1;
		}

		for (int i = 1; i <= tamanhoG2; i++) {
			m2 += (i * g2[i]);
			totalM2 += g2[i];
		}
		if (m2 > 0) {
			m2 = m2 / totalM2;
		}

		limiar = 0.5 * (m1 + m2);
	} while (limiar < limiarAntigo);

	return limiarizacao(aux, limiar);
}

float * PDI::gerarHistograma(Mat imagem) {
	float histograma[255] = {0};
	float totalDePixels = imagem.rows * imagem.cols;

	//	Contabiliza intensidades
	for (int x = 0; x < imagem.rows; x++) {
		for (int y = 0; y < imagem.cols; y++) {
			Vec3b pixel = imagem.at<Vec3b>(x, y);
			histograma[pixel[2]]++;
		}
	}

	//	Normaliza
	for (int i = 0; i < 255; i++) {
		histograma[i] = histograma[i] / totalDePixels;
	}

	return histograma;
}

int PDI::compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

vector<vector<int>> PDI::esCustom(int tamX, int tamY, ...) {
	vector<vector<int>> retorno;

	va_list args;
	va_start(args, tamY);

	for (int i = 0; i < tamX; i++) {
		vector<int> aux;
		for (int j = 0; j < tamY; j++) {
			aux.push_back(va_arg(args, int));
		}
		retorno.push_back(aux);
	}
	va_end(args);

	return retorno;
}

vector<vector<int>> PDI::esQuadrado(int tamanho) {
	return esRetangulo(tamanho, tamanho);
}

vector<vector<int>> PDI::esDiamante(int tamanho) {
	vector<vector<int>> retorno;

	for (int i = 0; i < tamanho; i++) {
		vector<int> aux;
		for (int j = 0; j < tamanho; j++) {
			if (i + j < tamanho / 2 || i + j >= tamanho + tamanho / 2 ||
				i - j < (tamanho / 2) * -1 || i - j > tamanho / 2)
				aux.push_back(0);
			else
				aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}

vector<vector<int>> PDI::esCirculo(int tamanho) {
	if (tamanho < 7)
		return esDiamante(tamanho);

	vector<vector<int>> retorno;

	for (int i = 0; i < tamanho; i++) {
		vector<int> aux;
		for (int j = 0; j < tamanho; j++) {
			if (sqrt((i - tamanho / 2) * (i - tamanho / 2) + (j - tamanho / 2) * (j - tamanho / 2)) > tamanho / 2 + 0.5f)
				aux.push_back(0);
			else
				aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}

vector<vector<int>> PDI::esRetangulo(int tamanhoX, int tamanhoY) {
	vector<vector<int>> retorno;

	for (int i = 0; i < tamanhoX; i++) {
		vector<int> aux;
		for (int j = 0; j < tamanhoY; j++) {
			aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}

vector<vector<int>> PDI::laplaciano() {
	return esCustom(3, 3,
		0, -1, 0,
		-1, 4, -1,
		0, -1, 0);
}

vector<vector<int>> PDI::laplacianoDiagonal() {
	return esCustom(3, 3,
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1);
}

vector<vector<int>> PDI::kernelPonderado5() {
	return esCustom(5, 5,
		1, 4, 6, 4, 1,
		4, 16, 24, 16, 4,
		6, 24, 36, 24, 6,
		4, 16, 24, 16, 4,
		1, 4, 6, 4, 1);
}

vector<vector<int>> PDI::kernelCompleto(int tamanho) {
	return esRetangulo(tamanho, tamanho);
}

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

Mat PDI::negativo(Mat imagemBase) {
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

Mat PDI::suavizacaoMedia(Mat imagemBase, vector<vector<int>> kernel) {
	Mat aux = imagemBase.clone();
	int somaKernel = somarKernel(kernel);
	int desl = kernel.size() / 2;

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

Mat PDI::detectarBordas(Mat imagemBase, vector<vector<int>> kernel, int baseCalculo) {
	Mat aux = imagemBase.clone();
	int desl = kernel.size() / 2;

	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			int soma = 0;
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					Vec3b pixel = imagemBase.at<Vec3b>(i, j);
					soma += pixel[2] * kernel[i - x + desl][j - y + desl];
				}
			}

			soma += baseCalculo;
			if (soma < 1)
				soma = 0;
			if (soma > 255)
				soma = 255;

			Vec3b pixel = imagemBase.at<Vec3b>(x, y);

			pixel[0] = soma;
			pixel[1] = soma;
			pixel[2] = soma;

			aux.at<Vec3b>(x, y) = pixel;

		}
	}
	return aux;
}

Mat PDI::somarImagens(Mat imagemBase, Mat imagemSoma, int baseCalculo) {
	Mat aux = imagemBase.clone();
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixelB = aux.at<Vec3b>(x, y);
			Vec3b pixelS = imagemSoma.at<Vec3b>(x, y);
			int novoPixel = pixelB[2] + pixelS[2] + baseCalculo;
			if (novoPixel > 255)
				novoPixel = 255;
			if (novoPixel < 0)
				novoPixel = 0;
			pixelB[2] = novoPixel;
			pixelB[1] = novoPixel;
			pixelB[0] = novoPixel;
			aux.at<Vec3b>(x, y) = pixelB;
		}
	}

	return aux;
}

Mat PDI::subtrairImagens(Mat imagemBase, Mat imagemSub, int baseCalculo) {
	Mat aux = imagemBase.clone();
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixelB = aux.at<Vec3b>(x, y);
			Vec3b pixelS = imagemSub.at<Vec3b>(x, y);
			int novoPixel = pixelB[2] - pixelS[2] + baseCalculo;
			if (novoPixel < 0)
				novoPixel = 0;
			if (novoPixel > 255)
				novoPixel = 255;
			pixelB[2] = novoPixel;
			pixelB[1] = novoPixel;
			pixelB[0] = novoPixel;
			aux.at<Vec3b>(x, y) = pixelB;
		}
	}

	return aux;
}

int PDI::somarKernel(vector<vector<int>> kernel) {
	int soma = 0;
	for (vector<int> linha : kernel) {
		for (int elemento : linha) {
			soma += elemento;
		}
	}
	return soma;
}

Mat PDI::agucamentoLaplaciano(Mat imagemBase, vector<vector<int>> kernel) {
	Mat aux = detectarBordas(imagemBase, kernel, 128);
	aux = somarImagens(imagemBase, aux, -128);

	return aux;
}

Mat PDI::agucamentoMascaraNitidez(Mat imagemBase, vector<vector<int>> kernelSuavizacao) {
	Mat aux = suavizacaoMedia(imagemBase, kernelSuavizacao);
	aux = subtrairImagens(imagemBase, aux, 128);
	aux = somarImagens(imagemBase, aux, -128);

	return aux;
}

Mat PDI::dilatacao(Mat imagemBase, vector<vector<int>> elementoEstruturante) {
	return dilatacao(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

Mat PDI::dilatacao(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	Mat aux = imagemBase.clone();

	int deslCima = cGravidadeX;
	int deslBaixo = elementoEstruturante.size() - cGravidadeX - 1;
	int deslEsq = cGravidadeY;
	int deslDir = elementoEstruturante[0].size() - cGravidadeY - 1;

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {

			if (testeDilatar(imagemBase, elementoEstruturante, deslEsq, deslDir, deslCima, deslBaixo, x, y)) {
				Vec3b pixel = imagemBase.at<Vec3b>(x, y);
				pixel[2] = 255;
				pixel[1] = 255;
				pixel[0] = 255;
				aux.at<Vec3b>(x, y) = pixel;
			}
		}
	}

	return aux;
}

Mat PDI::erosao(Mat imagemBase, vector<vector<int>> elementoEstruturante) {
	return erosao(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

Mat PDI::erosao(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	Mat aux = imagemBase.clone();

	int deslCima = cGravidadeX;
	int deslBaixo = elementoEstruturante.size() - cGravidadeX - 1;
	int deslEsq = cGravidadeY;
	int deslDir = elementoEstruturante[0].size() - cGravidadeY - 1;

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {

			if (testeErodir(imagemBase, elementoEstruturante, deslEsq, deslDir, deslCima, deslBaixo, x, y)) {
				Vec3b pixel = imagemBase.at<Vec3b>(x, y);
				pixel[2] = 0;
				pixel[1] = 0;
				pixel[0] = 0;
				aux.at<Vec3b>(x, y) = pixel;
			}
		}
	}

	return aux;
}

Mat PDI::abertura(Mat imagemBase, vector<vector<int>> elementoEstruturante) {
	return abertura(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

Mat PDI::abertura(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	Mat aux = erosao(imagemBase, elementoEstruturante, cGravidadeX, cGravidadeY);
	aux = dilatacao(aux, elementoEstruturante, cGravidadeX, cGravidadeY);

	return aux;
}

Mat PDI::fechamento(Mat imagemBase, vector<vector<int>> elementoEstruturante) {
	return fechamento(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

Mat PDI::fechamento(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	Mat aux = dilatacao(imagemBase, elementoEstruturante, cGravidadeX, cGravidadeY);
	aux = erosao(aux, elementoEstruturante, cGravidadeX, cGravidadeY);

	return aux;
}

bool PDI::testeDilatar(Mat imagemBase, vector<vector<int>> elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y) {
	for (int i = x - deslCima, esX = 0; i <= x + deslBaixo; i++, esX++) {
		for (int j = y - deslEsq, esY = 0; j <= y + deslDir; j++, esY++) {
			if (i < 0 || j < 0 || i >= imagemBase.rows || j >= imagemBase.cols)
				continue;
			Vec3b pixel = imagemBase.at<Vec3b>(i, j);
			if (pixel[0] == 255 && elementoEstruturante[esX][esY] == 1)
				return true;
		}
	}

	return false;
}

bool PDI::testeErodir(Mat imagemBase, vector<vector<int>> elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y) {
	for (int i = x - deslCima, esX = 0; i <= x + deslBaixo; i++, esX++) {
		for (int j = y - deslEsq, esY = 0; j <= y + deslDir; j++, esY++) {
			if (i < 0 || j < 0 || i >= imagemBase.rows || j >= imagemBase.cols)
				continue;
			Vec3b pixel = imagemBase.at<Vec3b>(i, j);
			if (pixel[0] == 0 && elementoEstruturante[esX][esY] == 1)
				return true;
		}
	}

	return false;
}
