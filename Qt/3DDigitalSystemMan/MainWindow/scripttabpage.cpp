#include "scripttabpage.h"
#include "mainwindow.h"
#include "addscriptwidget.h"

#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>
#include <QPropertyAnimation>

ScriptTabPage::ScriptTabPage(QWidget *parent) :
    QWidget(parent)
{
    m_addscriptBtn  = new QToolButton();
    m_runscriptBtn  = new QToolButton();
    m_editscriptBtn = new QToolButton();
    m_addscriptBtn ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_runscriptBtn ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_editscriptBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_addscriptBtn ->setFixedSize(60, 72);
    m_runscriptBtn ->setFixedSize(60, 72);
    m_editscriptBtn->setFixedSize(60, 72);

    m_addscriptBtn ->setText(tr("添加脚本"));
    m_runscriptBtn ->setText(tr("运行脚本"));
    m_editscriptBtn->setText(tr("编辑脚本"));
    m_addscriptBtn ->setToolTip(tr("添加脚本"));
    m_runscriptBtn ->setToolTip(tr("运行脚本"));
    m_editscriptBtn->setToolTip(tr("编辑脚本"));

    QIcon openscriptlIcon(":/icon/add_script.png");
    QIcon runscriptIcon(":/icon/run_plugin.png");
    QIcon editscriptIcon(":/icon/edit_script.png");
    m_addscriptBtn ->setIcon( openscriptlIcon );
    m_runscriptBtn ->setIcon( runscriptIcon );
    m_editscriptBtn->setIcon( editscriptIcon );
    m_addscriptBtn ->setIconSize(QSize(48,48));
    m_runscriptBtn ->setIconSize(QSize(48,48));
    m_editscriptBtn->setIconSize(QSize(48,48));

    m_addscriptBtn->setCheckable(1);
    m_runscriptBtn->setCheckable(1);
    m_editscriptBtn->setCheckable(1);

//    pushButton1->setStyleSheet("QPushButton:hover{border-image:url(:/icon/add_script.png);color:red}");
//    pushButton2->setStyleSheet("QPushButton:hover{border-image:url(:/icon/run_plugin.png);color:red}");
//    pushButton3->setStyleSheet("QPushButton:hover{border-image:url(:/icon/edit_script.png);color:red}");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_addscriptBtn );
    layout->addWidget(m_runscriptBtn );
    layout->addWidget(m_editscriptBtn);

    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setLayout(layout);

    connect( m_addscriptBtn, SIGNAL(clicked()), this, SLOT(OnAddScriptBtnSlot()) );
    connect(m_runscriptBtn, SIGNAL(clicked()), this, SLOT(OnRunScriptBtnSlot()));
    connect(m_editscriptBtn, SIGNAL(clicked()), this, SLOT(OnEditScriptBtnSlot()));
}

void ScriptTabPage::stateMachineWidget()
{
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        return;
    }
    QRect rect1 = main->m_addScriptWidget->geometry();
    QRect rect = QRect(2,-rect1.height(), rect1.width(),rect1.height());
    QStateMachine *machine = new QStateMachine;     //新建状态机
    QState *state1 = new QState(machine);     //状态1
    state1->assignProperty(main->m_addScriptWidget,"geometry", QRect(rect.x(), rect.y(), rect.width(), rect.height()));    //绑定geomertry属性
    machine->setInitialState(state1);     //state1设为初始化状态
    QState *state2 = new QState(machine);
    state2->assignProperty(main->m_addScriptWidget, "geometry", QRect(rect.x(), 2, rect.width(), rect.height()));

    QPropertyAnimation *ani=new QPropertyAnimation(main->m_addScriptWidget,"geometry");
    ani->setDuration(1000);
    ani->setEasingCurve(QEasingCurve::OutBounce);   //动画效果—弹跳

    //QSignalTransition *transition1 = state2->addTransition(main->m_addModelWidget->m_pbtnOk, SIGNAL(clicked()),state1);      //动画触发信号
    //transition1->addAnimation(ani);
    QSignalTransition *transition2 = state2->addTransition(main->m_addScriptWidget->m_pbtnCancel, SIGNAL(clicked()),state1);
    transition2->addAnimation(ani);

    QSignalTransition *transition3 = state2->addTransition(m_addscriptBtn, SIGNAL(clicked()),state1);
    transition3->addAnimation(ani);
    QSignalTransition *transition5 = state2->addTransition(m_runscriptBtn, SIGNAL(clicked()),state1);
    transition5->addAnimation(ani);
    QSignalTransition *transition6 = state2->addTransition(m_editscriptBtn, SIGNAL(clicked()),state1);
    transition6->addAnimation(ani);

    QSignalTransition *transition4 = state1->addTransition(m_addscriptBtn, SIGNAL(clicked()),state2);
    transition4->addAnimation(ani);

    machine->start();     //开启状态机
}

void ScriptTabPage::OnAddScriptBtnSlot()
{
    m_runscriptBtn->setChecked(0);
    m_editscriptBtn->setChecked(0);
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        QMessageBox::warning(this->window(), tr("未知错误:无法解析的主应用程序窗口指针"), tr("访问出错"), QMessageBox::Ok);
        return;
    }
    connect( main->m_addScriptWidget, SIGNAL(hideScriptWgtSignal()), this, SLOT(OnUpdateBtnSlot()) );
}

void ScriptTabPage::OnRunScriptBtnSlot()
{
    m_addscriptBtn->setChecked(0);
    m_editscriptBtn->setChecked(0);
}

void ScriptTabPage::OnEditScriptBtnSlot()
{
    m_runscriptBtn->setChecked(0);
    m_addscriptBtn->setChecked(0);
}

void ScriptTabPage::OnUpdateBtnSlot()
{
    m_runscriptBtn->setChecked(0);
    m_addscriptBtn->setChecked(0);
    m_editscriptBtn->setChecked(0);

    MainWindow* main = static_cast<MainWindow*>( this->window() );
    disconnect( main->m_addScriptWidget, SIGNAL(hideScriptWgtSignal()), this, SLOT(OnUpdateBtnSlot()) );
}
