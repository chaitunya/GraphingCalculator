#ifndef FUNCTION_H
#define FUNCTION_H

#include <QPainter>
#include <QColor>
#include <functional>
#include <QPen>

typedef std::function<double(double)> mathfunc_t;

class Function {
public:
    Function(mathfunc_t f);
    Function(mathfunc_t f, QPen pen);
    int graphFunction(QPainter *painter, double xMin, double xMax, double deltaX);
    double evaluateFunction(double x);

private:
    mathfunc_t mathFunc;
    QPen pen;
    bool isHidden = false;
};

#endif
