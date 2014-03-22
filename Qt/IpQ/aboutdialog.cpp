#include "aboutdialog.h"
#include <QtCore>
#include <QtGui>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    setWindowFlags( Qt::WindowStaysOnTopHint );

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    m_lbInfo = new QLabel(this);
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->addWidget(m_lbInfo, 0,0,1,1);
    gridLayout->addWidget(m_buttonBox, 1,0,1,1);

    retranslateUi();

    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void AboutDialog::retranslateUi()
{
    m_lbInfo->setText(QString(tr("<b>Name:\tIPQ</b>"
                               "<br>Dev:\tMacai"
                               "<br>Ver:\tv0.1"
                               "<br>Date:\t2013-6-4"
                               "<br>Copyright:\tSouthGis.com (c) 2013"
                               "<br>Use for:\tFor transfer data ip to ip at local network.")));

    setWindowTitle(tr("about IPQ"));
}
