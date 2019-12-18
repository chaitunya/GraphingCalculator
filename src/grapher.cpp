#include "grapher.h"
#include <iostream>
#include <cmath>

#include <QPainter>
#include <QInputEvent>
#include <array>
#include "window.h"

Grapher::Grapher(std::vector<EquationWidget*> *eqWidgets, QWidget *parent)
  : QWidget(parent), equationWidgets(eqWidgets) {
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
}

QSize Grapher::minimumSizeHint() const {
  return QSize(400, 400);
}

QSize Grapher::sizeHint() const {
  return QSize(400, 400);
}

void Grapher::paintEvent(QPaintEvent *) {
  // setup QPainter
  QPainter painter(this);
  painter.setBrush(brush);
  painter.setRenderHint(QPainter::Antialiasing, true);

  
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

  QPen linePen;
  linePen.setWidth(3);
  painter.setPen(linePen);

  QColor rel_min_color(255, 0, 0);
  QColor rel_max_color(0, 0, 255);
  QColor zero_color(0, 100, 100);
  QColor ip_color(200, 200, 100);
  QColor rd_color(160, 50, 160);
  int rel_min_width = 3;
  int rel_max_width = 3;
  int zero_width = 3;
  int ip_width = 3;
  int rd_width = 3;

  std::vector<Point> points;
  std::vector<int> vertical_asymptotes;

  for (EquationWidget *eqWidg: *equationWidgets) {
    Function *f = eqWidg->getFunction();
    if (!f->isHidden && f->getValid()) {
      if (f->get_n_derivative() != 0) {
        DataSet derivative_dataset;
        if (f->get_n_derivative() == 1)
          derivative_dataset = graphFunction(&painter, f, &Function::derivative);
        else if (f->get_n_derivative() == 2)
          derivative_dataset = graphFunction(&painter, f, &Function::second_derivative);
        std::transform(derivative_dataset.zeros.begin(), derivative_dataset.zeros.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), zero_color, zero_width}; }
        );
        std::transform(derivative_dataset.relative_minimums.begin(), derivative_dataset.relative_minimums.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rel_min_color, rel_min_width}; }
        );
        std::transform(derivative_dataset.relative_maximums.begin(), derivative_dataset.relative_maximums.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rel_max_color, rel_max_width}; }
        );
        std::transform(derivative_dataset.inflection_points.begin(), derivative_dataset.inflection_points.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), ip_color, ip_width}; }
        );
        std::transform(derivative_dataset.removable_discontinuities.begin(), derivative_dataset.removable_discontinuities.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rd_color, rd_width}; }
        );
        std::transform(derivative_dataset.vertical_asymptotes.begin(), derivative_dataset.vertical_asymptotes.end(), std::back_inserter(vertical_asymptotes),
          [=](double x) -> int { return cvtXToPx(x); }
        );
      }

      if (f->b_graphIntegral) {
        DataSet integral_dataset = graphFunction(&painter, f, &Function::integral0);
        std::transform(integral_dataset.zeros.begin(), integral_dataset.zeros.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), zero_color, zero_width}; }
        );
        std::transform(integral_dataset.relative_minimums.begin(), integral_dataset.relative_minimums.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rel_min_color, rel_min_width}; }
        );
        std::transform(integral_dataset.relative_maximums.begin(), integral_dataset.relative_maximums.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rel_max_color, rel_max_width}; }
        );
        std::transform(integral_dataset.inflection_points.begin(), integral_dataset.inflection_points.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), ip_color, ip_width}; }
        );
        std::transform(integral_dataset.removable_discontinuities.begin(), integral_dataset.removable_discontinuities.end(), std::back_inserter(points),
          [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rd_color, rd_width}; }
        );
        std::transform(integral_dataset.vertical_asymptotes.begin(), integral_dataset.vertical_asymptotes.end(), std::back_inserter(vertical_asymptotes),
          [=](double x) -> int { return cvtXToPx(x); }
        );
      }

      DataSet dataset = graphFunction(&painter, f, &Function::evaluateFunction);
      // Add dataset points vector
      std::transform(dataset.zeros.begin(), dataset.zeros.end(), std::back_inserter(points),
        [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), zero_color, zero_width}; }
      );
      std::transform(dataset.relative_minimums.begin(), dataset.relative_minimums.end(), std::back_inserter(points),
        [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rel_min_color, rel_min_width}; }
      );
      std::transform(dataset.relative_maximums.begin(), dataset.relative_maximums.end(), std::back_inserter(points),
        [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rel_max_color, rel_max_width}; }
      );
      std::transform(dataset.inflection_points.begin(), dataset.inflection_points.end(), std::back_inserter(points),
        [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), ip_color, ip_width}; }
      );
      std::transform(dataset.removable_discontinuities.begin(), dataset.removable_discontinuities.end(), std::back_inserter(points),
        [=](QPointF pt) -> Point { return Point{cvtCoordsToPx(pt), rd_color, rd_width}; }
      );
      std::transform(dataset.vertical_asymptotes.begin(), dataset.vertical_asymptotes.end(), std::back_inserter(vertical_asymptotes),
        [=](double x) -> int { return cvtXToPx(x); }
      );

    }
  }

  // graph axes
  QPen ax_pen; ax_pen.setWidth(2);
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

  QPen vert_asy_pen;
  QVector<qreal> dashes;
  dashes << 5 << 5;
  vert_asy_pen.setColor(QColor(175, 0, 0));
  vert_asy_pen.setWidth(5);
  vert_asy_pen.setDashPattern(dashes);
  painter.setPen(vert_asy_pen);
  for (int x : vertical_asymptotes) {
    painter.drawLine(x, 0, x, height() - 1);
  }

  // Plot points (relative extrema, points of inflection)
  QPen pointPen;
  for (Point pt : points) {
    pointPen.setColor(pt.color);
    pointPen.setWidth(pt.width);
    painter.setPen(pointPen);
    painter.setBrush(QBrush(pt.color));
    painter.drawEllipse(pt.point, pt.width, pt.width);
  }
  painter.setBrush(brush);
  

  // draw border
  QPen borderPen;
  borderPen.setWidth(3);
  borderPen.setColor(QColor(Qt::black));
  painter.setPen(borderPen);
  painter.drawRect(QRect(1, 1, width() - 2, height() - 2));
  
  // clean up
  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.setBrush(Qt::NoBrush);
}

