#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>

class FileClient : public QObject
{
    Q_OBJECT
public:
    explicit FileClient(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // FILECLIENT_H
