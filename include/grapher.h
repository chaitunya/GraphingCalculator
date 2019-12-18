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

struct DataSet {
  std::vector<QPointF> zeros;
  std::vector<QPointF> relative_minimums;
  std::vector<QPointF> relative_maximums;
  std::vector<QPointF> inflection_points;
  std::vector<QPointF> removable_discontinuities;
  std::vector<double> vertical_asymptotes;
};

class Grapher : public QWidget
{
  Q_OBJECT
public:
  explicit Grapher(std::vector<EquationWidget*> *eqWidgets, QWidget *parent = nullptr);
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;
  void addFunction(Function *f, int index = -1);
  void removeFunction(int index);

protected:
  void paintEvent(QPaintEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  QPoint cvtCoordsToPx(QPointF pt) const;
  QPointF cvtPxToCoords(QPoint pt) const;
  double cvtPxToX(int x) const;
  int cvtXToPx(double x) const;
  double cvtPxToY(int y) const;
  int cvtYToPx(double y) const;
  std::vector<EquationWidget*> *equationWidgets;
  DataSet graphFunction(QPainter *painter, Function *F, mathmethod_t func);
  QBrush brush;
  double xMin = -11;
  double xMax = 11;
  double yMin = -11;
  double yMax = 11;
  double deltaX;

  double xOffset;
  double yOffset;
  double xMinOriginal;
  double xMaxOriginal;
  double yMinOriginal;
  double yMaxOriginal;

  bool initial_resize = true;

  QPoint mouseOriginal;
  bool is_panning = false;

  friend class Function;

};

struct Point {
  QPoint point;
  QColor color;
  int width;
};

#endif // GRAPHER_H
