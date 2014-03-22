/*******************************************************************
 *  Copyright(c) 2013 麻菜,www.newdebug.com
 *  All rights reserved.
 *
 *  文件名称:Student.cpp
 *  简要描述:一个学生类，包含学生的学号，姓名，年龄，电话，班级，性别基本信息
 *          如果需要升级版本，可以添加额外的其他信息
 *
 *  当前版本:1.0
 *  作者:麻菜
 *  Email:1239597@qq.com
 *  日期:2013年11月14日
 *  说明:本软件代码开源，并用于学习交流使用，不得用于商业交易。
 *      本软件代码一切版权著作权归本人所有，并拥有知识产权，
 *      任何人不得以自己名义修改并用于交易用途,否则，将追究法律责任。
 *  网址:http://www.newdebug.com
 *
 ******************************************************************/


#include "Student.h"

Student::Student():
    m_id(0),
    m_name(""),
    m_age(""),
    m_tel(""),
    m_class(""),
    m_sex("男")
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
 * 打印学生信息
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
 * 设置学生学生
 * @param id 学生的学号
 *
 */
void Student::setId(int id)
{
    m_id = id;
}


/**
 * 获得学生的学号
 * @param no param
 *
 */
int Student::getId()
{
    return m_id;
}


/**
 * 设置学生的姓名
 * @param name 学生的姓名
 *
 */
void Student::setName(const string &name)
{
    m_name = name;
}


/**
 * 获得学生的姓名
 * @param no param
 *
 */
string Student::getName() const
{
    return m_name;
}

/**
 * 设置学生的班级
 * @param sex 班级
 *
 */
void Student::setClass(const string &cla)
{
    m_class = cla;
}

/**
 * 获得学生的班级
 * @param no param
 *
 */
string Student::getClass() const
{
    return m_class;
}

/**
 * 设置学生的年龄
 * @param sex 年龄
 *
 */
void Student::setAge(const string &age)
{
    m_age = age;
}

/**
 * 获得学生的年龄
 * @param no param
 *
 */
string Student::getAge() const
{
    return m_age;
}

/**
 * 设置学生的电话
 * @param sex 电话
 *
 */
void Student::setTelphone(const string &tel)
{
    m_tel = tel;
}

/**
 * 获得学生的电话
 * @param no param
 *
 */
string Student::getTelphone() const
{
    return m_tel;
}

/**
 * 设置学生的性别
 * @param sex 性别
 *
 */
void Student::setSex(const string &sex)
{
    // 默认是"男"
    if( sex != "男" && sex != "女")
        return;

    m_sex = sex;
}

/**
 * 获得学生的性别
 * @param no param
 *
 */
string Student::getSex() const
{
    return m_sex;
}

