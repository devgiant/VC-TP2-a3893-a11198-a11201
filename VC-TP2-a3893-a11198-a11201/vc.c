//////////////////////////////////////////////////////////////////
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE           //
//      ENGENHARIA DE SISTEMAS INFORMÁTICOS (PL) 2015/2016      //
//                                                              //
//                      TRABALHO PRÁTICO 1                      //
//                     VISÃO POR COMPUTADOR                     //
//                                                              //
//                   A3893  - ANDRÉ FERNANDES                   //
//                    A11198 - ANDRÉ MARTINS                    //
//                   A11201 - ÂNGELO FERREIRA                   //
//                                                              //
//////////////////////////////////////////////////////////////////


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "vc.h"


//////////////////////////////////////////////////////////////////
//                           FUNÇÕES                            //
//////////////////////////////////////////////////////////////////


// alocar memória para uma imagem
IVC *vc_image_new(int width, int height, int channels, int levels)
{
	IVC *image = (IVC *)malloc(sizeof(IVC));

	if (image == NULL) return NULL;
	if ((levels <= 0) || (levels > 255)) return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char *)malloc(image->width * image->height * image->channels * sizeof(char));

	if (image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}


// libertar memória de uma imagem
IVC *vc_image_free(IVC *image)
{
	if (image != NULL)
	{
		if (image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}


char *netpbm_get_token(FILE *file, char *tok, int len)
{
	char *t;
	int c;

	for (;;)
	{
		while (isspace(c = getc(file)));
		if (c != '#') break;
		do c = getc(file);
		while ((c != '\n') && (c != EOF));
		if (c == EOF) break;
	}

	t = tok;

	if (c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while ((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));

		if (c == '#') ungetc(c, file);
	}

	*t = 0;

	return tok;
}


long int unsigned_char_to_bit(unsigned char *datauchar, unsigned char *databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char *p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}


void bit_to_unsigned_char(unsigned char *databit, unsigned char *datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char *p = databit;

	countbits = 1;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}


IVC *vc_read_image(char *filename)
{
	FILE *file = NULL;
	IVC *image = NULL;
	unsigned char *tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;

	// Abre o ficheiro
	if ((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if (strcmp(tok, "P4") == 0) { channels = 1; levels = 1; }	// Se PBM (Binary [0,1])
		else if (strcmp(tok, "P5") == 0) channels = 1;				// Se PGM (Gray [0,MAX(level,255)])
		else if (strcmp(tok, "P6") == 0) channels = 3;				// Se PPM (RGB [0,MAX(level,255)])
		else
		{
#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
#endif

			fclose(file);
			return NULL;
		}

		if (levels == 1) // PBM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char *)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			if ((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca mem�ria para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			size = image->width * image->height * image->channels;

			if ((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}

		fclose(file);
	}
	else
	{
#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
#endif
	}

	return image;
}


int vc_write_image(char *filename, IVC *image)
{
	FILE *file = NULL;
	unsigned char *tmp;
	long int totalbytes, sizeofbinarydata;

	if (image == NULL) return 0;

	if ((file = fopen(filename, "wb")) != NULL)
	{
		if (image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char *)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);

			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if (fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);

			if (fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				return 0;
			}
		}

		fclose(file);

		return 1;
	}

	return 0;
}


// contar numero de pixeis azuis
int vc_rgb_to_hsv_count_blue_pixels(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	int countPix = 0;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		r = (float)data[i];
		g = (float)data[i + 1];
		b = (float)data[i + 2];

		// Calcula valores m·ximo e mÌnimo dos canais de cor R, G e B
		rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
		rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

		// Value toma valores entre [0,255]
		value = rgb_max;
		if (value == 0.0)
		{
			hue = 0.0;
			saturation = 0.0;
		}
		else
		{
			// Saturation toma valores entre [0,255]
			saturation = ((rgb_max - rgb_min) / rgb_max) * (float) 255.0;

			if (saturation == 0.0)
			{
				hue = 0.0;
			}
			else
			{
				// Hue toma valores entre [0,360]
				if ((rgb_max == r) && (g >= b))
				{
					hue = 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if ((rgb_max == r) && (b > g))
				{
					hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if (rgb_max == g)
				{
					hue = 120 + 60 * (b - r) / (rgb_max - rgb_min);
				}
				else // rgb_max == b  //
				{
					hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
				}
			}
		}


		// se hue entre 180 e 210; saturation maior que 60; value maior que 65
		if (((hue >= 180) && (hue <= 210)) && (saturation / 255.0*100.0 >= 60) & (value / 255.0*100.0 >= 65))
		{
			// somar
			countPix++;
		}
	}

	return countPix;
}

// contar numero de pixeis amarelos
int vc_rgb_to_hsv_count_yellow_pixels(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	int countPix = 0;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		r = (float)data[i];
		g = (float)data[i + 1];
		b = (float)data[i + 2];

		// Calcula valores maximo e minimo dos canais de cor R, G e B
		rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
		rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

		// Value toma valores entre [0,255]
		value = rgb_max;
		if (value == 0.0)
		{
			hue = 0.0;
			saturation = 0.0;
		}
		else
		{
			// Saturation toma valores entre [0,255]
			saturation = ((rgb_max - rgb_min) / rgb_max) * (float) 255.0;

			if (saturation == 0.0)
			{
				hue = 0.0;
			}
			else
			{
				// Hue toma valores entre [0,360]
				if ((rgb_max == r) && (g >= b))
				{
					hue = 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if ((rgb_max == r) && (b > g))
				{
					hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if (rgb_max == g)
				{
					hue = 120 + 60 * (b - r) / (rgb_max - rgb_min);
				}
				else // rgb_max == b  //
				{
					hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
				}
			}
		}


		// se hue for entre 45 e 65; saturation maior que 55; value maior que 75
		if (((hue >= 45) && (hue <= 65)) && (saturation / 255.0*100.0 >= 55) & (value / 255.0*100.0 >= 75))
		{
			countPix++;
		}
	}
	return countPix;
}


// filtrar dependendo do modo: azul, amarelo, azul e amarelo
// passa para branco, tudo o resto a preto
int vc_rgb_to_hsv_filter_blue_yellow(IVC *srcdst, int modeFilter)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;

	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		r = (float)data[i];
		g = (float)data[i + 1];
		b = (float)data[i + 2];

		// Calcula valores maximo e minimo dos canais de cor R, G e B
		rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
		rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

		// Value toma valores entre [0,255]
		value = rgb_max;
		if (value == 0.0)
		{
			hue = 0.0;
			saturation = 0.0;
		}
		else
		{
			// Saturation toma valores entre [0,255]
			saturation = ((rgb_max - rgb_min) / rgb_max) * (float) 255.0;

			if (saturation == 0.0)
			{
				hue = 0.0;
			}
			else
			{
				// Hue toma valores entre [0,360]
				if ((rgb_max == r) && (g >= b))
				{
					hue = 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if ((rgb_max == r) && (b > g))
				{
					hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if (rgb_max == g)
				{
					hue = 120 + 60 * (b - r) / (rgb_max - rgb_min);
				}
				else // rgb_max == b  //
				{
					hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
				}
			}
		}

		// se modo = 0 então filtra azul e amarelo
		if (modeFilter == 0)
		{
			// filtrar azul e amarelo
			if (((hue >= 160) && (hue <= 260)) && (saturation / 255.0*100.0 >= 50) & (value / 255.0*100.0 >= 50))
			{
				data[i] = 255; // branco 
				data[i + 1] = 255; // branco
				data[i + 2] = 255; // branco
			}
			// filtrar amarelo
			else if (((hue >= 45) && (hue <= 65)) && (saturation / 255.0*100.0 >= 55) & (value / 255.0*100.0 >= 75))
			{
				data[i] = 200; // cinza 
				data[i + 1] = 200; // cinza
				data[i + 2] = 200; // cinza
			}
			else
			{
				data[i] = 0; // preto
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
		}
		// se modo = 1 então filtra apenas azul
		else if (modeFilter == 1)
		{
			// filtrar azul
			if (((hue >= 160) && (hue <= 260)) && (saturation / 255.0*100.0 >= 50) & (value / 255.0*100.0 >= 50))
			{
				data[i] = 255; // branco 
				data[i + 1] = 255; // branco
				data[i + 2] = 255; // branco
			}
			else
			{
				data[i] = 0; // preto
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
		}
		// se modo = 2 então filtra apenas amarelo
		else if (modeFilter == 2)
		{
			// filtrar amarelo
			if (((hue >= 45) && (hue <= 65)) && (saturation / 255.0*100.0 >= 55) & (value / 255.0*100.0 >= 75))
			{
				data[i] = 200; // cinza 
				data[i + 1] = 200; // cinza
				data[i + 2] = 200; // cinza
			}
			else
			{
				data[i] = 0; // preto
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
		}
		else
		{
			return 0;
		}
	}
	return 1;
}


// localizar matricula usando inicio e fim
int vc_hsv_locate_plate(IVC *src, IVC *dst, int modeFilter)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	long int pos, pos2;

	int firstLine = 0, firstColumn = 0; // variaveis para guardar posição da linha e coluna do primeiro pixel "branco"
	int lastLine = 0, lastColumn = 0; // variaveis para guardar posição da linha e coluna do ultimo pixel "branco"
	int heightPlate = 0; // variavel para guardar altura de matricula, considerando primeiro pixel e ultimo branco no eixo y
	int lenghtPlate = 0; // variavel para guardar cumprimento da matricula, considerando primeiro pixel e ultimo branco no eixo x

	int limitUp, limitDown, limitLeft, limitRight; // limites do rectangulo

	int flag = 0; // verificação

				  // Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return 0;
	if (channels != 3) return 0;


	// percorrer matriz da imagem para verificar e guardar posições
	// modo de filtragem usada para definir primeira/ultima linha/coluna
	// flag usada para avançar quando ja encontrou o primeiro
	for (y = 0; y<height; y++)
	{
		for (x = 0; x<width; x++)
		{
			// posicao
			pos = y * bytesperline + x * channels;

			// se for branco
			if (datasrc[pos] == 255)
			{
				// se a flag for 0 e a filtragem azul ou azul e amarelo
				if (flag == 0 && modeFilter != 2)
				{
					firstLine = x; // posicao para primeira linha
					firstColumn = y; // posicao para primeira coluna
					flag = 1; // flag
				}
				// se a filtragem for apenas azul
				else if (modeFilter == 1)
				{
					lastLine = x; // posicao para ultima linha; substitui até ao final
					lastColumn = y; // posicao para ultima coluna; substitui até ao final
				}
			}
			// se for cinza
			else if (datasrc[pos] == 200)
			{
				// se a flag for 0 e a filtragem apenas amarelo
				if (flag == 0 && modeFilter == 2)
				{
					firstColumn = y; // posicao para primeira coluna
					flag = 1; // flag
				}
				else
				{
					lastLine = x; // posicao para ultima linha; substitui até ao final
					lastColumn = y; // posicao para ultima coluna; substitui até ao final

									// se filtragem em modo amarelo
					if (modeFilter == 2)
					{
						firstLine = x; // posicao para primeira linha
					}
				}
			}
		}
	}

	// calculo altura da matricula, com base na primeira e ultima coluna
	heightPlate = (lastColumn - firstColumn) + MARGIN * 2;

	// calculos limites	

	if (modeFilter == 0)
	{
		// calculo pela primeira e ultima linha
		lenghtPlate = (lastLine - firstLine) + MARGIN * 2;
		// limite esquerda e direita
		limitLeft = firstLine - MARGIN;
		limitRight = limitLeft + lenghtPlate;
	}
	else
	{
		// calculo pela primeira e ultima linha considerando o racio altura*comprimento
		lenghtPlate = ((lastColumn - firstColumn) * RACIO) + MARGIN * 2;

		if (modeFilter == 1)
		{
			// limite esquerda e direita
			limitLeft = firstLine - MARGIN;
			limitRight = limitLeft + lenghtPlate;
		}
		else
		{
			// limite esquerda e direita
			limitRight = firstLine + MARGIN;
			limitLeft = limitRight - lenghtPlate;
		}
	}

	// limites superior e inferior
	limitUp = firstColumn - MARGIN;
	limitDown = lastColumn + MARGIN;


	// "desenhar" rectangulo na matricula

	if (limitUp < 0)
	{
		limitUp = 0;
	}
	if (limitDown + ESPESSURA > height)
	{
		limitDown = height - ESPESSURA;
	}
	if (limitLeft < 0)
	{
		limitLeft = 0;
		lenghtPlate = limitRight;
		printf("coisas");
	}
	if (limitRight + ESPESSURA > width)
	{
		limitRight = width - ESPESSURA;
		lenghtPlate = width - limitLeft - ESPESSURA;
	}

	printf("\naltura matricula: %d", heightPlate);
	printf("\ncumprimento matricula: %d", lenghtPlate);
	printf("\n\nprimeiraColuna: %d", firstColumn);
	printf("\nultimaColuna: %d", lastColumn);
	printf("\nprimeiraLinha: %d", firstLine);
	printf("\nultimaLinha: %d", lastLine);
	printf("\n\nlimiteSuperior: %d", limitUp);
	printf("\nlimiteInferior: %d", limitDown);
	printf("\nlimiteEsquerda: %d", limitLeft);
	printf("\nlimiteDireita: %d", limitRight);



	// desenha colunas
	for (y = limitUp; y <= limitDown + ESPESSURA; y++)
	{
		for (x = limitLeft; x <= limitLeft + ESPESSURA; x++)
		{
			pos = y * bytesperline + x * channels;

			//printf(" PASSEI AQUI ");

			datadst[pos] = 155;
			datadst[pos + 1] = 0;
			datadst[pos + 2] = 15;

			pos2 = pos + (lenghtPlate*channels);

			datadst[pos2] = 155;
			datadst[pos2 + 1] = 0;
			datadst[pos2 + 2] = 15;
		}
	}

	// desenha linhas
	for (y = limitUp; y <= limitUp + ESPESSURA; y++)
	{
		for (x = limitLeft; x <= limitRight; x++)
		{
			pos = y * bytesperline + x * channels;

			datadst[pos] = 155;
			datadst[pos + 1] = 0;
			datadst[pos + 2] = 15;

			pos2 = (y + heightPlate) * bytesperline + x * channels;

			datadst[pos2] = 155;
			datadst[pos2 + 1] = 0;
			datadst[pos2 + 2] = 15;
		}
	}
	return 1;
}

// Etiquetagem de blobs
// src		: Imagem bin�ria
// dst		: Imagem grayscale (ir� conter as etiquetas)
// nlabels	: Endere�o de mem�ria de uma vari�vel inteira. Recebe o n�mero de etiquetas encontradas.
// OVC*		: Retorna lista de estruturas de blobs (objectos), com respectivas etiquetas. � necess�rio libertar posteriormente esta mem�ria.
OVC* vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, a, b;
	long int i, size;
	long int posX, posA, posB, posC, posD;
	int labeltable[256] = { 0 };
	int labelarea[256] = { 0 };
	int label = 1; // Etiqueta inicial.
	int num;
	OVC *blobs; // Apontador para lista de blobs (objectos) que ser� retornada desta fun��o.

				// Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return NULL;
	if (channels != 1) return NULL;

	// Copia dados da imagem bin�ria para imagem grayscale
	memcpy(datadst, datasrc, bytesperline * height);

	// Todos os pix�is de plano de fundo devem obrigat�riamente ter valor 0
	// Todos os pix�is de primeiro plano devem obrigat�riamente ter valor 255
	// Ser�o atribu�das etiquetas no intervalo [1,254]
	// Este algoritmo est� assim limitado a 254 labels
	for (i = 0, size = bytesperline * height; i<size; i++)
	{
		if (datadst[i] != 0) datadst[i] = 255;
	}

	// Limpa os rebordos da imagem bin�ria
	for (y = 0; y<height; y++)
	{
		datadst[y * bytesperline + 0 * channels] = 0;
		datadst[y * bytesperline + (width - 1) * channels] = 0;
	}
	for (x = 0; x<width; x++)
	{
		datadst[0 * bytesperline + x * channels] = 0;
		datadst[(height - 1) * bytesperline + x * channels] = 0;
	}

	// Efectua a etiquetagem
	for (y = 1; y<height - 1; y++)
	{
		for (x = 1; x<width - 1; x++)
		{
			// Kernel:
			// A B C
			// D X

			posA = (y - 1) * bytesperline + (x - 1) * channels; // A
			posB = (y - 1) * bytesperline + x * channels; // B
			posC = (y - 1) * bytesperline + (x + 1) * channels; // C
			posD = y * bytesperline + (x - 1) * channels; // D
			posX = y * bytesperline + x * channels; // X

													// Se o pixel foi marcado
			if (datadst[posX] != 0)
			{
				if ((datadst[posA] == 0) && (datadst[posB] == 0) && (datadst[posC] == 0) && (datadst[posD] == 0))
				{
					datadst[posX] = label;
					labeltable[label] = label;
					label++;
				}
				else
				{
					num = 255;

					// Se A est� marcado, j� tem etiqueta (j� n�o � 255), e � menor que a etiqueta "num"
					if ((datadst[posA] != 0) && (datadst[posA] != 255) && (datadst[posA] < num))
					{
						num = datadst[posA];
					}
					// Se B est� marcado, j� tem etiqueta (j� n�o � 255), e � menor que a etiqueta "num"
					if ((datadst[posB] != 0) && (datadst[posB] != 255) && (datadst[posB] < num))
					{
						num = datadst[posB];
					}
					// Se C est� marcado, j� tem etiqueta (j� n�o � 255), e � menor que a etiqueta "num"
					if ((datadst[posC] != 0) && (datadst[posC] != 255) && (datadst[posC] < num))
					{
						num = datadst[posC];
					}
					// Se D est� marcado, j� tem etiqueta (j� n�o � 255), e � menor que a etiqueta "num"
					if ((datadst[posD] != 0) && (datadst[posD] != 255) && (datadst[posD] < num))
					{
						num = datadst[posD];
					}

					// Actualiza a tabela de etiquetas
					if ((datadst[posA] != 0) && (datadst[posA] != 255))
					{
						if (labeltable[datadst[posA]] != labeltable[num])
						{
							for (a = 1; a<label; a++)
							{
								if (labeltable[a] == labeltable[datadst[posA]])
								{
									labeltable[a] = labeltable[num];
								}
							}
						}
					}
					if ((datadst[posB] != 0) && (datadst[posB] != 255))
					{
						if (labeltable[datadst[posB]] != labeltable[num])
						{
							for (a = 1; a<label; a++)
							{
								if (labeltable[a] == labeltable[datadst[posB]])
								{
									labeltable[a] = labeltable[num];
								}
							}
						}
					}
					if ((datadst[posC] != 0) && (datadst[posC] != 255))
					{
						if (labeltable[datadst[posC]] != labeltable[num])
						{
							for (a = 1; a<label; a++)
							{
								if (labeltable[a] == labeltable[datadst[posC]])
								{
									labeltable[a] = labeltable[num];
								}
							}
						}
					}
					if ((datadst[posD] != 0) && (datadst[posD] != 255))
					{
						if (labeltable[datadst[posD]] != labeltable[num])
						{
							for (a = 1; a<label; a++)
							{
								if (labeltable[a] == labeltable[datadst[posD]])
								{
									labeltable[a] = labeltable[num];
								}
							}
						}
					}
					labeltable[datadst[posX]] = num;

					// Atribui a etiqueta ao pixel
					datadst[posX] = num;
				}
			}
		}
	}

	// Volta a etiquetar a imagem
	for (y = 1; y<height - 1; y++)
	{
		for (x = 1; x<width - 1; x++)
		{
			posX = y * bytesperline + x * channels; // X

			if (datadst[posX] != 0)
			{
				datadst[posX] = labeltable[datadst[posX]];
			}
		}
	}

	// Contagem do n�mero de blobs
	// Passo 1: Eliminar, da tabela, etiquetas repetidas
	for (a = 1; a<label - 1; a++)
	{
		for (b = a + 1; b<label; b++)
		{
			if (labeltable[a] == labeltable[b]) labeltable[b] = 0;
		}
	}
	// Passo 2: Conta etiquetas e organiza a tabela de etiquetas, para que n�o hajam valores vazios (zero) entre etiquetas
	*nlabels = 0;
	for (a = 1; a<label; a++)
	{
		if (labeltable[a] != 0)
		{
			labeltable[*nlabels] = labeltable[a]; // Organiza tabela de etiquetas
			(*nlabels)++; // Conta etiquetas
		}
	}

	// Se n�o h� blobs
	if (*nlabels == 0) return NULL;

	// Cria lista de blobs (objectos) e preenche a etiqueta
	blobs = (OVC *)calloc((*nlabels), sizeof(OVC));
	if (blobs != NULL)
	{
		for (a = 0; a<(*nlabels); a++) blobs[a].label = labeltable[a];
	}
	else return NULL;

	return blobs;
}


// filtrar dependendo do modo: azul, amarelo, azul e amarelo
// passa para branco, tudo o resto a preto
int vc_rgb_to_hsv_filter(IVC *srcdst, int modeFilter)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;

	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	size = width * height * channels;

/*	printf("%d - ", channels);*/
	
	for (i = 0; i < size; i = i + channels)
	{
		b = (float)data[i];
		g = (float)data[i + 1];
		r = (float)data[i + 2];

		// Calcula valores maximo e minimo dos canais de cor R, G e B
		rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
		rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

		// Value toma valores entre [0,255]
		value = rgb_max;
		if (value == 0.0)
		{
			hue = 0.0;
			saturation = 0.0;
		}
		else
		{
			// Saturation toma valores entre [0,255]
			saturation = ((rgb_max - rgb_min) / rgb_max) * (float) 255.0;

			if (saturation == 0.0)
			{
				hue = 0.0;
			}
			else
			{
				// Hue toma valores entre [0,360]
				if ((rgb_max == r) && (g >= b))
				{
					hue = 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if ((rgb_max == r) && (b > g))
				{
					hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if (rgb_max == g)
				{
					hue = 120 + 60 * (b - r) / (rgb_max - rgb_min);
				}
				else /* rgb_max == b*/
				{
					hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
				}
			}
		}
			
		
		// se modo = 0 então filtra azul e amarelo
		if (modeFilter == 0)
		{
			// filtrar azul e amarelo
			if (((hue >= 160) && (hue <= 260)) && (saturation / 255.0*100.0 >= 50) & (value / 255.0*100.0 >= 50))
			{
				data[i] = 255; // branco 
				data[i + 1] = 255; // branco
				data[i + 2] = 255; // branco
			}
			// filtrar amarelo
			else if (((hue >= 45) && (hue <= 65)) && (saturation / 255.0*100.0 >= 55) & (value / 255.0*100.0 >= 75))
			{
				data[i] = 200; // cinza 
				data[i + 1] = 200; // cinza
				data[i + 2] = 200; // cinza
			}
			else
			{
				data[i] = 0; // preto
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
		}
		// se modo = 1 então filtra apenas laranjas
		else if (modeFilter == 1)
		{
			// filtrar vermelho/laranja escuro
			/*if (((hue >= 5) && (hue <= 30)) && (saturation / 255.0*100.0 >= 50) && (value / 255.0*100.0 >= 45))
			{
				data[i] = 0; // preto 
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
			// filtrar laranjas
			else */if (((hue >= 5) && (hue <= 30)) && (saturation / 255.0*100.0 >= 20) && (value / 255.0*100.0 >= 20))
			{
				data[i] = hue; // branco 
				data[i + 1] = saturation; // branco
				data[i + 2] = value; // branco
			}
			else
			{
				data[i] = 0; // preto
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
		}
		// se modo = 2 então filtra apenas amarelo
		else if (modeFilter == 2)
		{
			// filtrar amarelo
			if (((hue >= 0) && (hue <= 360)) && (saturation / 255.0*100.0 <= 50) & (value / 255.0*100.0 <= 50))
			{
				data[i] = hue; // cinza 
				data[i + 1] = saturation; // cinza
				data[i + 2] = value; // cinza
			}
			else
			{
				data[i] = 0; // preto
				data[i + 1] = 0; // preto
				data[i + 2] = 0; // preto
			}
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

// filtrar dependendo do modo: azul, amarelo, azul e amarelo
// passa para branco, tudo o resto a preto
int vc_rgb_to_hsv_filter2(IVC *srcdst, float min, float max)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;

	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	size = width * height * channels;

	/*	printf("%d - ", channels);*/

	//float med = min;
	float med = max - min;

	for (i = 0; i < size; i = i + channels)
	{
		hue = (float)data[i];
		saturation = (float)data[i + 1];
		value = (float)data[i + 2];

		// filtrar azul e amarelo
		if (saturation == min)
		{
			data[i] = 255; // branco 
			data[i + 1] = 0; // branco
			data[i + 2] = 0; // branco
		}
		/*else if (saturation >= max-min) */ // +/- a funcionar
		else if (saturation >= med) 
		{
			data[i] = 0; // branco 
			data[i + 1] = 255; // branco
			data[i + 2] = 0; // branco
		}
		else
		{
			data[i] = 0; // preto
			data[i + 1] = 0; // preto
			data[i + 2] = 0; // preto
		}		
	}
	return 1;
}


float vc_min_max(IVC *srcdst, float min_max, int modeFilter)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;

	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 3) return 0;

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		hue = (float)data[i];
		saturation = (float)data[i + 1];
		value = (float)data[i + 2];
		
		// se modo = 0
		// calcula minimo
		if (modeFilter == 0)
		{
			// filtrar azul e amarelo
			if (saturation < min_max && value != 0)
			{
				min_max = saturation;
			}
		}
		// se modo = 1
		// calcula maximo
		else if (modeFilter == 1)
		{
			// filtrar azul e amarelo
			if (saturation > min_max)
			{
				min_max = saturation;
			}
		}
		else
		{
			return 0;
		}
	}
	return min_max;
}


// erode
int vc_binary_erode(IVC *src, IVC *dst, int kernel)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	int xx, yy;
	int xxyymax = (kernel - 1) / 2;
	int xxyymin = -xxyymax;
	int max, min;
	long int pos, posk;
	unsigned char threshold;

	// Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return 0;
	if (channels != 1) return 0;

	int estado = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = datasrc[pos];
			min = datasrc[pos];

			// NxM Vizinhos
			for (yy = xxyymin; yy <= xxyymax; yy++)
			{
				for (xx = xxyymin; xx <= xxyymax; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						if (datasrc[posk] < 255) estado = 1;

					}
				}
			}

			if (estado == 1) datadst[pos] = 0;
			else datadst[pos] = 255;
			estado = 0;
		}
	}

	return 1;
}


int vc_gray_to_binary_global_mean(IVC *srcdst)
{
	unsigned char *data = (unsigned char *)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y, soma = 0, tr;
	long int pos;

	//verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 1) return 0;

	// inverter imagem rgb
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			soma += data[pos];
		}
	}

	tr = soma / (height*width);

	y = 0; x = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			if (data[pos] > tr)
			{
				data[pos] = 255;
			}
			else
			{
				data[pos] = 0;
			}
		}
	}

	return 1;

}

