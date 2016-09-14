#include "filters.h"

// Feel free to change return statement and arguments
int lowPassFilter(int signal[], int lowPass[]) {


	int yn =2*lowPass[0] - lowPass[1] + (signal[0] - 2*signal[6] + signal[12])/32;

	return yn;

}

int highPassFilter(int lowPass[], int highPass[]) {

	int yn = highPass[0] - (lowPass[0]/32) + lowPass[16] - lowPass[17] + (lowPass[32]/32);

	return yn;
}
