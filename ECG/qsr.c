#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void peakDetection(QRS_params *params, int mwi[]) {

	params->RR_interval++;

	if(isPeak(params, mwi)) {
		printf("test");
		if(mwi[1] > params->THRESHOLD1){
			params->RR_interval *= 30;
			if(params->RR_LOW <= params->RR_interval && params->RR_interval <= params->RR_HIGH) {


				/*if(params->R_peakCount < params->R_peaksSize) {
					params->R_peaks[params->R_peakCount] =  mwi[1];

				} else {
					resize(params);
					params->peaks[params->peakCount] =  mwi[1];
				}
				*/
				params->R_peakCount++;
				params->R_peaks[params->R_peakCount];

				savePeak(params->recentRR_OK, 8 , mwi[1]);
				savePeak(params->recentRR, 8 ,mwi[1]);
				params->SPKF = 0.125*mwi[1] + 0.875*params->SPKF;
				params->RR_AVERAGE2 = averageOf(params->recentRR_OK, 8);
				params->RR_AVERAGE1 = averageOf(params->recentRR, 8);
				params->RR_LOW = 0.92 * params->RR_AVERAGE2;
				params->RR_HIGH = 1.16 * params->RR_AVERAGE2;
				params->RR_MISS = 1.66 * params->RR_AVERAGE2;
				params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
				params->THRESHOLD2 = params->THRESHOLD1*0.5;

			} else {

				if(params->RR_interval > params->RR_MISS) {
					int i = 1;
					while(params->peaks[params->peakCount-i] >! params->THRESHOLD2) {
						i++;
					}

					savePeak(params->recentRR, 8 ,mwi[1]);
					params->SPKF = 0.125*mwi[1] + 0.875*params->SPKF;
					params->RR_AVERAGE1 = averageOf(params->recentRR, 8);
					params->RR_LOW = 0.92 * params->RR_AVERAGE2;
					params->RR_HIGH = 1.16 * params->RR_AVERAGE2;
					params->RR_MISS = 1.66 * params->RR_AVERAGE2;
					params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
					params->THRESHOLD2 = params->THRESHOLD1*0.5;

				}

			}

		} else {

			params -> NPKF = params->NPKF*0.875 + 0.125*params->peaks[params->peakCount];
			params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
			params->THRESHOLD2 = 0.5*params->THRESHOLD1;

		}




	}


}

void setStandardParams(QRS_params *params) {


	params->SPKF = 4800;
	params->NPKF = 1000;
	params->THRESHOLD1 = 4500;
	params->THRESHOLD2 = 2000;
	params->RR_interval = 0;
	params->RR_AVERAGE1 = 0;
	params->RR_AVERAGE2 = 0;
	params->RR_LOW = 0;
	params->RR_HIGH = INT_MAX;
	params->RR_MISS = 0;

	params->peakCount = -1;
	params->peaksSize = 1;
	//params->peaks = (int *) malloc( params->peaksSize * sizeof(int));

	params->R_peakCount = -1;
	params->R_peaksSize = 1;
	//params->R_peaks = (int *) malloc( params->peaksSize * sizeof(int));

	for(int i = 0; i < 8; i++) {
		params->recentRR[i] = 0;
		params->recentRR_OK[i] = 0;
	}

	for(int i = 0; i< 500; i++) {
		params->peaks[i] = 0;
		params->R_peaks[i] = 0;

	}


}

int isPeak(QRS_params *params, int mwi[])  {

	if(mwi[0]< mwi[1] && mwi[1] > mwi[2]) {
		params->peakCount++;

		/*	if(params->peakCount < params->peaksSize) {
				params->peaks[params->peakCount] =  mwi[1];

			} else {
				resize(params);
				params->peaks[params->peakCount] =  mwi[1];
			}
		*/

		params->peaks[params->peakCount];
		return 1;

	} else {

		return 0;
	}

}

void resize(QRS_params *params) {
	params->peaksSize *= 2;
	params->peaks = realloc(params->peaks, params->peaksSize * sizeof(int));

}

void savePeak(int *arr, int arr_size, int peak) {

	for(int i = arr_size; i > 0; i--) {
			arr[i] = arr[i-1];
		}
		arr[0] = peak;

}

double averageOf(int *arr, int size) {
	int total;
	for(int i = 0; i<size; i++) {
		total += arr[i];
	}

	return (double)total/(double)size;
}
