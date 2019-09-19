#include "function.h"

Function::Function(mathfunc_t mathFunc, QPen pen)
    : mathFunc(mathFunc), pen(pen)
{
}

double Function::derivative(double x, double dx)
{
    return (mathFunc(x + dx) - mathFunc(x)) / dx;
}

double Function::integral(double a, double b, double dx)
{
    double integral = 0;
    for (double i = a; i < b; i += dx) {
        integral += i * dx;
    }
    return integral;
}
