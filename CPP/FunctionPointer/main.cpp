#include <iostream>

using namespace std;

class PtrClass
{
public:
    PtrClass()
    {
        m_ptr[0] = &PtrClass::print0;
        m_ptr[1] = &PtrClass::print1;
        m_ptr[2] = &PtrClass::print2;
        m_ptr[3] = &PtrClass::print3;
    }
    ~PtrClass() {}

    void print0()
    {
        cout << "print 0" << endl;
    }

    void print1()
    {
        cout << "print 1" << endl;
    }

    void print2()
    {
        cout << "print 2" << endl;
    }

    void print3()
    {
        cout << "print 3" << endl;
    }

    void doSomething(int i)
    {
        (this->*m_ptr[i])();
    }

private:
    void (PtrClass::*m_ptr[4])();
};

int main()
{
    PtrClass* ptr = new PtrClass();

    for(int i = 0; i < 4; ++i)
        ptr->doSomething(i);

    delete ptr;

    return 0;
}

