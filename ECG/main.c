#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Udarbejdet af Seabstian Ameland Maltesen, s144475

void newSignal(int arr[], int arr_Length, int signal);

int main() {


	QRS_params qsr_params;
	setStandardParams(&qsr_params);

    FILE *file;
	file = openfile("ECG.txt");

	int signal[13] = { 0 };
	int lowPass[33] = { 0 };
	int highPass[5] = { 0 };
	int derivative[1] = {0};
	int squared[30] = {0};
	int mwi[3] = {0};

	int i = 1;

	while(!feof (file)) {


		newSignal(signal, 13, getNextData(file));

		newSignal(lowPass, 33, lowPassFilter(signal, lowPass));

		newSignal(highPass, 5, highPassFilter(lowPass, highPass));

		newSignal(derivative, 1, derivativeFilter(highPass));

		newSignal(squared, 30, squaredFilter(derivative));

		newSignal(mwi, 3, mwiFilter(squared));



		if((mwi[0]< mwi[1] && mwi[1] >= mwi[2]) ) {

			if(peakDetection(&qsr_params, mwi[1], i)) {
				i = 0;
				checkPeak(&qsr_params);

			}

		}

		i++;



	}



	return 0;
}


void newSignal(int arr[], int arr_Length, int signal) {

	for(int i = arr_Length; i > 0; i--) {
		arr[i] = arr[i-1];
	}
	arr[0] = signal;
}
