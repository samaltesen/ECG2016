#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>



// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.
// Defining the stack structure

void newSignal(int arr[], int arr_Length, int signal) {

	for(int i = arr_Length; i > 0; i--) {
		arr[i] = arr[i-1];
	}
	arr[0] = signal;
}


int main()
{	
    QRS_params qsr_params;       // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");

	FILE *lp;
	lp = fopen("lp_ECG.txt", "w");

	FILE *hp;
	hp = fopen("hp_ECG.txt", "w");

	FILE *der;
	der = fopen("deri_ECG.txt", "w");

	FILE *squ;
	squ = fopen("squ_ECG.txt", "w");

	FILE *mw;
	mw = fopen("mwi_ECG.txt", "w");


	int signal_Length = 13;
	int signal[13] = { 0 };

	int lowPass_Length = 33;
	int lowPass[33] = { 0 };

	int highPass_Length = 5;
	int highPass[5] = { 0 };

	int derivative_Length =  1;
	int derivative[1] = {0};

	int squared_Length = 30;
	int squared[30] = {0};

	int mwi_Length = 3;
	int mwi[3] = {0};

	int k = 33;
	while(!feof (file) && k > 0) {

		newSignal(signal, signal_Length, getNextData(file));
		newSignal(lowPass, lowPass_Length, lowPassFilter(signal, lowPass));
		newSignal(highPass, highPass_Length, highPassFilter(lowPass, highPass));
		newSignal(derivative, derivative_Length, derivativeFilter(highPass));
		newSignal(squared, squared_Length, squaredFilter(derivative));
		newSignal(mwi, mwi_Length, mwiFilter(squared));

		fprintf(lp, "%d\n", lowPass[0]);
		fprintf(hp, "%d\n", highPass[0]);
		fprintf(der, "%d\n", derivative[0]);
		fprintf(squ, "%d\n", squared[0]);
		fprintf(mw, "%d\n", mwi[0]);

	}


                                
    peakDetection(&qsr_params); // Perform Peak Detection


	return 0;
}
