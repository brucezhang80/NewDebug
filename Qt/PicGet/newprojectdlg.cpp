#include "newprojectdlg.h"
#include <QGridLayout>


NewProjectDlg::NewProjectDlg(QWidget *parent) :
    QDialog(parent)
{
    m_proUrlLedit           = new QLineEdit(tr("网址"), this);
    m_proTitleLedit         = new QLineEdit(tr("项目标题"), this);
    m_proSavepathLedit      = new QLineEdit(tr("保存位置"), this);
    m_proSetSavepathPbtn    = new QPushButton(tr("..."), this);
    m_proAddPbtn            = new QPushButton(tr("添加"), this);

    QGridLayout* gridLayout = new QGridLayout(this);

    gridLayout->addWidget(m_proUrlLedit, 0, 0, 1, 4);
    gridLayout->addWidget(m_proTitleLedit, 1, 0, 1, 4);
    gridLayout->addWidget(m_proSavepathLedit, 2, 0, 1, 4);
    gridLayout->addWidget(m_proSetSavepathPbtn, 2, 5, 1, 1);
    gridLayout->addWidget(m_proAddPbtn, 3, 5, 1, 1);

    this->setLayout(gridLayout);

    connect( m_proAddPbtn, SIGNAL(clicked()), this->parentWidget(), SLOT(addProjectSlot()) );
}
