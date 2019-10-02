#include "function.h"
#include "grapher.h"
#include <iostream>

#define getPxCoord(px, func, graph) graph.height() - ((func(px * (graph.xMax - graph.xMin) / graph.width() + graph.xMin) - graph.yMin) / (graph.yMax - graph.yMin) * graph.height())

Function::Function(mathfunc_t mathFunc)
    : mathFunc(mathFunc), is_parsed(false), is_valid(true)
{
    pen.setWidth(2);
}

Function::Function(mathfunc_t mathFunc, const QPen &pen)
    : pen(pen), mathFunc(mathFunc), is_parsed(false), is_valid(true)
{
}

Function::Function(const char* expr)
    : pen(), mpCtx(), mpExpr(), is_parsed(true)
{
    pen.setWidth(2);
    mpCtx.addBuiltIns();
    mpCtx.addVariable("x", 0); // add var to first position in container
    setText(expr);
}


Function::Function(const char* expr, const QPen &pen)
    : pen(pen), mpCtx(), mpExpr(), is_parsed(true)
{
    mpCtx.addBuiltIns();
    mpCtx.addVariable("x", 0); // add var to first position in container
    setText(expr);
}

void Function::setText(const char* expr)
{
    mathpresso::Error err = mpExpr.compile(mpCtx, expr, mathpresso::kNoOptions);
    if (err != mathpresso::kErrorOk) {
        // std::cerr << "Expression Error: " << err << std::endl;
        is_valid = false;
    } else {
        is_valid = true;
    }
}

double Function::derivative(double x)
{
    return (evaluateFunction(x + deltaX) - evaluateFunction(x)) / deltaX;
}

double Function::integral(double a, double b)
{
    double integral = 0;
    if (a < b) {
        for (double i = a; i < b; i += deltaX)
            integral += evaluateFunction(i) * deltaX;
    } else if (a > b) {
        for (double i = a; i > b; i -= deltaX)
            integral -= evaluateFunction(i) * deltaX;
    }
    return integral;
}

double Function::integral0(double x)
{
    return integral(0, x);
}

void Function::graphFunction(Grapher *grapher, QPainter *painter)
{
    painter->setPen(pen);
    QPainterPath path;
    path.moveTo(0, getPxCoord(0, evaluateFunction, (*grapher)));
    for (int p = 0; p < grapher->width(); p++)
        path.lineTo(p, getPxCoord(p, evaluateFunction, (*grapher)));
    painter->drawPath(path);
}

void Function::graphIntegral(Grapher *grapher, QPainter *painter)
{
    painter->setPen(pen);
    deltaX = (grapher->xMax - grapher->xMin) / grapher->width();
    QPainterPath path;
    path.moveTo(0, getPxCoord(0, integral0, (*grapher)));
    for (int p = 0; p < grapher->width(); p++)
        path.lineTo(p, getPxCoord(p, integral0, (*grapher)));
    painter->drawPath(path);
}

void Function::graphDerivative(Grapher *grapher, QPainter *painter)
{
    painter->setPen(pen);
    deltaX = (grapher->xMax - grapher->xMin) / grapher->width();
    QPainterPath path;
    path.moveTo(0, getPxCoord(0, derivative, (*grapher)));
    for (int p = 0; p < grapher->width(); p++)
        path.lineTo(p, getPxCoord(p, derivative, (*grapher)));
    painter->drawPath(path);
}


double Function::evaluateFunction(double x)
{
    if (is_parsed)
        return mpExpr.evaluate(&x);
    else
        return mathFunc(x);
}

bool Function::getValid() const
{
    return is_valid;
}
