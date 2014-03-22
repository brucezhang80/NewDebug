/*******************************************************************
 *  Copyright(c) 2013 麻菜,www.newdebug.com
 *  All rights reserved.
 *
 *  文件名称:mian.cpp
 *  简要描述: 程序入口文件
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

#include <iostream>
#include <string>
#include "Student.h"
#include "MyClass.h"

using namespace std;

void setup()
{
    cout<<"\t当前版本:1.01\t作者:麻菜\n";
    cout<<"\tCopyright(c) 2013 Macai\n";
    cout<<"\thttp://www.newdebug.com\n";
    cout<<"***************************************\n";
    cout<<"            请选择服务                 \n";
    cout<<"          1 - 录入学生信息             \n";
    cout<<"          2 - 列出学生信息             \n";
    cout<<"          3 - 修改学生信息             \n";
    cout<<"          4 - 删除学生信息             \n";
    cout<<"          5 - 排序                     \n";
    cout<<"          6 - 计数                     \n";
    cout<<"          7 - 退出                     \n";
    cout<<"***************************************\n";
}

void clean()
{
    system("cls");
}

int main()
{
    // new一个班级对象
    // 后面向该班级的学生做一些操作
    MyClass* myclass = new MyClass;

    do
    {
        setup();
        int nMenu;
        cin >> nMenu;
        clean();
        switch (nMenu)
        {
        case 1: // 录入学生记录
            myclass->addStudent();
            clean();
            break;
        case 2: // 列出学生信息
        {
            if( myclass->getCount() == 0)
                cout << "当前班级学生数为0，请录入学生记录。";
            else
                myclass->printInfo();

            system("pause");
            clean();
        };
            break;
        case 3: // 修改学生信息
            if( myclass->getCount() == 0)
                cout << "当前班级学生数为0，请录入学生记录。";
            else
            {
                myclass->printInfo();
                cout << "输入要修改的学生的学号：";
                int id;
                cin >> id;
                myclass->edit(id);
            }
            system("pause");
            clean();
            break;
        case 4: // 删除学生信息
            if( myclass->getCount() == 0)
                cout << "当前班级学生数为0，请录入学生记录。";
            else
            {
                myclass->printInfo();
                cout << "输出要删除的学生的学号：";
                int id;
                cin >> id;
                if ( myclass->remove(id) )
                    cout << "删除成功！";
                else
                    cout << "删除失败！没有该学号。";
            }
            system("pause");
            clean();
            break;
        case 5: // 排序
            if( myclass->getCount() == 0)
                cout << "当前班级学生数为0，请录入学生记录。";
            else
            {
                cout << "当前班级学生升序排序结果： \n";
                cout<< "学号\t姓名\t年龄\t电话\t班级\t性别\n";
                myclass->ascSort();
            }
            system("pause");
            clean();
            break;
        case 6: // 计数
            cout << "当前班级学生总数为: " << myclass->getCount() << endl;
            system("pause");
            clean();
            break;
        case 7: // 退出
            cout << "按回车退出程序。";
            return 0;
        }
    }while(1);

    return 0;
}

