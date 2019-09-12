#include "grapher.h"

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
    // setup QPainterPath and update the path
    QPainterPath *path = new QPainterPath;
    updatePath(path);

    // setup QPainter
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw path
    painter.drawPath(*path);

    // clean up
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    // delete path from memory
    delete path;
}

void Grapher::updatePath(QPainterPath *path)
{
    path->moveTo(20, 80);
    path->lineTo(20, 30);
    path->cubicTo(80, 0, 50, 50, 80, 80);
}
