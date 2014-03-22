/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/
#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>

static int pWidth = 220;
static int pHeight = 200;

YviColorPicker::YviColorPicker(QWidget *parent) :
    QFrame(parent)
{
    m_hue = 0; m_sat = 0;
    setColor(150, 255);

    setAttribute(Qt::WA_NoSystemBackground);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed) );
}

QPoint YviColorPicker::colPt()
{
    QRect r = contentsRect();
    return QPoint((360 - m_hue) * (r.width() - 1) / 360, (255 - m_sat) * (r.height() - 1) / 255);
}

int YviColorPicker::huePt(const QPoint &pt)
{
    QRect r = contentsRect();
    return 360 - pt.x() * 360 / (r.width() - 1);
}

int YviColorPicker::satPt(const QPoint &pt)
{
    QRect r = contentsRect();
    return 255 - pt.y() * 255 / (r.height() - 1);
}

void YviColorPicker::setCol(const QPoint &pt)
{
    setColor(huePt(pt), satPt(pt));
}

void YviColorPicker::setColor(int h, int s)
{
    int nhue = qMin(qMax(0,h), 359);
    int nsat = qMin(qMax(0,s), 255);
    if (nhue == m_hue && nsat == m_sat)
        return;

    QRect r(colPt(), QSize(20,20));
    m_hue = nhue; m_sat = nsat;
    r = r.united(QRect(colPt(), QSize(20,20)));
    r.translate(contentsRect().x()-9, contentsRect().y()-9);
    //    update(r);
    repaint(r);
}

QSize YviColorPicker::sizeHint() const
{
    return QSize(pWidth + 2*frameWidth(), pHeight + 2*frameWidth());
}

void YviColorPicker::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    drawFrame(&p);
    QRect r = contentsRect();

    p.drawPixmap(r.topLeft(), m_pix);
    QPoint pt = colPt() + r.topLeft();
    p.setPen(Qt::black);

    p.fillRect(pt.x()-9, pt.y(), 20, 2, Qt::black);
    p.fillRect(pt.x(), pt.y()-9, 2, 20, Qt::black);
}

void YviColorPicker::mouseMoveEvent(QMouseEvent *m)
{
    QPoint p = m->pos() - contentsRect().topLeft();
    setCol(p);
    emit newColor(m_hue, m_sat);
}

void YviColorPicker::mousePressEvent(QMouseEvent *m)
{
    QPoint p = m->pos() - contentsRect().topLeft();
    setCol(p);
    emit newColor(m_hue, m_sat);
}

void YviColorPicker::resizeEvent(QResizeEvent *ev)
{
    QFrame::resizeEvent(ev);

    int w = width() - frameWidth() * 2;
    int h = height() - frameWidth() * 2;
    QImage img(w, h, QImage::Format_RGB32);
    int x, y;
    uint *pixel = (uint *) img.scanLine(0);
    for (y = 0; y < h; y++)
    {
        const uint *end = pixel + w;
        x = 0;
        while (pixel < end)
        {
            QPoint p(x, y);
            QColor c;
            c.setHsv(huePt(p), satPt(p), 200);
            *pixel = c.rgb();
            ++pixel;
            ++x;
        }
    }
    m_pix = QPixmap::fromImage(img);
}
