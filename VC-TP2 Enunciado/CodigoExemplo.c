// Exemplo para OpenCV v2.1


#include <stdio.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


int main(void)
{
	// Vídeo
	char *videofile = "video-tp2.avi";
	CvCapture *capture;
	IplImage *frame;
	struct
	{
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
	char str[500] = {0};
	// Outros
	int key = 0;
	
	/* Leitura de vídeo de um ficheiro */
	capture = cvCaptureFromFile(videofile);
	
	/* Verifica se foi possível abrir o ficheiro de vídeo */
	if(!capture)
	{
		fprintf(stderr, "Erro ao abrir o ficheiro de vídeo!\n");
		return 1;
	}

	/* Número total de frames no vídeo */
	video.ntotalframes = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	/* Frame rate do vídeo */
	video.fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	/* Resolução do vídeo */
	video.width = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	video.height = (int) cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o vídeo */
	cvNamedWindow("VC - TP2", CV_WINDOW_AUTOSIZE);

	/* Inicializa a fonte */
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth);
	cvInitFont(&fontbkg, CV_FONT_HERSHEY_SIMPLEX, hScale, vScale, 0, lineWidth + 1);

	while( key != 'q' ) {
		/* Leitura de uma frame do vídeo */
		frame = cvQueryFrame(capture);

		/* Verifica se conseguiu ler a frame */
		if(!frame) break;

		/* Número da frame a processar */
		video.nframe = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);

		/* Exemplo de inserção texto na frame */
		sprintf(str, "RESOLUCAO: %dx%d", video.width, video.height);
		cvPutText (frame, str, cvPoint(20, 20), &fontbkg, cvScalar(0, 0, 0));
		cvPutText (frame, str, cvPoint(20, 20), &font, cvScalar(255, 255, 255));
		sprintf(str, "TOTAL DE FRAMES: %d", video.ntotalframes);
		cvPutText (frame, str, cvPoint(20, 40), &fontbkg, cvScalar(0, 0, 0));
		cvPutText (frame, str, cvPoint(20, 40), &font, cvScalar(255, 255, 255));
		sprintf(str, "FRAME RATE: %d", video.fps);
		cvPutText (frame, str, cvPoint(20, 60), &fontbkg, cvScalar(0, 0, 0));
		cvPutText (frame, str, cvPoint(20, 60), &font, cvScalar(255, 255, 255));
		sprintf(str, "N. FRAME: %d", video.nframe);
		cvPutText (frame, str, cvPoint(20, 80), &fontbkg, cvScalar(0, 0, 0));
		cvPutText (frame, str, cvPoint(20, 80), &font, cvScalar(255, 255, 255));

		// Faça o seu código aqui...

		/* Exibe a frame */
		cvShowImage("VC - TP2", frame);

		/* Sai da aplicação, se o utilizador premir a tecla 'q' */
		key = cvWaitKey(1);
	}

	/* Fecha a janela */
	cvDestroyWindow("VC - TP2");
	
	/* Fecha o ficheiro de vídeo */
	cvReleaseCapture(&capture);

	return 0;
}