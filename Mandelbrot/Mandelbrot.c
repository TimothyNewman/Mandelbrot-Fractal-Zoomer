/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
	u_int64_t totalIterations = 0;
	ComplexNumber *complexZ = newComplexNumber(0, 0);
	while (totalIterations <= maxiters) {
		if (ComplexAbs(complexZ) >= threshold) {
			freeComplexNumber(complexZ);
			return totalIterations;
		}
		ComplexNumber *complexX = ComplexProduct(complexZ, complexZ);
		ComplexNumber *complexY = ComplexSum(complexX, point);
		freeComplexNumber(complexX);
		freeComplexNumber(complexZ);
		complexZ = complexY;
		totalIterations++;
	}
	freeComplexNumber(complexZ);
  	return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
	int res = resolution;
	int pixels = res * 2 + 1;
	int height;
	int width;
	u_int64_t counter = 0;
	u_int64_t numIterations;
	double realDouble = Re(center);
	double imaginaryDouble = Im(center);
	for (height = 0; height < pixels; height++) {
		for (width = 0; width < pixels; width++) {
			double rComp = (scale / res) * (width - res) + realDouble;
			double iComp = (scale / res) * (res - height) + imaginaryDouble;
			ComplexNumber *complexC = newComplexNumber(rComp, iComp);
			numIterations = MandelbrotIterations(max_iterations, complexC, threshold);
			output[counter] = numIterations;
			counter++;
			freeComplexNumber(complexC);
		}
	}
}
