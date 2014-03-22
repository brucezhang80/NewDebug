#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CodeEditor;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void close();
    void about();
    void setActiveSubWindow(QWidget *window);
    void setMianWindowTitle();
    CodeEditor* createEditor();
    void switchLayoutDirection();
    void updateMenus();
    void updateWindowMenu();

private:
    void            createActions();
    CodeEditor*     activeEditor();
    QMdiSubWindow*  findEditor(const QString &fileName);
    void            readSettings();
    void            writeSettings();

    Ui::MainWindow *ui;
    CodeEditor*     m_codeEditor;
    QMdiArea*       m_mdiArea;
    QSignalMapper*  m_windowMapper;
    QAction*        actionSeparator;
};

#endif // MAINWINDOW_H
