

typedef struct {
    int nBins, totalData;
    int* counts;
    double start, end;
} Histogram;

Histogram newHistogram(int nBins, double start, double end);

void addData(Histogram* hist, double data);

void freeHistogram(Histogram* hist);

void printHistogram(Histogram* hist);

void clearHistogram(Histogram* hist);