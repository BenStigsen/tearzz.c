#ifndef ZIMAGE_H
#define ZIMAGE_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define is_png(a) has_header(a, HEADER_PNG)
#define is_jpg(a) has_header(a, HEADER_JPG)
#define is_gif(a) has_header(a, HEADER_GIF)
#define is_bmp(a) has_header(a, HEADER_BMP)
#define is_mng(a) has_header(a, HEADER_MNG)
#define is_ppm(a) has_header(a, HEADER_PPM)
#define is_psd(a) has_header(a, HEADER_PSD)

const int HEADER_PNG[8] = {137, 80, 78, 71, 13, 10, 26, 10};
const int HEADER_JPG[3] = {255, 216, 255};
const int HEADER_GIF[6] = {71, 73, 70, 56, 57, 97}; // {71, 73, 70, 56, 55, 97}
const int HEADER_BMP[2] = {66, 77};
const int HEADER_MNG[8] = {138, 77, 78, 71, 13, 10, 26, 10};
const int HEADER_PPM[2] = {80, 52};
const int HEADER_PSD[4] = {56, 66, 80, 83};

bool has_header(char *filename, const int *header)
{
	int byte;
	bool result = true;

	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {return false;}

	fseek(fp, 0, SEEK_SET);

	int i = 0;
	while (i < sizeof(header)/sizeof(header[0]) && (byte = (int)fgetc(fp)) != EOF)
	{
		if (byte != header[i])
		{
			result = false;
			break;
		}

		++i;
	}

	fclose(fp);
	return result;
}

#ifdef __cplusplus
}
#endif

#endif // ZIMAGE_H
