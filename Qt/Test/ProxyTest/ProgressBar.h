#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "Delegate.h"
#include <QProgressBar>

class ProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = 0);
    ~ProgressBar();

    void setDelegate(Delegate *delegate);
    void start();

signals:

public slots:
    void updated();

private:
    Delegate*   m_delegate;
};

#endif // PROGRESSBAR_H
