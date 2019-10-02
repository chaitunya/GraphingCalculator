#ifndef FUNCTION_H
#define FUNCTION_H

#include <QPainter>
#include <QColor>
#include <functional>
#include <QPen>
#include <mathpresso/mathpresso.h>


typedef double (*mathfunc_t)(double x);

class Grapher;

class Function {
public:
    Function(mathfunc_t mathFunc);
    Function(mathfunc_t mathFunc, const QPen &pen);
    Function(const char* expr);
    Function(const char* expr, const QPen &pen);
    void setText(const char* expr);
    double derivative(double x);
    double integral(double a, double b);
    void graphFunction(Grapher *grapher, QPainter *painter);
    void graphIntegral(Grapher *grapher, QPainter *painter);
    void graphDerivative(Grapher *grapher, QPainter *painter);
    void setPen(const QPen &pen);
    bool getValid() const;
    double evaluateFunction(double x);
    bool isHidden = false;
    bool b_graphDerivative = false;
    bool b_graphIntegral = false;

private:
    QPen pen;
    mathpresso::Context mpCtx;
    mathpresso::Expression mpExpr;
    mathfunc_t mathFunc;
    double integral0(double x);
    double deltaX = 0.001;
    bool is_parsed;
    bool is_valid;
    
};

#endif
