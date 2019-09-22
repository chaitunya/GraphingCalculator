#include "grapher.h"
#include <iostream>
#include <cmath>

#include <QPainter>
#include <QInputEvent>
#include <array>

Grapher::Grapher(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize Grapher::minimumSizeHint() const
{
    return QSize(400, 400);
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
    
    // calculate scale
    double ref = (xMax - xMin) / width() * 100;

    std::array<int, 3> scales = {1, 2, 5};
    
    int multipleOf = 1;

    for (int scale : scales) {
        double log = std::log10(ref / scale);
        double dist = std::abs(std::round(log) - log);
        double log_min = std::log10(ref / multipleOf);
        double dist_min = std::abs(std::round(log_min) - log_min);
        if (dist < dist_min)
            multipleOf = scale;
    }

    int exp = std::round(std::log10(ref / multipleOf));


    double intsPerMajorRule = multipleOf * std::pow(10, exp);

    double intsPerMinorRule;

    if (multipleOf == 2)
        intsPerMinorRule = intsPerMajorRule / 4;
    else
        intsPerMinorRule = intsPerMajorRule / 5;
    
    // minor rules

    QPen minorRulePen;
    minorRulePen.setWidth(1);
    minorRulePen.setColor(QColor(200, 200, 200));

    painter.setPen(minorRulePen);

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
    
    
    // setup text

    QPen textPen;
    textPen.setColor(QColor(Qt::black));
    
    QFont font = painter.font();
    font.setPixelSize(15);
    painter.setFont(font);

    for (double x = intsPerMajorRule * std::floor(xMin / intsPerMajorRule);
            x < intsPerMajorRule * std::ceil(xMax / intsPerMajorRule);
            x += intsPerMajorRule) {
        if (xMin <= x && x <= xMax && x != 0) {
            double px = (x - xMin) / (xMax - xMin) * (double)width();
            // draw rule
            painter.setPen(majorRulePen);
            painter.drawLine(px, 0, px, height());
            int py = height() - (-yMin / (yMax - yMin) * height());
            // round to certain precision
            double xRounded = std::round(x / intsPerMajorRule) * intsPerMajorRule;
            // draw hatch mark numbers
            painter.setPen(textPen);
            painter.drawText(px, py + font.pixelSize(), tr(" ") + QString::number(xRounded));
        }
    }

    for (double y = intsPerMajorRule * std::floor(yMin / intsPerMajorRule);
            y < intsPerMajorRule * std::ceil(yMax / intsPerMajorRule);
            y += intsPerMajorRule) {
        if (yMin <= y && y <= yMax) {
            double py = height() - ((y - yMin) / (yMax - yMin) * height());
            // draw rule
            painter.setPen(majorRulePen);
            painter.drawLine(0, py, width(), py);
            int px = -xMin / (xMax - xMin) * width();
            // round to certain precision
            double yRounded = std::round(y / intsPerMajorRule) * intsPerMajorRule;
            // draw hatch mark numbers
            painter.setPen(textPen);
            painter.drawText(px, py + font.pixelSize(), tr(" ") + QString::number(yRounded));
        }
    }

    
    // graph functions
    for (Function f : functions) {
        if (!f.isHidden) {
            f.graphFunction(this, painter);
            if (f.graph_derivative) {
                f.graphDerivative(this, painter);
            }
            if (f.graph_integral) {
                f.graphIntegral(this, painter);
            }
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
    double xLen = xMax - xMin;
    double yLen = yMax - yMin;
    xMin -= sign * (double)event->x() / width() * xLen * scrollIntensity;
    xMax += sign * (double)(width() - event->x()) / width() * xLen * scrollIntensity;
    yMin -= sign * (double)(height() - event->y()) / height() * yLen * scrollIntensity;
    yMax += sign * (double)event->y() / height() * yLen * scrollIntensity;
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

void Grapher::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!initial_resize) {
        double yNewSize = height() * (xMax - xMin) / width();
        double yCenter = (yMax + yMin) / 2;
        yMax = yCenter + yNewSize / 2;
        yMin = yCenter - yNewSize / 2;
    } else
        initial_resize = false;
}
