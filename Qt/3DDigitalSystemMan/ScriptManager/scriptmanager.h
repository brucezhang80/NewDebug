#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QtCore/qglobal.h>

#if defined(SCRIPTMANAGER_LIBRARY)
#  define SCRIPTMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCRIPTMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtGui>
#include <QListWidget>
#include <QtGui>
#include <QListWidget>
#include <QString>
#include <QMouseEvent>

class SCRIPTMANAGERSHARED_EXPORT ScriptManager : public QListWidget
{
    Q_OBJECT
public:
    explicit ScriptManager(QWidget *parent = 0);

protected:
    //overwrite
    void contextMenuEvent(QContextMenuEvent *event);

    //drag and drop
//    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
//    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    // member functions
    void        setScriptsDir(QString path){m_scriptDir = path;}
    QString     getScriptsDir(){return m_scriptDir;}
    void        addListItem(const QString path);

signals:

public slots:
    void OnFilterListItemSlot(const QString &text);
    void OnIcoScaleSlot(int size);

public:
    QStringList         m_itemTextList;//collect all items text for searching

private:
    QListWidgetItem     *m_item;
    QString             m_scriptDir;
    QPoint              m_startPos;
};

#endif // SCRIPTMANAGER_H
