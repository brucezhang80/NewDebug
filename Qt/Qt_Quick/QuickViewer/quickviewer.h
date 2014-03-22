#ifndef QUICKVIEWER_H
#define QUICKVIEWER_H

#include <QtGui/QWidget>
#include <QMouseEvent>
#include <QEvent>

class QDeclarativeView;

class QuickViewer : public QWidget
{
    Q_OBJECT
    
public:
    QuickViewer(QWidget *parent = 0);
    ~QuickViewer();

private slots:
    void quit();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    void initQmlUi();

    QDeclarativeView    *m_qmlView;
    QObject             *m_qmlRootObj;
    QPoint              m_startPos;
    QPoint              m_endPos;
    bool                m_isLeftBtnPressed;
};

#endif // QUICKVIEWER_H
