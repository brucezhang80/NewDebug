/*******************************************************************
 *  Copyright(c) 2013 ���,www.newdebug.com
 *  All rights reserved.
 *
 *  �ļ�����:Student.cpp
 *  ��Ҫ����:һ��ѧ���࣬����ѧ����ѧ�ţ����������䣬�绰���༶���Ա������Ϣ
 *          �����Ҫ�����汾��������Ӷ����������Ϣ
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


#include "Student.h"

Student::Student():
    m_id(0),
    m_name(""),
    m_age(""),
    m_tel(""),
    m_class(""),
    m_sex("��")
{
    // do nothing.
}

void Student::init()
{
    // current do nothing.
    // you can init students from a file
    // that has students of a class.
}


/**
 * ��ӡѧ����Ϣ
 * @param no param
 *
 */
void Student::outputInfo()
{
    cout << m_id << "\t" << m_name << "\t" \
        << m_age << "\t" << m_tel << "\t" \
        << m_class << "\t" << m_sex << endl;
}


/**
 * ����ѧ��ѧ��
 * @param id ѧ����ѧ��
 *
 */
void Student::setId(int id)
{
    m_id = id;
}


/**
 * ���ѧ����ѧ��
 * @param no param
 *
 */
int Student::getId()
{
    return m_id;
}


/**
 * ����ѧ��������
 * @param name ѧ��������
 *
 */
void Student::setName(const string &name)
{
    m_name = name;
}


/**
 * ���ѧ��������
 * @param no param
 *
 */
string Student::getName() const
{
    return m_name;
}

/**
 * ����ѧ���İ༶
 * @param sex �༶
 *
 */
void Student::setClass(const string &cla)
{
    m_class = cla;
}

/**
 * ���ѧ���İ༶
 * @param no param
 *
 */
string Student::getClass() const
{
    return m_class;
}

/**
 * ����ѧ��������
 * @param sex ����
 *
 */
void Student::setAge(const string &age)
{
    m_age = age;
}

/**
 * ���ѧ��������
 * @param no param
 *
 */
string Student::getAge() const
{
    return m_age;
}

/**
 * ����ѧ���ĵ绰
 * @param sex �绰
 *
 */
void Student::setTelphone(const string &tel)
{
    m_tel = tel;
}

/**
 * ���ѧ���ĵ绰
 * @param no param
 *
 */
string Student::getTelphone() const
{
    return m_tel;
}

/**
 * ����ѧ�����Ա�
 * @param sex �Ա�
 *
 */
void Student::setSex(const string &sex)
{
    // Ĭ����"��"
    if( sex != "��" && sex != "Ů")
        return;

    m_sex = sex;
}

/**
 * ���ѧ�����Ա�
 * @param no param
 *
 */
string Student::getSex() const
{
    return m_sex;
}

