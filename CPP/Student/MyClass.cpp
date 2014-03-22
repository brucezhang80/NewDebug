/*******************************************************************
 *  Copyright(c) 2013 ���,www.newdebug.com
 *  All rights reserved.
 *
 *  �ļ�����:MyClass.cpp
 *  ��Ҫ����:һ���༶�࣬��ӣ��޸ģ�ɾ������ѯ����ѧ����Ϣ
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


#include "MyClass.h"
#include <algorithm>

//bool (MyClass::*pLess)(Student *, Student* ) = &MyClass::less;

MyClass::MyClass()
{
    // do nothing
}

MyClass::~MyClass()
{
    students.clear();
}

/**
 * ���ѧ����¼����ѧ������Ϣ
 * @param no param
 *
 */
void MyClass::addStudent()
{
    while(1)
    {
        Student* stu = new Student;
        string* std = new string[7];
        int id = 0;

        do
        {
            cout<< "������ѧ��ѧ�š����������䡢�绰���༶���Ա���/Ů��\n";

            cin >> std[0];
            id = atoi(std[0].c_str());
            if ( find(id) != -1 )
                cout << "ѧ��Ϊ " << id << " ��ѧ���Ѿ����ڣ����������롣\n\n";
            else
                break;
        }while(1);

        cin >> std[1] >> std[2] >> std[3] >> std[4] >> std[5];

        stu->setId(id);
        stu->setName(std[1]);
        stu->setAge(std[2]);
        stu->setTelphone(std[3]);
        stu->setClass(std[4]);
        stu->setSex(std[5]);
        //��һ��������Ϣ��ѧ������༶
        add(stu);

        delete [] std;

        string str = "";
        cout << "����?��y/n)\n";
        cin >> str;
        if( str == "n" || str == "N")
        {
            cout << "¼�������";
            system("pause");
            break;
        }
    }
}

/**
 * ��ӡ�ð༶��ѧ����Ϣ
 * @param no param
 *
 */
void MyClass::printInfo()
{
    cout << "��ǰ�༶����ѧ����Ϣ���£� \n";
    cout<< "ѧ��\t����\t����\t�绰\t�༶\t�Ա�\n";
    for(unsigned int i = 0; i < students.size(); ++i)
    {
        students[i]->outputInfo();
    }
}

/**
 * �õ��༶��ѧ������
 * @param no param
 *
 */
int MyClass::getCount()
{
    return students.size();
}

/**
 * ���һ��ѧ��
 * @param id ѧ��
 *
 */
void MyClass::add(Student *student)
{
    students.push_back(student);
}


/**
 * ��ѧ��ɾ��ѧ��
 * @param id ѧ����ѧ��
 *
 */
bool MyClass::remove(int id)
{
    vector<Student*>::iterator it = students.begin();
    while (it != students.end())
    {
        int n = (*it)->getId();
        if( n == id )
        {
            // ����д�� students.erase(it);
            it = students.erase(it);
            return true;
        }
        else
        {
            it++;
        }
    }

    return false;
}

/**
 * ��ѧ�Ų���ѧ��
 * @param id ѧ����ѧ��
 *
 */
int MyClass::find(int id)
{
    vector<Student*>::iterator it = students.begin();
    for( ; it != students.end(); it++)
    {
        int n = (*it)->getId();
        if( n == id )
        {
            (*it)->outputInfo();
            return id;
        }
    }

    return -1;
}

/**
 * �༭ѧ��Ϊid��ѧ����Ϣ
 * @param id ѧ����ѧ��
 *
 */
void MyClass::edit(int id)
{
    if( find(id) == -1)
    {
        cout << "�Ҳ�����ѧ����Ϣ��\n";
        return;
    }

    vector<Student*>::iterator it = students.begin();
    for( ; it != students.end(); it++)
    {
        int n = (*it)->getId();
        if( n == id )
        {
            cout<<"******************************\n";
            cout<<"             ��ѡ��:          \n";
            cout<<"        1 - �޸�����          \n";
            cout<<"        2 - �޸�����          \n";
            cout<<"        3 - �޸ĵ绰          \n";
            cout<<"        4 - �޸İ༶          \n";
            cout<<"        5 - �޸��Ա�          \n";
            cout<<"******************************\n";
            int n;
            cin >> n;
            string strNew;
            switch( n )
            {
            case 1:
                cout << "������ѧ���µ����֣�";
                cin >> strNew;
                (*it)->setName( strNew );
                break;
            case 2:
                cout << "������ѧ���µ����䣺";
                cin >> strNew;
                (*it)->setAge( strNew );
                break;
            case 3:
                cout << "������ѧ���µĵ绰��";
                cin >> strNew;
                (*it)->setTelphone( strNew );
                break;
            case 4:
                cout << "������ѧ���µİ༶��";
                cin >> strNew;
                (*it)->setClass( strNew );
                break;
            case 5:
                cout << "������ѧ���Ա�";
                cin >> strNew;
                (*it)->setSex( strNew );
                break;
            }

            (*it)->outputInfo();
            break;
        }
    }
}

/**
 * ��ѧ����ѧ��Id��������
 * @param no param
 *
 */
void MyClass::ascSort()
{
    sort(students.begin(), students.end(), ::less);
    for(unsigned int i = 0; i < students.size(); ++i)
    {
        students[i]->outputInfo();
    }
}
