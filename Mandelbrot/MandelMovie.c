/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.

The remaining frames form a geometric sequence of scales, so
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.

As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/

void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){

    double common_ratio = exp((log(finalscale / initialscale)) / ((double)(framecount - 1)));
    for (int n = 0; n < framecount; n++) {
	double scale = initialscale * pow(common_ratio, n);
	Mandelbrot(threshold, max_iterations, center, scale, resolution, output[n]);
    }
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{

	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/
	if (argc != 11) {
		return 1;
	}
	double threshold, center_real, center_imaginary, initialscale, finalscale;
	u_int64_t max_iterations, resolution;
	int framecount;
	threshold = atof(argv[1]);
	max_iterations = atoi(argv[2]);
	center_real = atof(argv[3]);
	center_imaginary = atof(argv[4]);
	initialscale = atof(argv[5]);
	finalscale = atof(argv[6]);
	framecount = atoi(argv[7]);
	resolution = atoi(argv[8]);
	if ((threshold || max_iterations || initialscale || finalscale) <= 0) {
		return 1;
	}
	if ((framecount > 10000) || (framecount <= 0)) {
		return 1;
	}
	if (resolution < 0) {
		return 1;
	}
	if ((framecount == 1) && (initialscale != finalscale)) {
		return 1;
	}
	int colorcount;
	uint8_t** rgb = FileToColorMap(argv[10], &colorcount);
	if (rgb == NULL) {
		return 1;
	}

	u_int64_t num_size = 2 * resolution + 1;
	u_int64_t size = num_size * num_size;

	u_int64_t** frameoutput = malloc(sizeof(u_int64_t*) * framecount);
	for (int l = 0; l < framecount; l++) {
		frameoutput[l] = malloc(sizeof(u_int64_t) * size);
	}
	ComplexNumber *complexNum = newComplexNumber(center_real, center_imaginary);
        MandelMovie(threshold, max_iterations, complexNum, initialscale, finalscale,
			 framecount, resolution, frameoutput);

	int num;
	int pixel;
	int length = strlen(argv[9]);
	char arr[length + 16];
	for (int y = 0; y < framecount; y++) {
		sprintf(arr, "%s/frame%05u.ppm", argv[9], y);
		FILE *addFile;
		addFile = fopen(arr, "w");
		fprintf(addFile, "P6 %lu %lu 255\n", num_size, num_size);
		for (int x = 0; x < size; x++) {
			num = frameoutput[y][x];
			if (num == 0) {
				fputc(0, addFile);
				fputc(0, addFile);
				fputc(0, addFile);

			} else {
			pixel = (num - 1) % colorcount;
			fputc(rgb[pixel][0], addFile);
			fputc(rgb[pixel][1], addFile);
			fputc(rgb[pixel][2], addFile);
			}

		}
		fclose(addFile);
	}

	for (int d = 0; d < framecount; d++) {
		free(frameoutput[d]);
	}
	for (int f = 0; f < colorcount; f++) {
		free(rgb[f]);
	}
	freeComplexNumber(complexNum);
	free(rgb);
	free(frameoutput);
	return 0;
}
