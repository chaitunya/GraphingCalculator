#include "function.h"
#include "grapher.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <cstring>

#define MIN_DIFF 0.0001


Function::Function(mathfunc_t mathFunc)
  : color(0, 0, 0), mathFunc(mathFunc), is_parsed(false), is_valid(true) {
}

Function::Function(mathfunc_t mathFunc, const QColor &color)
  : color(color), mathFunc(mathFunc), is_parsed(false), is_valid(true) {
}

Function::Function(const char *expr)
  : color(0, 0, 0), is_parsed(true) {
  setText(expr);
}


Function::Function(const char *expr, const QColor &color)
  : color(color), is_parsed(true) {
  setText(expr);
}

void Function::setText(const char *expr) {
  is_changed = true;
  // Remove and replace evaluator
  if (evaluator)
    evaluator_destroy(evaluator);
  // Need to convert copy const char* to char* to use input
  // for evaluator_create, which for some reason requires only char*
  char* mod_string = new char[std::strlen(expr) + 1];
  std::strcpy(mod_string, expr);
  evaluator = evaluator_create(mod_string);
  int count;
  char **names;
  delete[] mod_string;
  is_valid = evaluator; // returns NULL value of invalid
  if (is_valid) {
    // Make sure that only variable referenced is x
    evaluator_get_variables(evaluator, &names, &count);
    switch (count) {
      case 1:
        if (strcmp(names[0], "x") != 0)
          is_valid = false;
        break;
      case 0: break;
      default: is_valid = false;
    }
  }
}

double Function::derivative(double x) {
  return (evaluateFunction(x + deltaX) - evaluateFunction(x)) / deltaX;
}

double Function::derivative(mathmethod_t func, double x) {
  return ((this->*func)(x + deltaX) - (this->*func)(x)) / deltaX;
}

double Function::integral(double a, double b) {
  double integral = 0;
  if (a < b) {
    for (double i = a; i < b; i += deltaX)
      integral += evaluateFunction(i) * deltaX;
  } else if (a > b) {
    for (double i = a; i > b; i -= deltaX)
      integral -= evaluateFunction(i) * deltaX;
  }
  return integral;
}

double Function::integral0(double x) {
  static double old_x = x;
  static double old_integral = 0;
  if (x - deltaX == old_x && !is_changed) {
    old_integral += evaluateFunction(x) * deltaX;
  } else if (old_x - deltaX == x && !is_changed) {
    old_integral -= evaluateFunction(x) * deltaX;
  } else if (x > 0) {
    old_integral = 0;
    for (double i = 0; i < x; i += deltaX) {
      old_integral += evaluateFunction(i) * deltaX;
    }
  } else if (x < 0) {
    old_integral = 0;
    for (double i = 0; i > x; i -= deltaX) {
      old_integral -= evaluateFunction(i) * deltaX;
    }
  }
  is_changed = false;
  old_x = x;
  return old_integral;
}

double Function::evaluateFunction(double x) {
  if (is_parsed) {
    return evaluator_evaluate_x(evaluator, x);
  } else {
    return mathFunc(x);
  }
}

bool Function::getValid() const {
  return is_valid;
}

double Function::brent(mathmethod_t func, double xMin, double xMax, double min_diff) {
  double f_xMin = (this->*func)(xMin);
  double f_xMax = (this->*func)(xMax);
  if ((f_xMin > 0 && f_xMax > 0) || (f_xMin < 0 && f_xMax < 0))
    return std::nan(""); // return invalid number
  double a;
  double b;
  if (abs(f_xMin) > abs(f_xMax)) {
    a = xMin;
    b = xMax;
  } else {
    a = xMax;
    b = xMin;
  }

  double c = a;
  double s;
  bool mflag = true;
  double d = c;

  do {
    if ((this->*func)(a) != (this->*func)(c) && (this->*func)(b) != (this->*func)(c))
      s = (a * (this->*func)(b) * (this->*func)(c)) / (((this->*func)(a) - (this->*func)(b)) * ((this->*func)(a) - (this->*func)(c)))
        + (b * (this->*func)(a) * (this->*func)(c)) / (((this->*func)(b) - (this->*func)(a)) * ((this->*func)(b) - (this->*func)(c)))
        + (c * (this->*func)(a) * (this->*func)(b)) / (((this->*func)(c) - (this->*func)(a)) * ((this->*func)(c) - (this->*func)(b)));
    else
      s = b - (this->*func)(b) * (b - a) / ((this->*func)(b) - (this->*func)(a));
    if (!((3 * a + b) / 4 < s && (s < b)) ||
        (mflag && abs(s - b) >= abs(b - c) / 2) ||
        (!mflag && abs(s - b) <= abs(c - d) / 2) ||
        (mflag && abs(b - c) < min_diff) ||
        (!mflag && abs(c - d) < min_diff)) {
      s = (a + b) / 2;
      mflag = true;
    } else {
      mflag = false;
    }
    d = c;
    c = b;
    if ((this->*func)(a) * (this->*func)(s) < 0)
      b = s;
    else
      a = s;
    if (abs((this->*func)(a)) < abs((this->*func)(b))) {
      double z = a;
      a = b;
      b = z;
    }
  } while (((this->*func)(b) != 0 && (this->*func)(s) != 0) && abs(b - a) > min_diff);

  if ((this->*func)(s) == 0)
    return s;
  else
    return b;
}

