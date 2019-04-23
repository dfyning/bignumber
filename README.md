# hugenumber

网上有许多c++ 的大数计算的库，不过大多不完整或低效率，自己试着写一个练练手。

支持”无限”长整形及“无限精度浮点型”的加减乘除等基本运算

具体实现为位hugeinteger.cpp文件的类hugeinteger和位于sldecimal.cpp文件的类sldecimal分别支持整形及浮点型。

hugeinteger实现原理为vector<unsigned int> 低位在数组头部，高位在数组尾部。还有一个bool类型表示正负。
  可以将这个数视为（4294967296）进制，长度为vector最大支持长度的数。
  
输入输出为字符串数字，支持2,8,10,16 进制及正负数，数字头部进制识别码为 "+0b" "+0X" "+0t" "+0x" 或"-0b" "-0X" "-0t" "-0x" 无正负识别时默认为正，
无进制识别时默认为十进制。

该类用友元函数重载了+ - * / % < > == != <= >= << >> 运算符， << 运算符指二进制数左移32位或16进制左移4位也既vector头部插入一个0
>> 运算符指二进制数右移32位或16进制右移4位也既vector头部删除一个数字。其他重载符号与一般认识相同。

还有.oppositenumber() 返回相反数 .abs() 返回绝对值。

sldecimal实现原理为hugeinteger 加一个小数点位pointer，精度控制靠全局变量sldecimal_precision和类内的precision变量控制，每当初始化一个sldecimal数字，这个数字的默认精度为当前sldecimal_precision的值。注意当小数末尾由于加法或减法导致全是零时，该类会自动将0删去节约空间，这可能导致pointer变为负数，这不会影响结果的正确性。

该类同样用友元函数重载了+ - * / % < > == != <= >= << >> 运算符，而且支持与类hugeinteger数字的混合运算。值得注意的是，在计算两个sldecimal数字是否相等时，不能使用 == 运算符，而应该像计算double类型是否相等一样时计算两数差的绝对值小于某个设定好的很小的阈值。

sldecimal类同样有.oppositenumber() 返回相反数 .abs() 返回绝对值。还多.getdecimal()返回小数部分和.getinteger()返回整形部分。
