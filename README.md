# Enunciado do Trabalho Prático 2 - Visão por Computador

## Resumo
Com este trabalho pretende-se dar aos alunos a oportunidade de aplicarem alguns conceitos de processamento e análise de imagem abordados nas aulas. Os alunos deverão desenvolver um programa em C, ou C++, que opere sobre o vídeo fornecido, de modo a obter o conjunto de informação desejada. Este trabalho culminará na elaboração de um programa compilado, que deverá ser entregue ao docente, bem como os ficheiros com o código fonte. O trabalho será alvo de apresentação oral.


## Realização do trabalho prático
No ficheiro “TP2.zip” é apresentada a respectiva descrição do trabalho e objectivos, contendo ainda:

  - Vídeo a utilizar;
  - Código fonte com exemplo de leitura e exibição de ficheiro de vídeo em OpenCV;
  - Excerto de legislação da União Europeia sobre a classificação de citrinos.

O trabalho deverá ser desenvolvido em grupo, sendo que os grupos deverão ser constituídos por 3 elementos.
Espera-se que os alunos utilizem os conhecimentos adquiridos ao longo das aulas de Visão por Computador, e desenvolvam as funcionalidades necessárias ao cumprimento dos objetivos.
Note que todo o código deverá ser realizado em linguagem C, ou C++, podendo o aluno utilizar (para além das funções OpenCV identificadas no ficheiro “CodigoExemplo.c”, ou funções similares, dependendo da versão do OpenCV, ou linguagem de programação, que o grupo utilizar) até mais 3 funções do OpenCV.
Não é permitida a utilização de outras bibliotecas de processamento de imagem.
O trabalho deverá ainda ser apresentado em PowerPoint, pelos alunos, numa apresentação oral.


## Objectivos
Os grupos deverão desenvolver um programa em linguagem C, ou C++, que processe o vídeo “video-tp2.avi”, de modo a obter o seguinte conjunto de informação em cada frame do vídeo:

  - Número total de laranjas;
  - Área e perímetro de cada laranja;
  - Localização (área delimitadora) e centro de gravidade de cada laranja;
  - Calibre das laranjas (ver anexo “Normas de Qualidade para os Citrinos”, secção “III. Calibre”);
  - Categoria de cada laranja (ver anexo “Normas de Qualidade para os Citrinos”, secção “II. Característica de Qualidade”, ponto “D. Classificação”).

Esta informação deverá ser exibida, sobre o vídeo fornecido.
Tenha ainda em consideração que:

  - O vídeo tem uma resolução de 1280x720;
  - O vídeo tem um frame rate de 25 fps;
  - 280 pixéis correspondem a 55mm.
