#include"hugenumber.h"


void intotwo(const std::string& strnum, int whichsys , std::string& answeroftwo) {
	std::string::const_iterator strindex = strnum.begin() + 3;
	if (whichsys == 2) {
		while (strindex < strnum.end()) {
			answeroftwo += *strindex;
			strindex++;
		}
	}
	if (whichsys == 4) {
		std::string strlistof4sys[4] = { "00", "01", "10", "11" };
		while (strindex < strnum.end()) {
			if (*strindex == '.') {
				answeroftwo += *strindex;
				continue;
			}
			answeroftwo += strlistof4sys[int(*strindex++) - 48];
		}
	}
	if (whichsys == 8) {
		std::string strlistof8sys[8] = { "000", "001", "010", "011", "100", "101", "110", "111" };
		while (strindex < strnum.end()) {
			if (*strindex == '.') {
				answeroftwo += *strindex;
				continue;
			}
			answeroftwo += strlistof8sys[int(*strindex++) - 48];
		}
	}
	if (whichsys == 16) {
		std::string strlistof16sys[16][2] = { {"0","0000"}, {"1","0001"} ,
		{"2","0010"}, {"3","0011"}, {"4","0100"}, {"5","0101"}, {"6","0110"}, {"7","0111"} ,
		{"8", "1000"}, {"9", "1001"},{"a", "1010"}, {"b", "1011"}, {"c","1100"}, {"d","1101"},
		{"e", "1110"}, {"f", "1111"} };
		while (strindex < strnum.end()) {
			if (*strindex == '.') {
				answeroftwo += *strindex;
				strindex++;
				continue;
			}
			for (int i = 0; i < 16; i++) {
				if (strlistof16sys[i][0][0] == *strindex) {
					answeroftwo += strlistof16sys[i][1];
					strindex++;
					break;
				}
			}
		}
	}

};

unsigned long long getlen(std::string& twosysstr) { // 得到二进制数字符串表示所需用长度。
	if (!(twosysstr.size() % 32)) {
		return (twosysstr.size() / 32);
	}
	return (twosysstr.size() / 32 + 1);
}


