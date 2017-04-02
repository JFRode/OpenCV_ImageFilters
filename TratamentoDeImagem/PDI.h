#pragma once
#include "opencv2\opencv.hpp"
using namespace cv;

#define VERMELHO 2
#define VERDE 1
#define AZUL 0

class PDI
{
public:
	static Mat escalaCinza(Mat imagemColorida);
	static Mat canal(Mat imagemColorida, int canal);
	static Mat negativoLinear(Mat imagemBase);
	
	static Mat logaritmo(Mat imagemBase);
	static Mat potencia(Mat imagemBase, float gama);
	
	static Mat alargamento(Mat imagemBase);
	static Mat limiarizacao(Mat imagemBase, int limiar);
	static Mat janelamento(Mat imagemBase, int li, int ls);
	
	static int intensidadeMinima(Mat imagemBase);
	static int intensidadeMaxima(Mat imagemBase);

	static Mat suavizacaoMedia(Mat imagemBase, int** kernel, int tamanhoKernel, int somaKernel);
	static Mat suavizacaoMediana(Mat imagemBase, int tamanhoKernel);

	static int** kernel(int tamanho);
	static int** kernelPonderado();
	static int compare(const void * a, const void * b);
	//static Mat somarImagens(Mat imagemBase, Mat imagemSoma, int baseSoma);
	//static Mat subtrairImagens(Mat imagemBase, Mat imagemSub);
};

