#pragma once

#include <fstream>
#include <stdint.h>

// typedef unsigned __int16 WORD;
typedef int FXPT2DOT30;

typedef struct {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct {
    uint8_t Y;
    uint8_t Cb;
    uint8_t Cr;
} YCbCr;

// bitmap file header
typedef struct {
    unsigned short bfType;
    uint32_t bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

// bitmap info header
typedef struct {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;

} BITMAPINFOHEADER;

// rgb quad
typedef struct {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
} RGBQUAD;

// read bytes „итаем файл  последовательно и побайтово
template <typename Type>
void read(std::ifstream &fin, Type &result, std::size_t size) {
    fin.read(reinterpret_cast<char *>(&result), size);
    // передаем файл,из которого читаем байты ,а эти данные сохра€ем в
    // result(так как передали ссылку)
}

template <typename Type>
void write(std::ofstream &fout, Type &result, std::size_t size) {
    fout.write(reinterpret_cast<char *>(&result), size);
    // передаем файл,из которого читаем байты ,а эти данные сохра€ем в
    // result(так как передали ссылку)
}

void save_bmp_file(std::ofstream &fout, BITMAPFILEHEADER fileHeader,
                   BITMAPINFOHEADER fileInfoHeader) {

    write(fout, fileHeader.bfType, sizeof(fileHeader.bfType));
    write(fout, fileHeader.bfSize, sizeof(fileHeader.bfSize));
    write(fout, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
    write(fout, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
    write(fout, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

    write(fout, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

    write(fout, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
    write(fout, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
    write(fout, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
    write(fout, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
    // получаем информацию о битности
    write(fout, fileInfoHeader.biCompression,
          sizeof(fileInfoHeader.biCompression));
    write(fout, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
    write(fout, fileInfoHeader.biXPelsPerMeter,
          sizeof(fileInfoHeader.biXPelsPerMeter));
    write(fout, fileInfoHeader.biYPelsPerMeter,
          sizeof(fileInfoHeader.biYPelsPerMeter));
    write(fout, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
    write(fout, fileInfoHeader.biClrImportant,
          sizeof(fileInfoHeader.biClrImportant));
}

/*
double get_math_ozh(__int8** intensity, int H, int W);

double get_math_ozh(double** intensity, int H, int W);




double get_disp(__int8** intensity, int H, int W);
double get_disp(double** intensity, int H, int W);

double get_correl(__int8** intensity_A, __int8** intensity_B, int H, int W);
void fillingIntensity(__int8** intensity, RGBQUAD** rgbInfo, int H, int W);


__int8 clipping(double I);

int SAD(__int8** intensity_A1, __int8** intensity_A2, int H, int W);


int SSD(__int8** intensity_A1, __int8** intensity_A2, int H, int W);


int MAE(__int8** intensity_A1, __int8** intensity_A2, int H, int W);



int MSE(__int8** intensity_A1, __int8** intensity_A2, int H, int W);

int SNR(__int8** intensity_A1, __int8** intensity_A2, int H, int W);

int PSNR(__int8** intensity_A1, __int8** intensity_A2, int H, int W, int L);

void colorspaceconversion(RGBQUAD** RGB, YCbCr** YCbCr, int H, int W);

void decolorspaceconversion(RGBQUAD** RGB, YCbCr** YCbCr, int H, int W);
*/
// bit extract

// word 16 байтов
// dword 32 байтов
// long 64
/*
typedef struct tagBITMAPFILEHEADER {
        WORD bfType; // WORD  отметка формата (два ascii символа: Ђbmї)
        uint32_t bfSize; //DWORD размер файла в байтах
        WORD bfReserved1; // WORD зарезервированное пространство (заполнено
нул€ми) WORD bfReserved2; // WORD зарезервированное пространство (заполнено
нул€ми) uint32_t bfOffBits; //DWORD положение данных о пиксел€х относительно
начала файла } BITMAPFILEHEADER, * PBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER {
        uint32_t biSize; //DWORD размер блока, также указывающий на его версию
        unsigned long biWidth; //LONG ширина изображени€ (в пиксел€х)
        unsigned long biHeight; //LONG высота изображени€ (в пиксел€х)
        WORD biPlanes; // WORD значение формата (дл€ .bmp файлов всегда равно 1)
        WORD biBitCount; // WORD количество бит на пиксель
        uint32_t biCompression; //DWORD метод компрессии
        uint32_t biSizeImage;//DWORD размер пиксельных данных (если не
используетс€ сжатие, может быть равно 0) unsigned long biXPelsPerMeter;//LONG
горизонтальное разрешение в пиксел€х на метр unsigned long
biYPelsPerMeter;//LONG вертикальное разрешение в пиксел€х на метр uint32_t
biClrUsed;//DWORD размер таблицы цветов в €чейках uint32_t
biClrImportant;//DWORD количество €чеек от начала таблицы цветов до последней
используемой (включа€ еЄ саму) } BITMAPFILEHEADER, * PBITMAPFILEHEADER;

*/

// CIEXYZTRIPLE stuff
