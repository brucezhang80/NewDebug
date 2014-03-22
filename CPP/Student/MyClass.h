/*******************************************************************
 *  Copyright(c) 2013 麻菜,www.newdebug.com
 *  All rights reserved.
 *
 *  文件名称:MyClass.h
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


#ifndef MYCLASS_H
#define MYCLASS_H
#include <iostream>
#include <string>
#include <vector>
#include "Student.h"

using namespace std;

// 1.试过声明为类成员，并在外部使用指向成员的指针时，并不成功，不知道为什么。
// 所以使用方法2
// 2.必须要在外部声明排序所需的less函数
// 然后在类中声明为友元成员
bool less(Student* s1, Student* s2);

class MyClass
{
public:
    MyClass();
    ~MyClass();

    //添加一个学生记录
    void addStudent();

    void printInfo();

    int getCount();


    // 删除一个学生，使用学号作为唯一索引值
    bool remove(int id);

    // 查找一个学生，使用学号作为唯一索引值
    int  find(int id);

    // 编辑一个学生，使用学生作为唯一索引值
    void edit( int id);

    // 对所有学生按id号升序排序
    void ascSort();

private:
    void add(Student* student);

    // 按学生的id号升序排列
    friend bool less(Student* s1, Student* s2)
    {
        return (s1->getId() < s2->getId());
    }
private:
    //string m_class;
    vector<Student*> students;
};

#endif // MYCLASS_H
