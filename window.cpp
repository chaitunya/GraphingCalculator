#include "grapher.h"
#include "window.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    grapher = new Grapher;
    equationLineEdit = new QLineEdit;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(grapher, 0, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("Sigma"));
}
