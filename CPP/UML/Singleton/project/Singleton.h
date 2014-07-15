//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Singleton
//  @ File Name : Singleton.h
//  @ Date : 2014/7/15
//  @ Author : www.newdebug.com
//
//


#if !defined(_SINGLETON_H)
#define _SINGLETON_H


class Singleton
{
public:
	~Singleton();
	static Singleton* instance();

protected:
	Singleton();

private:
	static Singleton* m_instance;
};

#endif  //_SINGLETON_H