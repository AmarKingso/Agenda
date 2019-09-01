#include<iostream>
#include<string>
#include<exception>

using namespace std;

class ErrReason :public exception{
public:
	ErrReason();
	ErrReason(string err_msg);
};