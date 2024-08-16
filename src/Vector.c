#include <stdio.h>
#include <math.h>
#include "../include/Vector.h"

Vector newVector(double x, double y){
    Vector v = {x,y};
    return v;
}

void printVector(Vector vec){
    printf("(%lf, %lf)\n", vec.x, vec.y);
}

Vector add(Vector a, Vector b){
    Vector v = {a.x + b.x, a.y + b.y};
    return v;
}

Vector sub(Vector a, Vector b){
    Vector v = {a.x - b.x, a.y - b.y};
    return v;
}

Vector mul(Vector v, double a){
    Vector result;
    result.x = a * v.x;
    result.y = a * v.y;
    return result;
}

double dot(Vector a, Vector b){
    return a.x * b.x + a.y * b.y;
}

double mag(Vector a){
    double result = a.x * a.x + a.y * a.y;
    return sqrt(result);
}

double mag2(Vector a){
    double result = a.x * a.x + a.y * a.y;
    return result;
}



