#include"bignumber.h"
#include<iostream>

int main() {
	biginteger num1("+0t123456789012345678901234567890123456789123456789123456789");
	biginteger num2("+0t111111111111111111111111111111111111111111111111111111111");
	biginteger one(1);

	std::cout << (num1 + num2).output() << std::endl;
	std::cout << (num1 - num2).output() << std::endl;
	std::cout << (num1 * num2).output() << std::endl;
	std::cout << (num1 / num2).output() << std::endl;
	std::cout << (num1 % num2).output() << std::endl;

	sldecimal num3("+0x1.ffffffffffff");
	sldecimal num4("+0x.000000000001");
	sldecimal num5("+0t1123412341234241234545241341324.45235234235623452345236223455634563523545234523423423451434256234523452362354");
	std::cout << (num3 + num4).output() << std::endl;
	std::cout << (num4 - num4).output() << std::endl;
	std::cout << (num5 * num5).output() << std::endl;
	std::cout << (num5 / num5).output() << std::endl;
	system("pause");
	return 0;
}