int vc_binary_dilate(IVC *src, IVC *dst, int kernel)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	int xx, yy;
	int xxyymax = (kernel - 1) / 2;
	int xxyymin = -xxyymax;
	int max, min;
	long int pos, posk;
	unsigned char threshold;

	// Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return 0;
	if (channels != 1) return 0;

	int estado = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = datasrc[pos];
			min = datasrc[pos];

			// NxM Vizinhos
			for (yy = xxyymin; yy <= xxyymax; yy++)
			{
				for (xx = xxyymin; xx <= xxyymax; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						if (datasrc[posk] >= 255) estado = 1;

					}
				}
			}

			if (estado == 1) datadst[pos] = 255;
			else datadst[pos] = 0;
			estado = 0;
		}
	}

	return 1;
}


// Contornos de uma imagem em tons de cinzento utilizando os operadores de Prewitt
int vc_gray_edge_prewitt(IVC *src, IVC *dst, float th)
{
	unsigned char *data = (unsigned char *)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width*src->channels;
	int channels = src->channels;
	int x, y;
	long int pos;
	long int posA, posB, posC, posD, posE, posF, posG, posH;
	long int mag, mx, my;

	if ((width <= 0) || (height <= 0) || (src->data == NULL)) return 0;
	if (channels != 1) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			posA = (y - 1)* bytesperline + (x - 1) * channels;
			posB = (y - 1)* bytesperline + (x)* channels;
			posC = (y - 1)* bytesperline + (x + 1)* channels;
			posD = (y)* bytesperline + (x - 1)* channels;
			posE = (y)* bytesperline + (x + 1)* channels;
			posF = (y + 1)* bytesperline + (x - 1)* channels;
			posG = (y + 1)* bytesperline + (x)* channels;
			posH = (y + 1)* bytesperline + (x + 1)* channels;
			mx = ((-1 * data[posA]) + (1 * data[posC]) + (-1 * data[posD]) + (1 * data[posE]) + (-1 * data[posF]) + (1 * data[posH])) / 3;
			my = ((-1 * data[posA]) + (1 * data[posF]) + (-1 * data[posB]) + (1 * data[posG]) + (-1 * data[posC]) + (1 * data[posH])) / 3;

			mag = sqrt((mx*mx) + (my * my));

			if (mag > th)
				dst->data[pos] = 255;
			else
				dst->data[pos] = 0;
		}
	}
}


