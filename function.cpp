#include "function.h"

Function::Function(mathfunc_t mathFunc)
    : mathFunc(mathFunc), pen()
{
    pen.setWidth(2);
}

Function::Function(mathfunc_t mathFunc, QPen pen)
    : mathFunc(mathFunc), pen(pen)
{
}

double Function::derivative(double x)
{
    return (mathFunc(x + deltaX) - mathFunc(x)) / deltaX;
}

double Function::integral(double a, double b)
{
    double integral = 0;
    if (a < b) {
        for (double i = a; i < b; i += deltaX) {
            integral += i * deltaX;
        }
    } else if (a > b) {
        for (double i = a; i > b; i -= deltaX) {
            integral += i * deltaX;
        }
    }
    return integral;
}

double Function::integral0(double x)
{
    return integral(0, x);
}
