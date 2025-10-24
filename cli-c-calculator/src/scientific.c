#include "scientific.h"
#include <math.h>

float power(float base, float exponent) { return pow(base, exponent); }
float square_root(float x) { return sqrt(x); }
int factorial(int n) {
    if (n < 0) return -1;
    int res = 1;
    for(int i=1;i<=n;i++) res*=i;
    return res;
}
float sine(float angle) { return sin(angle); }
float cosine(float angle) { return cos(angle); }
float tangent(float angle) { return tan(angle); }
float logarithm(float x) { return log(x); }
