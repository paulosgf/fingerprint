#ifndef FPENGINE_H
#define FPENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#define FPM_DEVICE		0x01
#define FPM_PLACE			0x02
#define FPM_LIFT			0x03
#define FPM_CAPTURE		0x04
#define FPM_GENCHAR		0x05
#define FPM_ENRFPT		0x06
#define FPM_NEWIMAGE	0x07
#define FPM_TIMEOUT		0x08
#define FPM_IMGVAL		0x09
#define FPM_CAPTUREEX		0x10
#define FPM_NEWIMAGEEX	0x11

int OpenDevice(void);
int CloseDevice(void);
int LinkDevice(unsigned int password);

int GetWorkMsg();
int GetRetMsg();

int CaptureImage();
int GenFpChar();
int EnrolFpChar();

int SaveImageToFile(const char * filename);
int GetImageData(unsigned char * imagedata,int * size);
int GetImageBmp(unsigned char *  bmpdata,int * size);

int GetFpCharByGen(unsigned char * tpbuf,int* tpsize);
int GetFpCharByEnl(unsigned char * fpbuf,int* fpsize);

int MatchTemplate(unsigned char * lpref, unsigned char * lpmat);
int MatchTemplateOne(unsigned char * lpmat,unsigned char * lpref, int refsize);
int MatchInDevice(unsigned char * lpref, int refsize, unsigned char * lpmat, int matsize);

int LedControl(int bonoff);

#ifdef __cplusplus
}
#endif

#endif // FPENGINE_H
