#include "toolbartabwidget.h"
#include <QDebug>

ToolbarTabWidget::ToolbarTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    this->setParent(parent);
    //Set Tabs Movable Here
    this->setMovable(false);
    QObject::connect(this,SIGNAL(currentChanged(int)),this,SLOT(itemChangeSlot(int)));
}

void ToolbarTabWidget::itemChangeSlot(int index)
{
}
