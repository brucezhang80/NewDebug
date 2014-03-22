#ifndef UNIQUEAPP_H
#define UNIQUEAPP_H

#include <QApplication>
#include <QLocalServer>

class UniqueApp : public QApplication
{
    Q_OBJECT
public:
    UniqueApp(int argc, char **argv);

    bool isRunning();

private slots:
    void newLocalConnection();

private:
    QLocalServer        *m_server;
    bool                m_isRunning;
};

#endif // UNIQUEAPP_H
