/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/

#ifndef MYCOLORDIALOG_H
#define MYCOLORDIALOG_H

#include "colorluminancepicker.h"
#include <QDialog>

class YviColorPicker;
class YviColorShower;

class YviColorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit YviColorDialog(QWidget *parent = 0);
    ~YviColorDialog();
signals:
    void currentColorChanged(const QColor &color);

private slots:
        void newHsv(int h, int s, int v);

private:
    YviColorPicker             *m_colorPicker;
    YviColorShower             *m_colorShower;
    YviColorLuminancePicker    *m_colorLuminancePicker;
};

#endif // MYCOLORDIALOG_H
