#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<climits>

#define max(len1, len2) ((len1 > len2)?(len1):len2)
#define min(len1, len2) ((len1 < len2)?(len1):len2)

typedef unsigned int uint32;

static uint32 sldecimal_precision = 16; // 默认精度

static const unsigned int lsofintow[32] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800,
0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
};



void intotwo(const std::string& strnum, int whichsys, std::string& answeroftwo);
unsigned int getlen(std::string& twosysstr);


class biginteger {
private:
	uint32 length;
	bool ifpositive = true;
	std::vector<unsigned int> nums; //低位在前 高位在后
	biginteger(const uint32 len, uint32 n);
public:
	biginteger();
	biginteger(const int num);
	biginteger(const biginteger& number);
	biginteger(const std::string& strnum);
	biginteger oppositenumber() const;
	biginteger abs() const;
	bool ispositive() const;
	biginteger operator+(const int& number) const;
	biginteger operator*(const uint32 number) const;
	biginteger operator<<(const uint32 leftlen) const;
	biginteger operator>>(const uint32 rightlen) const;
	friend bool operator==(const biginteger& number, const biginteger& number2);
	friend bool operator!=(const biginteger& number, const biginteger& number2);
	friend bool operator>(const biginteger number, const biginteger& number2);
	friend bool operator>=(const biginteger& number, const biginteger& number2);
	friend bool operator<(const biginteger& number, const biginteger& number2);
	friend bool operator<=(const biginteger& number, const biginteger& number2);
	friend biginteger operator+(const biginteger& number, const biginteger& number2);
	friend biginteger operator-(const biginteger& number, const biginteger& number2);
	friend biginteger operator*(const biginteger& number, const biginteger& number2);
	friend biginteger operator/(const biginteger& number, const biginteger& number2);
	friend biginteger operator%(const biginteger& number, const biginteger& number2);
	void show();
	std::string output(uint32 index = 10);
	unsigned int lastnumber();
	void delete0intop();
	uint32  delete0inbottom();
	unsigned int getnuminposition(uint32 position) const;
};



class sldecimal  { // 非常长的小数
private:	
	biginteger integer;
	uint32 pointer; // 小数点位数
	uint32  precision = sldecimal_precision; // 精度值 默认16
	void currectsetting(const std::string & strnum);
 public:
	void setprecision(uint32 pre);
	sldecimal();
	sldecimal(const std::string & strnum, uint32 numofdecimal);// 由字符串初始化， 指定小数点后最长长度
	sldecimal(const std::string & strnum);
	sldecimal(const biginteger& integer);
	sldecimal(const biginteger& integer, uint32 numofdecimal);// 由大数初始化 ，指定右移及产生的小数位数
	sldecimal(const sldecimal& slnum); 
	sldecimal(const sldecimal& slnum, uint32 numofdecimal); // 指定最长长度的拷贝
	biginteger getinteger()const;
	sldecimal getdecimal()const;
	sldecimal oppositenumber() const;
	sldecimal abs() const;
	friend bool operator==(const sldecimal & number, const sldecimal & number1);// 一般不用这个方法判断浮点数是否相等
	friend bool operator!=(const sldecimal & number, const sldecimal & number1);
	friend bool operator>(const sldecimal & number, const sldecimal & number1); 
	friend bool operator>=(const sldecimal & number, const sldecimal & number1);
	friend bool operator<(const sldecimal & number, const sldecimal & number1);
	friend bool operator<=(const sldecimal & number, const sldecimal & number1);
	friend bool operator==(const sldecimal & number, const biginteger & number1);
	friend bool operator==(const biginteger & number, const sldecimal & number1);
	friend bool operator!=(const sldecimal & number, const biginteger & number1);
	friend bool operator!=(const biginteger & number, const sldecimal & number1);
	friend bool operator>(const sldecimal & number, const biginteger & number1);
	friend bool operator>(const biginteger & number, const sldecimal & number1);
	friend bool operator>=(const sldecimal & number, const biginteger & number1);
	friend bool operator>=(const biginteger & number, const sldecimal & number1);
	friend bool operator<(const sldecimal & number, const biginteger & number1);
	friend bool operator<(const biginteger & number, const sldecimal & number1);
	friend bool operator<=(const sldecimal & number, const biginteger & number1);
	friend bool operator<=(const biginteger & number, const sldecimal & number1);
	friend sldecimal operator+(const sldecimal & number, const sldecimal & number1);
	friend sldecimal operator-(const sldecimal & number, const sldecimal & number1);
	friend sldecimal operator*(const sldecimal & number, const sldecimal & number1);
	friend sldecimal operator/(const sldecimal & number, const sldecimal & number1);
	friend sldecimal operator+(const biginteger & number, const sldecimal & number1);
	friend sldecimal operator+(const sldecimal & number, const biginteger & number1);
	friend sldecimal operator-(const biginteger & number, const sldecimal & number1);
	friend sldecimal operator-(const sldecimal & number, const biginteger & number1);
	friend sldecimal operator*(const biginteger & number, const sldecimal & number1);
	friend sldecimal operator*(const sldecimal & number, const biginteger & number1);
	friend sldecimal operator/(const biginteger & number, const sldecimal & number1);
	friend sldecimal operator/(const sldecimal & number, const biginteger & number1);
	sldecimal operator<<(const uint32 leftlen) const;
	sldecimal operator>>(const uint32 rightlen) const;
	void delete0inbottom();
	std::string output(uint32 index = 10);
	void show();

};
