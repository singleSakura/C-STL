#pragma once
#include <assert.h>
namespace myvector
{
	template <class T>   //ģ�壬���ý��ܲ�ͬ���͵�Ԫ��
	class vector
	{
	public:
		typedef T* iterator;               //T���͵�ָ�룬T�����ɱ��������ݲ�ʵ����
		typedef const T* const_iterator;    //const������,ֻ�ɶ�������д


		//���캯��
		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{
			;
		}

		//��������
		//��д�������Զ����ɣ������ɵ���ǳ�������漰��ָ�������⣬����ʱ���ظ��ͷţ���Ҫʹ�����
		vector(const vector<T>& arr)
			:_start(nullptr)
			,_finish(nullptr)
			,_endofstorage(nullptr)
		{
			reverse(arr.capacity());
			for (const auto& e : arr)  //����ֵ����
			{
				push_back(e);
			}

		}
		const_iterator begin()const
		{
			return _start;
		}

		const_iterator end()const
		{
			return _finish;
		}


		iterator begin()
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}

		size_t size()const
		{
			return _finish - _start;
		}

		size_t capacity()const
		{
			return _endofstorage - _start;
		}

		//���ݾͿ������������ʧЧ
		void reverse(size_t n)
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];
				if (_start)
				{
					memcpy(tmp, _start, sizeof(int) * sz);
					delete[] _start;    //_start��ʼ�ı�
				}
				_start = tmp;
				_finish = tmp + sz;     //����Ϊʲô����size()����Ϊ��ʱ_start��ָ���Ѿ��ı���
				                          //�����Ҫ��ǰ����size�Ĵ�С
				_endofstorage = tmp + n;  //ͬ�����ﲻ����capacity()����Ϊʵ���õ���_start

			}
			
		}

		//push_back()
		void push_back(const T& val)   //�����ã����ٲ���Ҫ�Ŀ���
		{
			if (_finish == _endofstorage)
			{
				size_t newcapacity = (capacity() == 0) ? 4 : 2 * capacity();
				reverse(newcapacity);
			}
			*_finish = val;
			_finish++;
			//insert(_finish, val);
		}

		//pop_back  ,βɾ
		void pop_back()
		{
			assert(_start < _finish);
			_finish--;
		}

		
		void insert(iterator pos, const T& val)
		{
			assert(pos <= _finish);   //��������=���൱��ʵ��β��
			if (_finish == _endofstorage)
			{
				size_t n = pos -_start;   //���ݺ󿪱����¿ռ䣬���ͷ���ԭ���ռ䣬��pos��ָ����Ȼָ��ԭ��
										  //�ʷ����˵�����ʧЧ����¼pos��start�ľ���
				size_t newcapacity = (capacity() == 0) ? 4 : 2 * capacity();
				reverse(newcapacity);
				pos = _start + n;       //���µ�_start�ͳ������¶�λ
			}
			iterator end = _finish;  //
			while (end >= pos)
			{
				*(end) = *(end-1);
				end--;
			}
			*pos = val;
			_finish++;
		}

		//ɾ��posλ��Ԫ�أ�������pos����һ��Ԫ��λ��(ʵ����ɾ��Ԫ��ǰ�ƣ����þ���posλ��)
		iterator erase(iterator pos)
		{
			assert(pos >= _start && pos < _finish);
			iterator it = pos;
			while (it < _finish)
			{
				*it = *(it + 1);
				it++;
			}
			_finish--;
			return pos;
		}

		//��n���ռ䣬����ɳ�ʼ��(3�����)
		void resize(size_t n, const T& val= T())   //T���Ͳ�ȷ������˸�T���͵�ȱʡֵ
		{
												//C++�п�����Ϊ����������Ҳ�й��죬�� int i=int()�ǿ��Ե�
			//ע��mem���������µ����⣡����
			if (n < size())
			{
				_finish = _start+ n;   //��ָ֤��
			}
			else
			{
				if (n > capacity())
				{
					reverse(n);
				}
				iterator start = _finish;
				while (start < _endofstorage)
				{
					*start = val;
					start++;
				}
				_finish = _start+n;
			}
		}

		T& operator[](size_t i)
		{
			assert(i < size());
			return _start[i];
		}

		T& operator[](size_t i)const
		{
			assert(i < size());
			return _start[i];
		}


		//��ͳд��
		/*vector<T>& operator=(const vector<T>& str)
		{
			if (this != &str)
			{
				delete[] _start;
				_start = new T[str.capacity()];
				memcpy(_start, str._start, sizeof(T) * str.size());
			}
			return *this;
		}*/

		//�ִ�д��
		vector<T>& operator=(vector<T> str)  //Ϊʲô���ﲻ�����ö��ǿ������죿
		{
			//��������ã���ô�������ı䴫�����Ķ���
			//�������ã���������vector���󣬽�������ɿ����������������꣬���Զ��ͷ���Դ
			swap(str);
			return *this;
		}

		void swap(vector<T>& str)     //�������
		{
			::swap(_start, str._start);
			::swap(_finish, str._finish);
			::swap(_endofstorage, str._endofstorage);
		}
		//��������
		~vector()
		{
			delete[]_start;
			_start = _finish = _endofstorage = nullptr;
		}
	private:
		iterator _start;			//��ʼλ�õ�ָ��
		iterator _finish;			//���һ����ЧԪ�ص���һ��λ��
		iterator _endofstorage;     //������������һ��λ��
	};

	//��ӡ
	void test1()
	{
		vector<int> arr;
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		arr.push_back(4);
		arr.push_back(5);

		vector<int>::iterator it1 = arr.begin();
		while (it1 != arr.end())
		{
			*it1 += 1;
			cout << *it1 << " ";
			it1++;
		}
		cout << endl;

		vector<int>::const_iterator it2 = arr.begin();
		while (it2 != arr.end())
		{
			// *it2 +=1   //ֻ�ɶ�
			cout << *it2 << " ";
			it2++;
		}
	}

	//������ɾ��
	void test2()
	{
		vector<int> arr;
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		arr.push_back(4);
		arr.push_back(5);

		vector<int>::iterator it1 = arr.begin();
		while (it1 != arr.end())
		{
			if (*it1 % 2 == 0)
			{
				it1 = arr.erase(it1);  

				//arr.erase(it1) 
				// =>����д�ᵼ�µ�����ʧЧ��ɾ����itָ�벻�䣬֮��++
				//������ɾ��Ԫ��ǰ�ƣ�����©��ĳЩԪ�ص��жϣ���������ı�,�������ʧЧ��it����ı�
				//��vs�±��������ϸ������gcc��������ġ�
				//���ۺ���ƽ̨��erase(it)��������ͷ����˸ı䣬�ᵼ�¸��ָ���������
			}
			else
			{
				it1++;
			}
			
		}
		vector<int>::const_iterator it2 = arr.begin();
		while (it2 != arr.end())
		{
			// *it2 +=1   //ֻ�ɶ�
			cout << *it2 << " ";
			it2++;
		}
		cout << endl;
	}


	//vector�ĵ�����ʧЧ
	void test3()
	{
		vector<int> arr;
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		vector<int>::iterator it1 = arr.begin();
		arr.push_back(4);
		arr.push_back(5);
		arr.push_back(6);    //������ܵ������ݣ����ݻ����¿��ٿռ䲢�ͷžɿռ䣬��it��ָ��ɿռ䣬ʧЧ
							 //������Ҳ�ᵼ�³�����Ϊ����ı���(�������ܵ�����ԭ��ָ������ݲ�һ�£�
						     //���������ָ������������������ʹ��(�Ƽ�)
		while (it1 != arr.end())
		{
			cout << *it1 << " ";
			it1++;
		}
	}

	//resize
	void test4()
	{
		vector<int> arr;
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		arr.push_back(4);
		arr.push_back(5);

		arr.resize(3, 0);
		for (int i = 0; i < arr.size(); i++)
		{
			cout << arr[i]<<" ";
		}
		cout <<endl<< "size=" << arr.size() << "  " << "capacity=" << arr.capacity() << endl;

		arr.resize(6, 0);
		for (int i = 0; i < arr.size(); i++)
		{
			cout << arr[i] << " ";
		}
		cout << endl << "size=" << arr.size() << "  " << "capacity=" << arr.capacity() << endl;

		arr.resize(12, 0);
		for (int i = 0; i < arr.size(); i++)
		{
			cout << arr[i] << " ";
		}
		cout << endl << "size=" << arr.size() << "  " << "capacity=" << arr.capacity() << endl;
	
	}

	//��������
	void test5()
	{
		vector<int> arr;
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		arr.push_back(4);
		arr.push_back(5);

		vector<int> str(arr);
		vector<int>::iterator it1 = str.begin();
		while (it1 != str.end())
		{
			cout << *it1 << " ";
			it1++;
		}
		cout << endl;

	}
}