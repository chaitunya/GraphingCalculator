#ifndef GRAPHER_H
#define GRAPHER_H

#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QResizeEvent>
#include <QWheelEvent>
#include <vector>
#include <functional>
#include "function.h"


class Grapher : public QWidget
{
    Q_OBJECT
public:
    explicit Grapher(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void addFunction(const Function &f, int index = -1);
    void removeFunction(int index);
    QPoint cvtCoords(double x, double y);

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::vector<Function> functions;
    QBrush brush;
    double xMin = -5;
    double xMax = 10;
    double yMin = -7;
    double yMax = 10;
    double deltaX = 1;

    double xOffset;
    double yOffset;
    double xMinOriginal;
    double xMaxOriginal;
    double yMinOriginal;
    double yMaxOriginal;
    bool is_panning = false;
};

#endif // GRAPHER_H
