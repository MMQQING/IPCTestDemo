#include <iostream>

using namespace std;

template<class T>
int getArrayLen(T &array)  /*ʹ��ģ�嶨��һ������getArrayLen,�ú�������������array�ĳ���*/ {
	return (sizeof(array) / sizeof(array[0]));
}