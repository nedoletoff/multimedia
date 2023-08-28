// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается
// и заканчивается выполнение программы.
//
#include "Windows.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

double entropy(uint8_t** intensity, int H, int W) {
	double* array_pix = new double[256];
	for (int i = 0;i < 256;i++) {
		array_pix[i] = 0;
	}
	int count = H * W;
	double result = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			array_pix[intensity[i][j]]++;
		}
	}

	for (int i = 0; i < 256; i++) {
		array_pix[i] = (array_pix[i] )/ count;
	}

	for (int i = 0; i < 256; i++) {
		if (array_pix[i]!=0) {
			result = result + array_pix[i] * log2(array_pix[i]);
		}
	}
	return -result;
}

double entropy(int** intensity, int H, int W) {

	int count = H * W;
	double result = 0;
	map <int, double> map_pix;
	for (int i = 1; i < H; i++) {
		for (int j = 1; j < W; j++) {
			
			map_pix[intensity[i][j]]++;
		}
	}
	/*
	for (int i = -255; i < 256; i++) {
		cout<<"i " << map_pix[i] << " ";
	}
	*/
	
		for (int i = -255; i < 256; i++) {
			map_pix[i] = (map_pix[i]) / count;
	}

	for (int i = -255; i < 256; i++) {
		if (map_pix[i] != 0) {
			result = result + map_pix[i] * log2(map_pix[i]);
		}
		
	}
	return -result;
}

void decimation1(YCbCr** YCbCrinfo, YCbCr** YCbCrinfoDecimation, int H, int W) {

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {

			// YCbCrinfoDecimation1[i][j] = YCbCrinfoRecovery[i][j];
			YCbCrinfoDecimation[i][j].Cb = YCbCrinfo[i][j].Cb;
			YCbCrinfoDecimation[i][j].Cr = YCbCrinfo[i][j].Cr;
			YCbCrinfoDecimation[i][j].Y = YCbCrinfo[i][j].Y;
			if (i == 0 || j == 0) {
				continue;
			}
			if (i % 2 == 0 || j % 2 == 0) { // четные
				YCbCrinfoDecimation[i][j].Cb = 0;
				YCbCrinfoDecimation[i][j].Cr = 0;
			}
		}
	}
}

void decimation2(YCbCr** YCbCrinfo, YCbCr** YCbCrinfoDecimation, int H, int W) {

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			YCbCrinfoDecimation[i][j].Y = YCbCrinfo[i][j].Y;
			if (i == 0 || j == 0) {
				YCbCrinfoDecimation[i][j].Cb = YCbCrinfo[i][j].Cb;
				YCbCrinfoDecimation[i][j].Cr = YCbCrinfo[i][j].Cr;
				continue;
			}

			if (i % 2 == 1 && j % 2 == 1) {
				YCbCrinfoDecimation[i][j].Cb = ((double)YCbCrinfo[i][j].Cb + (double)YCbCrinfo[i][j - 1].Cb +
					(double)YCbCrinfo[i - 1][j].Cb + (double)YCbCrinfo[i - 1][j - 1].Cb) /
					4;
				YCbCrinfoDecimation[i][j].Cr = ((double)YCbCrinfo[i][j].Cr + (double)YCbCrinfo[i][j - 1].Cr +
					(double)YCbCrinfo[i - 1][j].Cr + (double)YCbCrinfo[i - 1][j - 1].Cr) /
					4;
			}
		}
	}
}

void decimation4times1(YCbCr** YCbCrinfo, YCbCr** YCbCrinfoDecimation, int H, int W) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {

			YCbCrinfoDecimation[i][j].Cb = YCbCrinfo[i][j].Cb;
			YCbCrinfoDecimation[i][j].Cr = YCbCrinfo[i][j].Cr;
			YCbCrinfoDecimation[i][j].Y = YCbCrinfo[i][j].Y;

			if (i % 4 != 0 || j % 4 != 0) { // четные
				YCbCrinfoDecimation[i][j].Cb = 0;
				YCbCrinfoDecimation[i][j].Cr = 0;
			}
		}
	}
}

void decimation4timesB(YCbCr** YCbCrinfo, YCbCr** YCbCrinfoDecimation, int H, int W) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			YCbCrinfoDecimation[i][j].Y = YCbCrinfo[i][j].Y;
			if (i == 0 || j == 0) {
				YCbCrinfoDecimation[i][j].Cb = YCbCrinfo[i][j].Cb;
				YCbCrinfoDecimation[i][j].Cr = YCbCrinfo[i][j].Cr;
				continue;
			}

			if (i % 4 == 0 && j % 4 == 0) {
				YCbCrinfoDecimation[i][j].Cb = ((double)YCbCrinfo[i][j].Cb + (double)YCbCrinfo[i][j - 1].Cb +
					(double)YCbCrinfo[i - 1][j].Cb + (double)YCbCrinfo[i - 1][j - 1].Cb) /
					4;
				YCbCrinfoDecimation[i][j].Cr = ((double)YCbCrinfo[i][j].Cr + (double)YCbCrinfo[i][j - 1].Cr +
					(double)YCbCrinfo[i - 1][j].Cr + (double)YCbCrinfo[i - 1][j - 1].Cr) /
					4;
			}
		}
	}
}

void decimation_recovery4times(YCbCr** YCbCrinfoRecovery, YCbCr** YCbCrinfoDecimation, int H, int W) {

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			YCbCrinfoRecovery[i][j].Y = YCbCrinfoDecimation[i][j].Y;
			YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i][j].Cb;
			YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i][j].Cr;
			if (i == 0 || j == 0) {
				continue;
			}

			int distToTop = i % 4;
			int distToLeft = j % 4;
			bool fromTop = distToLeft == 0;
			bool fromLeft = distToTop == 0;

			if (fromTop && fromLeft) {
				continue;
			}

			if (fromTop) {
				YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i - distToTop][j].Cb;
				YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i - distToTop][j].Cr;
			}
			else if (fromLeft) {
				YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i][j - distToLeft].Cb;
				YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i][j - distToLeft].Cr;
			}
			else {
				YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i - distToTop][j - distToLeft].Cb;
				YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i - distToTop][j - distToLeft].Cr;
			}
		}
	}
}

void decimation_recovery(YCbCr** YCbCrinfoRecovery, YCbCr** YCbCrinfoDecimation, int H, int W) {

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			YCbCrinfoRecovery[i][j].Y = YCbCrinfoDecimation[i][j].Y;
			YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i][j].Cb;
			YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i][j].Cr;
			if (i == 0 || j == 0) {
				continue;
			}

			bool oddRow = i % 2 == 1;
			bool oddCol = j % 2 == 1;

			if (oddRow && oddCol) {
				continue;
			}

			if (oddCol) {
				YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i - 1][j].Cb;
				YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i - 1][j].Cr;
			}
			else if (oddRow) {
				YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i][j - 1].Cb;
				YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i][j - 1].Cr;
			}
			else {
				YCbCrinfoRecovery[i][j].Cb = YCbCrinfoDecimation[i - 1][j - 1].Cb;
				YCbCrinfoRecovery[i][j].Cr = YCbCrinfoDecimation[i - 1][j - 1].Cr;
			}
		}
	}
}

