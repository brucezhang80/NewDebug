/*******************************************************************
 *  Copyright(c) 2013 ���,www.newdebug.com
 *  All rights reserved.
 *
 *  �ļ�����:MyClass.h
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


#ifndef MYCLASS_H
#define MYCLASS_H
#include <iostream>
#include <string>
#include <vector>
#include "Student.h"

using namespace std;

// 1.�Թ�����Ϊ���Ա�������ⲿʹ��ָ���Ա��ָ��ʱ�������ɹ�����֪��Ϊʲô��
// ����ʹ�÷���2
// 2.����Ҫ���ⲿ�������������less����
// Ȼ������������Ϊ��Ԫ��Ա
bool less(Student* s1, Student* s2);

class MyClass
{
public:
    MyClass();
    ~MyClass();

    //���һ��ѧ����¼
    void addStudent();

    void printInfo();

    int getCount();


    // ɾ��һ��ѧ����ʹ��ѧ����ΪΨһ����ֵ
    bool remove(int id);

    // ����һ��ѧ����ʹ��ѧ����ΪΨһ����ֵ
    int  find(int id);

    // �༭һ��ѧ����ʹ��ѧ����ΪΨһ����ֵ
    void edit( int id);

    // ������ѧ����id����������
    void ascSort();

private:
    void add(Student* student);

    // ��ѧ����id����������
    friend bool less(Student* s1, Student* s2)
    {
        return (s1->getId() < s2->getId());
    }
private:
    //string m_class;
    vector<Student*> students;
};

#endif // MYCLASS_H
