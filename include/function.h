#ifndef FUNCTION_H
#define FUNCTION_H

#include <functional>
#include <matheval.h>
#include <QColor>
#include <QPainter>

class Grapher;

class Function;

typedef double(Function::*mathmethod_t)(double);
typedef double(*mathfunc_t)(double);

class Function {
public:
  explicit Function(mathfunc_t mathFunc);
  Function(mathfunc_t mathFunc, const QColor &color);
  explicit Function(const char *expr);
  Function(const char *expr, const QColor &color);
  void setText(const char *expr);

  // evaluateFunction and operations on Function
  double evaluateFunction(double x);
  double derivative(double x);
  double second_derivative(double x);
  double derivative(mathmethod_t func, double x);
  double integral(double a, double b);
  double integral0(double x);
  double reciprocal(double x);

  // Getters and setters
  void setColor(const QColor &new_color);
  QColor getColor() const;
  bool getValid() const;
  int get_n_derivative() const;
  void set_n_derivative(int n);

  // hidden bools
  bool isHidden = false;
  bool b_graphIntegral = false;

  void set_delta_x(double delta_x);

  // Calculate features of graph
  std::vector<QPointF> calculateSingleZeros(mathmethod_t func, double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateZeros(mathmethod_t func, double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelMins(double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelMaxs(double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelExtrema(double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelExtrema(mathmethod_t func, double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateInflectionPoints(double xMin, double xMax, double deltaX);
  std::vector<double> calculateVertAsymptotes(double xMin, double xMax, double deltaX);
  bool discontinuityBetween(mathmethod_t func, double xMin, double xMax, double deltaX);
  double limitAt(mathmethod_t func, double x, double deltaX, double max_diff);
  double leftLimitAt(mathmethod_t func, double x, double deltaX1, double deltaX2, double max_diff);
  double rightLimitAt(mathmethod_t func, double x, double deltaX1, double deltaX2, double max_diff);

private:
  // Base graphFunction
  void graphFunction(mathmethod_t func, Grapher *grapher, QPainter *painter);

  // brent algorithm
  double brent(mathmethod_t func, double xMin, double xMax, double min_diff);

  QColor color;
  mathfunc_t mathFunc = nullptr;

  // parser utilities
  void *evaluator = 0;

  double deltaX = 0.0001;
  bool is_parsed;
  bool is_valid;

  bool is_changed = false;

  int n_derivative = 0;

};

#endif
