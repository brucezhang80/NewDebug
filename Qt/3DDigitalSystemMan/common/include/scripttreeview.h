#ifndef SCRIPTTREEVIEW_H
#define SCRIPTTREEVIEW_H

#include <QtCore/qglobal.h>

#if defined(SCRIPTMANAGER_LIBRARY)
#  define SCRIPTTREEVIEW_EXPORT Q_DECL_EXPORT
#else
#  define SCRIPTTREEVIEW_EXPORT Q_DECL_IMPORT
#endif

#include <QTreeView>

QT_BEGIN_NAMESPACE
class TreeModel;
QT_END_NAMESPACE

class SCRIPTTREEVIEW_EXPORT ScriptTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ScriptTreeView(QWidget *parent = 0);
    ~ScriptTreeView();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void load(const QString& filename);
    void save(const QString& filename);
    void saveState();
    void restoreState();

signals:
    
public slots:
    void loadTree(const QByteArray& treeArray);
    void OnDeleteItemSlot();
    void OnInsertItemSlot();
    void OnRefreshItemSlot();

private:
    TreeModel       *m_model;
    QString         m_scriptFile;

};

#endif // SCRIPTTREEVIEW_H
