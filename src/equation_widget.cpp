#include "equation_widget.h"
#include "grapher.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <cstring>


EquationWidget::EquationWidget(Function *f, Window *window, Grapher *grapher, QWidget *parent)
  : QWidget(parent), func(f), grapher(grapher), window(window) {
  
  equation_input = new EquationInput(this);
  derivative_button = new QToolButton(this);
  derivative_button->setText("D");
  integral_button = new QToolButton(this);
  integral_button->setCheckable(true);
  integral_button->setText("I");
  layout = new QHBoxLayout;
  layout->addWidget(derivative_button);
  layout->addWidget(integral_button);
  layout->addWidget(equation_input);
  setLayout(layout);
  connect(equation_input, SIGNAL(textChanged(const QString&)), this, SLOT(updateFunction(const QString&)));
  connect(equation_input, SIGNAL(returnPressed()), this, SLOT(addFunction()));
  connect(equation_input, SIGNAL(deleteKeyReceived()), this, SLOT(delFunction()));
  connect(derivative_button, SIGNAL(released()), this, SLOT(setDisplayDerivative()));
  connect(integral_button, SIGNAL(toggled(bool)), this, SLOT(setDisplayIntegral(bool)));
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

void EquationWidget::setDisplayDerivative() {
  func->set_n_derivative((func->get_n_derivative() + 1) % 3);
  if (func->get_n_derivative() != 0)
    derivative_button->setText(tr("D") + QString::number(func->get_n_derivative()));
  else
    derivative_button->setText("D");
  grapher->update();
}

void EquationWidget::setDisplayIntegral(bool show) {
  func->b_graphIntegral = show;
  grapher->update();
}

Function *EquationWidget::getFunction() const {
  return func;
}

void EquationWidget::setFocus() {
  QWidget::setFocus();
  equation_input->setFocus();
}
