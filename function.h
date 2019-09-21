#ifndef FUNCTION_H
#define FUNCTION_H

#include <QPainter>
#include <QColor>
#include <functional>
#include <QPen>


typedef std::function<double(double)> mathfunc_t;

class Function {
public:
    Function(mathfunc_t mathFunc);
    Function(mathfunc_t mathFunc, QPen pen);
    double derivative(double x, double dx);
    double integral(double a, double b, double dx);
    mathfunc_t mathFunc;
    QPen pen;
    bool isHidden = false;
};

#endif
