#include "slider.h"
#include <QDebug>

Slider::Slider(Qt::Orientation orientation, QWidget *parent):
    QSlider(parent)
{
    this->setRange(32, 256);
    this->setValue(48);
    this->setPageStep(1);
    this->setOrientation(orientation);
    const int size = this->value();
    emit sendValueSignal(size);
}

Slider::~Slider()
{
}

int Slider::tmult_ok(int x, int y)
{
    int p = x * y;

    return !x || p / x == y;
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        int num = this->maximum() - this->minimum();
        int mousePos = ((double)event->x()/width()) * num;//这里必需要double，否则相除将返回0造成出错
        int pos =  mousePos + minimum() +1;

        if(abs(pos-this->value()) >  this->pageStep())
            setValue(pos);
    }

    QSlider::mousePressEvent(event);
}

void Slider::mouseMoveEvent(QMouseEvent *event)
{
    this->setToolTip(QString("%1").arg(this->value()));
    QSlider::mouseMoveEvent(event);
}

void Slider::updateValueSlot()
{
    const int size = this->value();
    emit sendValueSignal(size);
}
