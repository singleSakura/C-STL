#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>

//位图：用bit组成的数组来存储数值，用比特 1 ，0 表示存在不存在，取值和存储操作都采用位运算，其数据操作对象为无符号整数。
// 缺点是数据的范围不能太大，且数据最好是连续/稠密的。如果数据太过稀疏，会造成大量空间的浪费。

//位图与哈希表：位图与哈希表的功能类似(操作数据类型不同)，都可以以O(1)的复杂度查询或插入数据
// 当哈希表插入一个整型时，我们知道，整型为32位，但存储值可能只有几位，这就造成了空间上的浪费。
// 而位图就是用比特位来表示数据的数据结构。

//位图的存储：一个整型占32位，每个位表示一个数，因此一个整型变量存储32个数字，其每一位的值表示这些数字是否存在
//因此对于任何一个数x而言，[x/32](向上取整)即可知道该数存储在第几个整型变量中
// 而 (x%32)即可得到其在该整型变量的第几位上。

//向上取整的实现：对于非负数a,b，a/b向上取整等价于（a+b-1)/b

//初始化位图的大小，只支持0~n-1所有数字的增删查改
void Bitset(int** set, int n)   //由于要修改原数组，因此需要址传递
{
	*set = calloc(((n + 31) / 32), sizeof(int));  //初始时，各位必须为0，即位图里面没有数据
}


//把数n放入位图中
void Add(int** set, int n)
{
	(*set)[n / 32] |= 1 << (n % 32);
}


//把数n从位图中移除
void Remove(int** set, int n)
{
	(*set)[n / 32] &= ~(1 << (n % 32));

}


//如果位图没有n，则加入；否则，删除；
void Reverse(int** set, int n)
{
	(*set)[n / 32] ^= 1 << (n % 32);
}


//查询n是否在位图中。           对应状态位为0:n不存在      为1：n存在
int Contains(int* set, int n)
{
	return ((set[n / 32] >> (n % 32)) & 1) == 1;
}

int main()
{
	int* set = NULL;
	Bitset(&set,1000);
	Add(&set,14);
	Add(&set,13);
	Add(&set,17);
	Add(&set,19);
	Add(&set,11);
	printf("%d\n", Contains(set,15));
	printf("%d\n", Contains(set,19));
	printf("%d\n", Contains(set,17));
	printf("%d\n", Contains(set,18));
	printf("%d\n", Contains(set,11));
	Remove(&set,17);
	printf("%d\n", Contains(set,17));
	Reverse(&set, 100);
	printf("%d\n", Contains(set,100));
	return 0;
}