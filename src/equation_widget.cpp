#include "equation_widget.h"
#include "grapher.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <cstring>


EquationWidget::EquationWidget(Function *f, Window *window, Grapher *grapher, QWidget *parent)
  : QWidget(parent), func(f), grapher(grapher), window(window) {
  connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateFunction(const QString&)));
  connect(this, SIGNAL(returnPressed()), this, SLOT(addFunction()));
}

void EquationWidget::updateFunction(const QString &text) {
  std::cout << text.toStdString() << std::endl;
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
  if (event->key() == Qt::Key_Backspace && text() == QString("")) {
    window->delFunction(this);
  } else if (event->key() <= Qt::Key_A && event->key() <= Qt::Key_Z) {
    // Is a letter
    text_.append(event->text());
    textChanged(text_);
    QWidget::keyPressEvent(event);
  } else if (event->key() == Qt::Key_Return) {
    returnPressed();
  }
  
}

QString EquationWidget::text() const {
  return text_;
}
