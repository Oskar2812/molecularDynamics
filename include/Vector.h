#ifndef VECTOR
#define VECTOR

typedef struct{
    double x;
    double y;
} Vector;

Vector newVector(double x, double y);

void printVector(Vector vec);

Vector add(Vector a, Vector b);

Vector sub(Vector a, Vector b);

Vector mul(Vector v, double a);

double dot(Vector a, Vector b);

double mag(Vector a);
double mag2(Vector a);

#endif