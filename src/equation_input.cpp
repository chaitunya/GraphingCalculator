#include "equation_input.h"
#include "grapher.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <cstring>


EquationInput::EquationInput(QWidget *parent)
  : QLineEdit(parent) {
}

void EquationInput::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Backspace && text() == QString("")) {
    emit deleteKeyReceived();
  } else {
    QLineEdit::keyPressEvent(event);
  }
}
