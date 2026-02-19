#ifndef SM2025-BARWY_H_INCLUDED
#define SM2025-BARWY_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

void setYUV(int xx,int yy,float y,float u,float v);
void setYIQ(int xx, int yy, float y, float i, float q);
void setYCbCr(int xx,int yy,float y,float cb,float cr);
void wywolajYUV();
void wywolajYIQ();
void wywolajYCbCr();
void setHSL(int xx,int yy,float h,float s,float l) ;
void wywolajHSL();
void podprobkowanieYUV420();
void podprobkowanieYIQ420();
void podprobkowanieYCbCr420();
void podprobkowanieHSL_H420();
void podprobkowanieHSL_S420();
void podprobkowanieHSL_L420();
int PaethPredictor(int a, int b, int c);
void applyFilter(int offsetX, int offsetY, int filterType);
void reverseFilter(int offsetX, int offsetY, int filterType);
#endif // SM2025-BARWY_H_INCLUDED
