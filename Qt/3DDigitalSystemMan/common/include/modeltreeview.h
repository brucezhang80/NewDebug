#ifndef MODELTREEVIEW_H
#define MODELTREEVIEW_H

#include <QtCore/qglobal.h>
#if defined(MODELMANAGER_LIBRARY)
#  define MODELTREEVIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODELTREEVIEWSHARED_EXPORT Q_DECL_IMPORT
#endif
#include "modelmanager.h"
#include <QMouseEvent>
#include <QTreeView>

QT_BEGIN_NAMESPACE
class TreeModel;
QT_END_NAMESPACE

class MODELTREEVIEWSHARED_EXPORT ModelTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ModelTreeView(QWidget *parent = 0);
    ~ModelTreeView();
    void queryModelType(const QString &queryKey, QList<QStringList> &modelList);

protected:
    void mousePressEvent(QMouseEvent *event);
//    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

protected:
    void load(const QString& filename);
    bool create(const QString& filename);
    void save(const QString& filename);
    void saveChild(QModelIndex& parentIndex, QByteArray *byteArray, QString &space);
    void saveState();
    void restoreState();
    void travelTree(QModelIndex &parentIndex, bool bParent);

signals:
    void updateModelViewSignal(const QList<QStringList>&);
    void currentModelTypeSignal(const QString&);
    void setComboBoxModelTypeDateSignal(const QString &, const bool &);
    void itemClickedSignal(const QString&);

public slots:
    void loadTree(const QByteArray& treeArray);
    void OnDeleteItemSlot( );
    void OnInsertItemSlot();
    void OnRefreshItemSlot();
    void OnInsertSubItemSlot();
    void OnRenameCurrentItemSlot();
    void OnCurrentModelTypeSlot();
    void OnSetComboBoxModelTypeDataSlot();

public:
    TreeModel       *m_model;
    QString         m_modelFile;
};

#endif // MODELTREEVIEW_H
