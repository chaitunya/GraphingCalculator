#include "grapher.h"
#include <iostream>
#include <cmath>

#include <QPainter>
#include <QInputEvent>

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
    
    QPen ax_pen;
    ax_pen.setWidth(2);
    
    if (xMin < 0 && 0 < xMax) {
        painter.setPen(ax_pen);
        int p = -xMin / (xMax - xMin) * width();
        painter.drawLine(p, 0, p, height());
    }

    if (yMin < 0 && 0 < yMax) {
        painter.setPen(ax_pen);
        int p = height() - (-yMin / (yMax - yMin) * height());
        painter.drawLine(0, p, width(), p);
    }
    
    // graph rules

    // minor rules

    QPen minorRulePen;
    minorRulePen.setWidth(1);
    minorRulePen.setColor(QColor(200, 200, 200));

    painter.setPen(minorRulePen);

    double minorRulesPerInt = 2;
    
    for (double x = std::floor(xMin); x < std::ceil(xMax); x += 1 / minorRulesPerInt) {
        if (xMin < x && x < xMax) {
            int p = (x - xMin) / (xMax - xMin) * width();
            painter.drawLine(p, 0, p, height());
        }
    }

    for (double y = std::floor(yMin); y < std::ceil(yMax); y += 1 / minorRulesPerInt) {
        if (yMin < y && y < yMax) {
            int p = height() - ((y - yMin) / (yMax - yMin) * height());
            painter.drawLine(0, p, width(), p);
        }
    }

    QPen majorRulePen;
    majorRulePen.setWidth(1);
    majorRulePen.setColor(QColor(100, 100, 100));
    
    painter.setPen(majorRulePen);

    double majorRulesPerInt = 1;

    for (double x = std::floor(xMin); x < std::ceil(xMax); x += 1 / majorRulesPerInt) {
        if (xMin < x && x < xMax) {
            int p = (x - xMin) / (xMax - xMin) * width();
            painter.drawLine(p, 0, p, height());
        }
    }

    for (double y = std::floor(yMin); y < std::ceil(yMax); y += 1 / majorRulesPerInt) {
        if (yMin < y && y < yMax) {
            int p = height() - ((y - yMin) / (yMax - yMin) * height());
            painter.drawLine(0, p, width(), p);
        }
    }
    
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

void Grapher::mousePressEvent(QMouseEvent * /*event*/)
{
    /* is_panning = true; */   

    /* xMinOriginal = xMin; */
    /* xMaxOriginal = xMax; */
    /* yMinOriginal = yMin; */
    /* yMaxOriginal = yMax; */
}

void Grapher::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    /* is_panning = false; */
}

