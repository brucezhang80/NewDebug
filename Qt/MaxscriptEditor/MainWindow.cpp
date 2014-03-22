#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CodeEditor.h"

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QSettings>
#include <QSignalMapper>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_mdiArea = new QMdiArea;
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setViewMode(QMdiArea::TabbedView);
    m_mdiArea->setTabsClosable(true);
    m_mdiArea->setTabsMovable(true);
    setCentralWidget(m_mdiArea);
    setWindowTitle(tr("MAXScript Editor - By Macai"));
    statusBar()->showMessage(tr("Ready"));

    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
    m_windowMapper = new QSignalMapper(this);
    connect(m_windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();

    updateMenus();
    connect(ui->menu_Window, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    readSettings();

//    newFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_mdiArea->closeAllSubWindows();
    if (m_mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        writeSettings();
        event->accept();
    }
}

void MainWindow::newFile()
{
    CodeEditor *editor = createEditor();
    editor->newFile();
    setActiveSubWindow(editor);
    editor->showMaximized();

//    connect(editor, SIGNAL(lastClose()), this, SLOT(close()));
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        QMdiSubWindow *existing = findEditor(fileName);
        if (existing)
        {
            m_mdiArea->setActiveSubWindow(existing);
            return;
        }

        CodeEditor *editor = createEditor();
        if (editor->loadFile(fileName))
        {
            statusBar()->showMessage(tr("File loaded"), 2000);
            editor->show();
        }
        else
        {
            editor->close();
        }
    }
}

void MainWindow::save()
{
    if (activeEditor() && activeEditor()->save())
             statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs()
{
    if (activeEditor() && activeEditor()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::undo()
{
    if (activeEditor())
        activeEditor()->undo();
}

void MainWindow::redo()
{
    if (activeEditor())
        activeEditor()->redo();

}

void MainWindow::cut()
{
    if (activeEditor())
        activeEditor()->cut();
}

void MainWindow::copy()
{
    if (activeEditor())
        activeEditor()->copy();
}

void MainWindow::paste()
{
    if (activeEditor())
        activeEditor()->paste();
}

void MainWindow::close()
{
    m_mdiArea->closeActiveSubWindow();
    if( m_mdiArea->subWindowList().count() == 0 )
        newFile();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About MAXScript Editor"),
                 tr("The <b>MAXScript Editor</b> write maxscript programm langguage for 3dsmax plug-in."));
}

void MainWindow::setMianWindowTitle()
{
    if (activeEditor())
        setWindowTitle( activeEditor()->currentFile() + tr(" - MAXScript Editor - By Macai") );
}

CodeEditor *MainWindow::createEditor()
{
    CodeEditor *editor = new CodeEditor;
    m_mdiArea->addSubWindow(editor);

    connect(editor, SIGNAL(copyAvailable(bool)),
            ui->action_Cut, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)),
            ui->action_Copy, SLOT(setEnabled(bool)));

    connect(editor, SIGNAL(modificationChanged(bool)),
            ui->action_Undo, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(modificationChanged(bool)),
            ui->action_Redo, SLOT(setEnabled(bool)));

    return editor;
}

