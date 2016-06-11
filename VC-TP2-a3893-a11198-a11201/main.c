//////////////////////////////////////////////////////////////////
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE           //
//      ENGENHARIA DE SISTEMAS INFORMÁTICOS (PL) 2015/2016      //
//                                                              //
//                      TRABALHO PRÁTICO 2                      //
//                     VISÃO POR COMPUTADOR                     //
//                                                              //
//                   A3893 - ANDRÉ FERNANDES                    //
//                   A11198 - ANDRÉ MARTINS                     //
//                   A11201 - ÂNGELO FERREIRA                   //
//                                                              //
//////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "cxcore.h"
#include "cv.h"
#include "highgui.h"
#include "vc.h"


int main(int argc, char **argv) {
	// Vídeo
	char *videofile = "../VC-TP2_Enunciado/video-tp2.avi";
	CvCapture *captureOrig;
	CvCapture *capture;
	IplImage *frameOrig;
	IplImage *frame;

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
	int off_on = 1; // inicia a fazer categorização
	int nblobs;

	int count = 0;

	float min = 1000;
	float max = 0;

	int i;
	OVC *blobs;

	/* Leitura de vídeo de um ficheiro */
	captureOrig = cvCaptureFromFile(videofile);
	capture = cvCaptureFromFile(videofile);

	/* Verifica se foi possível abrir o ficheiro de vídeo */
	if (!captureOrig || !capture) {
		fprintf(stderr, "Erro ao abrir o ficheiro de vídeo!\n");
		return 1;
	}

	/* Número total de frames no vídeo */
	video.ntotalframes = (int)cvGetCaptureProperty(captureOrig, CV_CAP_PROP_FRAME_COUNT);

	/* Frame rate do vídeo */
	video.fps = (int)cvGetCaptureProperty(captureOrig, CV_CAP_PROP_FPS);

	/* Resolução do vídeo */
	video.width = (int)cvGetCaptureProperty(captureOrig, CV_CAP_PROP_FRAME_WIDTH);
	video.height = (int)cvGetCaptureProperty(captureOrig, CV_CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o vídeo */
	cvNamedWindow("VC - TP2", CV_WINDOW_AUTOSIZE);
	// cvNamedWindow("VC - TP2 - work", CV_WINDOW_AUTOSIZE); // desligar para aparecer apenas video trabalhado

	/* Inicializa a fonte */
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth, 0);
	cvInitFont(&fontbkg, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth + 1, 0);

	while (key != 'q') {

		// modo
		if (key == 'p') {
			if (off_on == 0) { off_on = 1; key = 0; }
			else { off_on = 0; key = 0; }
		}

		// Leitura de uma frame do vídeo
		frameOrig = cvQueryFrame(captureOrig);
		frame = cvQueryFrame(capture);

		// Verifica se conseguiu ler a frame
		if (!frameOrig) break;
		if (!frame) break;

		// Número da frame a processar
		video.nframe = (int)cvGetCaptureProperty(captureOrig, CV_CAP_PROP_POS_FRAMES);

		//cvShowImage("VC - TP2_2", frame);

		// se modo caracterização
		if (off_on == 1)
		{
			// frame original para IVC
			IVC *ivcFrameOrig = vc_image_new(frameOrig->width, frameOrig->height, frameOrig->nChannels, frameOrig->depth);

			ivcFrameOrig->data = frameOrig->imageData;
			ivcFrameOrig->bytesperline = frameOrig->width*frameOrig->nChannels;			

			// frame para IVC			
			IVC *ivcFrame = vc_image_new(frame->width, frame->height, frame->nChannels, frame->depth);
			IVC *ivcFrame_gray = vc_image_new(frame->width, frame->height, 1, 255);
			IVC *ivcFrame_bin = vc_image_new(frame->width, frame->height, 1, 255);			

			ivcFrame->data = frame->imageData;
			ivcFrame->bytesperline = frame->width*frame->nChannels;
			

			//
			// tratamento 
			//
			
			// filtragem por cor
			vc_bgr_to_hsv_filter(ivcFrame);			

			// calcular valores minimos e maximos
			min = vc_min_max(ivcFrame, min, 0);
			max = vc_min_max(ivcFrame, max, 1);

			// filtragem por minimos e maximos
			vc_min_max_filter(ivcFrame, min, max);

			// passagem para cinza
			vc_img_to_gray(ivcFrame, ivcFrame_bin);

			// passagem para binario
			vc_gray_to_binary_global_mean(ivcFrame_bin);

			// dilate
			vc_binary_dilate(ivcFrame_bin, ivcFrame_gray, 20);

			// blobs
			blobs = vc_binary_blob_labelling(ivcFrame_gray, ivcFrame_bin, &nblobs);

			vc_binary_blob_info(ivcFrame_bin, blobs, nblobs);
			
			system("cls");
			if (blobs != NULL)
			{
				int rad = 0, xr = 0, yr = 0;
				
				for (i = 0; i < nblobs; i++)
				{
					if (blobs[i].area > ((ivcFrame_bin->width * ivcFrame_bin->height) / 10))
					{
						// raio
						xr = (int)(blobs[i].xc - blobs[i].x);
						yr = (int)(blobs[i].yc - blobs[i].y);

						if (xr > yr) rad = xr; else rad = yr;

						// inicio e fim do blob
						if (((blobs[i].yc - rad) > 1) && ((blobs[i].yc + rad) < ivcFrame_bin->height))
						{							
							printf("\nNumber of labels: %d\n\n", nblobs);

							printf("-> Label %d\n", blobs[i].label);
							printf("-> Area %d\n", blobs[i].area);
							printf("-> Perimetro %d\n", blobs[i].perimeter);
							printf("-> XC %d\n", blobs[i].xc);
							printf("-> YC %d\n\n", blobs[i].yc);

							sprintf(str, "Area: %d", blobs[i].area);
							cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 50, blobs[i].yc - 20), &fontbkg, cvScalar(0, 0, 0, 0));
							cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 50, blobs[i].yc - 20), &font, cvScalar(255, 0, 0, 0));
							sprintf(str, "Perimetro: %d", blobs[i].perimeter);
							cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 50, blobs[i].yc), &fontbkg, cvScalar(0, 0, 0, 0));
							cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 50, blobs[i].yc), &font, cvScalar(255, 0, 0, 0));

							// desenhar o circulo
							cvCircle(frameOrig, cvPoint(blobs[i].xc, blobs[i].yc), rad, cvScalar(0, 0, 255, 0), 4, 8, 0);
						}
					}					
				}
				//free(blobs);
			}
			//return blobs;
						
			/* teste */

			// passar binario (apenas valor 255) para a imagem original
			//vc_pix_to_frame(ivcFrame_gray, ivcFrameOrig);

			/* teste: */
			//vc_change_rgb(ivcFrameOrig);
			vc_write_image("_teste.ppm", ivcFrameOrig);
			
		}

		/* Exemplo de inserção texto na frame */
		sprintf(str, "RESOLUCAO: %dx%d", video.width, video.height);
		cvPutText(frameOrig, str, cvPoint(20, 20), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frameOrig, str, cvPoint(20, 20), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "TOTAL DE FRAMES: %d", video.ntotalframes);
		cvPutText(frameOrig, str, cvPoint(20, 40), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frameOrig, str, cvPoint(20, 40), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "FRAME RATE: %d", video.fps);
		cvPutText(frameOrig, str, cvPoint(20, 60), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frameOrig, str, cvPoint(20, 60), &font, cvScalar(255, 0, 0, 0));
		sprintf(str, "N. FRAME: %d", video.nframe);
		cvPutText(frameOrig, str, cvPoint(20, 80), &fontbkg, cvScalar(0, 0, 0, 0));
		cvPutText(frameOrig, str, cvPoint(20, 80), &font, cvScalar(255, 0, 0, 0));

		/* Exibe a frame */
		cvShowImage("VC - TP2", frameOrig);

		/* Exibe a frame a trabalhar */
		//cvShowImage("VC - TP2 - work", frame);

		/* Sai da aplicação, se o utilizador premir a tecla 'q' */
		key = cvWaitKey(10);
	}

	/* Fecha a janela */
	cvDestroyWindow("VC - TP2");

	/* Fecha o ficheiro de vídeo */
	cvReleaseCapture(&capture);
	return 0;
}