// Contornos de uma imagem em tons de cinzento utilizando os operadores de Sobel
int vc_gray_edge_sobel(IVC *src, IVC *dst, float th)
{
	unsigned char *data = (unsigned char *)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->width*src->channels;
	int channels = src->channels;
	int x, y;
	long int pos;
	long int posA, posB, posC, posD, posE, posF, posG, posH;
	long int mag, mx, my;
	if ((width <= 0) || (height <= 0) || (src->data == NULL)) return 0;
	if (channels != 1) return 0;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			posA = (y - 1)* bytesperline + (x - 1) * channels;
			posB = (y - 1)* bytesperline + (x)* channels;
			posC = (y - 1)* bytesperline + (x + 1)* channels;
			posD = (y)* bytesperline + (x - 1)* channels;
			posE = (y)* bytesperline + (x + 1)* channels;
			posF = (y + 1)* bytesperline + (x - 1)* channels;
			posG = (y + 1)* bytesperline + (x)* channels;
			posH = (y + 1)* bytesperline + (x + 1)* channels;
			mx = ((-1 * data[posA]) + (1 * data[posC]) + (-2 * data[posD]) + (2 * data[posE]) + (-1 * data[posF]) + (1 * data[posH])) / 3;
			my = ((-1 * data[posA]) + (1 * data[posF]) + (-2 * data[posB]) + (2 * data[posG]) + (-1 * data[posC]) + (1 * data[posH])) / 3;
			mag = sqrt((mx*mx) + (my * my));
			if (mag > th)
				dst->data[pos] = 255;
			else
				dst->data[pos] = 0;
		}
	}
}


int vc_rgb_to_gray(IVC *src, IVC *dst)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;

	unsigned char *datadst = (unsigned char *)dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;

	int width = src->width;
	int height = src->height;
	int x, y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	// verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels != 1)) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			//datadst[pos_dst] = (unsigned char) ((rf * 0.299) + (gf * 0.587) + (bf * 0.114));

			datadst[pos_dst] = (unsigned char)((rf + gf + bf) / 3.0);
		}
	}

	return 1;

}