#include"bignumber.h"
void sldecimal::setprecision(uint32 precision) { // 精度调整
	if (this->pointer > static_cast<long long>(precision & 0x8fffffff)) { 
		this->integer = (this->integer >> (this->pointer - precision));
		this->pointer = precision;
	}
	this->precision = precision;
}

void sldecimal::currectsetting(const std::string & strnum) {
	uint32 positionofpoint = strnum.find('.'); // 小数点所在位置
	if (positionofpoint == std::string::npos) { // 无小数部分按整形处理
		this->integer = biginteger(strnum);
		this->pointer = 0;
	}
	else {
		std::string::const_iterator start = strnum.begin();
		std::string newstr, working;
		if (*start != '+' && *start != '-') {// 处理输入字符串无正负及进制信息情况	
			newstr += '+';
			if (strnum.size() > 2) {
				start += 2;
				if (*start != 'x' && *start != 'X' && *start != 'b' && *start != 't') {
					newstr += "0x";;
				}
			}
			else {
				newstr += "0x";
			}
			working = newstr + strnum;
		}
		else if (strnum.size() > 3) {
			if (*(start + 2) != 'x' && *(start + 2) != 'X' && *(start + 2) != 'b' && *(start + 2) != 't') {
				newstr = "0x";
				working = strnum[0] + newstr + working;
			}
			working = strnum;
		}
		int whichsys = 16; // 默认进制为16
		std::string::const_iterator strindex = working.begin();
		if ((*strindex == '+') | (*strindex == '-')) { // 忽略正负问题
			strindex++;
		}
		if ((*strindex == '0') && *(strindex + 1) == 'x') {
			whichsys = 16;
			strindex += 2;
		}
		if (((*strindex) == '0') && *(strindex + 1) == 'b') {
			whichsys = 2;
			strindex += 2;
		}
		if ((*strindex == '0') && *(strindex + 1) == 'X') {
			whichsys = 8;
			strindex += 2;
		}
		if ((*strindex == '0') && *(strindex + 1) == 't') {  // 十进制非常麻烦， 需要另外处理
			std::string::iterator beg = working.begin();
			sldecimal answer = biginteger(std::string(beg, beg + positionofpoint));
			biginteger ten("+0xa");
			sldecimal index = sldecimal("+0x1") / ten;
			beg = beg + positionofpoint + 1;
			if (answer.integer.ispositive()) {
				while (beg != working.end()) {
					answer = answer + index * int(*beg - '0');
					index = index / ten;
					beg++;
				}
			}
			else {
				while (beg != working.end()) {
					answer = answer - index * (*beg - '0');
					index = index / 10;
					beg++;
				}
			}
			answer.integer.delete0intop();
			this->integer = answer.integer;
			this->pointer = answer.pointer;
			this->precision = answer.precision;
			return;
		}
		std::string::const_iterator iter = working.begin();
		std::string sys2;
		sys2 = std::string((*iter == '+') ? "+" : "-") + "0b";
		intotwo(working, whichsys, sys2);
		positionofpoint = sys2.find('.'); // 新的二进制数的小数点位置
		std::string::iterator iterofpoint = sys2.begin() + static_cast<int>(positionofpoint);
		std::string newstrfordecimal(iterofpoint + 1, sys2.end());
		if ((newstrfordecimal.length() % 32)) {// 初始化小数部分的字符串 长度必须为32整数倍 以便对齐
			size_t dddd = 32 - (newstrfordecimal.length() % 32);
			std::string ing(dddd, '0');
			newstrfordecimal += ing;
		}
		std::string::iterator beg = sys2.begin();
		this->integer = biginteger(std::string(beg, iterofpoint) + newstrfordecimal);
		this->pointer = newstrfordecimal.length() / 32;
		this->integer.delete0intop();
	}
}

sldecimal::sldecimal() {
	this->integer = biginteger();
	this->pointer = 0;
}

sldecimal::sldecimal(const std::string & strnum, uint32 numofdecimal) {
	this->precision = numofdecimal;
	this->currectsetting(strnum);
	if (this->pointer > static_cast<long long>(numofdecimal & 0x8fffffffffffffff)) {
		this->integer = this->integer >> (this->pointer - numofdecimal);
		this->pointer = numofdecimal;
	}
}

sldecimal::sldecimal(const std::string & strnum){
	this->precision = sldecimal_precision;
	this->currectsetting(strnum);
	if (this->pointer > sldecimal_precision) {
		this->integer = this->integer >> (this->pointer - sldecimal_precision);
		this->pointer = 16;
	}
}

sldecimal::sldecimal(const biginteger& integer) {
	this->integer = integer;
	this->pointer = 0;
	this->precision = sldecimal_precision;
}

