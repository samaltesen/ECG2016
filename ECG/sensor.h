#ifndef SENSOR_H
#define SENSOR_H
#include <stdio.h>
#include <stdlib.h>




FILE* openfile(const char* filename);

int getNextData(FILE *file);


#endif
