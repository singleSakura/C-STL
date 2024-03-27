#pragma once
#include <assert.h>
namespace myvector
{
	template <class T>   //模板，可用接受不同类型的元素
	class vector
	{
	public:
		typedef T* iterator;               //T类型的指针，T类型由编译器推演并实例化
		typedef const T* const_iterator;    //const迭代器,只可读，不可写


		//构造函数
		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _endofstorage(nullptr)
		{
			;
		}

		//拷贝构造
		//不写编译器自动生成，但生成的是浅拷贝，涉及到指针会出问题，析构时会重复释放，故要使用深拷贝
		vector(const vector<T>& arr)
			:_start(nullptr)
			,_finish(nullptr)
			,_endofstorage(nullptr)
		{
			reverse(arr.capacity());
			for (const auto& e : arr)  //减少值拷贝
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

		//增容就可能引起迭代器失效
		void reverse(size_t n)
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];
				if (_start)
				{
					memcpy(tmp, _start, sizeof(int) * sz);
					delete[] _start;    //_start开始改变
				}
				_start = tmp;
				_finish = tmp + sz;     //这里为什么不用size()？因为此时_start的指向已经改变了
				                          //因此需要提前计算size的大小
				_endofstorage = tmp + n;  //同理，这里不能用capacity()，因为实现用到了_start

			}
			
		}

		//push_back()
		void push_back(const T& val)   //传引用，减少不必要的拷贝
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

		//pop_back  ,尾删
		void pop_back()
		{
			assert(_start < _finish);
			_finish--;
		}

		
		void insert(iterator pos, const T& val)
		{
			assert(pos <= _finish);   //这里允许=，相当于实现尾插
			if (_finish == _endofstorage)
			{
				size_t n = pos -_start;   //增容后开辟了新空间，并释放了原来空间，但pos的指针仍然指向原来
										  //故发生了迭代器失效，记录pos到start的距离
				size_t newcapacity = (capacity() == 0) ? 4 : 2 * capacity();
				reverse(newcapacity);
				pos = _start + n;       //由新的_start和长度重新定位
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

		//删除pos位置元素，并返回pos的下一个元素位置(实际上删了元素前移，正好就是pos位置)
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

		//开n个空间，并完成初始化(3种情况)
		void resize(size_t n, const T& val= T())   //T类型不确定，因此给T类型的缺省值
		{
												//C++中可以认为，内置类型也有构造，即 int i=int()是可以的
			//注意mem×××导致的问题！！！
			if (n < size())
			{
				_finish = _start+ n;   //保证指向
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


		//传统写法
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

		//现代写法
		vector<T>& operator=(vector<T> str)  //为什么这里不是引用而是拷贝构造？
		{
			//如果是引用，那么交换后会改变传过来的对象
			//不是引用，拷贝构造vector对象，交换后完成拷贝，而函数调用完，会自动释放资源
			swap(str);
			return *this;
		}

		void swap(vector<T>& str)     //减少深拷贝
		{
			::swap(_start, str._start);
			::swap(_finish, str._finish);
			::swap(_endofstorage, str._endofstorage);
		}
		//析构函数
		~vector()
		{
			delete[]_start;
			_start = _finish = _endofstorage = nullptr;
		}
	private:
		iterator _start;			//开始位置的指针
		iterator _finish;			//最后一个有效元素的下一个位置
		iterator _endofstorage;     //可用容量的下一个位置
	};

	//打印
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
			// *it2 +=1   //只可读
			cout << *it2 << " ";
			it2++;
		}
	}

	//插入与删除
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
				// =>这样写会导致迭代器失效，删除后it指针不变，之后++
				//但由于删除元素前移，导致漏掉某些元素的判断，导致意义改变,即这里的失效是it含义改变
				//在vs下编译器会严格检差，但在gcc下是允许的。
				//无论何种平台，erase(it)后，其意义就发生了改变，会导致各种各样的问题
			}
			else
			{
				it1++;
			}
			
		}
		vector<int>::const_iterator it2 = arr.begin();
		while (it2 != arr.end())
		{
			// *it2 +=1   //只可读
			cout << *it2 << " ";
			it2++;
		}
		cout << endl;
	}


	//vector的迭代器失效
	void test3()
	{
		vector<int> arr;
		arr.push_back(1);
		arr.push_back(2);
		arr.push_back(3);
		vector<int>::iterator it1 = arr.begin();
		arr.push_back(4);
		arr.push_back(5);
		arr.push_back(6);    //插入可能导致扩容，扩容会重新开辟空间并释放旧空间，但it仍指向旧空间，失效
							 //不增容也会导致出错，因为含义改变了(插入后可能导致与原来指向的内容不一致）
						     //解决：重新指定方向或者在任务完成使用(推荐)
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

	//拷贝构造
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