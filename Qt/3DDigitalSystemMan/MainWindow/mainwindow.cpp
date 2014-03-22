#include "toolbartabwidget.h"
#include "modeltabpage.h"
#include "scripttabpage.h"
#include "datatransfetabpage.h"
#include "settingtabpage.h"

#include "completelineedit.h"
#include "slider.h"

#include "../common/include/modelmanager.h"
#include "../common/include/modeltreeview.h"
#include "../common/include/scriptmanager.h"
#include "../common/include/scripttreeview.h"
#include "../common/include/datatransfe.h"
#include "../common/include/datatransfeoptions.h"
#include "../common/include/addmodelwidget.h"
#include "../common/include/addscriptwidget.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
  main winodw
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_toolbarTabWidget = NULL;
    m_modelTabPage = NULL;
    m_scriptTabPage = NULL;
    m_dataTransfeTabPage = NULL;
    m_settingTabPage = NULL;

    m_modelManager = NULL;
    m_modelTreeView = NULL;
    m_addModelWidget = NULL;
    m_scriptManager = NULL;
    m_scriptTreeView = NULL;
    m_addScriptWidget = NULL;
    //m_dataTransfe = NULL;
    //m_dataTransfeOptions = NULL;

    m_toolsBar = NULL;
    m_vSplitter = NULL;
    m_splitterLeft = NULL;
    m_splitterRight = NULL;
    m_spliteLocker = NULL;

    m_complineEdit = NULL;
    m_iconScaleSlider = NULL;
    m_iconScaleLabel = NULL;
    ui->setupUi(this);

    //初始化mainwindo上的widget
    InitMainWindow();

    //tabwidget changed
    QObject::connect(m_toolbarTabWidget,SIGNAL(currentChanged(int)),
                     this,SLOT(OnTabItemChangeSlot(int)));

    connect(m_complineEdit, SIGNAL(textChanged(QString)),
            m_modelManager,SLOT(OnFilterListItemSlot(const QString &)) );
    connect(m_iconScaleSlider, SIGNAL(sendValueSignal(const int&)),
            m_modelManager,SLOT(OnGetScaleIcoSizeSlot(const int&)) );

    connect(m_complineEdit, SIGNAL(textChanged(QString)),
            m_scriptManager,SLOT(OnFilterListItemSlot(const QString &)) );

    connect(m_iconScaleSlider, SIGNAL(valueChanged(int)),
            m_scriptManager,SLOT(OnIcoScaleSlot(int)) );
    connect(m_iconScaleSlider, SIGNAL(valueChanged(int)),
            m_modelManager,SLOT(OnIcoScaleSlot(int)) );

    connect(m_iconScaleSlider, SIGNAL(valueChanged(int)),
            this,SLOT(OnSetIcoSizeLabelSlot(int)) );

    // creat a thread to initalize left treeview
