#include "sidebar.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent)
{
    //this->setSizePolicy(QSizePolicy::Fixed);
    this->setMinimumSize(100, 100);
    this->setContentsMargins(0, 0, 0, 0);
    initSideBar();
}

void SideBar::initSideBar()
{
    QStringList btnCaptions;
    btnCaptions << tr("Job Sender") << tr("Job Reciever")
                << tr("Job History") << tr("Job In Storage")
                << tr("Job Status") << tr("Person Manager")
                << tr("Other Actions") << tr("Settings");
    m_jobSender         = new QToolButton(this);
    m_jobReciever       = new QToolButton(this);
    m_jobHistory        = new QToolButton(this);
    m_jobInStorage      = new QToolButton(this);
    m_jobStatus         = new QToolButton(this);
    m_personManager     = new QToolButton(this);
    m_otherActions      = new QToolButton(this);
    m_settings          = new QToolButton(this);

    m_btnList<<m_jobSender<<m_jobReciever<<m_jobHistory<<m_jobInStorage<<m_jobStatus<<m_personManager<<m_otherActions<<m_settings;

    //foreach( QToolButton* btn, m_btnList)
    for( int i = 0; i != m_btnList.count(); ++i)
    {
        m_btnList[i]->setText(btnCaptions[i]);
        m_btnList[i]->setFixedSize(100, 64);
        m_btnList[i]->setIconSize(QSize(48,48));
        m_btnList[i]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        //m_btnList[i]->setSizePolicy(QSizePolicy::Expanding);

    }

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    gridLayout->addWidget( m_jobSender, 0, 0, 1, 1 );
    gridLayout->addWidget( m_jobReciever, 1, 0, 1, 1 );
    gridLayout->addWidget( m_jobHistory, 2, 0, 1, 1 );
    gridLayout->addWidget( m_jobInStorage, 3, 0, 1, 1 );
    gridLayout->addWidget( m_jobStatus, 4, 0, 1, 1 );
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget( line, 5, 0, 1, 1 );

    gridLayout->addWidget( m_personManager, 6, 0, 1, 1 );
    gridLayout->addWidget( m_otherActions, 7, 0, 1, 1 );
    gridLayout->addWidget( m_settings, 8, 0, 1, 1 );

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 9, 0, 1, 1);

    this->setLayout( gridLayout );
}
