/**
  *����:���
  *����:2013��6��20��
  *����:�Զ�����ɫѡ�����,��QColorDialog������ȡ���·�װ.������ͨ�����pick��ȡ��ɫ�Ŀؼ�
  *˵��:��Դ,���,ʹ��ʱ�뱣�ֿ�Դ����.
  *��ϵ:12319597@qq.com
  *����:www.newdebug.com
**/
#include <QtGui/QApplication>
#include "yvicolordialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    YviColorDialog w;
    w.show();
    
    return a.exec();
}
