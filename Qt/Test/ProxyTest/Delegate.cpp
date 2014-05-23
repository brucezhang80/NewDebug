#include "Delegate.h"
#include <QDebug>
#include <QTimer>

Delegate::Delegate(QObject *parent) :
    Protocol(parent),
    m_value(0)
{
}

int Delegate::value() const
{
    return m_value;
}

void Delegate::setValue(int value)
{
    m_value = value;
}

void Delegate::start()
{
    qDebug() << "Delegate starting...";
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(100);
}

void Delegate::update()
{
    if( m_value == 100)
    {
        qDebug() << "Delegate finished.";
        m_timer->stop();
        emit finished();
        return;
    }
    m_value += 1;
    emit updated();
}
