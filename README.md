# OpenCV_ImageFilters
Código de aula do professor Rodrigo Lyra, disciplina de Computação Gráfica e implementações efetuadas para resolução de trabalhos da disciplina.

### Instalando OpenCV
Com o Visual Studio instalado, baixar o OpenCV em: http://docs.opencv.org/2.4/doc/tutorials/introduction/windows_install/windows_install.html

Criar um projeto C++ de Cmd;

Coloque o projeto com Release e x64 na parte superior do Visual Studio;

Em Project > Properties > C/C++ > General > Additional Include Directories adicione o seguinte caminho:
C:\Program Files (x86)\OpenCV\opencv\build\include

Em Project > Properties > Linker > General > Additional Library Directories adicione o seguinte caminho:
C:\Program Files (x86)\OpenCV\opencv\build\x64\vc14\lib

Em Project > Properties > Linker > Input > Additional Dependencies adicione o seguinte nome de arquivo:
opencv_world320.lib

Em Project > Properties > C/C++ > Preprocessor > Preprocessor Definitions adicione o seguinte termo:
_CRT_SECURE_NO_WARNINGS

### Resultados

Sharpening com função Laplaciana
![Sharpening com função Laplaciana](https://github.com/JFRode/OpenCV_ImageFilters/blob/master/Imagens/resultados/Metodo1_Laplace.PNG)

Sharpening com suavização, soma e subtração de imagens
![Sharpening com suavização, soma e subtração de imagens](https://github.com/JFRode/OpenCV_ImageFilters/blob/master/Imagens/resultados/Metodo2_Soma_e_subtracao.PNG)

