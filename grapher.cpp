#include "grapher.h"
#include <iostream>
#include <cmath>

#include <QPainter>

#define getPxCoord(px, Func) height() - (\
                                 (Func.mathFunc(px * (xMax - xMin) / width() + xMin) - yMin)\
                                 / (yMax - yMin)\
                                 * height()\
                             )


Grapher::Grapher(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize Grapher::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize Grapher::sizeHint() const
{
    return QSize(400, 400);
}

void Grapher::paintEvent(QPaintEvent *)
{
    // setup QPainter
    QPainter painter(this);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // graph axes
    
    // graph rules
    
    // graph functions
    for (Function f : functions) {
        painter.setPen(f.pen);
        QPainterPath path;
        path.moveTo(0, getPxCoord(0, f));
        for (int p = 0; p < width(); p++) {
            path.lineTo(p, getPxCoord(p, f));
        }
        painter.drawPath(path);
    }
    
    // clean up
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void Grapher::addFunction(const Function &f, int index)
{
    if (index == -1)
        functions.push_back(f);
    else
        functions.insert(functions.begin() + index, f);
}

void Grapher::wheelEvent(QWheelEvent * /*event*/)
{
}

void Grapher::resizeEvent(QResizeEvent * event)
{
    /* if (!initial_resize) { */
    /*     xMax = event->size().width() * (xMax - xMin) / event->oldSize().width() + xMin; */
    /*     yMax = event->size().height() * (yMax - yMin) / event->oldSize().height() + xMin; */
    /* } else { */
    /*     initial_resize = false; */
    /* } */
    QWidget::resizeEvent(event);
}
