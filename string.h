#ifndef STRING_H
#define STRING_H
#include <iostream>
using namespace std;
class String {
public:
	friend ostream & operator<<(ostream& cout, String& s);
	String(const char* str = "")
	{
		if (NULL == str) {
			str = "";
		}
		_pStr = new char[strlen(str) + 1];
		strcpy(_pStr, str);
	}
	//深拷贝构造
	String(const String& s)
		:_pStr(new char[strlen(s._pStr) + 1])
	{
		strcpy(_pStr, s._pStr);
	}

	String& operator=(String s) {
		swap(_pStr, s._pStr);
		return *this;
	}
	//四步赋值版本
	//String& operator=(const String& s)
	//{
	//  if (this != &s) {
	//      char* pStr = new char[strlen(s._pStr) + 1];
	//      strcpy(pStr, s._pStr);
	//      delete[] _pStr;
	//      _pStr = pStr;
	//  }
	//  return *this;
	//}
	~String()
	{
		if (_pStr) {
			delete[] _pStr;
		}
	}

	String operator+(String s) {
		int len = size() + s.size() + 1;
		char* tmp = new char[len];
		strcpy(tmp, _pStr);
		strcat(tmp, s._pStr);
		swap(tmp, s._pStr);
		delete[] tmp;
		return s;
	}
	bool operator==(const String &str)const
	{
		if (strcmp(_pStr, str._pStr) == 0) {
			return true;
		}
		return false;
	}

	size_t size() const
	{
		return strlen(_pStr);
	}

	const char* c_str() const
	{
		return _pStr;
	}
	//取从position所指位置连续取len个字符组成子串返回  
	String& sub_str(int position, int len) {
		if (position<0 || position >= size() || len<0 || len >size()) //参数不合理，不取子串  
		{
		}
		else
		{
			if (position + len - 1 >= size())            //字符串不够长  
				len = size() - position;
			for (int i = 0, j = position; i<len; i++, j++)
				_pStr[i] = _pStr[j];
			_pStr[len] = '\0';
		}
		return *this;
	}
private:
	char* _pStr;
};

ostream & operator<<(ostream& cout, String& s)
{
	cout << s._pStr;
	return cout;
}

#endif