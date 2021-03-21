#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdio.h>
using namespace std;

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

int main(){
    int counter;
    double integral;
    double a = 0.0;
    double b = 1.0;
    double h = 0.01;

    double n;

    n = (b - a) / h;

    integral = h * (f(a) + f(b)) / 6.0;

    double alt_integral = 0.0;

    #pragma omp parallel for shared(a,b,h,n) reduction(+:alt_integral)
        for (int counter = 1; counter<= (int)n; ++counter){
            alt_integral += h * f(a + h * counter);
        }

        integral += alt_integral;
        cout << "Integral = "<<integral<<"\n";


    return 0;
}