#include "personwidget.h"
#include "personmanagerview.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QToolButton>
#include <QWidget>

#include <QDebug>


PersonManagerView::PersonManagerView(QWidget *parent) :
    QTableWidget(parent)
{
    initTableView();

    m_personClient = new PersonClient(this);

    connect(m_personClient, SIGNAL(newParticipantSignal(const QString&, const QString&, const QString&)),
            this, SLOT(newParticipantSlot(const QString&, const QString&, const QString&)));
    connect(m_personClient, SIGNAL(participantLeftSignal(const QString&, const QString&, const QString&)),
            this, SLOT(participantLeftSlot(const QString&, const QString&, const QString&)));

    m_widgetModelIndex = currentIndex();

    m_myUserName = m_personClient->getUserName();
    m_myHostName = m_personClient->getHostName();
    m_myHostAddress = m_personClient->getHostAddress();
    m_myHostPort = m_personClient->getHostPort();
    this->newParticipantSlot(m_myUserName, m_myHostName, m_myHostPort);
//    QTimer::singleShot( 2200, this, SLOT(addMyself()) );
}

void PersonManagerView::initTableView()
{
    int row = 10;
    int col = 4;
    setRowCount( row );
    setColumnCount( col );
}

void PersonManagerView::newParticipantSlot(const QString& name, const QString& address, const QString& port)
{
    PersonWidget *person = new PersonWidget(this);
    person->setUserName(name);
    person->setUserIp(address+":"+port);

    bool done = false;
    for(int i = 0; i != rowCount(); ++i)
    {
        for(int j = 0; j != columnCount(); ++j)
        {
            if(!cellWidget(i, j))
            {
                qDebug() << "new person on line.";
                done = true;
                this->setCellWidget(i, j, person);
                break;
            }
        }
        if(done)
            break;
    }
}

void PersonManagerView::participantLeftSlot(const QString&, const QString&, const QString&)
{
}

void PersonManagerView::addMyself()
{

}

