#ifndef SEND_FILE_TRANSFER_H
#define SEND_FILE_TRANSFER_H

#include <QObject>

class SendFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit SendFileTransfer(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // SEND_FILE_TRANSFER_H
