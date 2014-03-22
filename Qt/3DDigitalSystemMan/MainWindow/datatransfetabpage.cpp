#include "../common/include/datatransfeplan.h"
#include "../common/include/datatransfehistory.h"
#include "../common/include/datatransfeoptions.h"
#include "../common/include/datatransfe.h"
#include "datatransfetabpage.h"
#include "mainwindow.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>

DataTransfeTabPage::DataTransfeTabPage(QWidget *parent) :
    QWidget(parent)
{

    m_dataTransfePlan = NULL;
    m_dataTransfeHistory = NULL;
    m_dataTransfeOptions = NULL;

    m_dataCommitBtn  = new QToolButton();
    m_dataHistoryBtn = new QToolButton();
    m_dataPlansBtn   = new QToolButton();

    m_dataCommitBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_dataHistoryBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_dataPlansBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_dataCommitBtn->setFixedSize(60, 72);
    m_dataHistoryBtn->setFixedSize(60, 72);
    m_dataPlansBtn->setFixedSize(60, 72);

    m_dataCommitBtn ->setText(tr("数据提交"));
    m_dataPlansBtn  ->setText(tr("计划提交"));
    m_dataHistoryBtn->setText(tr("历史提交"));

    m_dataCommitBtn ->setToolTip(tr("数据提交"));
    m_dataPlansBtn  ->setToolTip(tr("计划提交"));
    m_dataHistoryBtn->setToolTip(tr("历史提交"));

    QIcon dataCommit(":/icon/data_commit.png");
    QIcon dataPlan(":/icon/data_plan.png");
    QIcon dataHistory(":/icon/data_history.png");

    m_dataCommitBtn ->setIcon( dataCommit );
    m_dataPlansBtn  ->setIcon( dataPlan );
    m_dataHistoryBtn->setIcon( dataHistory );

    m_dataCommitBtn ->setIconSize(QSize(48,48));
    m_dataPlansBtn  ->setIconSize(QSize(48,48));
    m_dataHistoryBtn->setIconSize(QSize(48,48));

    m_dataCommitBtn->setCheckable(1);
    m_dataCommitBtn->setChecked(1);
    m_dataHistoryBtn->setCheckable(1);
    m_dataPlansBtn->setCheckable(1);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget( m_dataCommitBtn  );
    layout->addWidget( m_dataPlansBtn   );
    layout->addWidget( m_dataHistoryBtn );

    layout->addStretch(1);
    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setLayout(layout);

    setSlotSignalConnect();
}

void DataTransfeTabPage::setSlotSignalConnect()
{
    connect( m_dataCommitBtn, SIGNAL(clicked()), this, SLOT(OnDataCommitBtnSlot()) );
    connect( m_dataPlansBtn, SIGNAL(clicked()), this, SLOT(OnDataPlansBtnSlot()) );
    connect( m_dataHistoryBtn, SIGNAL(clicked()), this, SLOT(OnDataHistoryBtnSlot()) );
}

void DataTransfeTabPage::OnDataCommitBtnSlot()
{
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        QMessageBox::warning(this->window(), tr("未知错误:无法解析的主应用程序窗口指针"), tr("访问出错"), QMessageBox::Ok);
        return;
    }

    m_dataPlansBtn->setChecked(0);
    m_dataHistoryBtn->setChecked(0);
    m_dataCommitBtn->setChecked(1);

    //hide other
    if( m_dataTransfeHistory && !m_dataTransfeHistory->isHidden() )
        m_dataTransfeHistory->setHidden(1);
    if(m_dataTransfePlan && !m_dataTransfePlan->isHidden())
        m_dataTransfePlan->setHidden(1);

    //show data commit
    if(main->m_splitterLeft && main->m_splitterLeft->isHidden())
        main->m_splitterLeft->setShown(1);
    if(main->m_dataTransfe && main->m_dataTransfe->isHidden())
        main->m_dataTransfe->setShown(1);
}

void DataTransfeTabPage::OnDataPlansBtnSlot()
{
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        QMessageBox::warning(this->window(), tr("未知错误:无法解析的主应用程序窗口指针"), tr("访问出错"), QMessageBox::Ok);
        return;
    }

    m_dataCommitBtn->setChecked(0);
    m_dataHistoryBtn->setChecked(0);
    m_dataPlansBtn->setChecked(1);

    //hide other
    if(main->m_dataTransfe && !main->m_dataTransfe->isHidden())
        main->m_dataTransfe->setHidden(1);
    if( m_dataTransfeHistory && !m_dataTransfeHistory->isHidden() )
        m_dataTransfeHistory->setHidden(1);

    //show plan
    if(main->m_splitterLeft && main->m_splitterLeft->isHidden())
        main->m_splitterLeft->setShown(1);
    if( m_dataTransfeOptions && m_dataTransfeOptions->isHidden() )
        m_dataTransfeOptions->setShown(1);
    if(!m_dataTransfePlan)
    {
        m_dataTransfePlan = new DataTransfePlan(main->m_splitterRight);
    }
    else
    {
        if(m_dataTransfePlan->isHidden())
            m_dataTransfePlan->setShown(1);
    }
}

void DataTransfeTabPage::OnDataHistoryBtnSlot()
{
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        QMessageBox::warning(this->window(), tr("未知错误:无法解析的主应用程序窗口指针"), tr("访问出错"), QMessageBox::Ok);
        return;
    }

    m_dataCommitBtn->setChecked(0);
    m_dataPlansBtn->setChecked(0);
    m_dataHistoryBtn->setChecked(1);

//    main->m_dataTransfe->setHidden(1);

    //hide other
    if(m_dataTransfePlan && !m_dataTransfePlan->isHidden())
        m_dataTransfePlan->setHidden(1);
    if(main->m_dataTransfe && !main->m_dataTransfe->isHidden())
        main->m_dataTransfe->setHidden(1);
    if(main->m_splitterLeft && main->m_splitterLeft->isHidden())
        main->m_splitterLeft->setShown(1);
    if(m_dataTransfeOptions && !m_dataTransfeOptions->isHidden())
        m_dataTransfeOptions->setHidden(1);

    //show history
    if( !m_dataTransfeHistory)
    {
        m_dataTransfeHistory = new DataTransfeHistory(main->m_splitterRight);
    }
    else
    {
        if(m_dataTransfeHistory->isHidden())
            m_dataTransfeHistory->setShown(1);
    }
}