std::vector<QPointF> Function::calculateSingleZeros(mathmethod_t func, double xMin, double xMax, double deltaX) {
  // uses Brent's method to calculate zeros: https://en.wikipedia.org/wiki/Brent%27s_method
  std::vector<QPointF> zeros;
  double f_xLeft; 
  double f_xRight;
  double xLeft; 
  double xRight;
  QPointF zero;
  for (double x = xMin; x < xMax; x += deltaX) {
    xLeft = x;
    xRight = x + deltaX;
    f_xLeft = (this->*func)(xLeft);
    f_xRight = (this->*func)(xRight);
    if ((f_xLeft > 0 && f_xRight > 0) ||
        (f_xLeft < 0 && f_xRight < 0)) {
      continue;
    } else {
      zero = QPointF(0, 0);
      zero.setX(brent(func, xLeft, xRight, MIN_DIFF));
      zeros.push_back(zero);
    }
  }


  return zeros;
}

std::vector<QPointF> Function::calculateZeros(mathmethod_t func, double xMin, double xMax, double deltaX) {
  std::vector<QPointF> zeros = calculateSingleZeros(func, xMin, xMax, deltaX);
  std::vector<QPointF> rel_extrema = calculateRelExtrema(func, xMin, xMax, deltaX);
  for (QPointF extremum : rel_extrema) {
    if (abs(extremum.y()) <= MIN_DIFF)
      zeros.push_back(extremum);
  }
  return zeros;
}

std::vector<QPointF> Function::calculateRelExtrema(mathmethod_t func, double xMin, double xMax, double deltaX) {
  std::vector<QPointF> extrema;
  double f_xLeft; 
  double f_xRight;
  double xLeft; 
  double xRight;
  QPointF extremum;
  for (double x = xMin; x < xMax; x += deltaX) {
    xLeft = x;
    xRight = x + deltaX;
    f_xLeft = derivative(func, xLeft);
    f_xRight = derivative(func, xRight);
    if ((f_xLeft > 0 && f_xRight > 0) ||
        (f_xLeft < 0 && f_xRight < 0)) {
      continue;
    } else {
      extremum = QPoint();
      extremum.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
      extremum.setY(evaluateFunction(extremum.x()));
      extrema.push_back(extremum);
    }
  }
  return extrema;
}

std::vector<QPointF> Function::calculateRelExtrema(double xMin, double xMax, double deltaX) {
  std::vector<QPointF> extrema;
  QPointF extremum;
  for (double x = xMin; x < xMax; x += deltaX) {
    double xLeft = x;
    double xRight = x + deltaX;
    double f_xLeft = derivative(xLeft);
    double f_xRight = derivative(xRight);
    if ((f_xLeft > 0 && f_xRight > 0) ||
        (f_xLeft < 0 && f_xRight < 0)) {
      continue;
    } else {
      extremum = QPoint();
      extremum.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
      extremum.setY(evaluateFunction(extremum.x()));
      extrema.push_back(extremum);
    }
  }
  return extrema;
}

