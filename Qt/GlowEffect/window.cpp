#include "window.h"
#include <QLabel>
#include <QHBoxLayout>
#include "gloweffect.h"
#include <QToolTip>


Window::Window(QWidget *parent)
    : QWidget(parent)
{
    lbl = new QLabel("Let's have a try!", this);
    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(lbl);
    setLayout(layout);

    setFont(QToolTip::font());

    GlowEffect *effect = new GlowEffect(this);
    effect->setRadius(5);
    effect->setGlowColor(Qt::blue);
    lbl->setGraphicsEffect(effect);
}

Window::~Window()
{
}
