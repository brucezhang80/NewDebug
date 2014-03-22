/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/
#ifndef COLORLUMINANCEPICKER_H
#define COLORLUMINANCEPICKER_H

#include <QWidget>

class YviColorLuminancePicker : public QWidget
{
    Q_OBJECT
public:
    explicit YviColorLuminancePicker(QWidget *parent = 0);
    
signals:
    void newHsv(int h, int s, int v);

public slots:
    void setCol(int h, int s, int v);
    void setCol(int h, int s);

protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    enum { foff = 3, coff = 4 }; //frame and contents offset
    int val;
    int hue;
    int sat;

    int y2val(int y);
    int val2y(int v);
    void setVal(int v);

    QPixmap *pix;
};

#endif // COLORLUMINANCEPICKER_H
