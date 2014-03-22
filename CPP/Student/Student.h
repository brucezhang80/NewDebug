/*******************************************************************
 *  Copyright(c) 2013 麻菜,www.newdebug.com
 *  All rights reserved.
 *
 *  文件名称:Student.h
 *  简要描述:一个班级类，添加，修改，删除，查询排序学生信息
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

    // 初始化班级学生
    void init();
    void outputInfo();

    // 设置和获取学号
    void setId(int id);
    int  getId();

    // 设置和获取改名
    void setName(const string &name);
    string getName() const;

    // 设置和获取班级
    void setClass(const string &cla);
    string getClass() const;

    // 设置和获取生日
    void setAge(const string &age);
    string getAge() const;

    // 设置和获取电话号码
    void setTelphone(const string &tel);
    string getTelphone() const;

    // 设置和获取性别 仅"男"/"女"
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
