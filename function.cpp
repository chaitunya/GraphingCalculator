#include "function.h"

Function::Function(std::function<double(double)> f, QPen pen)
    : mathFunc(f), pen(pen)
{
    pen.setCosmetic(true);
}

Function::Function(std::function<double(double)> f)
    : mathFunc(f)
{
    pen = QPen();
    pen.setWidth(2);
    pen.setCosmetic(true);
}

int Function::graphFunction(QPainter *painter, double xMin, double xMax, double deltaX)
{
    if (!isHidden) {
        painter->setPen(pen);
        QPainterPath path;
        path.moveTo(xMin, mathFunc(xMin));
        for (double x = xMin + deltaX; x < xMax; x += deltaX) {
            path.lineTo(x, mathFunc(x));
        }

        painter->setPen(pen);
        painter->drawPath(path);
        
        return 0;
    } else {
        return 1;
    }
}

double Function::evaluateFunction(double x)
{
    return mathFunc(x);
}
