#include "traynotifydlg.h"
#include "ui_traynotifydlg.h"
#include <QTimer>
#include <QDesktopWidget>
#include <QToolTip>

#include <QDebug>

TrayNotifyDlg::TrayNotifyDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrayNotifyDlg)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
//    setWindowFlags( Qt::Popup );
    this->setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking( true );
    this->installEventFilter(this);
    ui->lb_message->installEventFilter(this);

    m_timeClose = new QTimer(this);
    connect( m_timeClose, SIGNAL(timeout()), this, SLOT(close()) );
    m_timeClose->start(4000);
}

TrayNotifyDlg::~TrayNotifyDlg()
{
    delete ui;
}

void TrayNotifyDlg::setNotifyMessage(const QString &information)
{
    ui->lb_message->setText(information);
}

TrayNotifyDlg::TaskbarPos TrayNotifyDlg::taskbarDirect()
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//取得屏幕大小
    QRect workRect = desktopWidget->availableGeometry();//取得可用屏幕大小（去除任务栏大小）


    // 屏幕高度 - 工作区高度 = 任务栏高度, 如果差值等于0.则任务栏在左右两边
    // 或者是直接比较两者的高度，相等即可
    if ( screenRect.height() == workRect.height() )
    {
        // 如果工作区左上角x坐标等于屏幕左上角x坐标， 则任务栏右边
        if ( screenRect.topLeft().x() == workRect.topLeft().x() )
        {
            qDebug() << "任务栏位于屏幕右边: " << screenRect.topRight().x() << workRect.topRight().x();
            taskbarPos = TrayNotifyDlg::Right;
        }
        else
        {
            qDebug() << "任务栏位于屏幕左边: " << workRect.topLeft().x() << workRect.topLeft().x();
            taskbarPos = TrayNotifyDlg::Left;
        }
    }
    // 宽度相等。说明任务栏位置在屏幕上方或下方
    else /*if(screenRect.width() == workRect.width())*/
    {
        // 如果工作区右下角y坐标等于屏幕右下角y坐标， 则任务栏上方
        if ( screenRect.bottomRight().y() == workRect.bottomRight().y() )
        {
            qDebug() << "任务栏位于屏幕上方: " << screenRect.topRight().y() << workRect.topRight().y();
            taskbarPos = TrayNotifyDlg::Top;
        }
        else
        {
            qDebug() << "任务栏位于屏幕下方: " << screenRect.bottomRight().y() << workRect.bottomRight().y();
            taskbarPos = TrayNotifyDlg::Bottom;
        }
    }

    return taskbarPos;
}

void TrayNotifyDlg::moveTo(const int &xbase, const int &ybase, TrayNotifyDlg::TaskbarPos taskbarPos)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//取得屏幕大小
    QRect workRect = desktopWidget->availableGeometry();//取得可用屏幕大小（去除任务栏大小）
    QRect thisRect = this->geometry();  //取得窗口本身的大小,如果是frameGeomety，则会包含窗口标题高度
    QPoint thisScreenPos = this->mapToGlobal( QPoint(thisRect.bottomRight().x(), thisRect.bottomRight().y()) ); // 窗口坐标转到屏幕坐标

    qDebug() << thisRect.bottomRight().x() << thisRect.bottomRight().y() << thisScreenPos.x() << thisScreenPos.y();
    // qt文档中说到bottomRight等这些函数： Note that for historical reasons this function returns QPoint(left() + width() -1, top() + height() - 1).
    // 都被送去1个单位像素了，所以这里的比较需要再加上这一个单位的像素
    switch(taskbarPos)
    {
    case TrayNotifyDlg::Left:
    {
        int thisWidth = thisRect.width();
        int taskbarWidth = screenRect.width() - workRect.width(); //任务栏宽度
        // 如果当前窗口超出屏幕以外（向下超出），y坐标需要上移,设置y坐标为屏幕y坐标，x再加上任务栏宽度。
        qDebug() <<thisScreenPos.y() << screenRect.bottomRight().y();
        if(thisScreenPos.y() > screenRect.bottomRight().y()+1)
        {
            int thisY = screenRect.bottomLeft().y() - thisRect.height();
            this->move(workRect.bottomLeft().x(), thisY);
        }
        else
            this->move(workRect.bottomLeft().x(), ybase);
    };break;

    case TrayNotifyDlg::Right:
    {
        int thisWidth = thisRect.width();
        int taskbarWidth = screenRect.width() - workRect.width(); //任务栏宽度
        // 如果当前窗口超出屏幕以外（向下超出），y坐标需要上移,设置y坐标为屏幕y坐标，x再减去任务栏宽度和自身宽度。

        qDebug() << thisScreenPos.y() << screenRect.bottomRight().y();

        if(thisScreenPos.y() > screenRect.bottomRight().y()+1)
        {
            int thisY = screenRect.bottomRight().y() - thisRect.height();
            this->move(workRect.bottomRight().x() - thisWidth-8, thisY);
        }
        else
            this->move(workRect.bottomRight().x() - thisWidth-8, ybase);
    };break;

    case TrayNotifyDlg::Top:
    {
        int thisHeight = thisRect.height();
        int taskbarHeight = screenRect.height() - workRect.height(); //任务栏高度
        // 如果当前窗口超出屏幕以外（向右超出），x坐标需要左移,设置x坐标为屏幕x坐标, y再加上任务栏高度。
        if(thisScreenPos.x() > screenRect.bottomRight().x()+1)
        {
            int thisX = screenRect.topRight().x() - thisRect.width();
            this->move(thisX, workRect.topRight().y());
        }
        else
            this->move(xbase, workRect.topRight().y());
    };break;

    case TrayNotifyDlg::Bottom:
    {
        int thisHeight = thisRect.height();
        int taskbarHeight = screenRect.height() - workRect.height(); //任务栏高度

        // 如果当前窗口超出屏幕以外（向右超出），x坐标需要左移,设置x坐标为屏幕x坐标, y再减去窗口自身高度和任务栏高度。
        qDebug() << thisScreenPos.x() << screenRect.bottomRight().x();

        if(thisScreenPos.x() > screenRect.bottomRight().x()+1)
        {
            int thisX = screenRect.bottomRight().x() - thisRect.width();
            this->move(thisX, workRect.bottomRight().y() - thisHeight - taskbarHeight);
        }
        else
            this->move(xbase, workRect.bottomRight().y() - thisHeight - taskbarHeight);
    };break;

    default:
        break;
    }
}

bool TrayNotifyDlg::eventFilter(QObject *obj, QEvent *event)
{
    QLabel *lb = static_cast<QLabel*>(obj);
    TrayNotifyDlg *_this = static_cast<TrayNotifyDlg*>(obj);

    if(lb != ui->lb_message || _this != this) return false;
    switch( event->type() )
    {
        case QEvent::Enter:
        {
                m_timeClose->stop();
                QToolTip::showText(QCursor::pos(), tr("单击打开消息盒子！"), this);
                this->setCursor(Qt::PointingHandCursor);
        };break;

        case QEvent::Leave:
        {
            m_timeClose->start(3000);
            this->setCursor(Qt::ArrowCursor);
        };break;
        case QEvent::MouseButtonPress:
        {
            emit showMessage(nameFrom);
            this->close();
            this->deleteLater();
        };break;
    }

//     return QWidget::eventFilter(obj, event);
}


void TrayNotifyDlg::mousePressEvent(QMouseEvent *)
{
    emit showMessage(nameFrom);
    this->close();
    this->deleteLater();
}
