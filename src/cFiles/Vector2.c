#include <stdio.h>
#include <math.h>
#include "/home/oskar/projects/molecularDynamics/include/Vector2.h"

Vector2 newVector2(double x, double y){
    Vector2 v = {x,y};
    return v;
}

void printVector(Vector2 vec){
    printf("(%lf, %lf)\n", vec.x, vec.y);
}

Vector2 add(Vector2 a, Vector2 b){
    Vector2 v = {a.x + b.x, a.y + b.y};
    return v;
}

Vector2 sub(Vector2 a, Vector2 b){
    Vector2 v = {a.x - b.x, a.y - b.y};
    return v;
}

Vector2 mul(Vector2 v, double a){
    Vector2 result;
    result.x = a * v.x;
    result.y = a * v.y;
    return result;
}

double dot(Vector2 a, Vector2 b){
    return a.x * b.x + a.y * b.y;
}

double mag(Vector2 a){
    double result = a.x * a.x + a.y * a.y;
    return sqrt(result);
}

double mag2(Vector2 a){
    double result = a.x * a.x + a.y * a.y;
    return result;
}



