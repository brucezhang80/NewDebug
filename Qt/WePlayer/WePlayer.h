/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef WEPLAYER_H
#define WEPLAYER_H

#include <QWidget>

namespace Ui {
class WePlayer;
}

class WePlayer : public QWidget
{
    Q_OBJECT

public:
    explicit WePlayer(QWidget *parent = 0);
    ~WePlayer();

private:
    Ui::WePlayer *ui;
};

#endif // WEPLAYER_H
