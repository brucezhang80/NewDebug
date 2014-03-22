#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QAction;
class QCloseEvent;
class QLabel;
class QProgressDialog;
class QSlider;
class QLineEdit;
class QPalette;
class QComboBox;
class QTreeWidgetItem;
QT_END_NAMESPACE

class PictureView;
class ProjectView;
class PicProject;
class NewProjectDlg;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initMainWindow();

protected:
    void setConnect();

private slots:
    void aboutApp();
    bool addProjectSlot();
    void removeProjectSlot();

    void setActionsEnabled();

    void getProjectUrlSlot(QTreeWidgetItem * item, int column);

private:
    PictureView     *m_pictureView;
    ProjectView     *m_projectView;
    PicProject      *m_picProject;
    NewProjectDlg   *m_newProjectDlg;

    QAction         *m_addNewProjectAction;
    QAction         *m_playProjectAction;
    QAction         *m_pauseProjectAction;
    QAction         *m_stopProjectAction;
    QAction         *m_removeProjectAction;
    QAction         *m_upActionTool;
    QAction         *m_downActionTool;

    QComboBox       *m_newProjectTitle;
    QComboBox       *m_newProjectUrl;
};

#endif // MAINWINDOW_H
