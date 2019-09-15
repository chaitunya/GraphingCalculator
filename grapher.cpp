#include "grapher.h"
#include <iostream>
#include <cmath>

#include <QPainter>

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
    QPainter *painter = new QPainter(this);
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    
    // transform painter to fit params
    painter->scale(1, -1);
    painter->setWindow(xMin, yMin, xMax - xMin, yMax - yMin);

    for (Function f : functions) {
        f.graphFunction(painter, xMin, xMax, deltaX);
    }
    
    // clean up
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setBrush(Qt::NoBrush);

    delete painter;
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
    QPoint angleDelta = event->angleDelta() / 8;
    if (angleDelta.x() != 0 && angleDelta.y() != 0) {
        /* std::cout << "Angle delta: (" << angleDelta.x() << ", " << angleDelta.y() << ")" << std::endl; */
        QPoint p = event->pos();
        /* std::cout << "Position: (" << p.x() << ", " << p.y() << ")" << std::endl; */
        int scroll_intensity = 1;
        xMin -= (2 * std::signbit(angleDelta.x()) - 1) * scroll_intensity * (1 - (double)p.x() / width());
        xMax += (2 * std::signbit(angleDelta.x()) - 1) * scroll_intensity * (double)p.x() / width();
        /* std::cout << "xMin, xMax: " << xMin << ", " << xMax << std::endl; */
        
        update();
    }
}

void Grapher::resizeEvent(QResizeEvent * event)
{
    if (!initial_resize) {
        xMax = event->size().width() * (xMax - xMin) / event->oldSize().width() + xMin;
        yMax = event->size().height() * (yMax - yMin) / event->oldSize().height() + xMin;
    } else {
        initial_resize = false;
    }
    QWidget::resizeEvent(event);
}
