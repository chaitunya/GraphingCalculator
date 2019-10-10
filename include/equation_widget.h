#ifndef EQUATION_WIDGET_H
#define EQUATION_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "function.h"

class Window;

class EquationWidget : public QLineEdit {
  Q_OBJECT
public:
  EquationWidget(Function *f, Window *window, Grapher *grapher, QWidget *parent = 0);
  Function *getFunction() const;

private slots:
  void updateFunction(const QString &text);
  void addFunction();

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  Function *func;
  Grapher *grapher;
  Window *window;
  

};

#endif
