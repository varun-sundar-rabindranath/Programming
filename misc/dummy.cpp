#include<iostream>
#include<string>
using namespace std;

int main()
{
#ifdef BSD_SOURCE
	string str = "Hello";	
	str.length();
	return 0;
#endif
}
