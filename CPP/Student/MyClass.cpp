/*******************************************************************
 *  Copyright(c) 2013 麻菜,www.newdebug.com
 *  All rights reserved.
 *
 *  文件名称:MyClass.cpp
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
 * 添加学生，录入新学生的信息
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
            cout<< "请输入学生学号、姓名、年龄、电话、班级、性别男/女：\n";

            cin >> std[0];
            id = atoi(std[0].c_str());
            if ( find(id) != -1 )
                cout << "学号为 " << id << " 的学生已经存在！请重新输入。\n\n";
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
        //把一个完整信息的学生加入班级
        add(stu);

        delete [] std;

        string str = "";
        cout << "继续?（y/n)\n";
        cin >> str;
        if( str == "n" || str == "N")
        {
            cout << "录入结束。";
            system("pause");
            break;
        }
    }
}

/**
 * 打印该班级的学生信息
 * @param no param
 *
 */
void MyClass::printInfo()
{
    cout << "当前班级所有学生信息如下： \n";
    cout<< "学号\t姓名\t年龄\t电话\t班级\t性别\n";
    for(unsigned int i = 0; i < students.size(); ++i)
    {
        students[i]->outputInfo();
    }
}

/**
 * 得到班级的学生数量
 * @param no param
 *
 */
int MyClass::getCount()
{
    return students.size();
}

/**
 * 添加一个学生
 * @param id 学生
 *
 */
void MyClass::add(Student *student)
{
    students.push_back(student);
}


/**
 * 按学号删除学生
 * @param id 学生的学号
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
            // 不能写成 students.erase(it);
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
 * 按学号查找学生
 * @param id 学生的学号
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
 * 编辑学号为id的学生信息
 * @param id 学生的学号
 *
 */
void MyClass::edit(int id)
{
    if( find(id) == -1)
    {
        cout << "找不到该学生信息。\n";
        return;
    }

    vector<Student*>::iterator it = students.begin();
    for( ; it != students.end(); it++)
    {
        int n = (*it)->getId();
        if( n == id )
        {
            cout<<"******************************\n";
            cout<<"             请选择:          \n";
            cout<<"        1 - 修改姓名          \n";
            cout<<"        2 - 修改年龄          \n";
            cout<<"        3 - 修改电话          \n";
            cout<<"        4 - 修改班级          \n";
            cout<<"        5 - 修改性别          \n";
            cout<<"******************************\n";
            int n;
            cin >> n;
            string strNew;
            switch( n )
            {
            case 1:
                cout << "请输入学生新的名字：";
                cin >> strNew;
                (*it)->setName( strNew );
                break;
            case 2:
                cout << "请输入学生新的年龄：";
                cin >> strNew;
                (*it)->setAge( strNew );
                break;
            case 3:
                cout << "请输入学生新的电话：";
                cin >> strNew;
                (*it)->setTelphone( strNew );
                break;
            case 4:
                cout << "请输入学生新的班级：";
                cin >> strNew;
                (*it)->setClass( strNew );
                break;
            case 5:
                cout << "请输入学生性别：";
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
 * 按学生的学号Id升序排序
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
