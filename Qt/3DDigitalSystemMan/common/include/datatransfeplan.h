#ifndef DATATRANSFEPLAN_H
#define DATATRANSFEPLAN_H


#include <QtCore\qglobal.h>
#if defined(DATATRANSFE_LIBRARY)
#  define DATATRANSFEPLANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATATRANSFEPLANSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QMouseEvent;
class QDir;
class QFileInfo;
class QUrl;
class QDebug;
QT_END_NAMESPACE

class DATATRANSFEPLANSHARED_EXPORT DataTransfePlan : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DataTransfePlan(QWidget *parent = 0);
    
    void addItem(const int &num, const QString &filename);
    void getFolderSize(const QString & path, qint64 &size);
    void getFolderSize(const QDir & dir, qint64 &size);
    void bytesToSize(const qint64 & bytes, QString & size);

protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

signals:
    
public slots:
    
};

#endif // DATATRANSFEPLAN_H
