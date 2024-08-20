#include "../include/Histogram.h"
#include <stdio.h>
#include <stdlib.h>

Histogram newHistogram(int nBins, double start, double end){
    Histogram result;

    result.nBins = nBins;
    result.start = start;
    result.end = end;
    result.totalData = 0;

    result.counts = (int*)malloc(sizeof(int) * nBins);
    if(result.counts == NULL){
        printf("Error allocating memory for histogram\n");
        exit(1);
    }

    for(int ii = 0; ii < nBins; ii++){
        result.counts[ii] = 0;
    }

    return result;
}

void freeHistogram(Histogram* hist){
    free(hist->counts);
}

void addData(Histogram* hist, double data){
    double inc = abs(hist->end - hist->start) / (double)hist->nBins;

    for(int ii = 0; ii < hist->nBins; ii++){
        if(hist->start + ii * inc < data && data <= hist->start + (ii + 1) * inc){
            hist->counts[ii] += 1;
            hist->totalData += 1;
            break;
        }
    }
}

void clearHistogram(Histogram* hist){
    for(int ii = 0; ii < hist->nBins; ii++){
        hist->counts[ii] = 0;
    }
}

void printHistogram(Histogram* hist){
    double inc = (hist->end - hist->start) / hist->nBins;
    printf("Histogram range is: %.2lf to %.2lf with %d datapoints\n", hist->start, hist->end, hist->totalData);
    for(int ii = 0; ii < hist->nBins; ii++){
        printf("Bin %d with range %.2lf - %.2lf has count: %d\n", ii, hist->start + ii*inc, hist->start + (ii+1)*inc, hist->counts[ii]);
    }
}