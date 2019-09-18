#ifndef FUNCTION_H
#define FUNCTION_H

#include <QPainter>
#include <QColor>
#include <functional>
#include <QPen>


typedef std::function<double(double)> mathfunc_t;

struct Function {
    mathfunc_t mathFunc;
    QPen pen = QPen();
    bool isHidden = false;
};

#endif
