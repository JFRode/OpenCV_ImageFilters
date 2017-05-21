#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <math.h>
#include "PDI.h"
typedef vector<vector<int>> ElementoEstruturante;

using namespace cv;

int main();

Mat escalaCinza(Mat imagemColorida);
Mat negativoLinear(Mat imagemBase);
Mat logaritmo(Mat imagemBase);
int intensidadeMaxima(Mat imagemBase);
Mat potencia(Mat imagemBase, float gama);
int intensidadeMinima(Mat imagemBase);
Mat alargamento(Mat imagemBase);
Mat limiarizacao(Mat imagemBase, int limiar);
Mat janelamento(Mat imagemBase, int li, int ls);

int main()
{
	Mat original = imread("C:\\Users\\user\\Desktop\\PrimeiroTeste.png");
	Mat image;

	float * histograma = PDI::gerarHistograma(original);
	image = PDI::limiarizacaoGlobalSimples(original, 0.5);

	/* Exemplo do elemento Estruturante
	ElementoEstruturante esCustom = PDI::esCustom(5, 5,
		0, 0, 1, 0, 0,
		0, 1, 1, 1, 0,
		1, 1, 1, 1, 1,
		0, 1, 1, 1, 0,
		0, 0, 1, 0, 0);

	image = PDI::dilatacao(original, esCustom);
	*/

	imshow("Original", original);
	imshow("Final", image);
	
	waitKey(0);
	return 0;
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