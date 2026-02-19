// podstawowe funkcje
#include "SM2025-Funkcje.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Paleta.h"
#include "SM2025-MedianCut.h"
#include "SM2025-Pliki.h"
#include "SM2025-Barwy.h"

inline Uint8 norm(float x) {
    if (x < 0) return 0;
    if (x > 255) return 255;
    return static_cast<Uint8>(x);
}


struct YUV{
    float y;
    float u;
    float v;
};
void setYUV(int xx,int yy,float y,float u,float v) {

   float R, G, B;

    R = y + 1.402 * (v - 128);
    G = y - 0.344136 * (u - 128) - 0.714136 * (v - 128);
    B = y + 1.772 * (u - 128);
    setPixel(xx, yy, norm(R), norm(G),norm(B));


}
YUV getYUV(int xx,int yy){
    SDL_Color color = getPixel(xx, yy);
    float y = 0.299*color.r+0.587*color.g+0.114*color.b;
    float u = 128 +( -0.14713*color.r-0.28886*color.g+0.436*color.b);
    float v =128+0.615*color.r-0.51499*color.g-0.10001*color.b;

    YUV yuv={y, u, v};
    return yuv;
}
void wywolajYUV() {

     for (int yy=0; yy<wysokosc/2; yy++){
        for (int xx=0; xx<szerokosc/2; xx++){
            SDL_Color pixel = getPixel(xx, yy);
            YUV yuv = getYUV(xx, yy);
             setPixel(xx+szerokosc/2, yy, norm(yuv.y), norm(yuv.u), norm(yuv.v));
        }
        }

    SDL_UpdateWindowSurface(window);
}
struct YIQ {
    float y;
    float i;
    float q;
};

void setYIQ(int xx, int yy, float y, float i, float q) {
    float R = y + 0.956 * i + 0.619 * q;
    float G = y - 0.272 * i - 0.647 * q;
    float B = y - 1.106 * i + 1.703 * q;

    setPixel(xx, yy, norm(R), norm(G), norm(B));
}

YIQ getYIQ(int xx, int yy) {
    SDL_Color color = getPixel(xx, yy);

    float y = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
    float i = 0.5959 * color.r - 0.2746 * color.g - 0.3213 * color.b;
    float q = 0.2115 * color.r - 0.5227 * color.g + 0.3112 * color.b;


    return {y,i,q};
}
void wywolajYIQ() {

     for (int yy=0; yy<wysokosc/2; yy++){
        for (int xx=0; xx<szerokosc/2; xx++){
            SDL_Color pixel = getPixel(xx, yy);
            YIQ yiq = getYIQ(xx, yy);
             setPixel(xx, yy + wysokosc /2, norm(yiq.y), norm(yiq.i), norm(yiq.q));
        }
        }

    SDL_UpdateWindowSurface(window);
}


struct YCbCr{
    float y;
    float cb;
    float cr;
};


void setYCbCr(int xx,int yy,float y,float cb,float cr) {

   float R, G, B;

    R = y + 1.402 * (cr - 128);
    G = y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128);
    B = y + 1.772 * (cb - 128);;

    setPixel(xx , yy, norm(R), norm(G), norm(B));
}
YCbCr getYCbCr(int xx,int yy){
    SDL_Color color = getPixel(xx, yy);
    float y= (0.299* color.r)+(0.587*color.g)+(0.114* color.b);
    float cb =128-(0.168736*color.r)-(0.331264*color.g)+(0.5*color.b);
    float cr = 128+(0.5*color.r)- (0.418688*color.g)-(0.081312*color.b);

    YCbCr ycbcr={y, cb, cr};
    return ycbcr;


}
void wywolajYCbCr() {

     for (int yy=0; yy<wysokosc/2; yy++){
        for (int xx=0; xx<szerokosc/2; xx++){
            SDL_Color pixel = getPixel(xx, yy);
            YCbCr ycbcr = getYCbCr(xx, yy);
             setPixel(xx+szerokosc/2, yy + wysokosc/2, norm(ycbcr.y), norm(ycbcr.cb), norm(ycbcr.cr));
        }
        }

    SDL_UpdateWindowSurface(window);
}
struct HSL{
    float h;
    float s;
    float l;
};

