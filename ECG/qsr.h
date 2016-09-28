#ifndef QSR_H
#define QSR_H


typedef struct QRS_params
{ // Structure for QRS parameters
   double SPKF;
   double NPKF; 
   double THRESHOLD1;
   double THRESHOLD2;
   double RR_AVERAGE1;
   double RR_AVERAGE2;
   double RR_LOW;
   double RR_HIGH;
   double RR_MISS;

   int peakCount;
   int peaksSize;
   int peaks[10000];
   int peakIntervals[10000];
   int realTime;

   int R_peakCount;
   int R_peaksSize;
   int R_peaks[10000];

   int recentRR_OK[8];
   int recentRR[8];

   int missedCount;


   
} QRS_params;

int peakDetection(QRS_params *params, int peak, int RR_interval);

void setStandardParams(QRS_params *params);

void resize(QRS_params *params);

void savePeak(int *arr, int arr_size, int RR);

double averageOf(int *arr, int size);

void printR_Peaks(QRS_params *params);

void checkPeak(QRS_params *params);
void resize(QRS_params *params);

#endif // QSR_H
