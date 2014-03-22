#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QtCore/qglobal.h>
#if defined(MODELMANAGER_LIBRARY)
#  define MODELMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODELMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtGui>
#include <QListWidget>
#include <QString>
#include <QMouseEvent>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

const int ICON_H = 256;         //item thumbnail width
const int ICON_W = ICON_H*1.4;  //item thumbnail height
const int ITEM_SIZE = 268;      //item height(include icon and text)
const QString MODEL_SYSTEM_DB = QString("modelSystemDB");
const QString MODEL_SYSTEM_TABEL = QString("modelSystemTB");

class MODELMANAGERSHARED_EXPORT ModelManager : public QListWidget
{
    Q_OBJECT
public:
    ModelManager(QWidget *parent = 0);
    ~ModelManager();
protected:
    //overwrite
    void contextMenuEvent( QContextMenuEvent * event );

    //drag and drop
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    //member functions
    void        setModelsDir(QString path){m_modelsDir = path;}
    QString     getModelsDir()const{return m_modelsDir;}
    void        InitListItem();

public:
    bool        CreateConnectDB(QSqlDatabase& db);
    void        AddItem(const QString& modelName, const QString& maxFilename, const QString& thumbFilename);
signals:
    void updateItemIconSignal();
    void getCurrentModelTypeSignal();

public slots:
    void OnFilterListItemSlot(const QString &text);
    void OnIcoScaleSlot(int size);
    void OnPopMenuDelModelSlot();
    void OnPopMenuRefleshSlot();
    void OnItemClickedSlot(QListWidgetItem* item);
    void OnGetScaleIcoSizeSlot(const int& size);
    void OnUpdateModelViewSlot(const QList<QStringList>&);

    void OnReFreshCurrentModelTypeSlot(const QString&);
public:
    QStringList         m_itemTextList;//collect all items text for searching
    QAction             *m_addAction;

private:
    QListWidgetItem         *m_item;    //the max models list item member
    QString                 m_modelsDir;//the max model files dir
    QString                 m_maxFile;
    QString                 m_thumbFile;
    QPoint                  m_startPos;
    //QSqlDatabase         m_modelListDB;
    int                     m_iconHight;
    int                     m_iconWidth;
};

#endif // MODELMANAGER_H