float testHSL(float zmiennaR, float zmienna1, float zmienna2){
    float R;
    if((6*zmiennaR)<1){
        R = zmienna2 + (zmienna1 - zmienna2)*6*zmiennaR;
    }else if((2* zmiennaR) <1){
        R = zmienna1;
    }else if((3*zmiennaR) < 2){
        R = zmienna2 + (zmienna1-zmienna2)*(0.666-zmiennaR)*6;
    }else{
        R = zmienna2;
    }
    return R;
}

void setHSL(int xx,int yy,float h,float s,float l) {

    float R, G, B;
    float zmienna1;
    float zmienna2;
   if(l < 0.5){
        zmienna1 = l * (1.0 + s);
   }else if( l >= 0.5){
        zmienna1 = l + s -(l*s);
   }
    zmienna2 = 2* l-zmienna1;
    float barwa = h/360;
    float zmiennaR = barwa + 0.333;
    float zmiennaG = barwa ;
    float zmiennaB = barwa - 0.333;
    if (zmiennaR <0 )
        zmiennaR = zmiennaR +1;
    if (zmiennaG <0 )
        zmiennaG = zmiennaG +1;
    if (zmiennaB <0 )
        zmiennaB = zmiennaB +1;
    if(zmiennaR > 1)
        zmiennaR = zmiennaR - 1;
    if(zmiennaG > 1)
        zmiennaG = zmiennaG - 1;
    if(zmiennaB > 1)
        zmiennaB = zmiennaB - 1;
    R = testHSL(zmiennaR, zmienna1, zmienna2);
    G = testHSL(zmiennaG, zmienna1, zmienna2);
    B = testHSL(zmiennaB, zmienna1, zmienna2);

    R =R*255;
    B = B*255;
    G = G*255;



    setPixel(xx , yy, R, G, B);
}
HSL getHSL(int xx,int yy){
    SDL_Color color = getPixel(xx, yy);
    float R = color.r / 255.0;
    float G = color.g / 255.0;
    float B = color.b / 255.0;
    float minR = std::min(std::min(R, G), B);
    float maxB = std::max(std::max(R, G), B);

    float l = (minR + maxB )/2.0;
    float s = 0.0;
    float h = 0.0;

    if(minR == maxB){
        s = 0;
    } else if(l <= 0.5 ){
        s = (maxB - minR) / (maxB + minR);
    }else if(l > 0.5){
        s = (maxB - minR) / (2.0 - maxB - minR);
    }

    if(R == maxB){
        h =(G - B) / (maxB - minR);
    }else if(G == maxB){
        h = 2.0 + (B - R) / (maxB - minR);
    }else if(B == maxB){
        h = 4.0 + (R-G) / (maxB - minR);
    }

    h *= 60.0;
    if (h < 0) h += 360.0;

    HSL hsl={h, s, l};
    return hsl;


}
void wywolajHSL() {
    for (int yy = 0; yy < wysokosc / 2; yy++) {
        for (int xx = 0; xx < szerokosc / 2; xx++) {

            SDL_Color pixel = getPixel(xx, yy);

            HSL hsl = getHSL(xx, yy);
            float h = hsl.h;
            float s = hsl.s;
            float l = hsl.l;

            setPixel(xx , yy, h, s, l);

        }
    }

    SDL_UpdateWindowSurface(window);
}

struct YUVBlock {
    float y[4];
    float u;
    float v;
};

