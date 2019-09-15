#include "grapher.h"
#include "window.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    grapher = new Grapher;
    grapher->addFunction(Function([](double x) {
        return x*x;
    }));
    grapher->addFunction(Function([](double x) {
        return (x - 5) * (x + 2);
    }));
    equationLineEdit = new QLineEdit;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(grapher, 0, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("Sigma"));
}
