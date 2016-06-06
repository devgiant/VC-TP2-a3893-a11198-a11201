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


#define VC_DEBUG

#define MAX3(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))
#define MIN3(a,b,c) (a<b?(a<c?a:c):(b<c?b:c))
#define ESPESSURA 3
#define RACIO 5.2
#define MARGIN 5


//////////////////////////////////////////////////////////////////
//                   ESTRUTURA DE UMA IMAGEM                    //
//////////////////////////////////////////////////////////////////

typedef struct {
	unsigned char *data;
	int width, height;
	int channels;			// Binario/Cinzentos=1; RGB=3
	int levels;				// Binario=1; Cinzentos [1,255]; RGB [1,255]
	int bytesperline;		// width * channels
} IVC;



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UM BLOB (OBJECTO)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
	int x, y, width, height;	// Caixa Delimitadora (Bounding Box)
	int area;					// �rea
	int xc, yc;					// Centro-de-massa
	int perimeter;				// Perimetro
	int label;					// Etiqueta

	unsigned char *mask;		// Not Used
	unsigned char *data;		// Not Used
	int channels;				// Not Used
	int levels;					// Not Used
} OVC;



//////////////////////////////////////////////////////////////////
//                           FUNÇÕES                            //
//////////////////////////////////////////////////////////////////

// alocar e libertar uma imagem
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);

// leitura e escrita de imagens (pbm; pgm; ppm)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);


// contar numero de pixeis azuis
int vc_rgb_to_hsv_count_blue_pixels(IVC *srcdst);

// contar numero de pixeis amarelos
int vc_rgb_to_hsv_count_yellow_pixels(IVC *srcdst);

// filtrar pixeis
int vc_rgb_to_hsv_filter_blue_yellow(IVC *srcdst, int modeFilter);

// localizar matricula
int vc_hsv_locate_plate(IVC *src, IVC *dst, int modeFilter);

OVC* vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels);


int vc_rgb_to_hsv_filter(IVC *srcdst, int modeFilter);

int vc_rgb_to_hsv_filter2(IVC *srcdst, float min, float max);


int vc_binary_erode(IVC *src, IVC *dst, int kernel);

int vc_gray_to_binary_global_mean(IVC *srcdst);

int vc_binary_dilate(IVC *src, IVC *dst, int kernel);

int vc_gray_edge_sobel(IVC *src, IVC *dst, float th);

int vc_gray_edge_prewitt(IVC *src, IVC *dst, float th);

int vc_rgb_to_gray(IVC *src, IVC *dst);

float vc_min_max(IVC *srcdst, float min_max, int modeFilter);