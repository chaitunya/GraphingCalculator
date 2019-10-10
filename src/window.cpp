#include "grapher.h"
#include "window.h"
#include "function.h"
#include "equation_widget.h"
#include <algorithm>
#include <iterator>
#include <iostream>


Window::Window(QWidget *parent)
  : QWidget(parent) {
  equationWidgets = new std::vector<EquationWidget*>;
  grapher = new Grapher(equationWidgets);
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QSplitter *split = new QSplitter;
  split->setOrientation(Qt::Horizontal);
  scrollArea = new QScrollArea;
  QWidget *widget = new QWidget;
  scrollArea->setWidget(widget);
  scrollArea->setWidgetResizable(true);
  equations_layout = new QVBoxLayout;
  equations_layout->setAlignment(Qt::AlignTop);
  widget->setLayout(equations_layout);
  addFunction(0);
  split->addWidget(grapher);
  split->addWidget(scrollArea);
  mainLayout->addWidget(split);
  setLayout(mainLayout);
  setWindowTitle(tr("Sigma"));
  (*equationWidgets)[0]->setFocus();
}

void Window::addFunction(EquationWidget *eqWidg) {
  auto it = std::find(equationWidgets->begin(), equationWidgets->end(), eqWidg);
  addFunction(it - equationWidgets->begin() + 1);
}

void Window::addFunction(int index) {
  Function *func = new Function("");
  EquationWidget *widget = new EquationWidget(func, this, grapher);
  equationWidgets->insert(equationWidgets->begin() + index, widget);
  equations_layout->insertWidget(index, widget);
  widget->setFocus();
}

void Window::delFunction(EquationWidget *eqWidg) {
  if (equationWidgets->size() > 1) {
    auto it = std::find(equationWidgets->begin(), equationWidgets->end(), eqWidg);
    delete eqWidg->getFunction();
    delete eqWidg;
    equationWidgets->erase(it);
    if (it - equationWidgets->begin() != 0)
      (*equationWidgets)[it - equationWidgets->begin() - 1]->setFocus();
    else
      (*equationWidgets)[0]->setFocus();
  }
}
