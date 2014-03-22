#ifndef DATATRANSFEHISTORY_H
#define DATATRANSFEHISTORY_H

#include <QtCore\qglobal.h>
#if defined(DATATRANSFE_LIBRARY)
#  define DATATRANSFEHISTORYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATATRANSFEHISTORYSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QDir;
class QFileInfo;
class QUrl;
class QDebug;
QT_END_NAMESPACE

class DATATRANSFEHISTORYSHARED_EXPORT DataTransfeHistory : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DataTransfeHistory(QWidget *parent = 0);
    void addItem(const int &num, const QString &filename);
    void getFolderSize(const QString & path, qint64 &size);
    void getFolderSize(const QDir & dir, qint64 &size);
    void bytesToSize(const qint64 & bytes, QString & size);

protected:

signals:
    
public slots:
    
};

#endif // DATATRANSFEHISTORY_H
