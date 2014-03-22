/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "WePlayer.h"
#include "ui_WePlayer.h"

WePlayer::WePlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WePlayer)
{
    ui->setupUi(this);
}

WePlayer::~WePlayer()
{
    delete ui;
}
