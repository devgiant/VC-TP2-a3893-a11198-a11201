# Instituto Politécnico do Cávado e do Ave  
# Escola Superior de Tecnologia  
# 2015/2016  

# Enunciado do Trabalho Prático 2 - Visão por Computador

## Resumo
Com este trabalho pretende-se dar aos alunos a oportunidade de aplicarem alguns conceitos de processamento e análise de imagem abordados nas aulas. Os alunos deverão desenvolver um programa em C, ou C++, que opere sobre o vídeo fornecido, de modo a obter o conjunto de informação desejada. Este trabalho culminará na elaboração de um __programa compilado__, que deverá ser entregue ao docente, bem como os __ficheiros com o código fonte__. O trabalho será alvo de apresentação oral.


## Realização do trabalho prático
No ficheiro “__TP2.zip__” é apresentada a respectiva descrição do trabalho e objectivos, contendo ainda:

  - Vídeo a utilizar;
  - Código fonte com exemplo de leitura e exibição de ficheiro de vídeo em OpenCV;
  - Excerto de legislação da União Europeia sobre a classificação de citrinos.

O trabalho deverá ser desenvolvido em grupo, sendo que os grupos deverão ser constituídos por 3 elementos.
Espera-se que os alunos utilizem os conhecimentos adquiridos ao longo das aulas de Visão por Computador, e desenvolvam as funcionalidades necessárias ao cumprimento dos objetivos.
Note que todo o código deverá ser realizado em linguagem C, ou C++, podendo o aluno utilizar (para além das funções OpenCV identificadas no ficheiro “__CodigoExemplo.c__”, ou funções similares, dependendo da versão do OpenCV, ou linguagem de programação, que o grupo utilizar) __até mais 3 funções do OpenCV__.

**_Não é permitida a utilização de outras bibliotecas de processamento de imagem_**.
**_O trabalho deverá ainda ser apresentado em PowerPoint, pelos alunos, numa apresentação oral_**.


## Objectivos
Os grupos deverão desenvolver um programa em linguagem C, ou C++, que processe o vídeo “__video-tp2.avi__”, de modo a obter o seguinte conjunto de informação **_em cada frame do vídeo_**:

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

## Avaliação
Serão tomados como critérios de avaliação os seguintes factores:

* Qualidade do programa:
 * o desenvolvimento das funcionalidades descritas no enunciado do trabalho;
 * o funcionamento correcto do programa;
 * o valor acrescentado (1).
* Qualidade da apresentação oral:
 *  descrição da abordagem ao problema;
 * o descrição correta e completa da estrutura do programa;
 * o descrição das técnicas aplicadas no desenvolvimento das funcionalidades.
* Respeito pelas regras de entrega do programa compilado, e ficheiros com o código fonte.

**_A natureza colectiva da realização de um trabalho em grupo não prejudica o facto de a avaliação ser individual para cada um dos elementos do grupo_**.


## Prazos
A realização do trabalho pressupõe a entrega do programa compilado, e dos ficheiros com o código fonte, em formato digital. O programa compilado deverá ser entregue de forma a permitir a sua imediata execução.
Note, no entanto, que para o executável não seja removido pelos clientes de e-mail, deverão alterar a extensão do executável de “__.exe__” para “__.vc2__”. Exemplo: __meuprograma.vc2__
A entrega do programa compilado, e ficheiros com o código fonte será efectuada através de e-mail, para __dduque@ipca.pt__. A mensagem de e-mail deverá respeitar __obrigatoriamente__ os seguintes requisitos:
* O __assunto da mensagem__ deverá ser “__VC-TP2-xxxx-xxxx-xxxx__”, em que xxxx deverá ser preenchido com *o número de aluno de cada um dos elementos do grupo*.
* O programa compilado, e ficheiros com o código fonte (não inclua o projeto do Visual Studio) deverão ser colocados num ficheiro zip com o nome “VC-TP2-xxxx-xxxx-xxxx.zip” e enviados em anexo.
O prazo de entrega dos elementos do trabalho termina no dia **12 de junho**, às **23:59**.
A **defesa** será realizada no dia **13 de junho**, às **19h**.


## Conduta ética
A falta de transparência em avaliações, presenciais ou não, é naturalmente ilegal e imoral. **__Qualquer plágio, cópia ou conduta académica imprópria será penalizada com a anulação do trabalho. Caso se verifique a existência de trabalhos notoriamente similares (onde por exemplo se tenha alterado apenas os nomes das variáveis de um outro código) entre grupos, todos os trabalhos similares serão anulados__**.  

___
(1) Por valor acrescentado entende-se a forma como o trabalho se destaca dos outros.
