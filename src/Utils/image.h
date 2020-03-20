#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <opengl_stuff.h>

typedef struct image_s {
    int width;
    int height;
    glm::vec3 *data;
} Image;

glm::vec3 *getPixelPtr(Image *img, size_t x, size_t y);
Image *initImage(size_t width, size_t height);
void freeImage(Image *img);
void saveImage(Image *img, char *basename);
Image *loadImagePNG(char *filename);
Image *loadImagePPM(char *filename);
Image *loadImageJPG(char *filename);


#endif