CodeEditor *MainWindow::activeEditor()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<CodeEditor *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MainWindow::findEditor(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, m_mdiArea->subWindowList())
    {
        CodeEditor *editor = qobject_cast<CodeEditor *>(window->widget());
        if (editor->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}
void MainWindow::createActions()
{
    ui->action_New->setShortcuts(QKeySequence::New);
    connect(ui->action_New, SIGNAL(triggered()), this, SLOT(newFile()));

    ui->action_Open->setShortcuts(QKeySequence::Open);
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));

    ui->action_Save->setShortcuts(QKeySequence::Save);
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));

    ui->action_SaveAs->setShortcuts(QKeySequence::SaveAs);
    connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

    ui->action_Exit->setShortcuts(QKeySequence::Quit);
    connect(ui->action_Exit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    ui->action_Undo->setShortcuts(QKeySequence::Undo);
    connect(ui->action_Undo, SIGNAL(triggered()), this, SLOT(undo()));

    ui->action_Redo->setShortcuts(QKeySequence::Redo);
    connect(ui->action_Redo, SIGNAL(triggered()), this, SLOT(redo()));

    ui->action_Cut->setShortcuts(QKeySequence::Cut);
    connect(ui->action_Cut, SIGNAL(triggered()), this, SLOT(cut()));

    ui->action_Copy->setShortcuts(QKeySequence::Copy);
    connect(ui->action_Copy, SIGNAL(triggered()), this, SLOT(copy()));

    ui->action_Paste->setShortcuts(QKeySequence::Paste);
    connect(ui->action_Paste, SIGNAL(triggered()), this, SLOT(paste()));

    connect(ui->action_Close, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->action_Close_All, SIGNAL(triggered()), m_mdiArea, SLOT(closeAllSubWindows()));

    connect(ui->action_Tile, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));

    connect(ui->action_Cascade, SIGNAL(triggered()), m_mdiArea, SLOT(cascadeSubWindows()));

    ui->action_Next->setShortcuts(QKeySequence::NextChild);
    connect(ui->action_Next, SIGNAL(triggered()), m_mdiArea, SLOT(activateNextSubWindow()));

    ui->action_Previous->setShortcuts(QKeySequence::PreviousChild);
    connect(ui->action_Previous, SIGNAL(triggered()), m_mdiArea, SLOT(activatePreviousSubWindow()));

    actionSeparator = new QAction(this);
    actionSeparator->setSeparator(true);

    connect(ui->action_Help, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::readSettings()
{
    QSettings settings("Macai", "Maxscript Editor");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 460)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("Macai", "Maxscript Editor");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    m_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::updateMenus()
{
    bool hasEditor = (activeEditor() != 0);
    ui->action_Save->setEnabled(hasEditor);
    ui->action_SaveAs->setEnabled(hasEditor);
    ui->action_Paste->setEnabled(hasEditor);
    ui->action_Close->setEnabled(hasEditor);
    ui->action_Close_All->setEnabled(hasEditor);
    ui->action_Tile->setEnabled(hasEditor);
    ui->action_Cascade->setEnabled(hasEditor);
    ui->action_Next->setEnabled(hasEditor);
    ui->action_Previous->setEnabled(hasEditor);
    actionSeparator->setVisible(hasEditor);

    bool hasSelection = (activeEditor() &&
                         activeEditor()->hasSelectedText());
    ui->action_Cut->setEnabled(hasSelection);
    ui->action_Copy->setEnabled(hasSelection);

    bool ismodified = (activeEditor() && activeEditor()->isModified());
    ui->action_Undo->setEnabled(ismodified);
    ui->action_Redo->setEnabled(!ismodified);

    setMianWindowTitle();
}

void MainWindow::updateWindowMenu()
{
    ui->menu_Window->clear();
    ui->menu_Window->addAction(ui->action_Close);
    ui->menu_Window->addAction(ui->action_Close_All);
    ui->menu_Window->addSeparator();
    ui->menu_Window->addAction(ui->action_Tile);
    ui->menu_Window->addAction(ui->action_Cascade);
    ui->menu_Window->addSeparator();
    ui->menu_Window->addAction( ui->action_Next);
    ui->menu_Window->addAction(ui->action_Previous);
    ui->menu_Window->addAction(actionSeparator);

    QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    actionSeparator->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
         CodeEditor *editor = qobject_cast<CodeEditor *>(windows.at(i)->widget());

         QString text;
         if (i < 9)
         {
             text = tr("&%1 %2").arg(i + 1).arg(editor->userFriendlyCurrentFile());
         }
         else
         {
             text = tr("%1 %2").arg(i + 1).arg(editor->userFriendlyCurrentFile());
         }
         QAction *action  = ui->menu_Window->addAction(text);
         action->setCheckable(true);
         action ->setChecked(editor == activeEditor());
         connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
         m_windowMapper->setMapping(action, windows.at(i));
    }
}
