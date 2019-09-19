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

#define LIMIT(a, b, x) a < x ? a : (b > x ? b : x)
#define LIMITL(a, x) a < x ? a : x
#define LIMITR(b, x) b > x ? b : x

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

    double minorRulesPerInt = 20 / (xMax - xMin);
    
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
    
    // setup text
    
    QFont font = painter.font();
    font.setPixelSize(LIMIT(10, (height() / 75), 30)); // Will make this variable font length
    painter.setFont(font);

    double majorRulesPerInt = 10 / (xMax - xMin);

    for (double x = std::floor(xMin); x < std::ceil(xMax); x += 1 / majorRulesPerInt) {
        if (xMin < x && x < xMax && x != 0) {
            int px = (x - xMin) / (xMax - xMin) * width();
            // draw rule
            painter.drawLine(px, 0, px, height());
            int py = height() - (-yMin / (yMax - yMin) * height());
            // draw hatch mark numbers
            painter.drawText(px, py + font.pixelSize(), QString::number(x));
        }
    }

    for (double y = std::floor(yMin); y < std::ceil(yMax); y += 1 / majorRulesPerInt) {
        if (yMin < y && y < yMax) {
            int py = height() - ((y - yMin) / (yMax - yMin) * height());
            // draw rule
            painter.drawLine(0, py, width(), py);
            int px = -xMin / (xMax - xMin) * width();
            // draw hatch mark numbers
            painter.drawText(px, py + font.pixelSize(), QString::number(y));
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
    // draw border
    /* painter.drawRect(QRect(0, 0, width() - 1, height() - 1)); */
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

