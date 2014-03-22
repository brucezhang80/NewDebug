/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/
#ifndef COLORSHOWLABEL_H
#define COLORSHOWLABEL_H

#include <QFrame>

class YviColorShowLabel : public QFrame
{
    Q_OBJECT
public:
    explicit YviColorShowLabel(QWidget *parent = 0);
    
    void setColor(QColor c) { m_color = c; }

signals:
    void colorDropped(QRgb);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QColor  m_color;
    bool    m_mousePressed;
    QPoint  m_pressPos;
};

#endif // COLORSHOWLABEL_H
