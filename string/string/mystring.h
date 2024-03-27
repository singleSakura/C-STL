#pragma once
//ע����Ժ�����λ�ã�Ϊ����ϵͳ��string���𿪣�����mystring�ռ��ڵĺ���
namespace mystring
{
	
	class string
	{
	public:
		typedef char* iterator;

		//����thisָ�룬�ʲ���Ҫ��ֵ
		iterator begin()
		{
			return _str;
		}
		iterator end()
		{
			return _str + _size;
		}

		//Ϊ�˽��const string s������const string* this����
		size_t size()const
		{
			return _size;
		}
		size_t capacity()const
		{
			return _capacity;
		}

		//[]���أ�����Ĭ�ϵ�thisָ������Ϊstring* this,�������������Ϊconst string s
		//��ôֱ�Ӵ��ᷢ��Ȩ�޷Ŵ������Ҫ��this���ͱ�Ϊ const������Ҫ��������
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

		//����C��ʽ���ַ���
		char* c_str()
		{
			return _str;
		}

		//���캯��
		string(const char* str = "")
			:_str(new char[strlen(str) + 1])    //�������Ŀռ��С��strlen(str)+1,��Ϊ��\0
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
		//�������죺_strָ���ڶ��Ͽ��ٵ�һ�ݿռ�(�������)
		string(const string& s)
		{
			_size = s._size;
			_capacity = s._capacity;
			char* arr = new char[_size + 1];
			strcpy(arr, s._str);
			delete[] _str;
			_str = arr;
		}

		//��ֵ����
		string& operator=(const string& s)    //�з���ֵ����ΪҪʵ������=
		{
			if (this != &s)      //������Լ���ֵ
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

		//+=����
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


		//�ַ�����
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

		//�ַ�������
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

		//insertʵ��,ע�⣺insert����string&�ķ���ֵ��
		string& insert(size_t pos,char ch)
		{
			assert(pos>=0&&pos < _size);
		
			if (_size >= _capacity)
			{
				size_t newcapacity = (_capacity == 0) ? 4 : 2 * _capacity;
				reverse(newcapacity);
			}
			int end = (int)_size;
			while (end>= (int)pos)     //������ﲻǿת����-1>0ʱ����תΪ�޷������͵ıȽϣ���˼���ѭ��
			{
				_str[end+1] = _str[end];
				end--;
			}
			_str[pos] = ch;
			_size++;
			return *this;
		}

		string& insert(size_t pos, const char* str)   //insert(...,"���г�����")
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
			strncpy(pos + _str, str,len);    //������str��/0
			_size += len;
			/*for (int i = 0; i < len; i++)
			{
				_str[pos++] = str[i];
			}*/
			return *this;
			
		}

		//��Ч�ַ��ĸ�����Ϊn���������ַ�c���
		//1.n<size      2.n>size&&n<=capacity         3.n>capacity
		//=>�������������ն�����size���ж���ռ�����䣬�ռ䲻�������������
		void resize(size_t n, char c = '\0')      //����c����Ĭ�Ͽ��ַ�
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
			_str[n] = '\0';      //������ݣ�����Ҫ��֤��Ч���ݵ���һλΪ\0
			                     //��������ݣ���Ҫ��֤��Ч���ݵ���һλΪ\0,
			                     //(abc\0)=>(a)bc\0    =>��Ҫ����Ч���ݵ���һλΪ\0

		}

		//erase,ɾ��posλ�ÿ�ʼ����Ϊlen���ַ�����len������Ĭ�ϵ�β
		//Ϊʲô����string&,Ϊ�˽��������ʧЧ
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
				while (i <= _size)           //�Ӻ���ǰ����
				{
					_str[i - len] = _str[i];
					i++;
				}
				_size -= len;
			}
			return *this;
		}

		//��pos��������ң��ҵ�������ʼ�±�
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

		//��������
		~string()
		{
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}
	private:
		char* _str;
		size_t _size;           //��ǰ��Ч�ַ���
		size_t _capacity;       //��ǰ�ַ�����(�ܴ���ٸ���Ч�ַ���\0����)

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
				// ����
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

