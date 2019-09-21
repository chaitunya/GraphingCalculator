#include "grapher.h"
#include "window.h"
#include "function.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    grapher = new Grapher;
    Function f([](double x) {
        return x*x;
    });
    f.graph_derivative = true;
    f.graph_integral = true;
    grapher->addFunction(f);
    equationLineEdit = new QLineEdit;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(grapher, 0, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("Sigma"));
}