double get_math_ozh(uint8_t** intensity, int H, int W) { // оценка  мат ожиданий
	// +
	double result = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			result = result + (double)intensity[i][j];
		}
	}
	return result / (W * H);
}

double get_math_ozh(double** intensity, int H, int W) { // оценка  мат ожиданий
	// +
	double result = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			result = result + intensity[i][j];
		}
	}
	return result / (W * H);
}

double get_disp(uint8_t** intensity, int H, int W) { //+ это дисперсия
	double result = 0;
	double temp = 0;

	double m = get_math_ozh(intensity, H, W);
	// std::cout << "РОССИЯ5" << std::endl;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			temp = abs((double)intensity[i][j] - m);
			result += temp * temp;
			// result= result+pow(intensity[i][j] - get_math_ozh(intensity, H,
			// W), 2);
		}
	}
	return sqrt(result / (W * H - 1));
}

double get_correl(uint8_t** intensity_A, uint8_t** intensity_B, int H, int W) {
	double** mathExpectation = new double* [H]; // числитель
	for (int i = 0; i < H; i++) {
		mathExpectation[i] = new double[W];
	}

	// std::cout << "Прошли1" << std::endl;

	double mathExpectation_A = get_math_ozh(intensity_A, H, W);
	double mathExpectation_B = get_math_ozh(intensity_B, H, W);
	// std::cout << "Прошли2" << std::endl;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			double tempmathExpectation_A = intensity_A[i][j] - mathExpectation_A;
			double tempmathExpectation_B = intensity_B[i][j] - mathExpectation_B;
			mathExpectation[i][j] = tempmathExpectation_A * tempmathExpectation_B;
		}
	}
	// std::cout << "Прошли3" << std::endl;

	double mathExpectation_AB = get_math_ozh(mathExpectation, H, W);
	// std::cout << "Прошли4" << std::endl;

	double sigma_A = get_disp(intensity_A, H, W);
	// std::cout << "Прошли5" << std::endl;
	double sigma_B = get_disp(intensity_B, H, W);

	return mathExpectation_AB / (sigma_A * sigma_B);
}

uint8_t clipping(unsigned int& count, double I) {
	double IMin = 0, IMax = 255;

	if (I < IMin) {
		count = count + 1;

		return (uint8_t)IMin;
	}

	if (I > IMax) {
		count = count + 1;
		return (uint8_t)IMax;
	}

	return (uint8_t)I;
}

uint8_t clipping( double I) {
	double IMin = 0, IMax = 255;

	if (I < IMin) {
	
		return (uint8_t)IMin;
	}

	if (I > IMax) {
	
		return (uint8_t)IMax;
	}

	return (uint8_t)I;
}

// RGBQUAD
void fillingIntensity(uint8_t** intensityBlue, uint8_t** intensityGreen, uint8_t** intensityRed, RGBQUAD** rgbInfo, int H,
	int W) { // заполнение интенсивности +
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			intensityBlue[i][j] = rgbInfo[i][j].rgbBlue;
			intensityGreen[i][j] = rgbInfo[i][j].rgbGreen;
			intensityRed[i][j] = rgbInfo[i][j].rgbRed;
		}
	}
}
// YCbCr
void fillingIntensity(uint8_t** intensityY, uint8_t** intensityCb, uint8_t** intensityCr, YCbCr** YCbCrInfo, int H,
	int W) { // заполнение интенсивности +
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			intensityY[i][j] = YCbCrInfo[i][j].Y;
			intensityCb[i][j] = YCbCrInfo[i][j].Cb;
			intensityCr[i][j] = YCbCrInfo[i][j].Cr;
		}
	}
}

void fillingIntensity(uint8_t** intensityCb, uint8_t** intensityCr, YCbCr** YCbCrInfo, int H,
	int W) { // заполнение интенсивности +
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			intensityCb[i][j] = YCbCrInfo[i][j].Cb;
			intensityCr[i][j] = YCbCrInfo[i][j].Cr;
		}
	}
}

int SAD(uint8_t** intensity_A1, uint8_t** intensity_A2, int H, int W) {
	int result = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			result = result + abs(intensity_A1[i][j] - intensity_A2[i][j]);
		}
	}
	return result;
}

int SSD(uint8_t** intensity_A1, uint8_t** intensity_A2, int H, int W) {
	int result = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			result = result + pow(intensity_A1[i][j] - intensity_A2[i][j], 2);
		}
	}
	return result;
}

int MAE(uint8_t** intensity_A1, uint8_t** intensity_A2, int H, int W) {
	int result = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			result = result + abs(intensity_A1[i][j] - intensity_A2[i][j]);
		}
	}
	return result / (W * H);
}

int MSE(uint8_t** intensity_A1, uint8_t** intensity_A2, int H, int W) {
	int result = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			result = result + pow(intensity_A1[i][j] - intensity_A2[i][j], 2);
		}
	}
	return result / (W * H);
}

double SNR(uint8_t** intensity_A1, uint8_t** intensity_A2, int H, int W) {
	double result = 0;
	double a = 0;
	double b = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			a = a + pow(intensity_A1[i][j], 2);
		}
	}

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			b = b + pow(intensity_A1[i][j] - intensity_A2[i][j], 2);
		}
	}

	result = log10(a / b);

	return 10 * result;
}

double PSNR(uint8_t** intensity_A1, uint8_t** intensity_A2, int H, int W, int L) {
	double result = 0;

	double a = 0;
	double b = 0;
	a = W * H * pow(pow(2, L) - 1, 2);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			b = b + pow((double)intensity_A1[i][j] - (double)intensity_A2[i][j], 2);
		}
	}

	result = log10(a / b);

	return 10 * result;
}



void colorspaceconversion(RGBQUAD** RGB, YCbCr** YCbCr, int H, int W) {
	// Y = 0.299*R + 0.587*G + 0.114*B;
	// Cb= 0.5643*(B - Y) + 128;
	// Cr= 0.7132*(R - Y) + 128;
	unsigned int count = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {

			YCbCr[i][j].Y =
				clipping(count, 0.299 * (double)RGB[i][j].rgbRed + 0.587 * (double)RGB[i][j].rgbGreen + 0.114 * (double)RGB[i][j].rgbBlue);
			YCbCr[i][j].Cb = clipping(count, 0.5643 * ((double)RGB[i][j].rgbBlue - (double)YCbCr[i][j].Y) + 128);
			YCbCr[i][j].Cr = clipping(count, 0.7132 * ((double)RGB[i][j].rgbRed - (double)YCbCr[i][j].Y) + 128);
		}
	}

	cout << "Количество выходов " << count << "---" << endl;
}

