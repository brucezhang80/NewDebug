#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QMouseEvent>

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(Qt::Orientation orientation, QWidget *parent = 0);
    ~Slider();

    int tmult_ok(int x, int y);//检测两个整数相乘是否溢出

signals:
        void sendValueSignal(const int&);

public slots:
        void updateValueSlot();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


};

#endif // SLIDER_H
