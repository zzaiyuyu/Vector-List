#define _CRT_SECURE_NO_WARNINGS 1
#include "List.hpp"
#include "Vector.hpp"
#include "Algorithm.hpp"
using namespace std;
int main()
{
	//TestVector();
	//TestLList();
	Vector<int> v(10, 0);
	v[0] = 2;
	v[1] = 10;
	v[2] = 6;
	v[3] = 1;
	v[4] = 7;
	v[5] = 5;
	Sort(v.Begin(), v.End());
	ForEach(v.Begin(), v.End(), Display<int>());
}