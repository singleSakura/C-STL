#pragma once
//注意测试函数的位置，为了与系统的string区别开，调用mystring空间内的函数
namespace mystring
{
	
	class string
	{
	public:
		typedef char* iterator;

		//隐含this指针，故不需要传值
		iterator begin()
		{
			return _str;
		}
		iterator end()
		{
			return _str + _size;
		}

		//为了解决const string s，故用const string* this接收
		size_t size()const
		{
			return _size;
		}
		size_t capacity()const
		{
			return _capacity;
		}

		//[]重载，由于默认的this指针类型为string* this,故如果变量类型为const string s
		//那么直接传会发生权限放大，因此需要将this类型变为 const，故需要函数重载
		char operator[](size_t i)
		{
			assert(i < _size);
			return _str[i];
		}
		const char& operator[](size_t i)const
		{
			assert(i < _size);
			return _str[i];
		}

		//返回C格式的字符串
		char* c_str()
		{
			return _str;
		}

		//构造函数
		string(const char* str = "")
			:_str(new char[strlen(str) + 1])    //真正开的空间大小是strlen(str)+1,因为有\0
		{
			_size = strlen(str);
			_capacity = _size;
			strcpy(_str, str);
		}
		void reverse(size_t n)
		{
			if (n > _capacity)
			{
				char* arr = new char[n+1];
				strcpy(arr, _str);
				delete[]_str;
				_str = arr;
				_capacity = n;
			}
		}
		//拷贝构造：_str指向在堆上开辟的一份空间(方便管理)
		string(const string& s)
		{
			_size = s._size;
			_capacity = s._capacity;
			char* arr = new char[_size + 1];
			strcpy(arr, s._str);
			delete[] _str;
			_str = arr;
		}

		//赋值重载
		string& operator=(const string& s)    //有返回值是因为要实现连续=
		{
			if (this != &s)      //避免给自己赋值
			{
				char* arr = new char[s._capacity + 1];
				strcpy(arr, s._str);
				delete[] _str;
				_str = arr;
				_size = s._size;
				_capacity = s._capacity;
			}
			return *this;
		}

		//+=重载
		string& operator+=(char ch)
		{
			this->push_back(ch);
			return *this;
		}
		string& operator+=(const char* ch)
		{
			this->append(ch);
			return *this;
		}


		//字符插入
		void push_back(char ch)
		{
			if (_size + 1 > _capacity)
			{
				size_t newcapacity = (_capacity == 0) ? 2 : 2 * _capacity;
				reverse(newcapacity);
			}
			_str[_size++] = ch;
			_str[_size] = '\0';
		}

		//字符串插入
		void append(const char* s)
		{
			size_t len = strlen(s);
			if (_size + len > _capacity)
			{
				size_t newcapacity = _size + len;
				reverse(newcapacity);
			}
			strcpy(_str + _size, s);
			_size += len;
		}

		//insert实现,注意：insert是有string&的返回值的
		string& insert(size_t pos,char ch)
		{
			assert(pos>=0&&pos < _size);
		
			if (_size >= _capacity)
			{
				size_t newcapacity = (_capacity == 0) ? 4 : 2 * _capacity;
				reverse(newcapacity);
			}
			int end = (int)_size;
			while (end>= (int)pos)     //如果这里不强转，则-1>0时，会转为无符号类型的比较，因此继续循环
			{
				_str[end+1] = _str[end];
				end--;
			}
			_str[pos] = ch;
			_size++;
			return *this;
		}

		string& insert(size_t pos, const char* str)   //insert(...,"具有常属性")
		{
			assert(pos>=0&&pos < _size);
			size_t len = strlen(str);
			if (len+_size >= _capacity)
			{
				size_t newcapacity = len + _size;
				reverse(newcapacity);
			}
			int end = (int)_size;
			while (end >=(int)pos)
			{
				_str[end+len] = _str[end];
				end--;
			}
			strncpy(pos + _str, str,len);    //不拷贝str的/0
			_size += len;
			/*for (int i = 0; i < len; i++)
			{
				_str[pos++] = str[i];
			}*/
			return *this;
			
		}