//    connect(&m_threadInitLeftTree, SIGNAL(loadModelTreeView(const QByteArray&)), m_modelTreeView, SLOT(loadTree(const QByteArray&)));
//    m_threadInitLeftTree.start();
//    m_threadInitLeftTree.stop();

    ////分类列表中切换分类时,同时更新模型管理列表
    connect( m_modelTreeView, SIGNAL(updateModelViewSignal(const QList<QStringList>&)), m_modelManager, SLOT(OnUpdateModelViewSlot(const QList<QStringList>&)) );

    //refresh model view
    connect(m_modelManager, SIGNAL(getCurrentModelTypeSignal()), m_modelTreeView,SLOT(OnCurrentModelTypeSlot()) );
    connect(m_modelTreeView, SIGNAL(currentModelTypeSignal(const QString&)), m_modelManager,SLOT(OnReFreshCurrentModelTypeSlot(const QString&)) );
    connect(m_modelTreeView, SIGNAL(currentModelTypeSignal(const QString&)), m_modelManager,SLOT(OnReFreshCurrentModelTypeSlot(const QString&)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMainWindow()
{
    // Top toolbar
    QToolBar *topBar = new QToolBar(tr("功能栏"));
    addToolBar(Qt::TopToolBarArea, topBar);
    topBar->setMovable(true);
    topBar->setFixedHeight(120);
    m_toolbarTabWidget = new ToolbarTabWidget(this);
    m_modelTabPage = new ModelTabPage(this);
    m_scriptTabPage = new ScriptTabPage(this);
    m_settingTabPage = new SettingTabPage(this);
    m_dataTransfeTabPage = new DataTransfeTabPage(this);

    m_toolbarTabWidget->addTab(m_modelTabPage , tr("模型"));
        m_toolbarTabWidget->tabBar()->setTabToolTip(0, tr("模型管理"));
    m_toolbarTabWidget->addTab(m_scriptTabPage, tr("脚本"));
        m_toolbarTabWidget->tabBar()->setTabToolTip(1, tr("脚本管理"));
    m_toolbarTabWidget->addTab(m_dataTransfeTabPage, tr("数据流转"));
        m_toolbarTabWidget->tabBar()->setTabToolTip(2, tr("数据提交"));
    m_toolbarTabWidget->addTab(m_settingTabPage, tr("设置"));
        m_toolbarTabWidget->tabBar()->setTabToolTip(2, tr("系统设置"));

    //m_toolbarTabWidget->tabBar()->setCurrentIndex(2);
    //创建模型管理
    CreateModelMrg();
    CreateScriptMrg();
    CreateSlipWin();
    CreateDataTransfeMrg();

    topBar->addWidget(m_toolbarTabWidget);

    // toolbar break
    addToolBarBreak(Qt::TopToolBarArea);
    //tools toolbar
    // Set header resize modes and initial section sizes
    QFontMetrics fm = fontMetrics();

    //创建一个空的widget，添加到工具上，让工具栏上的控件靠右
    //（模仿QSpacerItem,因QSpacerItem非Widget无法添加到工具栏）
    QWidget* spacerItem = new QWidget( this );
    spacerItem->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    m_toolsBar = new QToolBar(tr("工具栏"));
    addToolBar(Qt::TopToolBarArea, m_toolsBar);

    m_toolsBar->setMovable(true);
    QPushButton *toolBtnDetail = new QPushButton(tr("详细信息"));
    m_iconScaleLabel = new QLabel(tr("32 Size"));
    m_iconScaleLabel->setFixedSize(QSize(fm.width(tr("999 Size")), fm.lineSpacing()));
    m_iconScaleSlider = new Slider(Qt::Horizontal);
    m_toolsBar->addWidget(spacerItem);
    m_toolsBar->addWidget(toolBtnDetail);
    m_toolsBar->addSeparator();
    m_toolsBar->addWidget(new QLabel(tr("缩放:")));
    m_toolsBar->addWidget( m_iconScaleSlider );
    m_toolsBar->addWidget( m_iconScaleLabel );
    m_toolsBar->addSeparator();

    //botom status bar
    QToolBar *bottomBar = new QToolBar(tr("状态栏"));
    addToolBar(Qt::BottomToolBarArea, bottomBar);
    bottomBar->setMovable(true);
    QLabel *lable = new QLabel(tr("就绪"));
    bottomBar->addWidget(lable);

//    QStringList keyWordsList = m_modelManager->m_itemTextList;//+m_scriptManager->m_itemTextList;
//    m_complineEdit = new CompleteLineEdit(keyWordsList);
//    m_complineEdit->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
//    m_toolsBar->addWidget(m_complineEdit);
    m_complineEdit = new QLineEdit(m_toolsBar);
//    QCompleter *completer = new QCompleter(keyWordsList, this);
//    completer->setCaseSensitivity(Qt::CaseInsensitive);
//    m_complineEdit->setCompleter(completer);
    m_complineEdit->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_toolsBar->addWidget(m_complineEdit);

    m_vSplitter = new QSplitter(this);
    m_vSplitter->setOrientation(Qt::Horizontal);
    m_vSplitter->addWidget(m_splitterLeft);
    m_vSplitter->addWidget(m_splitterRight);
    m_vSplitter->setOpaqueResize(true);
    //m_vSplitter->setStretchFactor(0, 10);
    m_vSplitter->setStretchFactor(1, 1);

    setCentralWidget(m_vSplitter);

    this->setWindowTitle(tr("3Ds Data Manager System - demo v0.3 by Yvi"));
}

void MainWindow::CreateSlipWin()
{
    if( !m_addModelWidget )
        m_addModelWidget = new AddModelWidget(m_modelManager);

    if(!m_addScriptWidget)
        m_addScriptWidget = new AddScriptWidget(m_scriptManager);
    m_modelTabPage->stateMachineWidget();
    m_scriptTabPage->stateMachineWidget();
    connect(m_modelTreeView, SIGNAL(itemClickedSignal(const QString&)), m_addModelWidget->m_ledtType, SLOT(setText(const QString&)) );
}

void MainWindow::CreateModelMrg()
{
    //left splitter
    if(!m_splitterLeft)
    {
        m_splitterLeft = new QSplitter(this);
    }
    //m_splitterLeft->setHandleWidth(1);
    m_modelTreeView = new ModelTreeView(m_splitterLeft);
    //rigth splitter
    if(!m_splitterRight)
    {
        m_splitterRight = new QSplitter(this);
    }
    m_modelManager = new ModelManager(m_splitterRight);
}

void MainWindow::CreateScriptMrg()
{
    //left splitter
    if(!m_splitterLeft)
    {
        m_splitterLeft = new QSplitter(this);
    }
    m_scriptTreeView = new ScriptTreeView(m_splitterLeft);
    //rigth splitter
    if(!m_splitterRight)
        m_splitterRight = new QSplitter(this);

    m_scriptManager = new ScriptManager(m_splitterRight);

    m_scriptManager->setHidden(1);
    m_scriptTreeView->setHidden(1);
}

void MainWindow::CreateDataTransfeMrg()
{
    //left splitter
    if(!m_splitterLeft)
    {
        m_splitterLeft = new QSplitter(this);
    }
    m_dataTransfeOptions = new DataTransfeOptions(m_splitterLeft);
    //rigth splitter
    if(!m_splitterRight)
    {
        m_splitterRight = new QSplitter(this);
    }
    m_dataTransfe = new DataTransfe(m_splitterRight);

    m_dataTransfe->setHidden(1);
    m_dataTransfeOptions->setHidden(1);

    connect(m_dataTransfeOptions, SIGNAL(startCommitSignal()), m_dataTransfe, SLOT(startUploadDataClientSlot()));
    connect(m_dataTransfe, SIGNAL(updateControlSignal(bool)), m_dataTransfeOptions, SLOT(updateControlSlot(bool)));


}

void MainWindow::OnSetIcoSizeLabelSlot(int size)
{
   QString strSize =  QString("%1 size").arg(size);
   m_iconScaleLabel->setText(strSize);
}

//add item to ListWidget from addMaxModelDlg
void MainWindow::OnAddMaxModelSlot(const QString &modelType, const QString &maxFilename, const QString &thumbFilename, const QString &itemText)
{
    QListWidgetItem *item =  new QListWidgetItem(m_modelManager);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom | Qt::AlignAbsolute | Qt::AlignJustify);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled
                     | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);

    QFileInfo fileInfo(maxFilename);
    QPixmap pixmap(thumbFilename);
    //scale icon bitmap
    QIcon itemIcon( pixmap.scaled( QSize(ICON_W, ICON_H) ) );
    //QIcon itemIcon( pixmap );
    item->setIcon( itemIcon );
    //m_modelManager->setIconSize( QSize(m_iconScaleSlider->value()*1.6, m_iconScaleSlider->value()) );
    item->setSizeHint( QSize(m_iconScaleSlider->value()*1.6, m_iconScaleSlider->value()+24) );
    item->setText(itemText);
    for(int i = 0; i < m_modelManager->count(); ++i)
    {
        QBrush itemBrush = m_modelManager->item(i)->foreground();
        if(itemBrush.color() != QColor().black() )
            m_modelManager->item(i)->setTextColor(QColor().black());
    }
    item->setTextColor(QColor(255,80,0, 255));

    //m_itemTextList.append( itemText );
    m_modelManager->setCurrentItem(item);
    m_modelManager->update();

    //添加的文件，需要保存到数据库。
    //保存的对象：文件全路径，缩略图全路径，模型名称，所属分类
    QSqlDatabase db;
    if(QSqlDatabase::contains("ADDMODEL"))
        db = QSqlDatabase::database("ADDMODEL");
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "ADDMODEL");
        db.setDatabaseName(MODEL_SYSTEM_DB);
    }
    if( !db.isOpen() )
    {
        if( !db.open() )
        {
            QMessageBox::warning(this, tr("open error"), tr("error:")+ db.lastError().text(), QMessageBox::Ok);
            return;
        }
    }

    QSqlQuery query = QSqlQuery::QSqlQuery(db);
    bool isok = query.exec( QString("select * from %1;").arg(MODEL_SYSTEM_TABEL) );
    if (!isok)
    {
        QMessageBox::warning(this, tr("open error"), tr("Failed to open table:")+query.lastError().text(), QMessageBox::Ok);
        db.close();
        db.removeDatabase(MODEL_SYSTEM_DB);
        return;
    }

    query.prepare( QString("INSERT INTO %1 (id, typeName, modelName, modelPath, thumbnailPath) "
                       "VALUES (:id, :typeName, :modelName, :modelPath, :thumbnailPath)").arg(MODEL_SYSTEM_TABEL) );
    //query.bindValue(":id", NULL);//不需要设置，否则无法插入数据库
    query.bindValue(":typeName", modelType);
    query.bindValue(":modelName", itemText);
    query.bindValue(":modelPath", maxFilename);
    query.bindValue(":thumbnailPath", thumbFilename);
    query.exec();

    //qDebug() << modelType;
    query.clear();
    db.close();
    db.removeDatabase(MODEL_SYSTEM_DB);
}

