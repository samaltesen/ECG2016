#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{ // Structure for QRS parameters
   double SPKF;
   double NPKF; 
   double THRESHOLD1;
   double THRESHOLD2;
   int RR_interval;
   double RR_AVERAGE1;
   double RR_AVERAGE2;
   double RR_LOW;
   double RR_HIGH;
   double RR_MISS;

   int peakCount;
   int peaksSize;
   int peaks[500];

   int R_peakCount;
   int R_peaksSize;
   int R_peaks[500];

   int recentRR_OK[8];
   int recentRR[8];


   // Add parameters that are missing
   // or of use to you
   
} QRS_params;

// Feel free to change and add methods
void peakDetection(QRS_params *params, int peak);

void setStandardParams(QRS_params *params);

//int isPeak(QRS_params *params, int mwi[]);

void resize(QRS_params *params);

void savePeak(int *arr, int arr_size, int peak);

double averageOf(int *arr, int size);

#endif // QSR_H
