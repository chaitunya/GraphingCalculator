#include "grapher.h"
#include <iostream>
#include <cmath>

#include <QPainter>
#include <QInputEvent>

#define getPxCoord(px, func) height() - (\
                                 (func(px * (xMax - xMin) / width() + xMin) - yMin)\
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

    double intsPerMinorRule = 1;
    
    for (double x = intsPerMinorRule * std::floor(xMin / intsPerMinorRule);
            x < intsPerMinorRule * std::ceil(xMax / intsPerMinorRule);
            x += intsPerMinorRule) {
        if (xMin <= x && x <= xMax) {
            int p = (x - xMin) / (xMax - xMin) * width();
            painter.drawLine(p, 0, p, height());
        }
    }

    for (double y = intsPerMinorRule * std::floor(yMin / intsPerMinorRule);
            y < intsPerMinorRule * std::ceil(yMax / intsPerMinorRule);
            y += intsPerMinorRule) {
        if (yMin <= y && y <= yMax) {
            int p = height() - ((y - yMin) / (yMax - yMin) * height());
            painter.drawLine(0, p, width(), p);
        }
    }

    QPen majorRulePen;
    majorRulePen.setWidth(1);
    majorRulePen.setColor(QColor(100, 100, 100));
    
    painter.setPen(majorRulePen);
    
    // setup text

    QPen textPen;
    textPen.setColor(QColor(0, 0, 0));
    
    QFont font = painter.font();
    font.setPixelSize(15); // Will make this variable font length
    painter.setFont(font);

    
    double intsPerMajorRule = 2;

    if (intsPerMajorRule == 0) {
        intsPerMajorRule = (xMax - xMin) / 10;
    }

    for (double x = intsPerMajorRule * std::floor(xMin / intsPerMajorRule);
            x < intsPerMajorRule * std::ceil(xMax / intsPerMajorRule);
            x += intsPerMajorRule) {
        if (xMin <= x && x <= xMax && x != 0) {
            double px = (x - xMin) / (xMax - xMin) * (double)width();
            // draw rule
            painter.drawLine(px, 0, px, height());
            int py = height() - (-yMin / (yMax - yMin) * height());
            // round to certain precision
            double xRounded = std::round(x / intsPerMajorRule) * intsPerMajorRule;
            // draw hatch mark numbers
            painter.drawText(px, py + font.pixelSize(), tr(" ") + QString::number(xRounded));
        }
    }

    for (double y = intsPerMajorRule * std::floor(yMin / intsPerMajorRule);
            y < intsPerMajorRule * std::ceil(yMax / intsPerMajorRule);
            y += intsPerMajorRule) {
        if (yMin <= y && y <= yMax) {
            double py = height() - ((y - yMin) / (yMax - yMin) * height());
            // draw rule
            painter.drawLine(0, py, width(), py);
            int px = -xMin / (xMax - xMin) * width();
            // round to certain precision
            double yRounded = std::round(y / intsPerMajorRule) * intsPerMajorRule;
            // draw hatch mark numbers
            painter.drawText(px, py + font.pixelSize(), tr(" ") + QString::number(yRounded));
        }
    }

    
    // graph functions
    for (Function f : functions) {
        painter.setPen(f.pen);
        QPainterPath path;
        path.moveTo(0, getPxCoord(0, f.mathFunc));
        for (int p = 0; p < width(); p++) {
            path.lineTo(p, getPxCoord(p, f.mathFunc));
        }
        painter.drawPath(path);
        if (f.graph_derivative) {
            QPainterPath path;
            path.moveTo(0, getPxCoord(0, f.derivative));
            for (int p = 0; p < width(); p++) {
                path.lineTo(p, getPxCoord(p, f.derivative));
            }
            painter.drawPath(path);
        }
        if (f.graph_integral) {
            QPainterPath path;
            path.moveTo(0, getPxCoord(0, f.integral0));
            for (int p = 0; p < width(); p++) {
                path.lineTo(p, getPxCoord(p, f.integral0));
            }
            painter.drawPath(path);
        }
    }
    
    // clean up
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);
    // draw border
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

void Grapher::addFunction(const Function &f, int index)
{
    if (index == -1)
        functions.push_back(f);
    else
        functions.insert(functions.begin() + index, f);
}

void Grapher::wheelEvent(QWheelEvent * event)
{
    double numSteps = (double)event->angleDelta().y() / 120;
    int sign = 2 * std::signbit(numSteps) - 1;
    double scrollIntensity = 0.05;
    xMin -= sign * (double)event->x() / width() * (xMax - xMin) * scrollIntensity;
    xMax += sign * (double)(width() - event->x()) / width() * (xMax - xMin) * scrollIntensity;
    yMin -= sign * (double)(height() - event->y()) / height() * (yMax - yMin) * scrollIntensity;
    yMax += sign * (double)event->y() / height() * (yMax - yMin) * scrollIntensity;
    update();
}

void Grapher::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
    
    is_panning = true;   

    xMinOriginal = xMin;
    xMaxOriginal = xMax;
    yMinOriginal = yMin;
    yMaxOriginal = yMax;

    mouseOriginal = event->pos();
    update();
}

void Grapher::mouseMoveEvent(QMouseEvent * event)
{
    QWidget::mouseMoveEvent(event);

    if (is_panning) {
        QPoint deltaPos = mouseOriginal - event->pos();
        xMin = xMinOriginal + (double)deltaPos.x() / width() * (xMax - xMin);
        xMax = xMaxOriginal + (double)deltaPos.x() / width() * (xMax - xMin);
        yMin = yMinOriginal - (double)deltaPos.y() / height() * (yMax - yMin);
        yMax = yMaxOriginal - (double)deltaPos.y() / height() * (yMax - yMin);
        update();
    }
}

void Grapher::mouseReleaseEvent(QMouseEvent * event)
{
    QWidget::mouseReleaseEvent(event);
    is_panning = false;
}

