/*******************************************************************
 *  Copyright(c) 2013 ���,www.newdebug.com
 *  All rights reserved.
 *
 *  �ļ�����:Student.h
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

#ifndef STUDENT_H
#define STUDENT_H
#include <iostream>
#include <string>

using namespace std;
class Student
{
public:
    Student();
    ~Student(){}

    // ��ʼ���༶ѧ��
    void init();
    void outputInfo();

    // ���úͻ�ȡѧ��
    void setId(int id);
    int  getId();

    // ���úͻ�ȡ����
    void setName(const string &name);
    string getName() const;

    // ���úͻ�ȡ�༶
    void setClass(const string &cla);
    string getClass() const;

    // ���úͻ�ȡ����
    void setAge(const string &age);
    string getAge() const;

    // ���úͻ�ȡ�绰����
    void setTelphone(const string &tel);
    string getTelphone() const;

    // ���úͻ�ȡ�Ա� ��"��"/"Ů"
    void setSex(const string &sex);
    string getSex() const;

private:
    Student& operator =(const Student &s)
    {
        this->m_id = s.m_id;
        this->m_name = s.m_name;
        this->m_class = s.m_class;
        this->m_age = s.m_age;
        this->m_tel = s.m_tel;
        this->m_sex = s.m_sex;

        return *this;
    }

//    bool operator<(Student *s)
//    {
//        return this->m_id < s->m_id;
//    }

//     bool operator()(Student *s1, Student *s2)
//     {
//         return ( s1->m_id < s1->m_id );
//     }
private:
    int 	m_id;
    string 	m_name;
    string	m_class;
    string 	m_age;
    string	m_tel;
    string	m_sex;
};

#endif // STUDENT_H
