#ifndef PROGRESSPROTOCOL_H
#define PROGRESSPROTOCOL_H

#include "Protocol.h"

class QTimer;

class Delegate : public Protocol
{
    Q_OBJECT
public:
    explicit Delegate(QObject *parent = 0);

    virtual void start();
    int value() const;
    void setValue(int value);

signals:
    void updated();
    void finished();

private slots:
    void update();

protected:
    Delegate &operator =(Delegate *delegate)
    {
        this->m_value = delegate->m_value;

        return *this;
    }

private:
    QTimer* m_timer;
    int     m_value;
};

#endif // PROGRESSPROTOCOL_H
