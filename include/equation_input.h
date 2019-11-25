#ifndef EQUATION_INPUT_H
#define EQUATION_INPUT_H

#include <QWidget>
#include <QLineEdit>

class EquationInput : public QLineEdit {
  Q_OBJECT
public:
  EquationInput(QWidget *parent = 0);

signals:
  void deleteKeyReceived();

protected:
  void keyPressEvent(QKeyEvent *event);
};

#endif
