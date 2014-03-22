
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QToolBar>
#include <QSplitter>
#include <QSlider>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>

#include "thread.h"

QT_BEGIN_NAMESPACE
class ToolbarTabWidget;
class ModelTabPage;
class ScriptTabPage;
class DataTransfeTabPage;

class CompleteLineEdit;
class Slider;
class SettingTabPage;

class DataTransfe;
class DataTransfeOptions;
class ModelManager;
class ModelTreeView;
class AddModelWidget;
class ScriptManager;
class ScriptTreeView;
class AddScriptWidget;

QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void hideAndShowWidget(const QObjectList &objList, QWidget *destWidget);

private:
    Ui::MainWindow *ui;

signals:
    void setComboBoxModelTypeDateSignal();

public slots:
    void OnSetIcoSizeLabelSlot(int size);
    void OnAddMaxModelSlot(const QString &modelType, const QString& maxFilename, const QString& thumbFilename, const QString& itemText);
    void OnTabItemChangeSlot(int index);
    void OnAddScriptSlot(const QString &modelType, const QString& maxFilename, const QString& thumbFilename, const QString& itemText);

protected:
    void InitMainWindow();
    void CreateModelMrg();
    void CreateScriptMrg();
    void CreateDataTransfeMrg();
    void CreateSettingsMrg();
    void CreateSlipWin();

public:
    ToolbarTabWidget        *m_toolbarTabWidget;
    ModelTabPage            *m_modelTabPage;
    ScriptTabPage           *m_scriptTabPage;
    DataTransfeTabPage      *m_dataTransfeTabPage;
    DataTransfe             *m_dataTransfe;
    DataTransfeOptions      *m_dataTransfeOptions;
    SettingTabPage          *m_settingTabPage;

    ModelManager            *m_modelManager;
    ModelTreeView           *m_modelTreeView;
    AddModelWidget          *m_addModelWidget;
    ScriptManager           *m_scriptManager;
    ScriptTreeView          *m_scriptTreeView;
    AddScriptWidget         *m_addScriptWidget;

    //DataTransfe             *m_dataTransfe;
    //DataTransfeOptions      *m_dataTransfeOptions;

    QToolBar                *m_toolsBar;
    QSplitter               *m_vSplitter;
    QSplitter               *m_splitterLeft;
    QSplitter               *m_splitterRight;
    QSplitter               *m_spliteLocker;
    //toolsbar members
    //CompleteLineEdit        *m_complineEdit;    //search lineedit
    Slider                  *m_iconScaleSlider; //list item icon scale
    QLabel                  *m_iconScaleLabel;
    QLineEdit               *m_complineEdit;
    //AddMaxModelDlg          *m_addMaxModelDlg;
    Thread                  m_threadInitLeftTree;

};
#endif // MAINWINDOW_H