void MainWindow::hideAndShowWidget(const QObjectList &objList, QWidget *destWidget)
{
    foreach(QObject* obj, objList)
    {
        if(obj->isWidgetType())
        {
            QWidget* wid = static_cast<QWidget*>(obj);
            if( wid == destWidget)
            {
                wid->setShown(1);
            }
            else
                wid->setHidden(1);
        }
    }
}

void MainWindow::OnTabItemChangeSlot(int index)
{
    m_toolbarTabWidget->tabBar()->setTabTextColor(index, Qt::red);
    for(int i = 0;i<m_toolbarTabWidget->tabBar()->count();i++)
    {
        if(i != index)
           m_toolbarTabWidget->tabBar()->setTabTextColor(i,Qt::black);
    }

    if( 0 == index)//当前模型页面为活动状态
    {
        m_splitterLeft->show();
        m_splitterRight->show();
        QObjectList leftObjList = m_splitterLeft->children();
        hideAndShowWidget(leftObjList, m_modelTreeView);
        QObjectList rigthObjList = m_splitterRight->children();
        hideAndShowWidget(rigthObjList, m_modelManager);

        QStringList keyWordsList = m_modelManager->m_itemTextList;
        QCompleter *completer = new QCompleter(keyWordsList, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        m_complineEdit->setCompleter(completer);
        m_toolsBar->setShown(1);
        m_toolsBar->setEnabled(true);
    }
    else if( 1 == index )//当前“脚本”页面活动状态
    {
        m_splitterLeft->show();
        m_splitterRight->show();
        QObjectList leftObjList = m_splitterLeft->children();
        hideAndShowWidget(leftObjList, m_scriptTreeView);
        QObjectList rigthObjList = m_splitterRight->children();
        hideAndShowWidget(rigthObjList, m_scriptManager);

        QStringList keyWordsList = m_scriptManager->m_itemTextList;
        QCompleter *completer = new QCompleter(keyWordsList, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        m_complineEdit->setCompleter(completer);

        m_toolsBar->setShown(1);
        m_toolsBar->setEnabled(true);
    }
    else if( 2 == index )//当前"数据流转"页面为活动状态
    {
        m_splitterLeft->show();
        m_splitterRight->show();

        QObjectList leftObjList = m_splitterLeft->children();
        hideAndShowWidget(leftObjList, m_dataTransfeOptions);
        QObjectList rigthObjList = m_splitterRight->children();
        hideAndShowWidget(rigthObjList, m_dataTransfe);

        m_toolsBar->setHidden(1);
        m_toolsBar->setEnabled(false);
//        m_modelTreeView->setHidden(1);
//        m_scriptTreeView->setHidden(1);
//        m_modelManager->setHidden(1);
//        m_scriptManager->setHidden(1);

//        m_dataTransfe->setShown(1);
//        m_dataTransfeOptions->setShown(1);
    }
    else////当前设置页面为活动状态
    {
//        QObjectList leftObjList = m_splitterLeft->children();
//        hideAndShowWidget(leftObjList, m_dataTransfeOptions);
//        QObjectList rigthObjList = m_splitterRight->children();
//        hideAndShowWidget(rigthObjList, m_dataTransfe);
        m_splitterLeft->setHidden(1);
        m_splitterRight->hide();

        m_toolsBar->setHidden(1);
        m_toolsBar->setEnabled(false);
    }
}

void MainWindow::OnAddScriptSlot(const QString &modelType, const QString& maxFilename, const QString& thumbFilename, const QString& itemText)
{

}

