#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QVector>
#include <QString>
#include <QTime>
#include <time.h>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->number1Edit->installEventFilter( this );
    ui->number2Edit->installEventFilter( this );
    this->installEventFilter( this );

    connect( ui->computeBtn, SIGNAL(clicked()), this, SLOT(doCompute()) );
    connect( ui->cleanBtn, SIGNAL(clicked()), this, SLOT(doClean()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::multiply(const QVector<int> &multiplier,
                          const QVector<int> &multiplicand,
                          QVector<int> &result)
{
    int k = 0;
    for (int i = 0; i < multiplier.size(); ++i)
    {
        k = i;
        for ( int j = 0; j < multiplicand.size(); ++j)
        {
            result[k++] += multiplier[i] * multiplicand[j];
        }
    }

    for ( k = result.size() - 1; k >= 0; --k)
    {
        if (result[k] > 9)
        {
            if (k != 0)
            {
                result[k-1] += result[k] / 10;
                result[k] %= 10;
            }
            else   //如果是第一项大于9则添加一项于头部
            {
                int tmp = result[k] / 10;
                result[k] %=10;
                result.insert(result.begin(), tmp);
            }
        }
    }
}

void MainWindow::doCompute()
{
    QString strFirst = ui->number1Edit->toPlainText();
    QString strSecond = ui->number2Edit->toPlainText();

    if( strFirst.isEmpty() || strSecond.isEmpty() )
        return;

    QTime time;
    time.start();
    double time_Start = (double)clock(); //开始时间

    QVector<int> firstVec, seconedVec;
    firstVec.reserve(strFirst.size());
    seconedVec.reserve(strSecond.size());

    for (int i = 0; i < strFirst.size(); ++i)
        firstVec.push_back( strFirst.at(i).toAscii() - '0' );

    for (int i = 0; i < strSecond.size(); ++i)
        seconedVec.push_back( strSecond.at(i).toAscii() - '0' );


    QVector<int> resultVec( firstVec.size() + seconedVec.size() - 1, 0);
    multiply(firstVec, seconedVec, resultVec);


    QString result = "";
    for (int i = 0; i < resultVec.size(); ++i)
    {
        result += QString("%1").arg(resultVec[i]);
    }
    double time_Finish = (double)clock(); //结束时间
    qDebug("Time elapsed: %2f ms", time_Finish - time_Start );

    ui->resultEdit->setText(result);
    ui->timeLabel->setText( QString("用时 %1 毫秒").arg( time.elapsed() ) );

    int cnt = ui->resultEdit->toPlainText().count();
    ui->label3->setText( QString("%1 位数").arg( cnt ) );
}

void MainWindow::doClean()
{
    ui->number1Edit->clear();
    ui->number2Edit->clear();
    ui->resultEdit->clear();
    ui->label1->setText( QString("%1  位数").arg( 0 ) );
    ui->label2->setText( QString("%1  位数").arg( 0 ) );
    ui->label3->setText( QString("%1  位数").arg( 0 ) );
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        qDebug("Ate key press %d", keyEvent->key());

        switch( keyEvent->key() )
        {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        {
            QTextEdit* edit = qobject_cast<QTextEdit*>(obj);
            if( edit == ui->number1Edit )
            {
                int cnt = ui->number1Edit->toPlainText().count();
                ui->label1->setText( QString("%1  位数").arg( cnt+1 ) );
            }
            else if( edit == ui->number2Edit )
            {
                int cnt = ui->number2Edit->toPlainText().count();
                ui->label2->setText( QString("%1  位数").arg( cnt+1 ) );
            }
        };
        break;
        case Qt::Key_Backspace:
        {
            QTextEdit* edit = qobject_cast<QTextEdit*>( obj );

            if( edit == ui->number1Edit )
            {
                int cnt = ui->number1Edit->toPlainText().count();
                if( cnt != 0)
                    ui->label1->setText( QString("%1  位数").arg( cnt-1 ) );
            }
            else if( edit == ui->number2Edit )
            {
                int cnt = ui->number2Edit->toPlainText().count();
                if( cnt != 0)
                    ui->label2->setText( QString("%1  位数").arg( cnt-1 ) );
            }
        };
        break;

        case Qt::Key_Escape:
            this->close();
            break;
        default:
            return true;
        }


//        return true;
    }

    return QObject::eventFilter(obj, event);
}
















