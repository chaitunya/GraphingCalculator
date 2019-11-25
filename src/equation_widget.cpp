#include "equation_widget.h"
#include "grapher.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <cstring>


EquationWidget::EquationWidget(Function *f, Window *window, Grapher *grapher, QWidget *parent)
  : QWidget(parent), func(f), grapher(grapher), window(window) {
  
  equation_input = new EquationInput(this);
  layout = new QHBoxLayout;
  layout->addWidget(equation_input);
  setLayout(layout);
  connect(equation_input, SIGNAL(textChanged(const QString&)), this, SLOT(updateFunction(const QString&)));
  connect(equation_input, SIGNAL(returnPressed()), this, SLOT(addFunction()));
  connect(equation_input, SIGNAL(deleteKeyReceived()), this, SLOT(delFunction()));
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

void EquationWidget::delFunction() {
  window->delFunction(this);
}

Function *EquationWidget::getFunction() const {
  return func;
}

void EquationWidget::setFocus() {
  QWidget::setFocus();
  equation_input->setFocus();
}