sldecimal::sldecimal(const biginteger& integer, uint32 numofdecimal) {
	this->precision = numofdecimal;
	this->integer = integer;
	this->pointer = numofdecimal;
	this->precision = numofdecimal;
}

sldecimal::sldecimal(const sldecimal& slnum){
	this->precision = slnum.precision;
	this->integer = slnum.integer;
	this->pointer = slnum.pointer;
}

sldecimal::sldecimal(const sldecimal& slnum, uint32 numofdecimal) {
	if (slnum.pointer <= static_cast<long long>(numofdecimal & 0x8fffffffffffffff)) {
		this->integer = slnum.integer;
		this->pointer = slnum.pointer;
		this->precision = slnum.precision;
	}
	else {
		sldecimal once(slnum);
		this->integer = once.integer >> (slnum.pointer - numofdecimal);
		this->pointer = numofdecimal;
		this->precision = numofdecimal;
	}
}

biginteger sldecimal::getinteger() const{
	if (this->pointer == 0) return this->integer;
	else if (this->pointer > 0) return (this->integer >> (this->pointer));
	else return (this->integer << ( 0 - this->pointer));
}

sldecimal sldecimal::getdecimal() const {
	return (*this - this->getinteger());
}

sldecimal sldecimal::oppositenumber() const{
	sldecimal answer(*this);
	if (answer.integer == 0) return answer;
	answer.integer = answer.integer.oppositenumber();
	return answer;
}

sldecimal sldecimal::abs() const {
	sldecimal answer(*this);
	if (answer.integer == 0) return answer;
	answer.integer = answer.integer.abs();
	return answer;
}

bool operator==(const sldecimal & number, const sldecimal & number1) {
	if (number.integer != number1.integer) return false;
	if (number.pointer != number1.pointer) return false;
	return true;
}

bool operator!=(const sldecimal & number, const sldecimal & number1) {
	if (number == number1) return false;
	return true;
}

bool operator>(const sldecimal & number, const sldecimal & number1) {
	if (number.integer.ispositive() == 1 && number1.integer.ispositive() == 0) return true;
	if (number.integer.ispositive() == 0 && number1.integer.ispositive() == 1) return false;
	int longerdecimal = max(number.pointer, number1.pointer); // 对齐 此时无需考虑正负
	biginteger copy, copy1;
	copy = number.integer << (longerdecimal - number.pointer);
	copy1 = number1.integer << (longerdecimal - number1.pointer);
	if (copy > copy1) return true;
	return false;
}

bool operator>=(const sldecimal & number, const sldecimal & number1) {
	if ((number > number1) | (number == number1)) return true;
	return false;
}

bool operator<(const sldecimal & number, const sldecimal & number1) {
	if (number1 > number) return true;
	return false;
}

bool operator<=(const sldecimal & number, const sldecimal & number1) {
	if ((number1 > number) | (number1 == number)) return true;
	return false;
}

bool operator==(const sldecimal & number, const biginteger & number1) {
	if (number == sldecimal(number1)) return true;
	return false;
}

bool operator==(const biginteger & number, const sldecimal & number1) {
	if (sldecimal(number) == number1) return true;
	return false;
}

bool operator!=(const sldecimal & number, const biginteger & number1) {
	if (number == sldecimal(number1)) return false;
	return true;
}

bool operator!=(const biginteger & number, const sldecimal & number1) {
	if (sldecimal(number) == number1) return false;
	return true;
}

bool operator>(const sldecimal & number, const biginteger & number1) {
	if (number > sldecimal(number1)) return true;
	return false;
}

bool operator>(const biginteger & number, const sldecimal & number1) {
	if (sldecimal(number) > number1) return true;
	return false;
}

bool operator>=(const sldecimal & number, const biginteger & number1) {
	if (number >= sldecimal(number1)) return true;
	return false;
}

bool operator>=(const biginteger & number, const sldecimal & number1) {
	if (sldecimal(number) >= number1) return true;
	return false;
}

bool operator<(const sldecimal & number, const biginteger & number1) {
	if (number < sldecimal(number1)) return true;
	return false;
}

bool operator<(const biginteger & number, const sldecimal & number1) {
	if (sldecimal(number) < number1) return true;
	return false;
}

bool operator<=(const sldecimal & number, const biginteger & number1) {
	if (number <= sldecimal(number1)) return true;
	return false;
}

bool operator<=(const biginteger & number, const sldecimal & number1) {
	if (sldecimal(number) <= number1) return true;
	return false;
}

