#ifndef GRAPHER_H
#define GRAPHER_H

#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QResizeEvent>
#include <QWheelEvent>
#include <vector>
#include <functional>
#include "equation_widget.h"
#include "function.h"

struct EquationPair;

class Grapher : public QWidget
{
    Q_OBJECT
public:
    explicit Grapher(std::vector<EquationWidget*> *eqWidgets, QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void addFunction(Function *f, int index = -1);
    void removeFunction(int index);
    QPoint cvtCoords(double x, double y);

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    std::vector<EquationWidget*> *equationWidgets;
    QBrush brush;
    double xMin = -10;
    double xMax = 10;
    double yMin = -10;
    double yMax = 10;
    double deltaX = 1;

    double xOffset;
    double yOffset;
    double xMinOriginal;
    double xMaxOriginal;
    double yMinOriginal;
    double yMaxOriginal;

    bool initial_resize = true;

    QPoint mouseOriginal;
    bool is_panning = false;

    friend void Function::graphFunction(Grapher *grapher, QPainter *painter);
    friend void Function::graphIntegral(Grapher *grapher, QPainter *painter);
    friend void Function::graphDerivative(Grapher *grapher, QPainter *painter);
};

#endif // GRAPHER_H