void Grapher::wheelEvent(QWheelEvent * event) {
  double numSteps = (double)event->angleDelta().y() / 120;
  double scrollIntensity = 0.05;
  double xLen = xMax - xMin;
  double yLen = yMax - yMin;
  xMin += numSteps * (double)event->x() / width() * xLen * scrollIntensity;
  xMax -= numSteps * (double)(width() - event->x()) / width() * xLen * scrollIntensity;
  yMin += numSteps * (double)(height() - event->y()) / height() * yLen * scrollIntensity;
  yMax -= numSteps * (double)event->y() / height() * yLen * scrollIntensity;
  update();
}

void Grapher::mousePressEvent(QMouseEvent * event) {
  QWidget::mousePressEvent(event);
  is_panning = true;   
  xMinOriginal = xMin;
  xMaxOriginal = xMax;
  yMinOriginal = yMin;
  yMaxOriginal = yMax;
  mouseOriginal = event->pos();
  update();
}

void Grapher::mouseMoveEvent(QMouseEvent * event) {
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

void Grapher::mouseReleaseEvent(QMouseEvent * event) {
  QWidget::mouseReleaseEvent(event);
  is_panning = false;
}

void Grapher::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  if (!initial_resize) {
    double yNewSize = height() * (xMax - xMin) / width();
    double yCenter = (yMax + yMin) / 2;
    yMax = yCenter + yNewSize / 2;
    yMin = yCenter - yNewSize / 2;
  } else {
    initial_resize = false;
  }
}

DataSet Grapher::graphFunction(QPainter* painter, Function* F, mathmethod_t func) {
  DataSet dataset;
  QPen new_pen(painter->pen());
  new_pen.setColor(F->getColor());
  painter->setPen(new_pen);
  QPainterPath path;
  dataset.zeros = F->calculateSingleZeros(&Function::evaluateFunction, cvtPxToX(0), cvtPxToX(width() - 1), (xMax - xMin) / width());
  dataset.relative_minimums = F->calculateRelMins(cvtPxToX(0), cvtPxToX(width() - 1), (xMax - xMin) / width());
  dataset.relative_maximums = F->calculateRelMaxs(cvtPxToX(0), cvtPxToX(width() - 1), (xMax - xMin) / width());
  dataset.inflection_points = F->calculateInflectionPoints(cvtPxToX(0), cvtPxToX(width() - 1), (xMax - xMin) / width());
  dataset.vertical_asymptotes = F->calculateVertAsymptotes(cvtPxToX(0), cvtPxToX(width() - 1), (xMax - xMin) / width());
  bool out_of_bounds_top = false, out_of_bounds_bot = false;
  for (int px_x = 0; px_x < width(); px_x++) {
    double coord_x = cvtPxToX(px_x);
    double coord_x_round = round(coord_x * 10) / 10.;
    double coord_y_round = (F->*func)(coord_x_round);
    double lim = F->limitAt(func, coord_x, 0.00001, 1);
    double coord_y = (F->*func)(coord_x);
    int px_y = cvtYToPx(coord_y);
    bool discontinuity = false;
    if (std::isnan(coord_y_round) && !std::isnan(lim)) {
      discontinuity = true;
      dataset.removable_discontinuities.push_back({coord_x_round, lim});
    } 
    F->set_delta_x(coord_x - ((px_x - 1) * (xMax - xMin) / width() + xMin));
    if (discontinuity) {
      path.moveTo(px_x, px_y);
    } else if (px_x < 0) {
      path.moveTo(0, px_y);
    } else if (px_x > width()) {
      path.lineTo(width() - 1, px_y);
    } else if (px_y >= height()) {
      if (out_of_bounds_bot) {
        path.moveTo(px_x, height() - 1);
      } else {
        path.lineTo(px_x, height() - 1);
        out_of_bounds_bot = true;
        out_of_bounds_top = false;
      }
    } else if (px_y < 0) {
      if (out_of_bounds_top) {
        path.moveTo(px_x, 0);
      } else {
        path.lineTo(px_x, 0);
        out_of_bounds_top = true;
        out_of_bounds_bot = false;
      }
    } else {
      path.lineTo(px_x, px_y);
    }
  }
  painter->drawPath(path);
  return dataset;
}

QPoint Grapher::cvtCoordsToPx(QPointF pt) const {
  return QPoint((pt.x() - xMin) * width() / (xMax - xMin),
    height() - (pt.y() - yMin) * height() / (yMax - yMin));
}

int Grapher::cvtXToPx(double x) const {
  return (x - xMin) * width() / (xMax - xMin);
}

int Grapher::cvtYToPx(double y) const {
  return height() - (y - yMin) * height() / (yMax - yMin);
}

double Grapher::cvtPxToX(int x) const {
  return x * (xMax - xMin) / width() + xMin;
}

double Grapher::cvtPxToY(int y) const {
  return height() - (y  * (yMax - xMin) / height() + yMin);
}

QPointF Grapher::cvtPxToCoords(QPoint pt) const {
  return QPointF(pt.x() * (xMax - xMin) / width() + xMin,
    height() - (pt.y() * (yMax - yMin) / height() + yMin));
}
