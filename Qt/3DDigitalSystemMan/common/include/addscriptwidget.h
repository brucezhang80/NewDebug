#ifndef ADDSCRIPTWIDGET_H
#define ADDSCRIPTWIDGET_H

#include <QtCore/qglobal.h>
#if defined(SCRIPTMANAGER_LIBRARY)
#  define ADDSCRIPTWIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ADDSCRIPTWIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif
#include "scriptmanager.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QMouseEvent>

class ADDSCRIPTWIDGETSHARED_EXPORT AddScriptWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddScriptWidget(QWidget *parent = 0);
    ~AddScriptWidget(){};
    void initializWindow();
    void clearData();
    void setDefautData();

    //add Script widget window
    void setScriptFilename(const QString &filename){m_scriptFilename = filename;}
    void setScriptThumbnail();
    void setScriptType(const QString &text){ m_scriptType = text;}
    //void setScriptVersion(){}
    void setScriptMaterial();
    void setScriptName(const QString &text){m_scriptName = text;}
    void setRemark();

signals:
    void hideScriptWgtSignal();
    void addScriptSignal(const QString&, const QString&, const QString&, const QString&);

public slots:
    void OnCancelAddScriptWidgetSlot();
    void OnOkAddScriptWidgetSlot();
    void OnSelScriptFileBtnSlot();
    void OnSetScriptTypeSlot(const QString &);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    //void dragEnterEvent(QDragEnterEvent *event);
    //void dropEvent(QDropEvent *event);

    void fileDrop(QDropEvent *event, const QString &dragTypeRx, QString &fullFilename);
    void fileDragEnter(QDragEnterEvent *event, const QString &dragTypeRx);

public:
    QLineEdit       *m_ledtFileDir;
    QPushButton     *m_pbtnSelectFile;
    QLineEdit       *m_ledtType;
    QLineEdit       *m_ledtVersion;
    //QLineEdit       *m_ledtHasMat;
    //QPushButton     *m_pbtnAddMat;
    QLineEdit       *m_ledtName;
    QTextEdit       *m_edtRemark;
    QLabel          *m_lbThumb;
    QPushButton     *m_pbtnOk;
    QPushButton     *m_pbtnCancel;
private:
    QString     m_scriptFilename;
    QString     m_scriptThumbname;
    QString     m_scriptType;
    QString     m_scriptName;
    //QString     m_scriptMatPath;
    QString     m_scriptRemark;
};

#endif // AddScriptWidget
