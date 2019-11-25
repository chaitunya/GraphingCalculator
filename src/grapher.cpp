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

  QPen pointPen;
  pointPen.setWidth(10);

  for (EquationWidget *eqWidg: *equationWidgets) {
    Function *f = eqWidg->getFunction();
    if (!f->isHidden && f->getValid()) {
      painter.setPen(linePen);
      graphFunction(&painter, f, &Function::evaluateFunction);
      if (f->b_graphDerivative) {
        graphFunction(&painter, f, &Function::derivative);
      }
      if (f->b_graphIntegral) {
        graphFunction(&painter, f, &Function::integral0);
      }
    }
  }

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

void Grapher::graphFunction(QPainter* painter, Function* F, mathmethod_t func) {
  QPen new_pen(painter->pen());
  new_pen.setColor(F->getColor());
  painter->setPen(new_pen);
  QPainterPath path;
  bool out_of_bounds_top = false, out_of_bounds_bot = false;
  for (int px_x = 0; px_x < width(); px_x++) {
    double coord_x = px_x * (xMax - xMin) / width() + xMin;
    double coord_y = (F->*func)(coord_x);
    double px_y = height() - ((coord_y - yMin) / (yMax - yMin) * height());
    if (F->discontinuityBetween(func, coord_x - (xMax - xMin) / width(),
                                coord_x, (xMax - xMin) / width())) {
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
}

