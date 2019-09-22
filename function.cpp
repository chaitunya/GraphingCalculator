#include "function.h"
#include "grapher.h"

#define getPxCoord(px, func, graph) graph.height() - (\
                                        (func(px * (graph.xMax - graph.xMin) / graph.width() + graph.xMin) - graph.yMin)\
                                        / (graph.yMax - graph.yMin)\
                                        * graph.height()\
                                    )

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

void Function::graphFunction(Grapher *grapher, QPainter &painter)
{
    painter.setPen(pen);
    QPainterPath path;
    path.moveTo(0, getPxCoord(0, mathFunc, (*grapher)));
    for (int p = 0; p < grapher->width(); p++) {
        path.lineTo(p, getPxCoord(p, mathFunc, (*grapher)));
    }
    painter.drawPath(path);
}
void Function::graphIntegral(Grapher *grapher, QPainter &painter)
{
    painter.setPen(pen);
    deltaX = (grapher->xMax - grapher->xMin) / grapher->width();
    QPainterPath path;
    path.moveTo(0, getPxCoord(0, mathFunc, (*grapher)));
    for (int p = 0; p < grapher->width(); p++) {
        path.lineTo(p, getPxCoord(p, integral0, (*grapher)));
    }
    painter.drawPath(path);
}
void Function::graphDerivative(Grapher *grapher, QPainter &painter)
{
    painter.setPen(pen);
    deltaX = (grapher->xMax - grapher->xMin) / grapher->width();
    QPainterPath path;
    path.moveTo(0, getPxCoord(0, mathFunc, (*grapher)));
    for (int p = 0; p < grapher->width(); p++) {
        path.lineTo(p, getPxCoord(p, derivative, (*grapher)));
    }
    painter.drawPath(path);
}
