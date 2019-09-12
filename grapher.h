#ifndef GRAPHER_H
#define GRAPHER_H

#include <QWidget>
#include <QBrush>
#include <QPen>

class Grapher : public QWidget
{
    Q_OBJECT
public:
    explicit Grapher(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updatePath(QPainterPath *path);
    QPen pen;
    QBrush brush;
};

#endif // GRAPHER_H
