//////////////////////////////////////////////////////////////////
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE           //
//      ENGENHARIA DE SISTEMAS INFORM�TICOS (PL) 2015/2016      //
//                                                              //
//                      TRABALHO PR�TICO 1                      //
//                     VIS�O POR COMPUTADOR                     //
//                                                              //
//                   A3893  - ANDR� FERNANDES                   //
//                    A11198 - ANDR� MARTINS                    //
//                   A11201 - �NGELO FERREIRA                   //
//                                                              //
//////////////////////////////////////////////////////////////////


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "vc.h"

int main2(int argc, char** argv)
{
	IVC *image01, *image02;
	char *numImage, *pathImageOriginal, *pathImageNew;
	char *fGearOriginal, *fGearNewImage;

	int countBluePix, countYellowPix, modeFilter = 0, num;
	float ratioPix;

	char aux[3];
	char see;
	// numero de imagem
	//numImage = "Imagem1.ppm";

	/* menu ------------------------------------------------------- */
	printf("INSTITUTO POLITECNICO DO CAVADO E DO AVE\nENGENHARIA DE SISTEMAS INFORMATICOS (PL) 2015/2016");
	printf("\n\nVISAO POR COMPUTADOR\n\nA3893 - ANDRE FERNANDES\nA11198 - ANDRE MARTINS\nA11201 - ANGELO FERREIRA");
	printf("\n\n\nPress 'S' to see step by step or any other key to not show,\nand to start processing plates: ");
	scanf("%c", &see);

	printf("%c", see);

	/* ------------------------------------------------------------ */

	for (num = 1; num < 10; num++)
	{
		/* ficheiros -------------------------------------------------- */

		// alocar mem�ria

		// numero da imagem
		numImage = (char *)malloc(1 + 6 + strlen(aux) + 4);
		strcpy(numImage, "Imagem");
		sprintf(aux, "%d", num);
		strcat(numImage, aux);
		strcat(numImage, ".ppm");
		// imagem original
		pathImageOriginal = (char *)malloc(1 + 11 + strlen(numImage));
		strcpy(pathImageOriginal, "_originais/");
		strcat(pathImageOriginal, numImage);
		// imagem tratada
		pathImageNew = (char *)malloc(1 + 10 + strlen(numImage));
		strcpy(pathImageNew, "_tratadas/");
		strcat(pathImageNew, numImage);

		// filtergear - abrir imagens
		fGearOriginal = (char *)malloc(1 + 11 + strlen(pathImageOriginal));
		strcpy(fGearOriginal, "Filtergear ");
		strcat(fGearOriginal, pathImageOriginal);
		fGearNewImage = (char *)malloc(1 + 11 + strlen(pathImageNew));
		strcpy(fGearNewImage, "Filtergear ");
		strcat(fGearNewImage, pathImageNew);

		// recebe caminho de imagem
		image01 = vc_read_image(pathImageOriginal);
		image02 = vc_read_image(pathImageOriginal);

		// verificar se o ficheiro existe
		if (image01 == NULL)
		{
			printf("Error -> vc_read_image():\n\tFile not found!\n");
			return 0;
		}

		/* ------------------------------------------------------------ */


		// Output - imagem a tratar
		if (see == 'S' || see == 's') system(fGearOriginal);


		/* modo de procura/filtragem ---------------------------------- */

		// contar n� pixeis azuis
		countBluePix = vc_rgb_to_hsv_count_blue_pixels(image01);
		printf("\npixeis azuis: %d\n\n", countBluePix);
		// contar n� pixeis amarelos
		countYellowPix = vc_rgb_to_hsv_count_yellow_pixels(image02);
		printf("\npixeis amarelo: %d", countYellowPix);


		// rela��o pixeis azuis / pixeis amarelos
		ratioPix = (float)(countBluePix) / (float)(countYellowPix);
		printf("\n\nrelacao: %f", ratioPix);

		// filtragem dependendo da rela��o pixeis
		// se modo = 0 ent�o filtra azul e amarelo; se modo = 1 ent�o s� filtra por azul; se modo = 2 ent�o s� filtra por amarelo
		if (ratioPix > 2) modeFilter = 1;
		else if (ratioPix < 0.5) modeFilter = 2;
		else ratioPix = 0;
		printf("\n\nmodo relacao: %d", modeFilter);

		/* ------------------------------------------------------------ */


		/* filtrar pixeis por cor ------------------------------------- */

		// filtragem
		vc_rgb_to_hsv_filter_blue_yellow(image01, modeFilter);
		// guardar
		vc_write_image(pathImageNew, image01);
		// abrir
		if (see == 'S' || see == 's') system(fGearNewImage); // Output

															 /* ------------------------------------------------------------ */


															 /* localizar matricula ---------------------------------------- */

															 // localizar e desenhar
		vc_hsv_locate_plate(image01, image02, modeFilter);
		// guardar
		vc_write_image(pathImageNew, image02);
		// abrir
		if (see == 'S' || see == 's') system(fGearNewImage);

		/* ------------------------------------------------------------ */


		/* limpar mem�ria e sair -------------------------------------- */

		// libertar ficheiros
		vc_image_free(image01);
		vc_image_free(image02);

		/* ------------------------------------------------------------ */
	}

	// sair
	printf("\n\nPress any key to exit\n");
	getchar();

	return (EXIT_SUCCESS);
}