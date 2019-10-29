#include "equation_widget.h"
#include "grapher.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <cstring>


EquationWidget::EquationWidget(Function *f, Window *window, Grapher *grapher, QWidget *parent)
  : QLineEdit(parent), func(f), grapher(grapher), window(window) {
  connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateFunction(const QString&)));
  connect(this, SIGNAL(returnPressed()), this, SLOT(addFunction()));
}

void EquationWidget::updateFunction(const QString &text) {
  QByteArray ba = text.toLocal8Bit();
  const char *c_str = ba.data();
  func->setText(c_str);
  grapher->update();
}

void EquationWidget::addFunction() {
  window->addFunction(this);
}


Function *EquationWidget::getFunction() const {
  return func;
}

void EquationWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Backspace && text() == QString(""))
    window->delFunction(this);
  else
    QLineEdit::keyPressEvent(event);
}