		//有效字符的个数变为n，多余用字符c填充
		//1.n<size      2.n>size&&n<=capacity         3.n>capacity
		//=>无论怎样，最终都更新size，有多余空间则填充，空间不够则扩容在填充
		void resize(size_t n, char c = '\0')      //不给c，则默认空字符
		{
			assert(n >= 0);
			if (n > _capacity)
			{
				reverse(n);
			}
			for (size_t i = _size; i < n; i++)
			{
				_str[i] = c;
			}
			_size = n;
			_str[n] = '\0';      //如果扩容，则需要保证有效数据的下一位为\0
			                     //如果不扩容，需要保证有效数据的下一位为\0,
			                     //(abc\0)=>(a)bc\0    =>需要保有效数据的下一位为\0

		}

		//erase,删除pos位置开始，长为len的字符数，len不给则默认到尾
		//为什么返回string&,为了解决迭代器失效
		string& erase(size_t pos = 0, size_t len = npos)
		{
			if (len >= _size - pos)
			{
				_str[pos] = '\0';
				_size = pos;
			}
			else
			{
				size_t i = pos + len;
				while (i <= _size)           //从后往前覆盖
				{
					_str[i - len] = _str[i];
					i++;
				}
				_size -= len;
			}
			return *this;
		}

		//在pos处往后查找，找到返回起始下标
		size_t find(char ch, size_t pos=0)const
		{
			for (size_t i = pos; i < _size; i++)
			{
				if (_str[i] == ch)
				{
					return i;
				}
			}
			return npos;
		}

		size_t find(const char* s, size_t pos = 0)
		{
			char* p = strstr(_str, s);
			if (p == nullptr)
			{
				return npos;
			}
			else
			{
				return p - _str;
			}
		}

		bool operator<(const string& s)
		{
			int tmp = strcmp(_str, s._str);
			return tmp < 0;
		}
		bool operator==(const string& s)
		{
			int tmp = strcmp(_str, s._str);
			return tmp == 0;
		}
		bool operator<=(const string& s)
		{
			return (*this == s) || (*this < s);
		}
		bool operator>(const string& s)
		{
			int tmp = strcmp(_str, s._str);
			return tmp > 0;
		}
		bool operator>=(const string& s)
		{
			return (*this == s) || (*this > s);
		}
		bool operator!=(const string& s)
		{
			return !(*this == s);
		}

		void clear()
		{
			_size = 0;
			_str[0] = '\0';
		}

		//析构函数
		~string()
		{
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}
	private:
		char* _str;
		size_t _size;           //当前有效字符数
		size_t _capacity;       //当前字符容量(能存多少个有效字符，\0不算)

		const static size_t npos = -1;
	};

	ostream& operator<<(ostream& out, const string& s)
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			out << s[i];
		}

		return out;
	}

	istream& operator>>(istream& in, string& s)
	{
		s.clear();
		char buff[128] = { '\0' };
		size_t i = 0;
		char ch = in.get();
		while (ch != ' ' && ch != '\n')
		{
			if (i == 127)
			{
				// 满了
				s += buff;
				i = 0;
			}

			buff[i++] = ch;

			ch = in.get();
		}

		if (i > 0)
		{
			buff[i] = '\0';
			s += buff;
		}

		return in;
	}

	void test1()                            
	{
		string s1("hello world");
		string s2(s1);
		s1.insert(0, "a");
		s1.insert(1, 'b');
		s1.insert(2, 'c');
		s1.insert(3, "d");
		s1.insert(4, "e");
		for (int i = 0; i < s1.size(); i++)
		{
			cout << s1[i];
		}
	}

	void test2()
	{
		string s1("hello");
		s1.reverse(10);
		cout <<s1<< endl;
		cout << s1.size() << endl;
		cout << s1.capacity() << endl;

		s1.resize(8,'x');
		cout << s1 << endl;
		cout << s1.size() << endl;
		cout << s1.capacity() << endl;

		s1.resize(18,'a');
		cout << s1 << endl;
		cout << s1.size() << endl;
		cout << s1.capacity() << endl;
	}
}