void podprobkowanieYUV420() {
    for (int yy = 0; yy < wysokosc/2 - 1; yy += 2) {
        for (int xx = 0; xx < szerokosc/2 - 1; xx += 2) {
            YUV yuv00 = getYUV(xx,     yy);
            YUV yuv01 = getYUV(xx + 1, yy);
            YUV yuv10 = getYUV(xx,     yy + 1);
            YUV yuv11 = getYUV(xx + 1, yy + 1);

            float y00 = yuv00.y;
            float y01 = yuv01.y;
            float y10 = yuv10.y;
            float y11 = yuv11.y;

            float u_avg = (yuv00.u + yuv01.u + yuv10.u + yuv11.u) / 4.0f;
            float v_avg = (yuv00.v + yuv01.v + yuv10.v + yuv11.v) / 4.0f;

            setYUV(xx + szerokosc/2,     yy,     y00, u_avg, v_avg);
            setYUV(xx + 1 + szerokosc/2, yy,     y01, u_avg, v_avg);
            setYUV(xx + szerokosc/2,     yy + 1, y10, u_avg, v_avg);
            setYUV(xx + 1 + szerokosc/2, yy + 1, y11, u_avg, v_avg);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void podprobkowanieYIQ420() {
    for (int yy = 0; yy < wysokosc/2 - 1; yy += 2) {
        for (int xx = 0; xx < szerokosc/2 - 1; xx += 2) {
            YIQ yiq00 = getYIQ(xx,     yy);
            YIQ yiq01 = getYIQ(xx + 1, yy);
            YIQ yiq10 = getYIQ(xx,     yy + 1);
            YIQ yiq11 = getYIQ(xx + 1, yy + 1);

            float y00 = yiq00.y;
            float y01 = yiq01.y;
            float y10 = yiq10.y;
            float y11 = yiq11.y;
            float i_avg = (yiq00.i + yiq01.i + yiq10.i + yiq11.i) / 4.0f;
            float q_avg = (yiq00.q + yiq01.q + yiq10.q + yiq11.q) / 4.0f;

            setYIQ(xx,     yy + wysokosc/2,     y00, i_avg, q_avg);
            setYIQ(xx + 1, yy + wysokosc/2,     y01, i_avg, q_avg);
            setYIQ(xx,     yy + 1 + wysokosc/2, y10, i_avg, q_avg);
            setYIQ(xx + 1, yy + 1 + wysokosc/2, y11, i_avg, q_avg);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void podprobkowanieYCbCr420() {
    for (int yy = 0; yy < wysokosc/2 - 1; yy += 2) {
        for (int xx = 0; xx < szerokosc/2 - 1; xx += 2) {

            YCbCr ycbcr00 = getYCbCr(xx,     yy);
            YCbCr ycbcr01 = getYCbCr(xx + 1, yy);
            YCbCr ycbcr10 = getYCbCr(xx,     yy + 1);
            YCbCr ycbcr11 = getYCbCr(xx + 1, yy + 1);

            float y00 = ycbcr00.y;
            float y01 = ycbcr01.y;
            float y10 = ycbcr10.y;
            float y11 = ycbcr11.y;

            float cb_avg = (ycbcr00.cb + ycbcr01.cb + ycbcr10.cb + ycbcr11.cb) / 4.0f;
            float cr_avg = (ycbcr00.cr + ycbcr01.cr + ycbcr10.cr + ycbcr11.cr) / 4.0f;

            setYCbCr(xx + szerokosc/2,     yy + wysokosc/2,     y00, cb_avg, cr_avg);
            setYCbCr(xx + 1 + szerokosc/2, yy + wysokosc/2,     y01, cb_avg, cr_avg);
            setYCbCr(xx + szerokosc/2,     yy + 1 + wysokosc/2, y10, cb_avg, cr_avg);
            setYCbCr(xx + 1 + szerokosc/2, yy + 1 + wysokosc/2, y11, cb_avg, cr_avg);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void podprobkowanieHSL_H420() {
    for (int yy = 0; yy < wysokosc/2 - 1; yy += 2) {
        for (int xx = 0; xx < szerokosc/2 - 1; xx += 2) {

            HSL hsl00 = getHSL(xx,     yy);
            HSL hsl01 = getHSL(xx + 1, yy);
            HSL hsl10 = getHSL(xx,     yy + 1);
            HSL hsl11 = getHSL(xx + 1, yy + 1);

            float h_avg = (hsl00.h + hsl01.h + hsl10.h + hsl11.h) / 4.0f;

            float s00 = hsl00.s, l00 = hsl00.l;
            float s01 = hsl01.s, l01 = hsl01.l;
            float s10 = hsl10.s, l10 = hsl10.l;
            float s11 = hsl11.s, l11 = hsl11.l;

            setHSL(xx + szerokosc/2,     yy,     h_avg, s00, l00);
            setHSL(xx + 1 + szerokosc/2, yy,     h_avg, s01, l01);
            setHSL(xx + szerokosc/2,     yy + 1, h_avg, s10, l10);
            setHSL(xx + 1 + szerokosc/2, yy + 1, h_avg, s11, l11);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void podprobkowanieHSL_S420() {
    for (int yy = 0; yy < wysokosc/2 - 1; yy += 2) {
        for (int xx = 0; xx < szerokosc/2 - 1; xx += 2) {

            HSL hsl00 = getHSL(xx,     yy);
            HSL hsl01 = getHSL(xx + 1, yy);
            HSL hsl10 = getHSL(xx,     yy + 1);
            HSL hsl11 = getHSL(xx + 1, yy + 1);

            float h00 = hsl00.h, l00 = hsl00.l;
            float h01 = hsl01.h, l01 = hsl01.l;
            float h10 = hsl10.h, l10 = hsl10.l;
            float h11 = hsl11.h, l11 = hsl11.l;

            float s_avg = (hsl00.s + hsl01.s + hsl10.s + hsl11.s) / 4.0f;

            setHSL(xx + szerokosc/2,     yy + wysokosc/2,     h00, s_avg, l00);
            setHSL(xx + 1 + szerokosc/2, yy + wysokosc/2,     h01, s_avg, l01);
            setHSL(xx + szerokosc/2,     yy + 1 + wysokosc/2, h10, s_avg, l10);
            setHSL(xx + 1 + szerokosc/2, yy + 1 + wysokosc/2, h11, s_avg, l11);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void podprobkowanieHSL_L420() {
    for (int yy = 0; yy < wysokosc/2 - 1; yy += 2) {
        for (int xx = 0; xx < szerokosc/2 - 1; xx += 2) {

            HSL hsl00 = getHSL(xx,     yy);
            HSL hsl01 = getHSL(xx + 1, yy);
            HSL hsl10 = getHSL(xx,     yy + 1);
            HSL hsl11 = getHSL(xx + 1, yy + 1);

            float h00 = hsl00.h, s00 = hsl00.s;
            float h01 = hsl01.h, s01 = hsl01.s;
            float h10 = hsl10.h, s10 = hsl10.s;
            float h11 = hsl11.h, s11 = hsl11.s;

            float l_avg = (hsl00.l + hsl01.l + hsl10.l + hsl11.l) / 4.0f;

            setHSL(xx,     yy + wysokosc/2,     h00, s00, l_avg);
            setHSL(xx + 1, yy + wysokosc/2,     h01, s01, l_avg);
            setHSL(xx,     yy + 1 + wysokosc/2, h10, s10, l_avg);
            setHSL(xx + 1, yy + 1 + wysokosc/2, h11, s11, l_avg);
        }
    }
    SDL_UpdateWindowSurface(window);
}

int PaethPredictor(int a, int b, int c) {
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);

    if (pa <= pb && pa <= pc) return a;
    else if (pb <= pc) return b;
    else return c;
}

void applyFilter(int offsetX, int offsetY, int filterType) {
    for (int yy = 0; yy < wysokosc / 2; yy++) {
        for (int xx = 0; xx < szerokosc / 2; xx++) {

            SDL_Color pixel = getPixel(xx, yy);

            SDL_Color left = (xx > 0) ? getPixel(xx - 1, yy) : SDL_Color{0,0,0,0};
            SDL_Color above = (yy > 0) ? getPixel(xx, yy - 1) : SDL_Color{0,0,0,0};
            SDL_Color upperLeft = (xx > 0 && yy > 0) ? getPixel(xx - 1, yy - 1) : SDL_Color{0,0,0,0};

            Uint8 R = pixel.r;
            Uint8 G = pixel.g;
            Uint8 B = pixel.b;

            switch(filterType) {
                case 0:
                    break;
                case 1:
                    R = (Uint8)((R - left.r) & 0xFF);
                    G = (Uint8)((G - left.g) & 0xFF);
                    B = (Uint8)((B - left.b) & 0xFF);
                    break;
                case 2:
                    R = (Uint8)((R - above.r) & 0xFF);
                    G = (Uint8)((G - above.g) & 0xFF);
                    B = (Uint8)((B - above.b) & 0xFF);
                    break;
                case 3:
                    R = (Uint8)((R - ((left.r + above.r)/2)) & 0xFF);
                    G = (Uint8)((G - ((left.g + above.g)/2)) & 0xFF);
                    B = (Uint8)((B - ((left.b + above.b)/2)) & 0xFF);
                    break;
                case 4:
                    R = (Uint8)((R - PaethPredictor(left.r, above.r, upperLeft.r)) & 0xFF);
                    G = (Uint8)((G - PaethPredictor(left.g, above.g, upperLeft.g)) & 0xFF);
                    B = (Uint8)((B - PaethPredictor(left.b, above.b, upperLeft.b)) & 0xFF);
                    break;
            }

            setPixel(xx + offsetX, yy + offsetY, R, G, B);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void reverseFilter(int offsetX, int offsetY, int filterType) {
    for (int yy = 0; yy < wysokosc / 2; yy++) {
        for (int xx = 0; xx < szerokosc / 2; xx++) {

            SDL_Color filtered = getPixel(xx + offsetX, yy + offsetY);

            SDL_Color left = (xx > 0) ? getPixel(xx - 1 + offsetX, yy + offsetY) : SDL_Color{0,0,0,0};
            SDL_Color above = (yy > 0) ? getPixel(xx + offsetX, yy - 1 + offsetY) : SDL_Color{0,0,0,0};
            SDL_Color upperLeft = (xx > 0 && yy > 0) ? getPixel(xx - 1 + offsetX, yy - 1 + offsetY) : SDL_Color{0,0,0,0};

            Uint8 R = filtered.r;
            Uint8 G = filtered.g;
            Uint8 B = filtered.b;

            switch(filterType) {
                case 0:
                    break;
                case 1:
                    R = (Uint8)((R + left.r) & 0xFF);
                    G = (Uint8)((G + left.g) & 0xFF);
                    B = (Uint8)((B + left.b) & 0xFF);
                    break;
                case 2:
                    R = (Uint8)((R + above.r) & 0xFF);
                    G = (Uint8)((G + above.g) & 0xFF);
                    B = (Uint8)((B + above.b) & 0xFF);
                    break;
                case 3:
                    R = (Uint8)((R + ((left.r + above.r)/2)) & 0xFF);
                    G = (Uint8)((G + ((left.g + above.g)/2)) & 0xFF);
                    B = (Uint8)((B + ((left.b + above.b)/2)) & 0xFF);
                    break;
                case 4:
                    R = (Uint8)((R + PaethPredictor(left.r, above.r, upperLeft.r)) & 0xFF);
                    G = (Uint8)((G + PaethPredictor(left.g, above.g, upperLeft.g)) & 0xFF);
                    B = (Uint8)((B + PaethPredictor(left.b, above.b, upperLeft.b)) & 0xFF);
                    break;
            }

            setPixel(xx + offsetX, yy + offsetY, R, G, B);
        }
    }
    SDL_UpdateWindowSurface(window);
}