hugeinteger::hugeinteger() {
	this->length = 0;
	this->ifpositive = 1;
}
hugeinteger::hugeinteger(const unsigned int num) {
	if (num == 0) {
		this->ifpositive = 1;
		this->length = 0;
		return;
	}
	this->length = 1;
	this->ifpositive = 1;
	std::vector<unsigned int> newvec(1, num);
	nums.swap(newvec);
}
hugeinteger::hugeinteger(const int num) {
	if (num == 0) {
		this->ifpositive = 1;
		this->length = 0;
		return;
	}
	this->length = 1;
	this->ifpositive = (num > 0) ? 1 : 0;
	std::vector<unsigned int> newvec(1, num & 0x7ffffffff);
	nums.swap(newvec);
}
hugeinteger::hugeinteger(const unsigned long long len) {
	this->length = len;
	this->ifpositive = 1;
	std::vector<unsigned int> newvec(this->length, 0);
	nums.swap(newvec);
}
hugeinteger::hugeinteger(const unsigned long long len, unsigned int n) {
	this->length = len;
	this->ifpositive = 1;
	std::vector<unsigned int> newvec(this->length, n);
	nums.swap(newvec);
}
hugeinteger::hugeinteger(const hugeinteger& number) {
	this->length = number.length;
	this->ifpositive = number.ifpositive;
	this->nums.assign(number.nums.begin(), number.nums.end());
}
hugeinteger::hugeinteger(const std::string& strnum) {
	std::string working = strnum; 
	if (strnum.size() == 0) { // 输入空字符串
		hugeinteger(0);
		return;
	}
	std::string::const_iterator start = strnum.begin();
	std::string newstr;
	if (*start != '+' && *start != '-') {// 处理输入字符串无正负及进制信息情况	
		newstr += '+';
		if (strnum.size() > 2) {
			start += 2;
			if (*start != 'x' && *start != 'X' && *start != 'b' && *start != 't') {
				newstr += "0t";;
			}
		}
		else {
			newstr += "0t";
		}
		working = newstr + strnum;
	}
	else if (strnum.size() > 3) {
		if (*(start + 2) != 'x' && *(start + 2) != 'X' && *(start + 2) != 'b' && *(start + 2) != 't') {
			newstr = "0t";
			working = strnum[0] + newstr + working;
		}
	}
	
	int whichsys = 10; // 将处理好的标准字符串转为数字
	std::string answeroftwo;
	std::string::const_iterator strindex = working.begin(), strindexnum;
	if (*strindex == '-') { 
		ifpositive = 0; 
		strindex++;
	} 
	if (*strindex == '+') {
		ifpositive = 1;
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
	if ((*strindex == '0') && *(strindex + 1) == 't') {
		whichsys = 10;
		strindex += 2;
	}
	if (whichsys == 10) { // 十进制字符串无法简单转化为二进制字符串
		hugeinteger index(unsigned int (1));
		hugeinteger answer(0);
		for (std::string::const_iterator iter = working.end() - 1; iter >= strindex; iter--) {
			answer = answer + index * unsigned int(*iter - '0');
			index = index * 10;
		}
		this->length = answer.length;
		this->nums = std::vector<unsigned int>(answer.nums.begin(), answer.nums.end());
		return;
	}
	intotwo(working, whichsys, answeroftwo);
	this->length = getlen(answeroftwo);
	nums = std::vector<unsigned int>(this->length, 0);
	int index;
	for (int i = 0; i < this->length; i++) { // 二进制字符串转数字
		strindex = answeroftwo.end() - 1 - i * 32; // 一个数字的开始
		if (i == this->length - 1) {
			strindexnum = answeroftwo.begin(); 
		}
		else {
			strindexnum = answeroftwo.end() - 32 - i * 32;  // 一个数字的结束
		}
		index = 1;
		for (std::string::const_iterator j = strindex; j > strindexnum; j--) {
			(nums)[i] += int(*j - '0') * index;
			index *= 2;
		}
		(nums)[i] += int(*strindexnum - '0') * index;
	}
}
hugeinteger hugeinteger::oppositenumber() const{
	hugeinteger answer = *this;
	answer.ifpositive = !(this->ifpositive);
	return answer;
}
hugeinteger hugeinteger::abs() const {
	hugeinteger answer = *this;
	answer.ifpositive = 1;
	return answer;
}
bool hugeinteger::ispositive() const{
	return this->ifpositive;
}
bool operator==(const hugeinteger& number, const hugeinteger& number2) {
	if (number.length != number2.length) return false;
	if (number.ifpositive != number2.ifpositive) return false;
	for (int i = 0; i < number.length; i++) {
		if (number.nums[i] != number2.nums[i]) return false;
	}
	return true;
}
bool operator!=(const hugeinteger& number, const hugeinteger& number2) {
	if (number == number2) return false;
	return true;
}
bool operator>(const hugeinteger number, const hugeinteger& number2) {
	if (number.ifpositive && !(number2.ifpositive)) return true;
	if (!(number.ifpositive) && (number2.ifpositive)) return false;
	if (number.ifpositive && number2.ifpositive) {
		if (number.length > number2.length) return true;
		if (number.length < number2.length) return false;
		std::vector<unsigned int>::const_iterator iter1 = (number.nums).end() - 1, iter2 = number2.nums.end() - 1;
		while ((iter1 != (number.nums).begin()) && (*iter1 == *iter2)) {
			iter1--;
			iter2--;
		}
		if (*iter1 > *iter2) return true;
		else { return false; }
	}
	if (!number.ifpositive && !number2.ifpositive) {
		if (number.length > number2.length) return false;
		if (number.length < number2.length) return true;
		std::vector<unsigned int>::const_iterator iter1 = (number.nums).end(), iter2 = number2.nums.end();
		while ((*iter1 == *iter2) && (iter1 != (number.nums).begin())) {
			iter1--;
			iter2--;
		}
		if (*iter1 > *iter2) return false;
		else { return true; }
	}
	return false;// 不会运行到这里。
}
bool operator>=(const hugeinteger& number, const hugeinteger& number2) {
	if ((number > number2) || (number == number2)) return true;
	return false;
}
bool operator<(const hugeinteger& number, const hugeinteger& number2) {
	if (number2 > number) return true;
	return false;
}
bool operator<=(const hugeinteger& number, const hugeinteger& number2) {
	if ((number < number2) || (number == number2)) return true;
	return false;
}
hugeinteger hugeinteger::operator+(const int& number) const {
	unsigned long long maxlen = this->length;
	hugeinteger answer(maxlen);
	unsigned long long add = 0;
	if (number > 0 && this->ifpositive) {
		unsigned long long  carryout = number;
		for (unsigned long long i = 0; i < maxlen; i++) {
			add = carryout + this->nums[i];
			answer.nums[i] = add % (unsigned long long(UINT_MAX) + 1);
			carryout = add / (unsigned long long(UINT_MAX) + 1);
			if (carryout == 0) return answer;
		}
		if (carryout) {
			answer.nums.push_back(carryout);
			answer.length += 1;
		}
	}
	return answer;
}

hugeinteger operator+(const hugeinteger& number, const hugeinteger& number2) {
	if (number.ifpositive && number2.ifpositive) {
		unsigned long long maxlen = max(number.length, number2.length);
		hugeinteger answer(maxlen);
		unsigned long long add;
		unsigned long long carryout = 0;
		for (unsigned long long i = 0; i < maxlen; i++) {
			if ((number2.length == i) || (number.length == i)) { // 相加的两数有一个加完了
				if (number2.length == i) {
					if (carryout == 0) {
						for (unsigned long long j = i; j < maxlen; j++) {
							answer.nums[j] = number.nums[j];
						}
						break;
					}
					while (carryout || i < maxlen) {
						add = carryout + number.nums[i];
						answer.nums[i++] = add % (unsigned long long(UINT_MAX) + 1);
						carryout = add / (unsigned long long(UINT_MAX) + 1);
					}
					if (carryout) {
						answer.nums.push_back(carryout);
						answer.length += 1;
					}
					break;
				}
				if (number.length == i) {
					if (carryout == 0) {
						for (unsigned long long j = i; j < maxlen; j++) {
							answer.nums[j] = number2.nums[j];
						}
						break;
					}
					while (carryout || (i < maxlen)) {
						add = carryout + static_cast<unsigned long long>(number2.nums[i]);
						answer.nums[i++] = static_cast<unsigned int>(add % (unsigned long long(UINT_MAX) + 1));
						carryout = add / (unsigned long long(UINT_MAX) + 1);
					}
					if (carryout) {
						answer.nums.push_back(carryout);
						answer.length += 1;
					}
					break;
				}
			}
			add = carryout; // 两数都没完
			if (number.length > i) add += number.nums[i];
			if (number2.length > i) add += number2.nums[i];
			answer.nums[i] = add % (unsigned long long(UINT_MAX) + 1);
			carryout = add / (unsigned long long(UINT_MAX) + 1);
			if ((number2.length == i + 1) && (number.length == i + 1)) { // 两数都完了
				if (carryout) {
					answer.nums.push_back(static_cast<unsigned int>(carryout));
					answer.length += 1;
				}
				break;
			}
		}
		return answer;
	}
	else if (number.ifpositive && (!number2.ifpositive)) {
		return (number - number2.oppositenumber());
	}
	else if ((!number.ifpositive) && number2.ifpositive) {
		return (number2 - number.oppositenumber());
	}
	else {
		return (number.oppositenumber() + number2.oppositenumber()).oppositenumber();
	}
}

hugeinteger operator-(const hugeinteger& number, const hugeinteger& number2) {
	hugeinteger acopy(number);
	if (acopy == number2) return hugeinteger();
	if (acopy.ifpositive && number2.ifpositive) {
		hugeinteger answer;
		if (acopy.length == number2.length) {
			if (acopy > number2) {
				answer.ifpositive = true;
				for (unsigned long long i = 0; i < acopy.length; i++) {
					if (acopy.nums[i] >= number2.nums[i]) {
						answer.nums.push_back((acopy.nums[i] - number2.nums[i]));
						answer.length += 1;
					}
					else {
						answer.nums.push_back(static_cast<unsigned int>(unsigned long long(UINT_MAX) + 1 - (number2.nums[i] - acopy.nums[i])));
						unsigned long long now = i;
						while (!(acopy.nums[now + 1])) {  //借位
							acopy.nums[now + 1] = unsigned long long(UINT_MAX);
							now++;
						}
						acopy.nums[now + 1] -= 1;
						answer.length += 1;
					}
				}
				answer.delete0intop();
				return answer;
			}
			else {
				answer = number2 - acopy;
				return answer.oppositenumber();
			}
		}
		else if (acopy.length > number2.length) {
			answer.ifpositive = true;
			for (unsigned long long i = 0; i < number2.length; i++) {
				if (acopy.nums[i] >= number2.nums[i]) {
					answer.nums.push_back((acopy.nums[i] - number2.nums[i]));
					answer.length += 1;
				}
				else {
					answer.nums.push_back(static_cast<unsigned int>(unsigned long long(UINT_MAX) + 1 - (number2.nums[i] - acopy.nums[i])));
					unsigned long long  now = i;
					while (!(acopy.nums[now + 1])) {  //借位
						acopy.nums[now + 1] = (UINT_MAX);
						now++;
					}
					acopy.nums[now + 1] -= 1;
					answer.length += 1;
				}
			}
			for (unsigned long long i = number2.length; i < acopy.length; i++) {
				answer.nums.push_back(acopy.nums[i]);
				answer.length += 1;
			}
			answer.delete0intop();
			return answer;
		}
		else {
			answer = (number2 - acopy);
			return answer.oppositenumber();
		}
	}
	else if (acopy.ifpositive && (!number2.ifpositive)) {
		return (acopy + number2.oppositenumber());
	}
	else if ((!acopy.ifpositive) && number2.ifpositive) {
		return (number2 - acopy.oppositenumber()).oppositenumber();
	}
	else {
		return (number2.oppositenumber() - acopy.oppositenumber());
	}
}
hugeinteger hugeinteger::operator<<(const unsigned long long leftlen) const { //向左位移增大数字
	hugeinteger acopy = hugeinteger(*this);
	acopy.length += leftlen;
	acopy.nums.insert(acopy.nums.begin(), leftlen, 0);
	return acopy;
}
hugeinteger hugeinteger::operator>>(const unsigned long long rightlen) const { //向右位移减小数字
	hugeinteger acopy = hugeinteger(*this);
	if (rightlen >= acopy.length) {
		std::vector<unsigned int> newvec;
		acopy.length = 0;
		newvec.swap(acopy.nums);
		return acopy;
	}
	std::vector<unsigned int>::iterator iter = acopy.nums.begin();
	acopy.nums.erase(iter, iter + rightlen);
	acopy.length -= rightlen;
	return acopy;
}
hugeinteger hugeinteger::operator*(const unsigned int onenumber) const {
	hugeinteger answer(this->length);
	unsigned long long now = 0;
	for (unsigned long long i = 0; i < this->length; i++) {
		now += static_cast<unsigned long long>(onenumber) * (nums[i]);
		answer.nums[i] = now % (unsigned long long(UINT_MAX) + 1);
		now /= (unsigned long long(UINT_MAX) + 1);
	}
	if (now) {
		answer.nums.push_back(now);
		answer.length += 1;
	}
	return answer;
}
hugeinteger operator*(const hugeinteger& number, const hugeinteger& number2) {
	hugeinteger answer, index;
	for (unsigned long long i = 0; i < number.length; i++) {
		index = (number2 * number.nums[i]);
		answer = answer + (index << i);
	}
	return answer;
}

hugeinteger operator/(const hugeinteger& number, const hugeinteger& number2) {
	hugeinteger acopy(number), answer, index, infor;
	unsigned int now;
	if (number2.abs() > acopy.abs()) {
		return answer;
	}
	if (number2.abs() == acopy.abs()) {
		answer = hugeinteger(1, 1);
		answer.ifpositive = !(acopy.ifpositive ^ number2.ifpositive);
		return answer;
	}
	unsigned long long len_dif_value = number.length - number2.length;
	index = number2 << len_dif_value;
	if (index > acopy) { //确定返回值长度
		answer = hugeinteger(len_dif_value);
		for (unsigned long long i = len_dif_value - 1; i < len_dif_value; i--) {//！！无符号数永远不会小于0，故用这种越界法判断循环
			index = number2 << i;
			now = 0;
			for (int j = 31; j >= 0; j--) {
				infor = index * lsofintow[j];
				if (acopy >= (infor)) {
					now += lsofintow[j];
					acopy = acopy - infor;
				}
			}
			answer.nums[i] = now;
		}
	}
	else if(index < acopy){
		answer = hugeinteger(len_dif_value + 1);
		for (unsigned long long i = len_dif_value; i <= len_dif_value; i--) {
			index = number2 << i;
			now = 0;
			for (int j = 31; j >= 0; j--) {
				infor = index * lsofintow[j];
				if (acopy >= (infor)) {
					now += lsofintow[j];
					acopy = acopy - infor;
				}
			}
			answer.nums[i] = now;
		}
	}
	else { //index == acopy
		return hugeinteger("+0x1") << len_dif_value;
	}
	answer.ifpositive = !(acopy.ifpositive ^ number2.ifpositive);//确定返回值正负
	return answer;
}
hugeinteger operator%(const hugeinteger& number, const hugeinteger& number2) {
	hugeinteger answer;
	if (number2 > (number)) {
		answer = number;
		return answer;
	}
	else if (number2 == (number)) {
		return answer;
	}
	hugeinteger index = ((number) / number2) * number2;
	answer = number - index;
	return answer;
}
void hugeinteger::delete0intop() {
	if (this->nums.size() == 0) return;
	std::vector<unsigned int>::iterator iter = this->nums.end() - 1; //相减导致任意数目高位为零

	while (!(*iter)) {
		this->nums.erase(iter);
		if (this->nums.size() == 0) {
			this->length = 0;
			return;
		}
		this->length -= 1;
		iter = this->nums.end() - 1;
	}
}
unsigned long long hugeinteger::delete0inbottom() { // 这个函数hugeinteger类用不到， sldecimal类用于清楚小数末尾的无用的0
	unsigned long long answer = 0;
	if (this->nums.size() == 0) return answer;
	std::vector<unsigned int>::iterator iter = this->nums.begin(); //最低位为零
	while (!(*iter)) {
		this->nums.erase(iter);
		if (this->nums.size() == 0) {
			this->length -= 1;
			answer += 1;
			return answer;
		}
		this->length -= 1;
		iter = this->nums.begin();
		answer += 1;
	}
	return answer;
}
unsigned int hugeinteger::getnuminposition(unsigned long long position) const{
	if (position <= this->nums.size()) {
		return this->nums[position];
	}
	return 0;
}
void hugeinteger::show() {
	if (nums.size() == 0) {
		std::cout << "空的" << std::endl;
		return;
	}
	std::cout << "长度" << this->length << std::endl;
	if (this->ifpositive) { 
		std::cout << "正负为1" <<std::endl; 
	}
	else {
		std::cout << "正负为0" << std::endl;
	}
	for (auto i : nums) {
		std::cout << i << std::endl;
	}
}
std::string hugeinteger::output(int index) {
	hugeinteger acopy(*this);
	std::string answer;
	hugeinteger a;
	std::string listofchar = "0123456789abcdef";
 	while (acopy != 0) {
		a = (acopy % index);
		answer = listofchar[a.lastnumber()] + answer;
		acopy = acopy / index;
	}
	char sys;
	switch (index) {
		case 2: sys = 'b'; break;
		case 8: sys = 'X'; break;
		case 10: sys = 't'; break;
		case 16: sys = 'x'; break;
	}
	answer = ((this->ifpositive) ? (std::string("+0") + sys) : (std::string("-0") + sys)) + answer;
	return answer;
}
unsigned int hugeinteger::lastnumber() {
	if (this->nums.size() == 0) return 0;
	return this->nums[0];
}