#include "ProgressBar.h"

#include <QDebug>

ProgressBar::ProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
    m_delegate = 0;
    this->setRange(0,100);
}

ProgressBar::~ProgressBar()
{
    disconnect( m_delegate, SIGNAL(updated()), this, SLOT(updated()) );
    delete m_delegate;
}

void ProgressBar::setDelegate(Delegate *delegate)
{
    m_delegate = delegate;

    connect( m_delegate, SIGNAL(updated()), this, SLOT(updated()) );
}

void ProgressBar::start()
{
    m_delegate->start();
}

void ProgressBar::updated()
{
    this->setValue(m_delegate->value());
}

