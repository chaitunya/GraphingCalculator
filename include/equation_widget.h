#ifndef EQUATION_WIDGET_H
#define EQUATION_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "function.h"
#include "equation_input.h"
#include <QHBoxLayout>
#include <QToolButton>

class Window;

class EquationWidget : public QWidget {
  Q_OBJECT
public:
  EquationWidget(Function *f, Window *window, Grapher *grapher, QWidget *parent = 0);
  Function *getFunction() const;
  void setFocus();

private slots:
  void updateFunction(const QString &text);
  void addFunction();
  void delFunction();
  void setDisplayDerivative();
  void setDisplayIntegral(bool show);

private:
  Function *func;
  Grapher *grapher;
  Window *window;
  EquationInput *equation_input;
  QHBoxLayout *layout;
  QToolButton *derivative_button;
  QToolButton *integral_button;
  

};

#endif
