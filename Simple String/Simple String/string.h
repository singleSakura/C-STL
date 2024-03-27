#pragma once

//一个简单的string 类
namespace mystring
{
	class string
	{
	public:

		//构造函数
		//无参+带参 => 全缺省合二为一
		string(const char* str="")
			:_str(new char[strlen(str)+1])
		{
			strcpy(_str, str);
		}

		//拷贝构造函数
	
		//没有写拷贝构造，编译器会自动生成拷贝构造，对于自定义类型会造成浅拷贝问题!!!!
		//即 两个_str指向同一个内存，在栈帧结束时，会调用两次析构函数，因此，造成了对空间的重复释放
		//解决： 深拷贝：为_str在堆上开一份空间，然后将内容拷给_str即可
		//为什么不是在栈上？栈上不方便扩容
		// 传统写法
		/*string(const string& s)
			:_str(new char[strlen(s._str)+1])
		{
			strcpy(_str, s._str);
		}*/

		//现代写法1
		string(const string& s)
			:_str(nullptr)   //必须初始化，因为交换后会调用析构函，析构随机值会崩溃
		{
			string tmp(s._str);     //传s的字符串，调构造函数，如果传s，则传对象，调拷贝构造，造成死循环   
			//调用构造函数，在堆上new空间
			//在栈上创建与s同样大小的空间  
			swap(_str, tmp._str);    //两个指向互换，*this只需要用别人开好的即可
			//变量在栈上，但指向的空间在堆上
			//tmp是临时对象，出作用域调用析构函数进行清空
		}


		// = 重载    (赋值)
		//赋值操作是通过函数实现的，而不是靠返回值，即使是void也应该能够完成赋值操作
		//有反正是因为 s1=s2=s3,即连续赋值的情况
		/*string& operator=(const string& s)
		{
			if (this != &s)      //避免 s1=s1,即自己对自己赋值
			{
				char* arr = new char[strlen(s._str) + 1];
				strcpy(arr, s._str);
				delete[] _str;
				_str = arr;
			}
			return *this;
		}*/

		//现代写法
		string& operator=(string s)
		{
			swap(_str, s._str);
			return *this;
		}

		size_t size()
		{
			char* s = this->_str;
			size_t count = 0;
			while (*s != '\0')
			{
				s++;
				count++;
			}
			return count;
		}

		//析构函数
		~string()
		{
			if (_str)
			{
				delete[] _str;
				_str = nullptr;
			}
		}
	private:
		char* _str;
	};

	void test()
	{
		string s("hello world");
		string s1(s);
		cout << s.size() <<"-"<< s1.size() << endl;
	}
}
