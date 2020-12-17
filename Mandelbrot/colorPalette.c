/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int horizontal = width;
	if ((width || heightpercolor) < 1) {
		return 1;
	}
	int * colorcount = (int *) malloc(sizeof(int));
	uint8_t** rgb = FileToColorMap(colorfile, colorcount);
	int vertical = heightpercolor * (*colorcount);
	if (rgb == NULL) {
		free(colorcount);
		return 1;
	}
	if (*colorcount < 1) {
		for (int l = 0; l < (*colorcount); l++) {
			free(rgb[l]);
		}
		free(rgb);
		free(colorcount);
		return 1;
	}
	FILE *ofile;
	ofile = fopen(outputfile, "w");
	fprintf(ofile, "P3 %d %d 255\n", horizontal, vertical);
	for (int y = 0; y < (*colorcount); y++) {
		for (int counter = 0; counter < heightpercolor; counter++){
			for (int x = 0; x < horizontal - 1; x++) {
				fprintf(ofile, "%hhu %hhu %hhu ", rgb[y][0], rgb[y][1], rgb[y][2]);
			}
			fprintf(ofile, "%hhu %hhu %hhu\n", rgb[y][0], rgb[y][1], rgb[y][2]);
		}
	}
	for (int f = 0; f < (*colorcount); f++) {
		free(rgb[f]);
	}
	free(colorcount);
	free(rgb);
	fclose(ofile);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int horizontal = width;
	if ((width || heightpercolor) < 1) {
		return 1;
	}
	int * colorcount = (int *) malloc(sizeof(int));
	uint8_t** rgb = FileToColorMap(colorfile, colorcount);
	int vertical = heightpercolor * (*colorcount);
	if (rgb == NULL) {
		free(colorcount);
		return 1;
	}
	if (*colorcount < 1) {
		for (int l = 0; l < (*colorcount); l++) {
			free(rgb[l]);
		}
		free(rgb);
		free(colorcount);
	}
	FILE *ofile;
	ofile = fopen(outputfile, "w");
	fprintf(ofile, "P6 %d %d 255\n", horizontal, vertical);
	for (int y = 0; y < (*colorcount); y++) {
		for (int counter = 0; counter < heightpercolor; counter++) {
			for (int x = 0; x < horizontal; x++) {
				fwrite(&rgb[y][0], sizeof(char), 1, ofile);
				fwrite(&rgb[y][1], sizeof(char), 1, ofile);
				fwrite(&rgb[y][2], sizeof(char), 1, ofile);
			}
		}
	}
	for (int f = 0; f < (*colorcount); f++) {
		free(rgb[f]);
	}
	free(colorcount);
	free(rgb);
	fclose(ofile);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}
