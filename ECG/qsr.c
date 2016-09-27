#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int peakDetection(QRS_params *params, int peak, int RR_interval) {


	params->peakCount++;

	params->peaks[params->peakCount] = peak;

	if(peak > params->THRESHOLD1){
		if(params->RR_LOW <= RR_interval && RR_interval <= params->RR_HIGH) {


			/*if(params->R_peakCount < params->R_peaksSize) {
				params->R_peaks[params->R_peakCount] =  mwi[1];

			} else {
				resize(params);
				params->peaks[params->peakCount] =  mwi[1];
			}
			*/
			params->R_peakCount++;
			params->R_peaks[params->R_peakCount] = params->peaks[params->peakCount];

			savePeak(params->recentRR_OK, 8 , RR_interval);
			savePeak(params->recentRR, 8 ,RR_interval);
			params->SPKF = 0.125*params->peaks[params->peakCount] + 0.875*params->SPKF;
			params->RR_AVERAGE2 = averageOf(params->recentRR_OK, 8);
			params->RR_AVERAGE1 = averageOf(params->recentRR, 8);
			params->RR_LOW = 0.92 * params->RR_AVERAGE2;
			params->RR_HIGH = 1.16 * params->RR_AVERAGE2;
			params->RR_MISS = 1.66 * params->RR_AVERAGE2;
			params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
			params->THRESHOLD2 = params->THRESHOLD1*0.5;
			return 1;

		} else {

			if(RR_interval > params->RR_MISS) {
				int i = 1;
				while(params->peaks[params->peakCount-i] >! params->THRESHOLD2) {
					i++;
				}

				savePeak(params->recentRR, 8 , params->peaks[params->peakCount]);
				params->SPKF = 0.125*params->peaks[params->peakCount] + 0.875*params->SPKF;
				params->RR_AVERAGE1 = averageOf(params->recentRR, 8);
				params->RR_LOW = 0.92 * params->RR_AVERAGE2;
				params->RR_HIGH = 1.16 * params->RR_AVERAGE2;
				params->RR_MISS = 1.66 * params->RR_AVERAGE2;
				params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
				params->THRESHOLD2 = params->THRESHOLD1*0.5;
				return 1;


			}


		}

	} else {

		params -> NPKF = params->NPKF*0.875 + 0.125*params->peaks[params->peakCount];
		params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
		params->THRESHOLD2 = 0.5*params->THRESHOLD1;

	}

	return 0;
}

void setStandardParams(QRS_params *params) {


	params->SPKF = 4800;
	params->NPKF = 1000;
	params->THRESHOLD1 = 4500;
	params->THRESHOLD2 = 2000;
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

void savePeak(int *arr, int arr_size, int peak) {

	for(int i = arr_size; i > 0; i--) {
			arr[i] = arr[i-1];
		}
		arr[0] = peak;

}

double averageOf(int *arr, int size) {
	int total = 0;
	for(int i = 0; i<size; i++) {
		total += arr[i];
	}

	return (double)total/(double)size;
}