sldecimal operator+(const sldecimal & number, const sldecimal & number1) {
	sldecimal answer;
	if (number.pointer < number1.pointer) {
		answer.integer = biginteger((number.integer << (number1.pointer - number.pointer)) + number1.integer);
		answer.pointer = number1.pointer;
	}
	else if (number.pointer > number1.pointer) {
		biginteger copy(number1.integer);
		answer.integer = biginteger((copy << (number.pointer - number1.pointer)) + number.integer);
		answer.pointer = number.pointer;
	}
	else {
		answer.integer = (number.integer + number1.integer);
		answer.pointer = number.pointer;
	}
	answer.delete0inbottom();
	return answer;
}

sldecimal operator-(const sldecimal & number, const sldecimal & number1) {
	if (number1 == 0) return number;
	if (number == number1) return sldecimal();
	sldecimal answer;
	if (number.pointer < number1.pointer) {
		answer.integer = biginteger((number.integer << (number1.pointer - number.pointer)) - number1.integer);
		answer.pointer = number1.pointer;
	}
	else if (number.pointer > number1.pointer) {
		answer.integer = biginteger(number.integer - (number1.integer << (number.pointer - number1.pointer)));
		answer.pointer = number.pointer;
	}
	else {
		answer.integer = (number.integer - number1.integer);
		answer.pointer = number.pointer;
	}
	answer.delete0inbottom();
	return answer;
}

sldecimal operator*(const sldecimal & number, const sldecimal & number1) {
	sldecimal answer;
	answer.integer = number.integer * number1.integer;
	if (number.pointer + number1.pointer > sldecimal_precision) {
		answer.pointer = sldecimal_precision;
		answer.precision = sldecimal_precision;
		answer.integer = answer.integer >> (number.pointer + number1.pointer - sldecimal_precision);
		return answer;
	}
	answer.pointer = number.pointer + number1.pointer;
	answer.precision = sldecimal_precision;
	answer.delete0inbottom();
	return answer;
}

sldecimal operator/(const sldecimal & number, const sldecimal & number1) {
	sldecimal answer;
	answer.integer = number.integer / number1.integer;
	answer.pointer = number.pointer - number1.pointer;
	biginteger remainder = number.integer % number1.integer;
	while (remainder != 0 && answer.pointer < sldecimal_precision) { // 精度需求未达到且余数不为零
		remainder = remainder << 1;
		answer.integer = (answer.integer << 1) + remainder / number1.integer;
		remainder = remainder % number1.integer;
		answer.pointer += 1;
	}
	answer.precision = sldecimal_precision;
	answer.integer.delete0intop();
	answer.delete0inbottom();
	return answer;
}

sldecimal operator+(const biginteger & number, const sldecimal & number1) {
	return sldecimal(number) + number1;
}

sldecimal operator+(const sldecimal & number, const biginteger & number1) {
	return number + sldecimal(number1);
}

sldecimal operator-(const biginteger & number, const sldecimal & number1) {
	return sldecimal(number) - number1;
}

sldecimal operator-(const sldecimal & number, const biginteger & number1) {
	return number - sldecimal(number1);
}

sldecimal operator*(const biginteger & number, const sldecimal & number1) {
	return sldecimal(number) * number1;
}

sldecimal operator*(const sldecimal & number, const biginteger & number1) {
	return number * sldecimal(number1);
}

sldecimal operator/(const biginteger & number, const sldecimal & number1) {
	return sldecimal(number) / number1;
}

sldecimal operator/(const sldecimal & number, const biginteger & number1) {
	return number / sldecimal(number1);
}

sldecimal sldecimal::operator<<(const uint32 leftlen) const {
	sldecimal answer(*this);
	answer.pointer = answer.pointer - leftlen;
	return answer;
}

sldecimal sldecimal::operator>>(const uint32 rightlen) const {
	sldecimal answer(*this);
	answer.pointer = answer.pointer + rightlen;
	return answer;
}

void sldecimal::delete0inbottom() {
	uint32 numsof0;
	numsof0 = this->integer.delete0inbottom();
	this->pointer -= numsof0;
}

std::string sldecimal::output(uint32 index) {
	std::string outforint = (this->getinteger().output(index));
	sldecimal onlydecimal = (*this - this->getinteger());
	std::string strofdecimal;
	std::string listofchar = "0123456789abcdef";
	while ((strofdecimal.length() < sldecimal_precision * index) && (onlydecimal != 0)) {
		onlydecimal = onlydecimal * index;
		int s = onlydecimal.getinteger().lastnumber();
		strofdecimal += listofchar[onlydecimal.getinteger().lastnumber()];
		onlydecimal = onlydecimal.getdecimal();
	}
	return outforint + "." + strofdecimal;
}

void sldecimal::show() {
	std::cout << "小数点在" << this->pointer << std::endl;
	std::cout << "精度为" << this->precision << std::endl;
	this->integer.show();
}