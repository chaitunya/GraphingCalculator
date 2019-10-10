#include "function.h"
#include "grapher.h"
#include <iostream>
#include <limits>
#include <cmath>

#define MIN_DIFF 0.0001

Function::Function(mathfunc_t mathFunc)
    : color(0, 0, 0), mathFunc(mathFunc), is_parsed(false), is_valid(true)
{
}

Function::Function(mathfunc_t mathFunc, const QColor &color)
    : color(color), mathFunc(mathFunc), is_parsed(false), is_valid(true)
{
}

Function::Function(const char* expr)
    : color(0, 0, 0), mpCtx(), mpExpr(), is_parsed(true)
{
    mpCtx.addBuiltIns();
    mpCtx.addVariable("x", 0); // add var to first position in container
    setText(expr);
}


Function::Function(const char* expr, const QColor &color)
    : color(color), mpCtx(), mpExpr(), is_parsed(true)
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

void Function::graphFunction(double(Function::*func)(double), Grapher *grapher, QPainter *painter)
{
    QPen new_pen = painter->pen();
    new_pen.setColor(color);
    painter->setPen(new_pen);
    QPainterPath path;
    double coord_x;
    double coord_y;
    double px_y;
    bool is_nan = true;
    for (int px_x = 0; px_x < grapher->width(); px_x++) {
        coord_x = px_x * (grapher->xMax - grapher->xMin) / grapher->width() + grapher->xMin;
        coord_y = (this->*func)(coord_x);
        if (std::isnan(coord_y)) {
            is_nan = true;
            continue;
        } else {
            px_y = grapher->height() - ((coord_y - grapher->yMin) / (grapher->yMax - grapher->yMin) * grapher->height());
        }
        if (is_nan) {
            path.moveTo(px_x, px_y);
            std::cout << "skipping pt " << px_x << std::endl;
        }
        else
            path.lineTo(px_x, px_y);
        is_nan = false;
    }
    painter->drawPath(path);
}
void Function::graphFunction(Grapher *grapher, QPainter *painter)
{
    graphFunction(&Function::evaluateFunction, grapher, painter);
}

void Function::graphIntegral(Grapher *grapher, QPainter *painter)
{
    graphFunction(&Function::integral0, grapher, painter);
}

void Function::graphDerivative(Grapher *grapher, QPainter *painter)
{
    graphFunction(&Function::derivative, grapher, painter);
}


double Function::evaluateFunction(double x)
{
    if (is_parsed) {
        return mpExpr.evaluate(&x);
    } else {
        return mathFunc(x);
    }
}

bool Function::getValid() const
{
    return is_valid;
}

double Function::brent(double(Function::*func)(double), double xMin, double xMax, double min_diff) {

    double f_xMin = (this->*func)(xMin);
    double f_xMax = (this->*func)(xMax);
    if ((f_xMin > 0 && f_xMax > 0) || (f_xMin < 0 && f_xMax < 0))
        return std::nan(""); // return invalid number
    double a;
    double b;
    if (abs(f_xMin) > abs(f_xMax)) {
        a = xMin;
        b = xMax;
    } else {
        a = xMax;
        b = xMin;
    }

    double c = a;
    double s;

    bool mflag = true;
    double d = c;


    do {
        if ((this->*func)(a) != (this->*func)(c) && (this->*func)(b) != (this->*func)(c))
            s = (a * (this->*func)(b) * (this->*func)(c)) / (((this->*func)(a) - (this->*func)(b)) * ((this->*func)(a) - (this->*func)(c)))
                + (b * (this->*func)(a) * (this->*func)(c)) / (((this->*func)(b) - (this->*func)(a)) * ((this->*func)(b) - (this->*func)(c)))
                + (c * (this->*func)(a) * (this->*func)(b)) / (((this->*func)(c) - (this->*func)(a)) * ((this->*func)(c) - (this->*func)(b)));
        else
            s = b - (this->*func)(b) * (b - a) / ((this->*func)(b) - (this->*func)(a));
        if (!((3 * a + b) / 4 < s && (s < b)) ||
                (mflag && abs(s - b) >= abs(b - c) / 2) ||
                (!mflag && abs(s - b) <= abs(c - d) / 2) ||
                (mflag && abs(b - c) < min_diff) ||
                (!mflag && abs(c - d) < min_diff)) {
            s = (a + b) / 2;
            mflag = true;
        } else {
            mflag = false;
        }
        d = c;
        c = b;
        if ((this->*func)(a) * (this->*func)(s) < 0)
            b = s;
        else
            a = s;
        if (abs((this->*func)(a)) < abs((this->*func)(b))) {
            double z = a;
            a = b;
            b = z;
        }

    } while (((this->*func)(b) != 0 && (this->*func)(s) != 0) && abs(b - a) > min_diff);

    if ((this->*func)(s) == 0)
        return s;
    else
        return b;
}

