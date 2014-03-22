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
    QRect screenRect = desktopWidget->screenGeometry();//ȡ����Ļ��С
    QRect workRect = desktopWidget->availableGeometry();//ȡ�ÿ�����Ļ��С��ȥ����������С��


    // ��Ļ�߶� - �������߶� = �������߶�, �����ֵ����0.������������������
    // ������ֱ�ӱȽ����ߵĸ߶ȣ���ȼ���
    if ( screenRect.height() == workRect.height() )
    {
        // ������������Ͻ�x���������Ļ���Ͻ�x���꣬ ���������ұ�
        if ( screenRect.topLeft().x() == workRect.topLeft().x() )
        {
            qDebug() << "������λ����Ļ�ұ�: " << screenRect.topRight().x() << workRect.topRight().x();
            taskbarPos = TrayNotifyDlg::Right;
        }
        else
        {
            qDebug() << "������λ����Ļ���: " << workRect.topLeft().x() << workRect.topLeft().x();
            taskbarPos = TrayNotifyDlg::Left;
        }
    }
    // �����ȡ�˵��������λ������Ļ�Ϸ����·�
    else /*if(screenRect.width() == workRect.width())*/
    {
        // ������������½�y���������Ļ���½�y���꣬ ���������Ϸ�
        if ( screenRect.bottomRight().y() == workRect.bottomRight().y() )
        {
            qDebug() << "������λ����Ļ�Ϸ�: " << screenRect.topRight().y() << workRect.topRight().y();
            taskbarPos = TrayNotifyDlg::Top;
        }
        else
        {
            qDebug() << "������λ����Ļ�·�: " << screenRect.bottomRight().y() << workRect.bottomRight().y();
            taskbarPos = TrayNotifyDlg::Bottom;
        }
    }

    return taskbarPos;
}

void TrayNotifyDlg::moveTo(const int &xbase, const int &ybase, TrayNotifyDlg::TaskbarPos taskbarPos)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();//ȡ����Ļ��С
    QRect workRect = desktopWidget->availableGeometry();//ȡ�ÿ�����Ļ��С��ȥ����������С��
    QRect thisRect = this->geometry();  //ȡ�ô��ڱ���Ĵ�С,�����frameGeomety�����������ڱ���߶�
    QPoint thisScreenPos = this->mapToGlobal( QPoint(thisRect.bottomRight().x(), thisRect.bottomRight().y()) ); // ��������ת����Ļ����

    qDebug() << thisRect.bottomRight().x() << thisRect.bottomRight().y() << thisScreenPos.x() << thisScreenPos.y();
    // qt�ĵ���˵��bottomRight����Щ������ Note that for historical reasons this function returns QPoint(left() + width() -1, top() + height() - 1).
    // ������ȥ1����λ�����ˣ���������ıȽ���Ҫ�ټ�����һ����λ������
    switch(taskbarPos)
    {
    case TrayNotifyDlg::Left:
    {
        int thisWidth = thisRect.width();
        int taskbarWidth = screenRect.width() - workRect.width(); //���������
        // �����ǰ���ڳ�����Ļ���⣨���³�������y������Ҫ����,����y����Ϊ��Ļy���꣬x�ټ�����������ȡ�
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
        int taskbarWidth = screenRect.width() - workRect.width(); //���������
        // �����ǰ���ڳ�����Ļ���⣨���³�������y������Ҫ����,����y����Ϊ��Ļy���꣬x�ټ�ȥ��������Ⱥ������ȡ�

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
        int taskbarHeight = screenRect.height() - workRect.height(); //�������߶�
        // �����ǰ���ڳ�����Ļ���⣨���ҳ�������x������Ҫ����,����x����Ϊ��Ļx����, y�ټ����������߶ȡ�
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
        int taskbarHeight = screenRect.height() - workRect.height(); //�������߶�

        // �����ǰ���ڳ�����Ļ���⣨���ҳ�������x������Ҫ����,����x����Ϊ��Ļx����, y�ټ�ȥ��������߶Ⱥ��������߶ȡ�
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
                QToolTip::showText(QCursor::pos(), tr("��������Ϣ���ӣ�"), this);
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
