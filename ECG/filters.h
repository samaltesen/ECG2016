#ifndef FILTERS_H
#define FILTERs_H


int lowPassFilter(int signal[], int lp[]);

int highPassFilter(int lowPass[], int highPass[]);

int derivativeFilter(int highPass[]);

int squaredFilter(int derivative[]);

int mwiFilter(int squared[]);

#endif