void decolorspaceconversion(RGBQUAD** RGB, YCbCr** YCbCr, int H, int W) {
	// G = Y - 0.714*(Cr -128) - 0.334*(Cb - 128);
	// R = Y + 1.402*(Cr -128);
	// B = Y + 1.772*(Cb -128);
	unsigned int count = 0;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			RGB[i][j].rgbGreen =
				clipping(count, (double)YCbCr[i][j].Y - 0.714 * ((double)YCbCr[i][j].Cr - 128) - 0.334 * ((double)YCbCr[i][j].Cb - 128));

			RGB[i][j].rgbRed = clipping(count, (double)YCbCr[i][j].Y + 1.402 * ((double)YCbCr[i][j].Cr - 128));

			RGB[i][j].rgbBlue = clipping(count, (double)YCbCr[i][j].Y + 1.772 * ((double)YCbCr[i][j].Cb - 128));
		}
	}
	cout << "Количество выходов " << count << "---" << endl;
}




int** DPCM(uint8_t** intensity, int H, int W, int r) {
	double result = 0;

	int** intensity_DPCM = new int * [H];

	for (int i = 0; i < H; i++) {             // двумерный
		intensity_DPCM[i] = new int[W];
	}


	if (r == 1) {
		for (int i = 1; i < H; i++) {
			for (int j = 1; j < W; j++) {
				intensity_DPCM[i][j] = (int)intensity[i][j] - (int)intensity[i][j - 1];
			}
		}
	}

	if (r == 2) {
		for (int i = 1; i < H; i++) {
			for (int j = 1; j < W; j++) {
				intensity_DPCM[i][j] = (int)intensity[i][j] - (int)intensity[i - 1][j];
			}
		}
	}

	if (r == 3) {
		for (int i = 1; i < H; i++) {
			for (int j = 1; j < W; j++) {
				intensity_DPCM[i][j] = (int)intensity[i][j] - (int)intensity[i - 1][j - 1];
			}
		}
	}

	if (r == 4) {
		for (int i = 1; i < H; i++) {
			for (int j = 1; j < W; j++) {
				int average_value = ((int)intensity[i][j - 1] + (int)intensity[i - 1][j] + (int)intensity[i - 1][j - 1]) / 3;
				intensity_DPCM[i][j] = (int)intensity[i][j] - average_value;
			}
		}
	}


	return intensity_DPCM;

}
/*
int* count_number_DPCM(int** intensity, int H, int W) {
	int* count = new int[256];
	for (int i = 0;i < 256;i++) {
		count[i] = 0;
	}

	for (int i = 1; i < H; i++) {
		for (int j = 1; j < W; j++) {
			count[intensity[i][j]] ++;

		}
	}
	return count;
}
*/

map<int,int> count_number_DPCM(int** intensity, int H, int W) {
	//int* count = new int[256];
	map <int, int> map_count;
	

	for (int i = 1; i < H; i++) {
		for (int j = 1; j < W; j++) {
			//count[intensity[i][j]] ++;
			map_count[intensity[i][j]]++;
		}
	}
	return map_count;
}

map<int, int> count_number_DPCM(uint8_t** intensity, int H, int W) {
	//int* count = new int[256];
	map <int, int> map_count;


	for (int i = 1; i < H; i++) {
		for (int j = 1; j < W; j++) {
			//count[intensity[i][j]] ++;
			map_count[intensity[i][j]]++;
		}
	}
	return map_count;
}

/*
void save_file_count(std::ofstream& fout, int* count_array) {
	for (int i = 0;i < 256;i++) {
		//std::cout << i << " ";
		//std::cout << i << " " << " coun " << count_array[i]<<" ";
		fout << i << " " << count_array[i] << std::endl;
		//std::cout<<endl;
	}
}
*/

void save_file_count(std::ofstream& fout, map<int,int> count_array) {
		for (int i = -255; i < 256; i++) {  // выводим их
			//cout << i << ") Ключ " << count_array[i] << endl;
			fout << i << " " << count_array[i] << std::endl;
		}
}

