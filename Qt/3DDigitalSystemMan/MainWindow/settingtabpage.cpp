#include "settingtabpage.h"
#include <QGroupBox>
#include <QDebug>

SettingTabPage::SettingTabPage(QWidget *parent) :
    QWidget(parent)
{
    m_signalMapper = new QSignalMapper(this);

    m_modelSettingBtn = new QToolButton();
    m_scriptSettingBtn = new QToolButton();
    m_systemSettingBtn = new QToolButton();
    m_updateSysBtn = new QToolButton();
    m_aboutSysBtn = new QToolButton();
    m_dataSettingBtn = new QToolButton();

    m_modelSettingBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_scriptSettingBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_systemSettingBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_updateSysBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_aboutSysBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_dataSettingBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_modelSettingBtn->setFixedSize(60, 72);
    m_scriptSettingBtn->setFixedSize(60, 72);
    m_systemSettingBtn->setFixedSize(60, 72);
    m_updateSysBtn->setFixedSize(60, 72);
    m_aboutSysBtn->setFixedSize(60, 72);
    m_dataSettingBtn->setFixedSize(60, 72);

    m_modelSettingBtn->setText(tr("ģ������"));
    m_scriptSettingBtn->setText(tr("�ű�����"));
    m_dataSettingBtn->setText(tr("�ύ����"));
    m_systemSettingBtn->setText(tr("ϵͳ����"));
    m_updateSysBtn->setText(tr("ϵͳ����"));
    m_aboutSysBtn->setText(tr("����"));

    m_modelSettingBtn->setToolTip(tr("ģ�ͷ�������"));
    m_scriptSettingBtn->setToolTip(tr("�ű���������"));
    m_systemSettingBtn->setToolTip(tr("ϵͳ����"));
    m_updateSysBtn->setToolTip(tr("ϵͳ����"));
    m_aboutSysBtn->setToolTip(tr("����ϵͳ��Ϣ"));
    m_dataSettingBtn->setToolTip(tr("������ת����"));


    QIcon setting_model(":/icon/setting_model.png");
    QIcon setting_script(":/icon/setting_script.png");
    QIcon setting_data(":/icon/setting_data.png");
    QIcon setting_system(":/icon/setting_system.png");
    QIcon setting_update(":/icon/setting_update.png");
    QIcon setting_support(":/icon/setting_support.png");

    m_modelSettingBtn->setIcon( setting_model );
    m_scriptSettingBtn->setIcon( setting_script );
    m_systemSettingBtn->setIcon( setting_system );
    m_updateSysBtn->setIcon( setting_update );
    m_aboutSysBtn->setIcon( setting_support );
    m_dataSettingBtn->setIcon( setting_data );

    m_modelSettingBtn->setIconSize(QSize(48,48));
    m_scriptSettingBtn->setIconSize(QSize(48,48));
    m_systemSettingBtn->setIconSize(QSize(48,48));
    m_updateSysBtn->setIconSize(QSize(48,48));
    m_aboutSysBtn->setIconSize(QSize(48,48));
    m_dataSettingBtn->setIconSize(QSize(48,48));

    m_modelSettingBtn->setCheckable(true);
    m_scriptSettingBtn->setCheckable(true);
    m_systemSettingBtn->setCheckable(true);
    m_updateSysBtn->setCheckable(true);
    m_aboutSysBtn->setCheckable(true);
    m_dataSettingBtn->setCheckable(true);

    QHBoxLayout *hLayout0 = new QHBoxLayout();
    hLayout0->addWidget( m_modelSettingBtn );
    hLayout0->addWidget( m_scriptSettingBtn );
    hLayout0->addWidget( m_dataSettingBtn );
    hLayout0->addStretch(300);

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget( m_systemSettingBtn );
    hLayout1->addWidget( m_updateSysBtn );
    hLayout1->addWidget( m_aboutSysBtn );

    QGroupBox *groupBox0 = new QGroupBox(this);
    groupBox0->setLayout(hLayout0);

    QGroupBox *groupBox1 = new QGroupBox(this);
    groupBox1->setLayout(hLayout1);
    groupBox1->setGeometry(220, 0, 220, 92);

    //���������źŵĲ�
    connect(m_signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(clickedButton(QWidget*)));
    setButtonMapping();
}

void SettingTabPage::setButtonMapping()
{
    QList<QToolButton *> allButtons = this->findChildren<QToolButton *>();
    foreach( QToolButton* obj, allButtons)
    {
        //ԭʼ�źŴ��ݸ� m_signalMapper
        connect(obj, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
        //����signalmapper��ת������, ת��Ϊ����Ϊ QString ���͵��źţ� ���� text ��������Ϊ�������ݡ�
        m_signalMapper->setMapping(obj, obj);
    }
}

void SettingTabPage::clickedButton(QWidget* widget)
{
    QToolButton *btn = (QToolButton *)widget;
    QList<QToolButton *> allButtons = this->findChildren<QToolButton *>();
    foreach( QToolButton* obj, allButtons)
    {
        if(obj != btn)
            obj->setChecked(false);
    }

//    QString str = "abc_di58+4.4/e2[f4s]h{h8";
//    QStringList numList = str.split( QRegExp("([^0-9]+)") );
//    QStringList charList = str.split( QRegExp("([^A-Za-z]+)") );
}







