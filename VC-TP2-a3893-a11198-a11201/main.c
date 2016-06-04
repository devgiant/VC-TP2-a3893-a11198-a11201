#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include "vc.h"
/*
*
*/

int main(int argc, char **argv) {
	// Vídeo
	char *videofile = "../VC-TP2_Enunciado/video-tp2.avi";
	CvCapture *capture;
	IplImage *frame;
	IplImage *frameAUX;

	struct {
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;
	// Texto
	CvFont font, fontbkg;
	double hScale = 0.5;
	double vScale = 0.5;
	int lineWidth = 1;
	char str[500] = { 0 };
	// Outros
	int key = 0;

	int count = 0;

	/* Leitura de vídeo de um ficheiro */
	capture = cvCaptureFromFile(videofile);

	/* Verifica se foi possível abrir o ficheiro de vídeo */
	if (!capture) {
		fprintf(stderr, "Erro ao abrir o ficheiro de vídeo!\n");
		return 1;
	}

	/* Número total de frames no vídeo */
	video.ntotalframes = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	/* Frame rate do vídeo */
	video.fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	/* Resolução do vídeo */
	video.width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	video.height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o vídeo */
	cvNamedWindow("VC - TP2", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("VC - TP2_2", CV_WINDOW_AUTOSIZE);

	/* Inicializa a fonte */
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth, 0);
	cvInitFont(&fontbkg, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth + 1, 0);

	while (key != 'q') {
		/* Leitura de uma frame do vídeo */
		frame = cvQueryFrame(capture);
		frameAUX = cvQueryFrame(capture);
		/* Verifica se conseguiu ler a frame */
		if (!frame) break;

		/* Número da frame a processar */
		video.nframe = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);

		cvShowImage("VC - TP2_2", frame);

		// Faça o seu código aqui...

		#pragma region BLOB_MAIN

		IVC *newFrame = vc_image_new(frame->width, frame->height, frame->nChannels, frame->depth);

		newFrame->data = frame->imageData;
		newFrame->bytesperline = frame->width*frame->nChannels;

		
		
		// filtragem
		vc_rgb_to_hsv_filter(newFrame, 1, video.nframe);

		//vc_binary_blob_labelling(newFrame, frame, count);
		

		/* Exemplo de inserção texto na frame */
		sprintf(str, "RESOLUCAO: %dx%d", video.width, video.height);
		cvPutText(frame, str, cvPoint(20, 20), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 20), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "TOTAL DE FRAMES: %d", video.ntotalframes);
		cvPutText(frame, str, cvPoint(20, 40), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 40), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "FRAME RATE: %d", video.fps);
		cvPutText(frame, str, cvPoint(20, 60), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 60), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "N. FRAME: %d", video.nframe);
		cvPutText(frame, str, cvPoint(20, 80), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 80), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "Canais: %d", frame->nChannels);
		cvPutText(frame, str, cvPoint(20, 100), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frame, str, cvPoint(20, 100), &font, cvScalar(255, 0, 0, 0));

		//	typedef struct {
		//	unsigned char *data;
		//	int width, height;
		//	int channels;			// Binario/Cinzentos=1; RGB=3
		//	int levels;				// Binario=1; Cinzentos [1,255]; RGB [1,255]
		//	int bytesperline;		// width * channels
		//} IVC;


		////IVC image[0] = frame;

		////

		////
		//// Abrir imagem em grayscale, fazer a binariza��o manual, etiquetar os blobs e gravar em novo ficheiro
	
		////	IVC *image[2];


		////	int i, nblobs;
		////	OVC *blobs;

		////	/*image[0] = vc_read_image("Images/FLIR/flir-01.pgm");
		////	if (image[0] == NULL)
		////	{
		////		printf("ERROR -> vc_read_image():\n\tFile not found!\n");
		////		getchar();
		////		return 0;
		////	}*/

		////	/*image[1] = vc_image_new(image[0]->width, image[0]->height, 1, 255);
		////	if (image[1] == NULL)
		////	{
		////		printf("ERROR -> vc_image_new():\n\tOut of memory!\n");
		////		getchar();
		////		return 0;
		////	}*/

		////	vc_gray_to_binary(image[0], 127);
		////	blobs = vc_binary_blob_labelling(frame, frameAUX, &nblobs);

		////	if (blobs != NULL)
		////	{
		////		printf("\nNumber of labels: %d\n", nblobs);
		////		for (i = 0; i<nblobs; i++)
		////		{
		////			printf("-> Label %d\n", blobs[i].label);
		////		}

		////		free(blobs);
		////	}

		////	/*vc_write_image("vc0022.pgm", frameAUX);

		////	vc_image_free(image[0]);
		////	vc_image_free(image[1]);

		////	printf("Press any key to exit...\n");
		////	getchar();

		////	return 0;
		////}*/

#pragma endregion

	


		//vc_image_free(newFrame);

		/* Exibe a frame */
		cvShowImage("VC - TP2", frame);
		

		/* Sai da aplicação, se o utilizador premir a tecla 'q' */
		key = cvWaitKey(20);
	}

	/* Fecha a janela */
	cvDestroyWindow("VC - TP2");

	/* Fecha o ficheiro de vídeo */
	cvReleaseCapture(&capture);

	return 0;
}