int main() // int argc, char* argv[]
{
	setlocale(LC_ALL, "Russian");

	string fileName = "kodim02.bmp";

	// открываем файл
	std::ifstream fileStream(fileName, std::ifstream::binary);
	if (!fileStream) {
		std::cout << "Error opening file '" << fileName << "'." << std::endl;
		return 0;
	}

	// заголовок изображения. загружаем заголовок
	BITMAPFILEHEADER fileHeader;
	read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
	read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

	// Проверяем на соответствие типа файла
	if (fileHeader.bfType != 0x4D42) {
		std::cout << "Error: '" << fileName << "' is not BMP file." << std::endl;
		return 0;
	}

	// информация изображения
	BITMAPINFOHEADER fileInfoHeader;
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));
	// bmp core
	read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
	read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
	read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
	read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	// получаем информацию о битности
	read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
	read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
	read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
	read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
	read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
	read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));

	std::cout << "biSizeImage" << std::endl;
	std::cout << fileInfoHeader.biSizeImage << std::endl;

	// rgb info
	RGBQUAD** rgbInfo = new RGBQUAD * [fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfo[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	// определение размера отступа в конце каждой строки
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3; // результа & 11 в двоичной системе

	// чтение

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			read(fileStream, rgbInfo[i][j].rgbBlue, 1);  // Голубой
			read(fileStream, rgbInfo[i][j].rgbGreen, 1); // Зеленый
			read(fileStream, rgbInfo[i][j].rgbRed, 1);   // Красный
			// read(fileStream, YCbCrInfo[i][j].rgbReserved, 1);// резерв
		}
		fileStream.seekg(linePadding,
			std::ios_base::cur); // Seekg() это функция в библиотеке
		// iostream, которая позволяет искать
		// произвольную позицию в файле.
	}

	// вывод

	// ВТОРОЕ ЗАДАНИЕ

	string blueKodim = "bluekodim02.bmp";
	string greenKodim = "greenkodim02.bmp";
	string redKodim = "redkodim02.bmp";

	ofstream foutBlue(blueKodim, ios::binary);
	ofstream foutGreen(greenKodim, ios::binary);
	ofstream foutRed(redKodim, ios::binary);

	save_bmp_file(foutBlue, fileHeader, fileInfoHeader);
	save_bmp_file(foutGreen, fileHeader, fileInfoHeader);
	save_bmp_file(foutRed, fileHeader, fileInfoHeader);

	uint8_t nullRGB = 0;

	RGBQUAD** rgbInfoBlue = new RGBQUAD * [fileInfoHeader.biHeight];
	RGBQUAD** rgbInfoGreen = new RGBQUAD * [fileInfoHeader.biHeight];
	RGBQUAD** rgbInfoRed = new RGBQUAD * [fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfoBlue[i] = new RGBQUAD[fileInfoHeader.biWidth];
		rgbInfoGreen[i] = new RGBQUAD[fileInfoHeader.biWidth];
		rgbInfoRed[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			//
			write(foutBlue, rgbInfo[i][j].rgbBlue, 1); // Голубой
			write(foutBlue, nullRGB, 1);               // Зеленый
			write(foutBlue, nullRGB, 1);               // Красный

			rgbInfoBlue[i][j].rgbBlue = rgbInfo[i][j].rgbBlue;
			rgbInfoBlue[i][j].rgbGreen = nullRGB;
			rgbInfoBlue[i][j].rgbRed = nullRGB;

			//
			write(foutGreen, nullRGB, 1);                // Голубой
			write(foutGreen, rgbInfo[i][j].rgbGreen, 1); // Зеленый
			write(foutGreen, nullRGB, 1);                // Красный

			rgbInfoGreen[i][j].rgbBlue = nullRGB;
			rgbInfoGreen[i][j].rgbGreen = rgbInfo[i][j].rgbGreen;
			rgbInfoGreen[i][j].rgbRed = nullRGB;

			//
			write(foutRed, nullRGB, 1);              // Голубой
			write(foutRed, nullRGB, 1);              // Зеленый
			write(foutRed, rgbInfo[i][j].rgbRed, 1); // Красный

			rgbInfoRed[i][j].rgbBlue = nullRGB;
			rgbInfoRed[i][j].rgbGreen = nullRGB;
			rgbInfoRed[i][j].rgbRed = rgbInfo[i][j].rgbRed;
		}
	}

	// ------------------------------ 4-ое задание -------------------------

	uint8_t** intensityBlue = new uint8_t * [fileInfoHeader.biHeight]; // строки
	uint8_t** intensityGreen = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityRed = new uint8_t * [fileInfoHeader.biHeight];
	for (int i = 0; i < fileInfoHeader.biHeight; i++) {         // двумерный
		intensityBlue[i] = new uint8_t[fileInfoHeader.biWidth]; // столбцы
		intensityGreen[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityRed[i] = new uint8_t[fileInfoHeader.biWidth];
	}

	fillingIntensity(intensityBlue, intensityGreen, intensityRed, rgbInfo, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	// коэффициенты корреляции
	std::cout << "Коэффициенты корреляции" << std::endl;
	std::cout << "Коэффициенты корреляции r_RG "
		<< get_correl(intensityRed, intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Коэффициенты корреляции r_RB " << get_correl(intensityRed, intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth)
		<< std::endl;
	std::cout << "Коэффициенты корреляции r_BG "
		<< get_correl(intensityBlue, intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	// задание 5

	YCbCr** YCbCrinfo = new YCbCr * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		YCbCrinfo[i] = new YCbCr[fileInfoHeader.biWidth];
	}
	colorspaceconversion(rgbInfo, YCbCrinfo, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	// intensityYCbCr по отдельности
	uint8_t** intensityY = new uint8_t * [fileInfoHeader.biHeight]; // строки
	uint8_t** intensityCb = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityCr = new uint8_t * [fileInfoHeader.biHeight];
	for (int i = 0; i < fileInfoHeader.biHeight; i++) {      // двумерный
		intensityY[i] = new uint8_t[fileInfoHeader.biWidth]; // столбцы
		intensityCb[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityCr[i] = new uint8_t[fileInfoHeader.biWidth];
	}

	//  YCbCr по отдельности
	YCbCr** YCbCrinfoY = new YCbCr * [fileInfoHeader.biHeight];
	YCbCr** YCbCrinfoCb = new YCbCr * [fileInfoHeader.biHeight];
	YCbCr** YCbCrinfoCr = new YCbCr * [fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		YCbCrinfoY[i] = new YCbCr[fileInfoHeader.biWidth];
		YCbCrinfoCb[i] = new YCbCr[fileInfoHeader.biWidth];
		YCbCrinfoCr[i] = new YCbCr[fileInfoHeader.biWidth];
	}

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			//

			YCbCrinfoY[i][j].Y = YCbCrinfo[i][j].Y;
			YCbCrinfoY[i][j].Cb = nullRGB;
			YCbCrinfoY[i][j].Cr = nullRGB;

			YCbCrinfoCb[i][j].Y = nullRGB;
			YCbCrinfoCb[i][j].Cb = YCbCrinfo[i][j].Cb;
			YCbCrinfoCb[i][j].Cr = nullRGB;

			YCbCrinfoCr[i][j].Y = nullRGB;
			YCbCrinfoCr[i][j].Cb = nullRGB;
			YCbCrinfoCr[i][j].Cr = YCbCrinfo[i][j].Cr;
		}
	}

	fillingIntensity(intensityY, intensityCb, intensityCr, YCbCrinfo, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	// коэффициенты корреляции
	std::cout << "Коэффициенты корреляции 2" << std::endl;
	std::cout << "Коэффициенты корреляции r_YCb " << get_correl(intensityY, intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth)
		<< std::endl;
	std::cout << "Коэффициенты корреляции r_YCr " << get_correl(intensityY, intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth)
		<< std::endl;
	std::cout << "Коэффициенты корреляции r_CrCb " << get_correl(intensityCr, intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth)
		<< std::endl;

	// задание 6
	string greyYKodim = "greyYkodim02.bmp";
	string greyCbKodim = "greyCbkodim02.bmp";
	string greyCrKodim = "greyCrkodim02.bmp";

	ofstream foutGreyY(greyYKodim, ios::binary);
	ofstream foutGreyCb(greyCbKodim, ios::binary);
	ofstream foutGreyCr(greyCrKodim, ios::binary);

	save_bmp_file(foutGreyY, fileHeader, fileInfoHeader);
	save_bmp_file(foutGreyCb, fileHeader, fileInfoHeader);
	save_bmp_file(foutGreyCr, fileHeader, fileInfoHeader);

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {

			write(foutGreyY, YCbCrinfo[i][j].Y, 1); // Голубой
			write(foutGreyY, YCbCrinfo[i][j].Y, 1); // Зеленый
			write(foutGreyY, YCbCrinfo[i][j].Y, 1);

			write(foutGreyCb, YCbCrinfo[i][j].Cb, 1); // Голубой
			write(foutGreyCb, YCbCrinfo[i][j].Cb, 1); // Зеленый
			write(foutGreyCb, YCbCrinfo[i][j].Cb, 1);

			write(foutGreyCr, YCbCrinfo[i][j].Cr, 1); // Голубой
			write(foutGreyCr, YCbCrinfo[i][j].Cr, 1); // Зеленый
			write(foutGreyCr, YCbCrinfo[i][j].Cr, 1);
		}
	}

	// 7

	RGBQUAD** rgbInfoNew = new RGBQUAD * [fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfoNew[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	decolorspaceconversion(rgbInfoNew, YCbCrinfo, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	string rgbrecor = "rgb_recor_kodim02_punkt_7.bmp";

	ofstream foutRgbRecor(rgbrecor, ios::binary);

	save_bmp_file(foutRgbRecor, fileHeader, fileInfoHeader);

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {

			write(foutRgbRecor, rgbInfoNew[i][j].rgbBlue, 1);  // Голубой
			write(foutRgbRecor, rgbInfoNew[i][j].rgbGreen, 1); // Зеленый
			write(foutRgbRecor, rgbInfoNew[i][j].rgbRed, 1);
		}
	}

	RGBQUAD** rgbInfoBlueNew = new RGBQUAD * [fileInfoHeader.biHeight];
	RGBQUAD** rgbInfoGreenNew = new RGBQUAD * [fileInfoHeader.biHeight];
	RGBQUAD** rgbInfoRedNew = new RGBQUAD * [fileInfoHeader.biHeight];

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfoBlueNew[i] = new RGBQUAD[fileInfoHeader.biWidth];
		rgbInfoGreenNew[i] = new RGBQUAD[fileInfoHeader.biWidth];
		rgbInfoRedNew[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			//

			rgbInfoBlueNew[i][j].rgbBlue = rgbInfoNew[i][j].rgbBlue;
			rgbInfoBlueNew[i][j].rgbGreen = nullRGB;
			rgbInfoBlueNew[i][j].rgbRed = nullRGB;

			rgbInfoBlueNew[i][j].rgbBlue = nullRGB;
			rgbInfoBlueNew[i][j].rgbGreen = rgbInfoNew[i][j].rgbGreen;
			rgbInfoBlueNew[i][j].rgbRed = nullRGB;

			rgbInfoRedNew[i][j].rgbBlue = nullRGB;
			rgbInfoRedNew[i][j].rgbGreen = nullRGB;
			rgbInfoRedNew[i][j].rgbRed = rgbInfoNew[i][j].rgbRed;
		}
	}

	uint8_t** intensityBlueNew = new uint8_t * [fileInfoHeader.biHeight]; // строки
	uint8_t** intensityGreenNew = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityRedNew = new uint8_t * [fileInfoHeader.biHeight];
	for (int i = 0; i < fileInfoHeader.biHeight; i++) {            // двумерный
		intensityBlueNew[i] = new uint8_t[fileInfoHeader.biWidth]; // столбцы
		intensityGreenNew[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityRedNew[i] = new uint8_t[fileInfoHeader.biWidth];
	}

	fillingIntensity(intensityBlueNew, intensityGreenNew, intensityRedNew, rgbInfoNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	std::cout << "Вычисления PSNR 7 пункт" << std::endl;
	// первый начальный второй изменный
	std::cout << "Вычисления PSNR B =" << PSNR(intensityBlue, intensityBlueNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR G =" << PSNR(intensityGreen, intensityGreenNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR R =" << PSNR(intensityRed, intensityRedNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	// 8
	std::cout << "Децимация" << std::endl;

	YCbCr** YCbCrinfoDecimation1 = new YCbCr * [fileInfoHeader.biHeight];
	YCbCr** YCbCrinfoDecimation2 = new YCbCr * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		YCbCrinfoDecimation1[i] = new YCbCr[fileInfoHeader.biWidth];
		YCbCrinfoDecimation2[i] = new YCbCr[fileInfoHeader.biWidth];
	}

	YCbCr** YCbCrinfoDecimation1_recovery = new YCbCr * [fileInfoHeader.biHeight];
	YCbCr** YCbCrinfoDecimation2_recovery = new YCbCr * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		YCbCrinfoDecimation1_recovery[i] = new YCbCr[fileInfoHeader.biWidth];
		YCbCrinfoDecimation2_recovery[i] = new YCbCr[fileInfoHeader.biWidth];
	}

	RGBQUAD** rgbInfoRecovery = new RGBQUAD * [fileInfoHeader.biHeight];
	RGBQUAD** rgbInfoRecovery2 = new RGBQUAD * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfoRecovery[i] = new RGBQUAD[fileInfoHeader.biWidth];
		rgbInfoRecovery2[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}

	std::cout << "Децимация первый способ (2)" << std::endl;

	decimation1(YCbCrinfo, YCbCrinfoDecimation1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	// Децимация первый способ (2)
	decimation_recovery(YCbCrinfoDecimation1_recovery, YCbCrinfoDecimation1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	decolorspaceconversion(rgbInfoRecovery, YCbCrinfoDecimation1_recovery, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	string greyDecimation1Kodim = "greyDecimation1Recoversykodim02.bmp";

	ofstream foutGreyDecimation1(greyDecimation1Kodim, ios::binary);

	save_bmp_file(foutGreyDecimation1, fileHeader, fileInfoHeader);

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			write(foutGreyDecimation1, rgbInfoRecovery[i][j].rgbBlue,
				1); // Голубой
			write(foutGreyDecimation1, rgbInfoRecovery[i][j].rgbGreen,
				1); // Зеленый
			write(foutGreyDecimation1, rgbInfoRecovery[i][j].rgbRed, 1);
		}
	}

	std::cout << "Децимация второй способ (2) " << std::endl;
	decimation2(YCbCrinfo, YCbCrinfoDecimation2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	decimation_recovery(YCbCrinfoDecimation2_recovery, YCbCrinfoDecimation2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	decolorspaceconversion(rgbInfoRecovery2, YCbCrinfoDecimation2_recovery, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	string greyDecimation2Kodim = "greyDecimation2Recoversykodim02.bmp";

	ofstream foutGreyDecimation2(greyDecimation2Kodim, ios::binary);

	save_bmp_file(foutGreyDecimation2, fileHeader, fileInfoHeader);

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {

			write(foutGreyDecimation2, rgbInfoRecovery2[i][j].rgbBlue,
				1); // Голубой
			write(foutGreyDecimation2, rgbInfoRecovery2[i][j].rgbGreen,
				1); // Зеленый
			write(foutGreyDecimation2, rgbInfoRecovery2[i][j].rgbRed, 1);
		}
	}

	// 9

	// 10

	uint8_t** intensityBlueNew2 = new uint8_t * [fileInfoHeader.biHeight]; // строки
	uint8_t** intensityGreenNew2 = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityRedNew2 = new uint8_t * [fileInfoHeader.biHeight];

	uint8_t** intensityCbNew = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityCrNew = new uint8_t * [fileInfoHeader.biHeight];

	for (int i = 0; i < fileInfoHeader.biHeight; i++) {             // двумерный
		intensityBlueNew2[i] = new uint8_t[fileInfoHeader.biWidth]; // столбцы
		intensityGreenNew2[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityRedNew2[i] = new uint8_t[fileInfoHeader.biWidth];

		intensityCbNew[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityCrNew[i] = new uint8_t[fileInfoHeader.biWidth];
	}

	fillingIntensity(intensityBlueNew2, intensityGreenNew2, intensityRedNew2, rgbInfoRecovery, fileInfoHeader.biHeight,
		fileInfoHeader.biWidth);

	fillingIntensity(intensityCbNew, intensityCrNew, YCbCrinfoDecimation1_recovery, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	std::cout << "Вычисления PSNR 10 пункт 1 способ" << std::endl;
	// первый начальный второй изменный

	std::cout << "Вычисления PSNR B =" << PSNR(intensityBlue, intensityBlueNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR G =" << PSNR(intensityGreen, intensityGreenNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR R =" << PSNR(intensityRed, intensityRedNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;

	std::cout << "Вычисления PSNR Cb =" << PSNR(intensityCb, intensityCbNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR Cr =" << PSNR(intensityCr, intensityCrNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	cout << std::endl;

	fillingIntensity(intensityBlueNew2, intensityGreenNew2, intensityRedNew2, rgbInfoRecovery2, fileInfoHeader.biHeight,
		fileInfoHeader.biWidth);

	fillingIntensity(intensityCbNew, intensityCrNew, YCbCrinfoDecimation2_recovery, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	std::cout << "Вычисления PSNR 10 пункт 2 способ" << std::endl;
	// первый начальный второй изменный

	std::cout << "Вычисления PSNR B =" << PSNR(intensityBlue, intensityBlueNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR G =" << PSNR(intensityGreen, intensityGreenNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR R =" << PSNR(intensityRed, intensityRedNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;

	std::cout << "Вычисления PSNR Cb =" << PSNR(intensityCb, intensityCbNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR Cr =" << PSNR(intensityCr, intensityCrNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	cout << std::endl;
	// 11

	YCbCr** YCbCrinfoDecimation1_4 = new YCbCr * [fileInfoHeader.biHeight];
	YCbCr** YCbCrinfoDecimation2_4 = new YCbCr * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		YCbCrinfoDecimation1_4[i] = new YCbCr[fileInfoHeader.biWidth];
		YCbCrinfoDecimation2_4[i] = new YCbCr[fileInfoHeader.biWidth];
	}

	YCbCr** YCbCrinfoDecimation1_recovery_4 = new YCbCr * [fileInfoHeader.biHeight];
	YCbCr** YCbCrinfoDecimation2_recovery_4 = new YCbCr * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		YCbCrinfoDecimation1_recovery_4[i] = new YCbCr[fileInfoHeader.biWidth];
		YCbCrinfoDecimation2_recovery_4[i] = new YCbCr[fileInfoHeader.biWidth];
	}

	RGBQUAD** rgbInfoRecovery_4 = new RGBQUAD * [fileInfoHeader.biHeight];
	RGBQUAD** rgbInfoRecovery2_4 = new RGBQUAD * [fileInfoHeader.biHeight];
	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		rgbInfoRecovery_4[i] = new RGBQUAD[fileInfoHeader.biWidth];
		rgbInfoRecovery2_4[i] = new RGBQUAD[fileInfoHeader.biWidth];
	}




	std::cout << "Децимация первый способ (4)" << std::endl;

	decimation4times1(YCbCrinfo, YCbCrinfoDecimation1_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	// Децимация первый способ (2)
	decimation_recovery4times(YCbCrinfoDecimation1_recovery_4, YCbCrinfoDecimation1_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	decolorspaceconversion(rgbInfoRecovery_4, YCbCrinfoDecimation1_recovery_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	string greyDecimation1_4Kodim = "greyDecimation1_4Recoversykodim02.bmp";

	ofstream foutGreyDecimation1_4(greyDecimation1_4Kodim, ios::binary);

	save_bmp_file(foutGreyDecimation1_4, fileHeader, fileInfoHeader);

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
			write(foutGreyDecimation1_4, rgbInfoRecovery_4[i][j].rgbBlue,
				1); // Голубой
			write(foutGreyDecimation1_4, rgbInfoRecovery_4[i][j].rgbGreen,
				1); // Зеленый
			write(foutGreyDecimation1_4, rgbInfoRecovery_4[i][j].rgbRed, 1);
		}
	}

	// decimation1(YCbCrinfo, YCbCrinfoDecimation1, 4, fileInfoHeader.biHeight,
	// fileInfoHeader.biWidth);



	std::cout << "Децимация второй способ (4)" << std::endl;

	decimation4timesB(YCbCrinfo, YCbCrinfoDecimation2_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	decimation_recovery4times(YCbCrinfoDecimation2_recovery_4, YCbCrinfoDecimation2_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	decolorspaceconversion(rgbInfoRecovery2_4, YCbCrinfoDecimation2_recovery_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	string greyDecimation2Kodim_4 = "greyDecimation2_4Recoversykodim02.bmp";

	ofstream foutGreyDecimation2_4(greyDecimation2Kodim_4, ios::binary);

	save_bmp_file(foutGreyDecimation2_4, fileHeader, fileInfoHeader);

	for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
		for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {

			write(foutGreyDecimation2_4, rgbInfoRecovery2[i][j].rgbBlue,
				1); // Голубой
			write(foutGreyDecimation2_4, rgbInfoRecovery2[i][j].rgbGreen,
				1); // Зеленый
			write(foutGreyDecimation2_4, rgbInfoRecovery2[i][j].rgbRed, 1);
		}
	}

	uint8_t** intensityBlueNew2_4 = new uint8_t * [fileInfoHeader.biHeight]; // строки
	uint8_t** intensityGreenNew2_4 = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityRedNew2_4 = new uint8_t * [fileInfoHeader.biHeight];

	uint8_t** intensityCbNew_4 = new uint8_t * [fileInfoHeader.biHeight];
	uint8_t** intensityCrNew_4 = new uint8_t * [fileInfoHeader.biHeight];

	for (int i = 0; i < fileInfoHeader.biHeight; i++) {             // двумерный
		intensityBlueNew2_4[i] = new uint8_t[fileInfoHeader.biWidth]; // столбцы
		intensityGreenNew2_4[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityRedNew2_4[i] = new uint8_t[fileInfoHeader.biWidth];

		intensityCbNew_4[i] = new uint8_t[fileInfoHeader.biWidth];
		intensityCrNew_4[i] = new uint8_t[fileInfoHeader.biWidth];
	}

	fillingIntensity(intensityBlueNew2_4, intensityGreenNew2_4, intensityRedNew2_4, rgbInfoRecovery_4, fileInfoHeader.biHeight,
		fileInfoHeader.biWidth);

	fillingIntensity(intensityCbNew_4, intensityCrNew_4, YCbCrinfoDecimation1_recovery_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	std::cout << "Вычисления PSNR 11 пункт децимация (4) 1 способ" << std::endl;
	// первый начальный второй изменный

	std::cout << "Вычисления PSNR B =" << PSNR(intensityBlue, intensityBlueNew2_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR G =" << PSNR(intensityGreen, intensityGreenNew2_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR R =" << PSNR(intensityRed, intensityRedNew2_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;

	std::cout << "Вычисления PSNR Cb =" << PSNR(intensityCb, intensityCbNew_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR Cr =" << PSNR(intensityCr, intensityCrNew_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	cout << std::endl;
	fillingIntensity(intensityBlueNew2, intensityGreenNew2, intensityRedNew2, rgbInfoRecovery2_4, fileInfoHeader.biHeight,
		fileInfoHeader.biWidth);

	fillingIntensity(intensityCbNew, intensityCrNew, YCbCrinfoDecimation2_recovery_4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	std::cout << "Вычисления PSNR 10 11 пункт децимация (4) 2 способ" << std::endl;
	// первый начальный второй изменный

	std::cout << "Вычисления PSNR B =" << PSNR(intensityBlue, intensityBlueNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR G =" << PSNR(intensityGreen, intensityGreenNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR R =" << PSNR(intensityRed, intensityRedNew2, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;

	std::cout << "Вычисления PSNR Cb =" << PSNR(intensityCb, intensityCbNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;
	std::cout << "Вычисления PSNR Cr =" << PSNR(intensityCr, intensityCrNew, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8)
		<< std::endl;

	cout<<std::endl;
	// decimation2(YCbCrinfo, YCbCrinfoDecimation1, 4, fileInfoHeader.biHeight,
	// fileInfoHeader.biWidth);

	// decimation_recovery(YCbCrinfo, YCbCrinfoDecimation1, 4,
	// fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	/*
	std::cout << "Вычисления PSNR B =" << PSNR(intensityBlue, intensityBlueNew2,
	fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8) << std::endl; std::cout
	<< "Вычисления PSNR G =" << PSNR(intensityGreen, intensityGreenNew2,
	fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8) << std::endl; std::cout
	<< "Вычисления PSNR R =" << PSNR(intensityRed, intensityRedNew2,
	fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8) << std::endl;

	std::cout << "Вычисления PSNR Cb =" << PSNR(intensityCb, intensityCbNew,
	fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8) << std::endl; std::cout
	<< "Вычисления PSNR Cr =" << PSNR(intensityCr, intensityCrNew,
	fileInfoHeader.biHeight, fileInfoHeader.biWidth, 8) << std::endl;
	*/
	cout << "12 пункт" << endl;
	map<int, int> count_R = count_number_DPCM(intensityRed, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	

	map<int, int> count_G = count_number_DPCM(intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	

	map<int, int> count_B = count_number_DPCM(intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth);


	map<int, int> count_Y = count_number_DPCM(intensityY, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_Cb = count_number_DPCM(intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_Cr = count_number_DPCM(intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth);


	ofstream foutR("count_R.txt", ios_base::out | ios_base::trunc); //ios_base::out | ios_base::trunc


	save_file_count(foutR, count_R);


	ofstream foutG("count_G.txt", ios_base::out | ios_base::trunc);


	save_file_count(foutG, count_G);


	ofstream foutB("count_B.txt", ios_base::out | ios_base::trunc);


	save_file_count(foutB, count_B);


	//-----------

	ofstream foutY("count_Y.txt", ios_base::out | ios_base::trunc);


	save_file_count(foutY, count_Y);


	ofstream foutCb("count_Cb.txt", ios_base::out | ios_base::trunc);


	save_file_count(foutCb, count_Cb);


	ofstream foutCr("count_Cr.txt", ios_base::out | ios_base::trunc);

	save_file_count(foutCr, count_Cr);


	// 13
	
	std::cout << "Число бит затрачиваемых при поэлементном сжатии " << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии R =" <<
	entropy(intensityRed, fileInfoHeader.biHeight, fileInfoHeader.biWidth)
	<<std::endl; std::cout << "Число бит затрачиваемых при поэлементном сжатии G=" << entropy(intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth) <<
	std::endl; std::cout << "Число бит затрачиваемых при поэлементном сжатии B ="<< entropy(intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth) <<std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Y =" << entropy(intensityY, fileInfoHeader.biHeight, fileInfoHeader.biWidth) <<std::endl; 
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cb=" << entropy(intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cr=" << entropy(intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth) <<std::endl;
	


	//14     intensityCr

	cout << "14 пункт DPCM" << endl;
	int** intensityDPCM_R1 = DPCM(intensityRed, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 1);
	int** intensityDPCM_R2 = DPCM(intensityRed, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 2);
	int** intensityDPCM_R3 = DPCM(intensityRed, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 3);
	int** intensityDPCM_R4 = DPCM(intensityRed, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 4);

	int** intensityDPCM_G1 = DPCM(intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 1);
	int** intensityDPCM_G2 = DPCM(intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 2);
	int** intensityDPCM_G3 = DPCM(intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 3);
	int** intensityDPCM_G4 = DPCM(intensityGreen, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 4);

	int** intensityDPCM_B1 = DPCM(intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 1);
	int** intensityDPCM_B2 = DPCM(intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 2);
	int** intensityDPCM_B3 = DPCM(intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 3);
	int** intensityDPCM_B4 = DPCM(intensityBlue, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 4);

	int** intensityDPCM_Y1 = DPCM(intensityY, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 1);
	int** intensityDPCM_Y2 = DPCM(intensityY, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 2);
	int** intensityDPCM_Y3 = DPCM(intensityY, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 3);
	int** intensityDPCM_Y4 = DPCM(intensityY, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 4);

	int** intensityDPCM_Cb1 = DPCM(intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 1);
	int** intensityDPCM_Cb2 = DPCM(intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 2);
	int** intensityDPCM_Cb3 = DPCM(intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 3);
	int** intensityDPCM_Cb4 = DPCM(intensityCb, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 4);

	int** intensityDPCM_Cr1 = DPCM(intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 1);
	int** intensityDPCM_Cr2 = DPCM(intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 2);
	int** intensityDPCM_Cr3 = DPCM(intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 3);
	int** intensityDPCM_Cr4 = DPCM(intensityCr, fileInfoHeader.biHeight, fileInfoHeader.biWidth, 4);
	//15
	cout << "15 пункт гистограммы" << endl;
	map<int,int> count_R1 = count_number_DPCM(intensityDPCM_R1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_R2 = count_number_DPCM(intensityDPCM_R2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_R3 = count_number_DPCM(intensityDPCM_R3, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_R4 = count_number_DPCM(intensityDPCM_R4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_G1 = count_number_DPCM(intensityDPCM_G1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_G2 = count_number_DPCM(intensityDPCM_G2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_G3 = count_number_DPCM(intensityDPCM_G3, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_G4 = count_number_DPCM(intensityDPCM_G4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_B1 = count_number_DPCM(intensityDPCM_B1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_B2 = count_number_DPCM(intensityDPCM_B2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_B3 = count_number_DPCM(intensityDPCM_B3, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_B4 = count_number_DPCM(intensityDPCM_B4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_Y1 = count_number_DPCM(intensityDPCM_Y1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Y2 = count_number_DPCM(intensityDPCM_Y2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Y3 = count_number_DPCM(intensityDPCM_Y3, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Y4 = count_number_DPCM(intensityDPCM_Y4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_Cb1 = count_number_DPCM(intensityDPCM_Cb1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Cb2 = count_number_DPCM(intensityDPCM_Cb2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Cb3 = count_number_DPCM(intensityDPCM_Cb3, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Cb4 = count_number_DPCM(intensityDPCM_Cb4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	map<int, int> count_Cr1 = count_number_DPCM(intensityDPCM_Cr1, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Cr2 = count_number_DPCM(intensityDPCM_Cr2, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Cr3 = count_number_DPCM(intensityDPCM_Cr3, fileInfoHeader.biHeight, fileInfoHeader.biWidth);
	map<int, int> count_Cr4 = count_number_DPCM(intensityDPCM_Cr4, fileInfoHeader.biHeight, fileInfoHeader.biWidth);

	ofstream foutR1("count_R1.txt", ios_base::out | ios_base::trunc); //ios_base::out | ios_base::trunc
	ofstream foutR2("count_R2.txt",ios_base::out | ios_base::trunc);
	ofstream foutR3("count_R3.txt", ios_base::out | ios_base::trunc);
	ofstream foutR4("count_R4.txt", ios_base::out | ios_base::trunc);
	
	save_file_count(foutR1, count_R1);
	save_file_count(foutR2, count_R2);
	save_file_count(foutR3, count_R3);
	save_file_count(foutR4, count_R4);
	
	ofstream foutG1("count_G1.txt", ios_base::out | ios_base::trunc);
	ofstream foutG2("count_G2.txt", ios_base::out | ios_base::trunc);
	ofstream foutG3("count_G3.txt", ios_base::out | ios_base::trunc);
	ofstream foutG4("count_G4.txt", ios_base::out | ios_base::trunc);
	
	save_file_count(foutG1, count_G1);
	save_file_count(foutG2, count_G2);
	save_file_count(foutG3, count_G3);
	save_file_count(foutG4, count_G4);
	
	ofstream foutB1("count_B1.txt", ios_base::out | ios_base::trunc);
	ofstream foutB2("count_B2.txt", ios_base::out | ios_base::trunc);
	ofstream foutB3("count_B3.txt", ios_base::out | ios_base::trunc);
	ofstream foutB4("count_B4.txt", ios_base::out | ios_base::trunc);
	
	save_file_count(foutB1, count_B1);
	save_file_count(foutB2, count_B2);
	save_file_count(foutB3, count_B3);
	save_file_count(foutB4, count_B4);
	
	//-----------
	
	ofstream foutY1("count_Y1.txt", ios_base::out | ios_base::trunc);
	ofstream foutY2("count_Y2.txt", ios_base::out | ios_base::trunc);
	ofstream foutY3("count_Y3.txt", ios_base::out | ios_base::trunc);
	ofstream foutY4("count_Y4.txt", ios_base::out | ios_base::trunc);
	
	save_file_count(foutY1, count_Y1);
	save_file_count(foutY2, count_Y2);
	save_file_count(foutY3, count_Y3);
	save_file_count(foutY4, count_Y4);
	
	ofstream foutCb1("count_Cb1.txt", ios_base::out | ios_base::trunc);
	ofstream foutCb2("count_Cb2.txt", ios_base::out | ios_base::trunc);
	ofstream foutCb3("count_Cb3.txt", ios_base::out | ios_base::trunc);
	ofstream foutCb4("count_Cb4.txt", ios_base::out | ios_base::trunc);
	
	save_file_count(foutCb1, count_Cb1);
	save_file_count(foutCb2, count_Cb2);
	save_file_count(foutCb3, count_Cb3);
	save_file_count(foutCb4, count_Cb4);
	
	ofstream foutCr1("count_Cr1.txt", ios_base::out | ios_base::trunc);
	ofstream foutCr2("count_Cr2.txt", ios_base::out | ios_base::trunc);
	ofstream foutCr3("count_Cr3.txt", ios_base::out | ios_base::trunc);
	ofstream foutCr4("count_Cr4.txt", ios_base::out | ios_base::trunc);
	
	save_file_count(foutCr1, count_Cr1);
	save_file_count(foutCr2, count_Cr2);
	save_file_count(foutCr3, count_Cr3);
	save_file_count(foutCr4, count_Cr4);
	
	//-------- закрываем
	/*
	foutR1.close();
	foutR2.close();
	foutR3.close();
	foutR4.close();

	
	foutG1.close();
	foutG2.close();
	foutG3.close();
	foutG4.close();

	foutB1.close();
	foutB2.close();
	foutB3.close();
	foutB4.close();
	
	foutY1.close();
	foutY2.close();
	foutY3.close();
	foutY4.close();

	foutCb1.close();
	foutCb2.close();
	foutCb3.close();
	foutCb4.close();

	foutCr1.close();
	foutCr2.close();
	foutCr3.close();
	foutCr4.close();
	*/
	//--закрыли
	std::cout << " Пункт 16 число бит затрачиваемых при поэлементном сжатии " << std::endl;

	std::cout << "Число бит затрачиваемых при поэлементном сжатии " << std::endl;
	
	std::cout << "Число бит затрачиваемых при поэлементном сжатии R1 =" <<entropy(intensityDPCM_R1, fileInfoHeader.biHeight, fileInfoHeader.biWidth)<< std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии R2 =" << entropy(intensityDPCM_R2, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии R3 =" << entropy(intensityDPCM_R3, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии R4 =" << entropy(intensityDPCM_R4, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;

	std::cout << "Число бит затрачиваемых при поэлементном сжатии G1 =" << entropy(intensityDPCM_G1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) <<std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии G2 =" << entropy(intensityDPCM_G2, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии G3 =" << entropy(intensityDPCM_G3, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии G4 =" << entropy(intensityDPCM_G4, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;

	std::cout << "Число бит затрачиваемых при поэлементном сжатии B1 =" << entropy(intensityDPCM_B1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии B2 =" << entropy(intensityDPCM_B2, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии B3 =" << entropy(intensityDPCM_B3, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии B4 =" << entropy(intensityDPCM_B4, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;

	std::cout << "Число бит затрачиваемых при поэлементном сжатии Y1 =" << entropy(intensityDPCM_Y1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Y2 =" << entropy(intensityDPCM_Y2, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Y3 =" << entropy(intensityDPCM_Y3, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Y4 =" << entropy(intensityDPCM_Y4, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;


	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cb1 =" << entropy(intensityDPCM_Cb1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cb2 =" << entropy(intensityDPCM_Cb1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cb3 =" << entropy(intensityDPCM_Cb1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cb4 =" << entropy(intensityDPCM_Cb1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;

	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cr1 =" << entropy(intensityDPCM_Cr1, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cr2 =" << entropy(intensityDPCM_Cr2, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cr3 =" << entropy(intensityDPCM_Cr3, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	std::cout << "Число бит затрачиваемых при поэлементном сжатии Cr4 =" << entropy(intensityDPCM_Cr4, fileInfoHeader.biHeight, fileInfoHeader.biWidth) << std::endl;
	
	std::cout << "Конец" << std::endl;
	return 0;
}
