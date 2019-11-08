#ifndef EQUATION_WIDGET_H
#define EQUATION_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "function.h"
#include <QPainter>

class Window;

class EquationWidget : public QWidget {
  Q_OBJECT
public:
  EquationWidget(Function* f, Window* window, Grapher* grapher, QWidget* parent = 0);
  Function *getFunction() const;
  QString text() const;

private slots:
  void updateFunction(const QString &text);
  void addFunction();

signals:
  void textChanged(const QString &text);
  void returnPressed();

protected:
  void keyPressEvent(QKeyEvent* event);
  void paintEvent(QPainter* event);

private:
  Function *func;
  Grapher *grapher;
  Window *window;

  QString text_;
  

};

#endif
