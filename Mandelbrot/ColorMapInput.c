/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE *cfile;
	cfile = fopen(colorfile, "r");
	if (cfile == NULL) {
		return NULL;
	}
	*colorcount = 0;
	fscanf(cfile, "%d", colorcount);
	uint8_t** buffer;
	buffer = (uint8_t**) malloc(sizeof(uint8_t*) * (*colorcount));
	int r, g, b, rgbvals;
	int counter = 0;
	while (counter < (*colorcount)) {
		uint8_t *colorArray;
		colorArray = (uint8_t *) malloc(sizeof(uint8_t) * 3);
		rgbvals = fscanf(cfile, "%d %d %d", &r, &g, &b);
		if ((rgbvals!=3) || (rgbvals==-1) || (rgbvals==EOF) ||
		    (r>255||r<0) || (g>255||g<0) || (b>255||b<0)) {
			for (int f = 0; f < (*colorcount); f++) {
				free(buffer[f]);
			}
			free(colorArray);
			free(buffer);
			return NULL;
		}
		colorArray[0] = (uint8_t) r;
		colorArray[1] = (uint8_t) g;
		colorArray[2] = (uint8_t) b;
		buffer[counter] = colorArray;
		counter++;
	}
	fclose(cfile);
	return buffer;
}
