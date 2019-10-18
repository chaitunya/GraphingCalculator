#ifndef FUNCTION_H
#define FUNCTION_H

#include <functional>
#include <mathpresso/mathpresso.h>
#include <QColor>
#include <QPainter>

class Grapher;


class Function {
  typedef double(*mathfunc_t)(double);
  typedef double(Function::*mathmethod_t)(double);

public:
  Function(mathfunc_t mathFunc);
  Function(mathfunc_t mathFunc, const QColor &color);
  Function(const char* expr);
  Function(const char* expr, const QColor &color);
  void setText(const char* expr);
  
  // evaluateFunction and operations on Function
  double evaluateFunction(double x);
  double derivative(double x);
  double derivative(mathmethod_t func, double x);
  double integral(double a, double b);
  double reciprocal(double x);

  // Graphing Utilities
  void graphFunction(Grapher *grapher, QPainter *painter);
  void graphIntegral(Grapher *grapher, QPainter *painter);
  void graphDerivative(Grapher *grapher, QPainter *painter);

  // Getters and setters
  void setColor(const QColor &color);
  bool getValid() const;

  // hidden bools
  bool isHidden = false;
  bool b_graphDerivative = false;
  bool b_graphIntegral = false;

  // Calculate features of graph
  std::vector<QPointF> calculateSingleZeros(mathmethod_t func, double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateZeros(mathmethod_t func, double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelMins(double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelMaxs(double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelExtrema(double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateRelExtrema(mathmethod_t func, double xMin, double xMax, double deltaX);
  std::vector<QPointF> calculateVertAsymptotes(double xMin, double xMax, double deltaX);
  bool discontinuityBetween(double xMin, double xMax, double deltaX);

private:
  // Base graphFunction
  void graphFunction(mathmethod_t func, Grapher *grapher, QPainter *painter);

  // brent algorithm
  double brent(mathmethod_t func, double xMin, double xMax, double min_diff);

  QColor color;
  mathpresso::Context mpCtx;
  mathpresso::Expression mpExpr;
  mathfunc_t mathFunc;
  double integral0(double x);
  double deltaX = 0.0001;
  bool is_parsed;
  bool is_valid;
  
};

#endif
