#ifndef ADDMODELWIDGET_H
#define ADDMODELWIDGET_H

#include <QtCore/qglobal.h>
#if defined(MODELMANAGER_LIBRARY)
#  define ADDMODELWIDGETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ADDMODELWIDGETSHARED_EXPORT Q_DECL_IMPORT
#endif
#include "modelmanager.h"
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QMouseEvent>

class ADDMODELWIDGETSHARED_EXPORT AddModelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddModelWidget(QWidget *parent = 0);
    ~AddModelWidget(){};
    void initializWindow();
    void clearData();
    void setDefautData();

    //add model widget window
    void setModelFilename(const QString &filename){m_modelFilename = filename;}
    void setModelThumbnail();
    void setModelType(const QString &text){ m_modelType = text;}
    //void setModelVersion(){}
    void setModelMaterial();
    void setModelName(const QString &text){m_modelName = text;}
    void setRemark();

signals:
    void hideAddModelWgtSignal();
    void addMaxModelSignal(const QString&, const QString&, const QString&, const QString&);

public slots:
    void OnCancelAddModelWidgetSlot();
    void OnOkAddModelWidgetSlot();
    void OnSelModelFileBtnSlot();
    void OnSetModelTypeSlot(const QString &);

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
    QLineEdit       *m_ledtHasMat;
    QPushButton     *m_pbtnAddMat;
    QLineEdit       *m_ledtName;
    QTextEdit       *m_edtRemark;
    QLabel          *m_lbThumb;
    QPushButton     *m_pbtnOk;
    QPushButton     *m_pbtnCancel;
private:
    QString     m_modelFilename;
    QString     m_modelThumbname;
    QString     m_modelType;
    QString     m_modelName;
    QString     m_modelMatPath;
    QString     m_modelRemark;
};

#endif // ADDMODELWIDGET_H
