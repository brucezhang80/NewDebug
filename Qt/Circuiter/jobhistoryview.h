#ifndef JOBHISTORYVIEW_H
#define JOBHISTORYVIEW_H

#include <QTreeWidget>

class JobHistoryView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit JobHistoryView(QWidget *parent = 0);
    
signals:
    
public slots:
    void saveSenderData(const QString &resource, const QString &destination);

};

#endif // JOBHISTORYVIEW_H
