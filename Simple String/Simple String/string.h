#pragma once

//һ���򵥵�string ��
namespace mystring
{
	class string
	{
	public:

		//���캯��
		//�޲�+���� => ȫȱʡ�϶�Ϊһ
		string(const char* str="")
			:_str(new char[strlen(str)+1])
		{
			strcpy(_str, str);
		}

		//�������캯��
	
		//û��д�������죬���������Զ����ɿ������죬�����Զ������ͻ����ǳ��������!!!!
		//�� ����_strָ��ͬһ���ڴ棬��ջ֡����ʱ�����������������������ˣ�����˶Կռ���ظ��ͷ�
		//����� �����Ϊ_str�ڶ��Ͽ�һ�ݿռ䣬Ȼ�����ݿ���_str����
		//Ϊʲô������ջ�ϣ�ջ�ϲ���������
		// ��ͳд��
		/*string(const string& s)
			:_str(new char[strlen(s._str)+1])
		{
			strcpy(_str, s._str);
		}*/

		//�ִ�д��1
		string(const string& s)
			:_str(nullptr)   //�����ʼ������Ϊ�������������������������ֵ�����
		{
			string tmp(s._str);     //��s���ַ����������캯���������s���򴫶��󣬵��������죬�����ѭ��   
			//���ù��캯�����ڶ���new�ռ�
			//��ջ�ϴ�����sͬ����С�Ŀռ�  
			swap(_str, tmp._str);    //����ָ�򻥻���*thisֻ��Ҫ�ñ��˿��õļ���
			//������ջ�ϣ���ָ��Ŀռ��ڶ���
			//tmp����ʱ���󣬳�������������������������
		}


		// = ����    (��ֵ)
		//��ֵ������ͨ������ʵ�ֵģ������ǿ�����ֵ����ʹ��voidҲӦ���ܹ���ɸ�ֵ����
		//�з�������Ϊ s1=s2=s3,��������ֵ�����
		/*string& operator=(const string& s)
		{
			if (this != &s)      //���� s1=s1,���Լ����Լ���ֵ
			{
				char* arr = new char[strlen(s._str) + 1];
				strcpy(arr, s._str);
				delete[] _str;
				_str = arr;
			}
			return *this;
		}*/

		//�ִ�д��
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

		//��������
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
