/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/
#include "colorshowlabel.h"
#include <QApplication>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

YviColorShowLabel::YviColorShowLabel(QWidget *parent) :
    QFrame(parent)
{
    this->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    this->setAcceptDrops(true);
    m_mousePressed = false;
}

void YviColorShowLabel::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    drawFrame(&p);
    p.fillRect(contentsRect()&e->rect(), m_color);
}

void YviColorShowLabel::mousePressEvent(QMouseEvent *e)
{
    m_mousePressed = true;
    m_pressPos = e->pos();
}

void YviColorShowLabel::mouseMoveEvent(QMouseEvent *e)
{
    if (!m_mousePressed)
        return;
    if ((m_pressPos - e->pos()).manhattanLength() > QApplication::startDragDistance())
    {
        QMimeData *mime = new QMimeData;
        mime->setColorData(m_color);
        QPixmap pix(30, 20);
        pix.fill(m_color);
        QPainter p(&pix);
        p.drawRect(0, 0, pix.width() - 1, pix.height() - 1);
        p.end();
        QDrag *drg = new QDrag(this);
        drg->setMimeData(mime);
        drg->setPixmap(pix);
        m_mousePressed = false;
        drg->start();
    }
}

void YviColorShowLabel::mouseReleaseEvent(QMouseEvent *)
{
    if (!m_mousePressed)
        return;
    m_mousePressed = false;
}
