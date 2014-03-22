#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QLocalServer>

// 程序只能有惟一一个实例
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);

    bool isRunning();

protected:
    bool winEventFilter( MSG *msg, long *result );

private slots:
    void newLocalConnection();

private:
    QLocalServer        *m_server;
    bool                m_isRunning;
};

#endif // APPLICATION_H
