/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/
#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QFrame>

class YviColorPicker : public QFrame
{
    Q_OBJECT
public:
    explicit YviColorPicker(QWidget *parent = 0);
    
signals:
    void newColor(int h, int s);

public slots:
    void setColor(int h, int s);

protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

private:
    int m_hue;
    int m_sat;

    QPoint colPt();
    int huePt(const QPoint &pt);
    int satPt(const QPoint &pt);
    void setCol(const QPoint &pt);

    QPixmap m_pix;
};

#endif // COLORPICKER_H
