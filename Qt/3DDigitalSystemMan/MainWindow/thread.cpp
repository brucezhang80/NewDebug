#include "thread.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>

Thread::Thread()
{
    stopped = false;
}

void Thread::stop()
{
    stopped = true;

    QThread::exit();
}

void Thread::run()
{
    QByteArray modelByteArr;
    QString modelFile = "model.txt";
    readLeftTreeFile(modelFile, modelByteArr);
    const QByteArray treeArray = modelByteArr;
    emit  loadModelTreeView(treeArray);
}

void Thread::readLeftTreeFile(QString treeFile, QByteArray& byteArray)
{
    QFile modelFile(treeFile);
    if(!modelFile.exists())
    {
        return;
    }
    if( !modelFile.open(QIODevice::ReadOnly) )
    {
        return;
    }
    QTextStream stream(modelFile.readAll(),QIODevice::ReadOnly);
    stream.setCodec( QTextCodec::codecForName("SYSTEM") );
    byteArray.append(stream.readAll());
    modelFile.close();
}