std::vector<QPointF> Function::calculateRelMaxs(double xMin, double xMax, double deltaX) {
  std::vector<QPointF> maxs;
  QPointF max;
  for (double x = xMin; x < xMax; x += deltaX) {
    double xLeft = x;
    double xRight = x + deltaX;
    double f_xLeft = derivative(xLeft);
    double f_xRight = derivative(xRight);
    if ((f_xLeft < 0 && f_xRight > 0) ||
        (f_xLeft > 0 && f_xRight > 0)
        || (f_xLeft < 0 && f_xRight < 0)) {
      continue;
    } else {
      max = QPoint();
      max.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
      max.setY(evaluateFunction(max.x()));
      maxs.push_back(max);
    }
  }
  return maxs;
}

std::vector<QPointF> Function::calculateRelMins(double xMin, double xMax, double deltaX) {
  std::vector<QPointF> mins;
  QPointF min;
  for (double x = xMin; x < xMax; x += deltaX) {
    double xLeft = x;
    double xRight = x + deltaX;
    double f_xLeft = derivative(xLeft);
    double f_xRight = derivative(xRight);
    if ((f_xLeft > 0 && f_xRight < 0) ||
        (f_xLeft > 0 && f_xRight > 0) ||
        (f_xLeft < 0 && f_xRight < 0)) {
      continue;
    } else {
      min = QPoint();
      min.setX(brent(&Function::derivative, xLeft, xRight, MIN_DIFF));
      min.setY(evaluateFunction(min.x()));
      mins.push_back(min);
    }
  }
  return mins;
}

std::vector<double> Function::calculateVertAsymptotes(double xMin, double xMax, double deltaX) {
  std::vector<double> asys;
  double f_xLeft;
  double f_xRight;
  double xLeft;
  double xRight;
  double asy;
  for (double x = xMin; x < xMax; x += deltaX) {
    xLeft = x;
    xRight = x + deltaX;
    f_xLeft = evaluateFunction(xLeft);
    f_xRight = evaluateFunction(xRight);
    if ((f_xLeft > 0 && f_xRight > 0) ||
        (f_xLeft < 0 && f_xRight < 0)) {
      continue;
    } else {
      asy = brent(&Function::evaluateFunction, xLeft, xRight, MIN_DIFF);
      asys.push_back(asy);
    }
  }
  return asys;
}

bool Function::discontinuityBetween(mathmethod_t func, double xMin, double xMax, double deltaX) {
   auto zeros = calculateSingleZeros(func, xMin, xMax, deltaX);
   double zero;
   double lim;
   for (QPointF pt : zeros) {
     zero = pt.x();
     lim = limitAt(func, zero, 0.01, 0.5);
     if (std::isnan(lim))
       return true;
   }
  return false;
}

double Function::limitAt(mathmethod_t func, double x, double deltaX, double max_diff) {
  double leftLim = leftLimitAt(func, x, deltaX / 2, deltaX, max_diff);
  double rightLim = rightLimitAt(func, x, deltaX / 2, deltaX, max_diff);
  if (abs(leftLim - rightLim) <= 2 * max_diff)
    return (rightLim + leftLim) / 2; // get average of both limits
  else
    return NAN; // Equivalent to DNE
}

double Function::leftLimitAt(mathmethod_t func, double x, double deltaX1, double deltaX2, double max_diff) {
  double f_dx1 = (this->*func)(x - deltaX1);
  double f_dx2 = (this->*func)(x - deltaX2);
  if (!std::isinf(f_dx1) && !std::isinf(f_dx2)
      && !std::isinf(f_dx1) && !std::isinf(f_dx2)
      && abs(f_dx1 - f_dx2) <= max_diff)
    return f_dx1;
  else
    return NAN;
}

double Function::rightLimitAt(mathmethod_t func, double x, double deltaX1, double deltaX2, double max_diff) {
  double f_dx1 = (this->*func)(x + deltaX1);
  double f_dx2 = (this->*func)(x + deltaX2);
  if (!std::isinf(f_dx1) && !std::isinf(f_dx2)
      && !std::isinf(f_dx1) && !std::isinf(f_dx2)
      && abs(f_dx1 - f_dx2) <= max_diff)
    return f_dx1;
  else
    return NAN;
}
double Function::reciprocal(double x) {
  return 1 / evaluateFunction(x);
}
void Function::setColor(const QColor &new_color) {
  color = QColor(new_color);
}

QColor Function::getColor() const {
  return color;
}

void Function::set_delta_x(double delta_x) {
  deltaX = delta_x;
}
