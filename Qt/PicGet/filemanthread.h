#ifndef FILEMANTHREAD_H
#define FILEMANTHREAD_H

#include <QThread>

class FileManThread : public QThread
{
    Q_OBJECT
public:
    explicit FileManThread(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // FILEMANTHREAD_H
