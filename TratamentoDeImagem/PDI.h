#pragma once
#include <cstdarg>
#include <math.h>
#include <iostream>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

#define VERMELHO 2
#define VERDE 1
#define AZUL 0

class PDI
{
public:
	static Mat escalaCinza(Mat imagemColorida);
	static Mat canal(Mat imagemColorida, int canal);
	static Mat negativo(Mat imagemBase);
	static Mat limiarizacao(Mat imagemBase, int limiar);

	static Mat logaritmo(Mat imagemBase);
	static Mat potencia(Mat imagemBase, float gama);
	static Mat alargamento(Mat imagemBase);
	static Mat janelamento(Mat imagemBase, int li, int ls);

	static Mat suavizacaoMedia(Mat imagemBase, vector<vector<int>> kernel);
	static Mat suavizacaoMediana(Mat imagemBase, int tamanhoKernel);

	static Mat agucamentoLaplaciano(Mat imagemBase, vector<vector<int>> kernel);
	static Mat agucamentoMascaraNitidez(Mat imagemBase, vector<vector<int>> kernelSuavizacao);

	static Mat detectarBordas(Mat imagemBase, vector<vector<int>> kernel, int baseCalculo = 0);
	static Mat somarImagens(Mat imagemBase, Mat imagemSoma, int baseCalculo = 0);
	static Mat subtrairImagens(Mat imagemBase, Mat imagemSub, int baseCalculo = 0);

	static Mat dilatacao(Mat imagemBase, vector<vector<int>> elementoEstruturante);
	static Mat dilatacao(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY);
	static Mat erosao(Mat imagemBase, vector<vector<int>> elementoEstruturante);
	static Mat erosao(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY);

	static Mat abertura(Mat imagemBase, vector<vector<int>> elementoEstruturante);
	static Mat abertura(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY);
	static Mat fechamento(Mat imagemBase, vector<vector<int>> elementoEstruturante);
	static Mat fechamento(Mat imagemBase, vector<vector<int>> elementoEstruturante, int cGravidadeX, int cGravidadeY);


	static vector<vector<int>> esCustom(int tamX, int tamY, ...);
	static vector<vector<int>> esQuadrado(int tamanho);
	static vector<vector<int>> esDiamante(int tamanho);
	static vector<vector<int>> esCirculo(int tamanho);
	static vector<vector<int>> esRetangulo(int tamanhoX, int tamanhoY);

	static vector<vector<int>> laplaciano();
	static vector<vector<int>> laplacianoDiagonal();

	static vector<vector<int>> kernelCompleto(int tamanho);
	static vector<vector<int>> kernelPonderado5();

private:
	static int intensidadeMinima(Mat imagemBase);
	static int intensidadeMaxima(Mat imagemBase);
	static int somarKernel(vector<vector<int>> kernel);
	static bool testeDilatar(Mat imagemBase, vector<vector<int>> elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y);
	static bool testeErodir(Mat imagemBase, vector<vector<int>> elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y);


	static int compare(const void * a, const void * b);
};

