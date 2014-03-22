#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QString>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    void stop();

protected:
    void run();
    void readLeftTreeFile(QString treeFile, QByteArray& byteArray);

signals:
//    void initalizedLeftTree(const QByteArray& byteArray);
    void loadModelTreeView(const QByteArray& byteArray);
//    void loadScriptTreeView(const QByteArray& byteArray);

public slots:


private:
    volatile bool stopped;
};

#endif // THREAD_H
