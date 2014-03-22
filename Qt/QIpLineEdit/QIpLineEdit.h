/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/3 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef QIPLINEEDIT_H
#define QIPLINEEDIT_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QKeyEvent;
QT_END_NAMESPACE

class QIpLineEdit : public QFrame
{
    Q_OBJECT

public:
    QIpLineEdit(QWidget *parent = 0);
    ~QIpLineEdit(){}

signals:
    void textChanged( QLineEdit* edit );

public slots:
    void setText( QLineEdit* edit );

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    enum
    {
        MAX_DIGITS = 3, //
        IP_SIZE   = 4//
    };

    QLineEdit *(m_lineEdit[IP_SIZE]);

    void moveNextLineEdit (int i);
    void movePrevLineEdit (int i);
};

#endif
