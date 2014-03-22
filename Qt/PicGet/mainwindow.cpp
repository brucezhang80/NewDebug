#include "mainwindow.h"
#include "pictureview.h"
#include "projectview.h"
#include "newprojectdlg.h"

#include <QtCore>
#include <QtGui>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QSplitter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    this->initMainWindow();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::initMainWindow()
{
    // Create common actions
    m_addNewProjectAction = new QAction(QIcon(":/icons/edit_add.png"), tr("新增下载(&A)"), this);
    m_playProjectAction = new QAction(QIcon(":/icons/palyer_play.png"), tr("开始下载(&A)"), this);
    m_pauseProjectAction = new QAction(QIcon(":/icons/player_pause.png"), tr("暂停下载(&S)"), this);
    m_stopProjectAction = new QAction(QIcon(":/icons/player_stop.png"), tr("停止下载(&S)"), this);
    m_removeProjectAction = new QAction(QIcon(":/icons/edit_remove.png"), tr("&删除下载(&D)"), this);
    // File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(m_addNewProjectAction);
    fileMenu->addAction(m_playProjectAction);
    fileMenu->addAction(m_pauseProjectAction);
    fileMenu->addAction(m_stopProjectAction);
    fileMenu->addAction(m_removeProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(QIcon(":/icons/exit.png"), tr("退出(&E)"), this, SLOT(close()));

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(tr("关于(&A)"), this, SLOT(aboutApp()));
    //helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

    // Top toolbar
    QToolBar *topBar = new QToolBar(tr("Tools"));
    addToolBar(Qt::TopToolBarArea, topBar);
    topBar->setMovable(true);
    topBar->addAction(m_addNewProjectAction);
    topBar->addAction(m_playProjectAction);
    topBar->addAction(m_pauseProjectAction);
    topBar->addAction(m_stopProjectAction);
    topBar->addAction(m_removeProjectAction);
    topBar->addSeparator();
    m_downActionTool = topBar->addAction(QIcon(tr(":/icons/1downarrow.png")), tr("下移"));
    m_upActionTool = topBar->addAction(QIcon(tr(":/icons/1uparrow.png")), tr("上移"));

    // Url toolbar
    QToolBar *urlBar = new QToolBar(tr("Url"));
    addToolBar(Qt::TopToolBarArea, urlBar);
    urlBar->setMovable(true);

    QLabel *urlTile = new QLabel(tr("网址："));
    m_newProjectUrl = new QComboBox(this);
    m_newProjectUrl->setEditable(true);
    m_newProjectUrl->setFixedWidth(320);
    QLabel *proTile = new QLabel(tr("项目名："));
    m_newProjectTitle = new QComboBox(this);
    m_newProjectTitle->setEditable(true);
    m_newProjectTitle->setFixedWidth(160);

    urlBar->addWidget(urlTile);
    urlBar->addWidget(m_newProjectUrl);
    urlBar->addWidget(proTile);
    urlBar->addWidget(m_newProjectTitle);



    //view
    QSplitter *splitter = new QSplitter(this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    m_projectView = new ProjectView(this);
    m_pictureView = new PictureView(this);
    m_newProjectDlg = new NewProjectDlg(this);

    hLayout->addWidget(m_projectView);
    hLayout->addWidget(m_pictureView);
    splitter->setLayout(hLayout);

    //设置分割窗口比例
    splitter->setStretchFactor(0,3);
    splitter->setStretchFactor(1,6);
    setCentralWidget(splitter);

    setConnect();

    // Load settings and start
    setWindowTitle(tr("图片下载客户端 v0.1 - copyleft (c) 2012 Power by 麻菜"));
    setActionsEnabled();
    //QMetaObject::invokeMethod(this, "loadSettings", Qt::QueuedConnection);
}

void MainWindow::setConnect()
{
    connect( m_addNewProjectAction, SIGNAL(triggered()), m_newProjectDlg, SLOT( open() ) );
    connect( m_removeProjectAction, SIGNAL(triggered()), this, SLOT(removeProjectSlot()) );
//    connect( m_projectView, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(getProjectUrlSlot(QTreeWidgetItem *, int)) );
}


void MainWindow::aboutApp()
{
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/icons/peertopeer.png"));

    QLabel *text = new QLabel;
    text->setWordWrap(true);
    text->setText("<p>The <b>Picture Download Client</b> example demonstrates batch"
                  " download jpg pictures from a specify http url."
                  " application using Qt's network and thread classes.</p>"
                  "<p>This feature complete client implementation of"
                  " the http protocol can efficiently"
                  " maintain several hundred network connections"
                  " simultaneously.</p>");

    QPushButton *quitButton = new QPushButton("OK");

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setMargin(10);
    topLayout->setSpacing(10);
    topLayout->addWidget(icon);
    topLayout->addWidget(text);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(quitButton);
    bottomLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    QDialog about(this);
    about.setModal(true);
    about.setWindowTitle(tr("About Picture Download Client"));
    about.setLayout(mainLayout);

    connect(quitButton, SIGNAL(clicked()), &about, SLOT(close()));

    about.exec();
}

bool MainWindow::addProjectSlot()
{
    m_picProject = new PicProject;
    m_picProject->setProjectId( m_projectView->model()->rowCount() );
    m_picProject->setProjectUrl( m_newProjectDlg->m_proUrlLedit->text() );
    m_picProject->setProjectPath( m_newProjectDlg->m_proSavepathLedit->text() );
    m_picProject->setProjectTitle( m_newProjectDlg->m_proTitleLedit->text() );
    m_projectView->addProject( m_picProject );

    return 0;
}

void MainWindow::removeProjectSlot()
{
    QModelIndex index = m_projectView->currentIndex();
    if(index.child(0, 0).isValid())
    {
        m_projectView->model()->removeRow(index.row());
    }
}


void MainWindow::setActionsEnabled()
{
    // Find the view item and client for the current row, and update
    // the states of the actions.
    QTreeWidgetItem *item = 0;
    if (!m_pictureView->selectedItems().isEmpty())
        item = m_pictureView->selectedItems().first();

    bool pauseEnabled = true;
    //m_removeProjectAction->setEnabled(item != 0);
    m_pauseProjectAction->setEnabled(item != 0 && pauseEnabled);
    m_playProjectAction->setIcon(QIcon(":/icons/player_play.png"));
    m_playProjectAction->setText(tr("恢复下载"));

    m_pauseProjectAction->setIcon(QIcon(":/icons/player_pause.png"));
    m_pauseProjectAction->setText(tr("暂停下载"));
    int row = m_pictureView->indexOfTopLevelItem(item);
    m_upActionTool->setEnabled(item && row != 0);
    m_downActionTool->setEnabled(item && row != 0);
}

void MainWindow::getProjectUrlSlot(QTreeWidgetItem *item, int column)
{
    QString textUrl = item->text(1);
    this->m_newProjectUrl->setEditText(textUrl);
}

static int rateFromValue(int value)
{
    int rate = 0;
    if (value >= 0 && value < 250)
    {
        rate = 1 + int(value * 0.124);
    }
    else if (value < 500)
    {
        rate = 32 + int((value - 250) * 0.384);
    }
    else if (value < 750)
    {
        rate = 128 + int((value - 500) * 1.536);
    }
    else
    {
        rate = 512 + int((value - 750) * 6.1445);
    }
    return rate;
}


