#ifndef FUNCTION_H
#define FUNCTION_H

#include <QPainter>
#include <QColor>
#include <functional>
#include <QPen>


typedef std::function<double(double)> mathfunc_t;

class Grapher;

class Function {
public:
    Function(mathfunc_t mathFunc);
    Function(mathfunc_t mathFunc, QPen pen);
    double derivative(double x);
    double integral(double a, double b);
    double integral0(double x);
    void graphFunction(Grapher *grapher, QPainter &painter);
    void graphIntegral(Grapher *grapher, QPainter &painter);
    void graphDerivative(Grapher *grapher, QPainter &painter);
    mathfunc_t mathFunc;
    QPen pen;
    bool isHidden = false;
    bool graph_derivative = false;
    bool graph_integral = false;
    double deltaX = 0.001;
};

#endif
