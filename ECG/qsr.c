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
			params->realTime += RR_interval;
			params->missedCount = 0;
			return 1;

		} else if(RR_interval > (params-> RR_MISS)){


			int i = 1;
			while(params->peaks[params->peakCount-i] >! params->THRESHOLD2 && i < params->peakCount) {
				i++;
			}

			params->R_peakCount++;
			params->R_peaks[params->R_peakCount] = params->peaks[params->peakCount];

			savePeak(params->recentRR, 8 , RR_interval-i);
			params->SPKF = 0.25*params->peaks[params->peakCount-i] + 0.75*params->SPKF;
			params->RR_AVERAGE1 = averageOf(params->recentRR, 8);
			params->RR_LOW = 0.92 * params->RR_AVERAGE2;
			params->RR_HIGH = 1.16 * params->RR_AVERAGE2;
			params->RR_MISS = 1.66 * params->RR_AVERAGE2;
			params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
			params->THRESHOLD2 = params->THRESHOLD1*0.5;
			params->realTime += RR_interval;
			params->missedCount++;

			if(params->missedCount == 5) {
				printf("Warning! Last five successive RR-intervals were missed!\n");
				params->missedCount = 0;
			}

			return 1;

		}
	} else {
		params -> NPKF = params->NPKF*0.875 + 0.125*params->peaks[params->peakCount];
		params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
		params->THRESHOLD2 = 0.5*params->THRESHOLD1;
		params->missedCount = 0;

	}





	return 0;
}

void setStandardParams(QRS_params *params) {


	params->SPKF = 4800;
	params->NPKF = 1000;
	params->THRESHOLD1 = 4500;
	params->THRESHOLD2 = 2250;
	params->RR_AVERAGE1 = 0;
	params->RR_AVERAGE2 = 0;
	params->RR_LOW = 0;
	params->RR_HIGH = INT_MAX;
	params->RR_MISS = 0;
	params->realTime = 0;
	params->peakCount = -1;
	params->peaksSize = 1;
	//params->peaks = (int *) malloc( params->peaksSize * sizeof(int));

	params->R_peakCount = -1;
	params->R_peaksSize = 1;

	params->missedCount = 0;
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

void savePeak(int *arr, int arr_size, int RR) {

	for(int i = arr_size; i > 0; i--) {
			arr[i] = arr[i-1];
		}
		arr[0] = RR;

}

double averageOf(int *arr, int size) {
	int total = 0;
	int k = 0;
	for(int i = 0; i<size; i++) {
		total += arr[i];
		if(arr[i] != 0)  {
			k++;
		}
	}

	return (double)total/(double)k;
}

void printR_Peaks(QRS_params *params) {

	for(int i = 0; i<100; i++) {
		printf("%d. \t %d\n", i+1, params->R_peaks[i]);
	}

}

void checkPeak(QRS_params *params) {
	double pulse = 3600.0 / (params->RR_AVERAGE1 * ( 60.0 / 250.0));
	double realTime_seconds = ((float)params->realTime / 250.0)*60.0;
	int miliseconds = (int)realTime_seconds%60;
	int seconds = realTime_seconds/60;
	int minutes = seconds/60;

	if(seconds < 10) {
		printf("R-peak detected with value of: %d, at time: %d:0%d", params->R_peaks[params->R_peakCount],minutes, seconds);
	} else {
		printf("R-peak detected with value of: %d, at time(m:s:ms): %d:%d", params->R_peaks[params->R_peakCount],minutes, seconds);
	}

	if(miliseconds <10) {
		printf(":0%d, and pulse: %f bpm\n", miliseconds, pulse);
	} else {
		printf(":%d, and pulse: %f bpm\n", miliseconds, pulse);
	}

	if(params->R_peaks[params->R_peakCount] < 2000) {
		printf("Warning! R-peakvalue is to low: %d\n", params->R_peaks[params->R_peakCount] );
	}

}
