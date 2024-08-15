#ifndef VECTOR_2
#define VECTOR_2

typedef struct{
    double x;
    double y;
} Vector2;

Vector2 newVector2(double x, double y);

void printVector(Vector2 vec);

Vector2 add(Vector2 a, Vector2 b);

Vector2 sub(Vector2 a, Vector2 b);

Vector2 mul(Vector2 v, double a);

double dot(Vector2 a, Vector2 b);

double mag(Vector2 a);
double mag2(Vector2 a);

#endif