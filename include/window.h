#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QtWidgets>
#include <vector>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE
class Grapher;

class Function;
class EquationWidget;

struct EquationPair {
    Function *func;
    EquationWidget *widget;
};
    

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    std::vector<EquationWidget*> *equationWidgets;
    void addFunction(int index);
    void addFunction(EquationWidget *eqPair);
    void delFunction(EquationWidget *eqPair);

private:

    Grapher *grapher;
    QVBoxLayout *equations_layout;
    QScrollArea *scrollArea;
};

#endif // WINDOW_H
