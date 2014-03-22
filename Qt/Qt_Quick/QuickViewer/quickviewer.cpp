#include "quickviewer.h"
#include <QApplication>
#include <QGridLayout>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QAbstractScrollArea>

#include <QDebug>

QuickViewer::QuickViewer(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background:transparent;");
    this->setContentsMargins(0,0,0,0);
    this->setObjectName("QuickViewer");


    m_isLeftBtnPressed = false;

    initQmlUi();
}

QuickViewer::~QuickViewer()
{
    delete m_qmlRootObj;
    delete m_qmlView;
}

void QuickViewer::initQmlUi()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    m_qmlView = new QDeclarativeView(this);
    m_qmlView->setObjectName("qmlView"); //必须设置对象名，以便拖动窗口时可以方便找到该对象
    m_qmlView->installEventFilter(this);
    m_qmlView->viewport()->installEventFilter(this);
    m_qmlView->viewport()->setObjectName("viewport"); //同上
    m_qmlView->setSource( QUrl("../quickDemo0/quickDemo0.qml") );
    m_qmlRootObj = m_qmlView->rootObject();

    gridLayout->addWidget(m_qmlView);
    gridLayout->setContentsMargins(0,0,0,0);
    this->setLayout(gridLayout);
    this->setFixedSize(m_qmlView->width(), m_qmlView->height());

    connect( m_qmlRootObj, SIGNAL(quit()), this, SLOT(quit()) );

}
void QuickViewer::quit()
{
    qApp->quit();
}

bool QuickViewer::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget*>(obj);
    if ( !widget )
    {
        qDebug() << "can not find object.";
        return false;
    }

    if ( event->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if( mouseEvent->button() == Qt::LeftButton &&
                widget->objectName() == "qmlView" )
        {
            QPoint thisPos = mouseEvent->globalPos();
            m_startPos = thisPos - this->pos();
            m_isLeftBtnPressed = true;
        }
    }
    if( event->type() == QEvent::MouseButtonRelease )
    {
        m_isLeftBtnPressed = false;
    }

    if( widget->objectName() == "viewport" )
    {
        if( event->type() ==  QMouseEvent::MouseMove && m_isLeftBtnPressed)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint thisPos = mouseEvent->globalPos();
            this->move( thisPos - m_startPos);
        }
    }
    return QWidget::eventFilter(obj, event);
}





