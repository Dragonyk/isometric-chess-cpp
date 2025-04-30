#ifndef IMG_UTILS_H
#define IMG_UTILS_H
#include "Image.h"

class ImgUtils
{
public:
	ImgUtils(int,int);
	Image loadImageNormal(char * fileName);

	int winWidth;
	int winHeight;
};
#endif // !IMG_UTILS_H