std::vector<QPointF> Function::calculateZeros(double xMin, double xMax, double deltaX)
{
    // uses Brent's method to calculate zeros: https://en.wikipedia.org/wiki/Brent%27s_method
    std::vector<QPointF> zeros;
    double f_xLeft; 
    double f_xRight;
    double xLeft; 
    double xRight;
    QPointF zero;
    for (double x = xMin; x < xMax; x += deltaX)
    {
        xLeft = x;
        xRight = x + deltaX;
        f_xLeft = evaluateFunction(xLeft);
        f_xRight = evaluateFunction(xRight);
        if ((f_xLeft > 0 && f_xRight > 0) ||
                (f_xLeft < 0 && f_xRight < 0)) {
            continue;
        } else {
            zero = QPointF(0, 0);
            zero.setX(brent(&Function::evaluateFunction, xLeft, xRight, MIN_DIFF));
            zeros.push_back(zero);
        }
    }

    std::vector<QPointF> rel_extrema = calculateRelExtrema(xMin, xMax, deltaX);
    for (QPointF extremum : rel_extrema) {
        if (abs(extremum.y()) <= MIN_DIFF)
            zeros.push_back(extremum);
    }

    return zeros;
}

std::vector<QPointF> Function::calculateRelMaxs(double xMin, double xMax, double deltaX)
{
    std::vector<QPointF> maxs;
    double f_xLeft; 
    double f_xRight;
    double xLeft; 
    double xRight;
    QPointF max;
    for (double x = xMin; x < xMax; x += deltaX)
    {
        xLeft = x;
        xRight = x + deltaX;
        f_xLeft = derivative(xLeft);
        f_xRight = derivative(xRight);
        if ((f_xLeft < 0 && f_xRight > 0) ||
                (f_xLeft > 0 && f_xRight > 0)
                || (f_xLeft < 0 && f_xRight < 0)) {
            continue;
        } else {
            max = QPoint();
            max.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
            max.setY(evaluateFunction(max.x()));
            maxs.push_back(max);
        }
    }
    return maxs;
}

std::vector<QPointF> Function::calculateRelMins(double xMin, double xMax, double deltaX)
{
    std::vector<QPointF> mins;
    double f_xLeft; 
    double f_xRight;
    double xLeft; 
    double xRight;
    QPointF min;
    for (double x = xMin; x < xMax; x += deltaX)
    {
        xLeft = x;
        xRight = x + deltaX;
        f_xLeft = derivative(xLeft);
        f_xRight = derivative(xRight);
        if ((f_xLeft > 0 && f_xRight < 0) ||
                (f_xLeft > 0 && f_xRight > 0) ||
                (f_xLeft < 0 && f_xRight < 0)) {
            continue;
        } else {
            min = QPoint();
            min.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
            min.setY(evaluateFunction(min.x()));
            mins.push_back(min);
        }
    }
    return mins;
}

std::vector<QPointF> Function::calculateRelExtrema(double xMin, double xMax, double deltaX) {
    std::vector<QPointF> extrema;
    double f_xLeft; 
    double f_xRight;
    double xLeft; 
    double xRight;
    QPointF extremum;
    for (double x = xMin; x < xMax; x += deltaX)
    {
        xLeft = x;
        xRight = x + deltaX;
        f_xLeft = derivative(xLeft);
        f_xRight = derivative(xRight);
        if ((f_xLeft > 0 && f_xRight > 0) ||
                (f_xLeft < 0 && f_xRight < 0)) {
            continue;
        } else {
            extremum = QPoint();
            extremum.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
            extremum.setY(evaluateFunction(extremum.x()));
            extrema.push_back(extremum);
        }
    }
    return extrema;
}
