/*******************************************************************
 *  Copyright(c) 2013 ���,www.newdebug.com
 *  All rights reserved.
 *
 *  �ļ�����:mian.cpp
 *  ��Ҫ����: ��������ļ�
 *
 *  ��ǰ�汾:1.0
 *  ����:���
 *  Email:1239597@qq.com
 *  ����:2013��11��14��
 *  ˵��:��������뿪Դ��������ѧϰ����ʹ�ã�����������ҵ���ס�
 *      ���������һ�а�Ȩ����Ȩ�鱾�����У���ӵ��֪ʶ��Ȩ��
 *      �κ��˲������Լ������޸Ĳ����ڽ�����;,���򣬽�׷���������Ρ�
 *  ��ַ:http://www.newdebug.com
 *
 ******************************************************************/

#include <iostream>
#include <string>
#include "Student.h"
#include "MyClass.h"

using namespace std;

void setup()
{
    cout<<"\t��ǰ�汾:1.01\t����:���\n";
    cout<<"\tCopyright(c) 2013 Macai\n";
    cout<<"\thttp://www.newdebug.com\n";
    cout<<"***************************************\n";
    cout<<"            ��ѡ�����                 \n";
    cout<<"          1 - ¼��ѧ����Ϣ             \n";
    cout<<"          2 - �г�ѧ����Ϣ             \n";
    cout<<"          3 - �޸�ѧ����Ϣ             \n";
    cout<<"          4 - ɾ��ѧ����Ϣ             \n";
    cout<<"          5 - ����                     \n";
    cout<<"          6 - ����                     \n";
    cout<<"          7 - �˳�                     \n";
    cout<<"***************************************\n";
}

void clean()
{
    system("cls");
}

int main()
{
    // newһ���༶����
    // ������ð༶��ѧ����һЩ����
    MyClass* myclass = new MyClass;

    do
    {
        setup();
        int nMenu;
        cin >> nMenu;
        clean();
        switch (nMenu)
        {
        case 1: // ¼��ѧ����¼
            myclass->addStudent();
            clean();
            break;
        case 2: // �г�ѧ����Ϣ
        {
            if( myclass->getCount() == 0)
                cout << "��ǰ�༶ѧ����Ϊ0����¼��ѧ����¼��";
            else
                myclass->printInfo();

            system("pause");
            clean();
        };
            break;
        case 3: // �޸�ѧ����Ϣ
            if( myclass->getCount() == 0)
                cout << "��ǰ�༶ѧ����Ϊ0����¼��ѧ����¼��";
            else
            {
                myclass->printInfo();
                cout << "����Ҫ�޸ĵ�ѧ����ѧ�ţ�";
                int id;
                cin >> id;
                myclass->edit(id);
            }
            system("pause");
            clean();
            break;
        case 4: // ɾ��ѧ����Ϣ
            if( myclass->getCount() == 0)
                cout << "��ǰ�༶ѧ����Ϊ0����¼��ѧ����¼��";
            else
            {
                myclass->printInfo();
                cout << "���Ҫɾ����ѧ����ѧ�ţ�";
                int id;
                cin >> id;
                if ( myclass->remove(id) )
                    cout << "ɾ���ɹ���";
                else
                    cout << "ɾ��ʧ�ܣ�û�и�ѧ�š�";
            }
            system("pause");
            clean();
            break;
        case 5: // ����
            if( myclass->getCount() == 0)
                cout << "��ǰ�༶ѧ����Ϊ0����¼��ѧ����¼��";
            else
            {
                cout << "��ǰ�༶ѧ�������������� \n";
                cout<< "ѧ��\t����\t����\t�绰\t�༶\t�Ա�\n";
                myclass->ascSort();
            }
            system("pause");
            clean();
            break;
        case 6: // ����
            cout << "��ǰ�༶ѧ������Ϊ: " << myclass->getCount() << endl;
            system("pause");
            clean();
            break;
        case 7: // �˳�
            cout << "���س��˳�����";
            return 0;
        }
    }while(1);

    return 0;
}

