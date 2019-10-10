#ifndef FUNCTION_H
#define FUNCTION_H

#include <QPainter>
#include <QColor>
#include <functional>
#include <QColor>
#include <mathpresso/mathpresso.h>


typedef double(*mathfunc_t)(double);

class Grapher;

class Function {
public:
    Function(mathfunc_t mathFunc);
    Function(mathfunc_t mathFunc, const QColor &color);
    Function(const char* expr);
    Function(const char* expr, const QColor &color);
    void setText(const char* expr);
    double derivative(double x);
    double integral(double a, double b);
    void graphFunction(Grapher *grapher, QPainter *painter);
    void graphIntegral(Grapher *grapher, QPainter *painter);
    void graphDerivative(Grapher *grapher, QPainter *painter);
    void setColor(const QColor &color);
    bool getValid() const;
    double evaluateFunction(double x);
    bool isHidden = false;
    bool b_graphDerivative = false;
    bool b_graphIntegral = false;
    std::vector<QPointF> calculateZeros(double xMin, double xMax, double deltaX);
    std::vector<QPointF> calculateRelMins(double xMin, double xMax, double deltaX);
    std::vector<QPointF> calculateRelMaxs(double xMin, double xMax, double deltaX);
    std::vector<QPointF> calculateRelExtrema(double xMin, double xMax, double deltaX);
    std::vector<QPointF> calculateAsymptotes(double xMin, double xMax, double deltaX);

private:
    void graphFunction(double(Function::*func)(double), Grapher *grapher, QPainter *painter);
    double brent(double(Function::*func)(double), double xMin, double xMax, double min_diff);
    QColor color;
    mathpresso::Context mpCtx;
    mathpresso::Expression mpExpr;
    mathfunc_t mathFunc;
    double integral0(double x);
    double deltaX = 0.0001;
    bool is_parsed;
    bool is_valid;
    
};

#endif
