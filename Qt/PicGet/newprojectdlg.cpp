#include "newprojectdlg.h"
#include <QGridLayout>


NewProjectDlg::NewProjectDlg(QWidget *parent) :
    QDialog(parent)
{
    m_proUrlLedit           = new QLineEdit(tr("��ַ"), this);
    m_proTitleLedit         = new QLineEdit(tr("��Ŀ����"), this);
    m_proSavepathLedit      = new QLineEdit(tr("����λ��"), this);
    m_proSetSavepathPbtn    = new QPushButton(tr("..."), this);
    m_proAddPbtn            = new QPushButton(tr("���"), this);

    QGridLayout* gridLayout = new QGridLayout(this);

    gridLayout->addWidget(m_proUrlLedit, 0, 0, 1, 4);
    gridLayout->addWidget(m_proTitleLedit, 1, 0, 1, 4);
    gridLayout->addWidget(m_proSavepathLedit, 2, 0, 1, 4);
    gridLayout->addWidget(m_proSetSavepathPbtn, 2, 5, 1, 1);
    gridLayout->addWidget(m_proAddPbtn, 3, 5, 1, 1);

    this->setLayout(gridLayout);

    connect( m_proAddPbtn, SIGNAL(clicked()), this->parentWidget(), SLOT(addProjectSlot()) );
}
