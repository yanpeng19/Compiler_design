﻿/*
手工词法分析器，接受一个字符串流，根据流内容，判断该词的身份以及分组存储。
输入: 字符流
输出: vector<C_word> 词集合

流程：
1.循环从流中读取内容,可以分为两大类： a.关键词/变量 b.符号
	1.0 如果第一个字符是空格则跳过
		1.1.0 如果第一个字符是非符号: 按照正则表达式 [_a-zA-Z]{1} [_a-zA-Z0-9]* 直到读取到不合法的字符。
		1.1.1 判断截取的字符流 s ，通过哈希计算，判断是否是 关键字，如果，则向vectgor 中push一个关键字的结构体
			  如果不是，则向vector中push一个 变量的结构体
		1.1.2 如果第一个字符是符号，那么获取下一个字符，将其合并在一起，通过计算hash 判断是否 合法符号。
			  如果是，则生成一个可以表征 此符号的 结构体,push到vector中；
			  如果不是，则将后退一个字节，生成 有一个字符组成的符号结构体，push 到voctor中
		1.1.3 如果第一个字符是数字，那么后面是一个数字；合法的数字表示法为:
			  a.[-]![0-9]![0-9]*   普通的数字,例如  123, -123
			  b.各种进制n   n      进制数字表示法，二进制: 0b111  八进制: 017,-05 十六进制:0x13 -0x3A
			  c.科学表示法         1.0e5f
2.最后生成一个vector<C_word> 词序列

！词法分析器仅仅判断某个字符流的含义是什么，而对语法的正确性不做判断
*/

#include <cstdio>
#include "Lexical_analyzer.h"

using namespace std;

// 状态转移表

/*
	 首先判断流接下来的字符类型，如下,转移表返回结果直流，流恢复状态0 ，进行下一个内容判定:

     0 = 流初始状态   1 = 下一个关键字为word词/变量 类型 进入变量转移表   2 = 流为符号/数字中间状态 如果后续错误，退回到该位置，则为符号，进入符号转移表
	                  3 = 流为符号状态   进入符号转移表                   4 = 流为数字状态，流进入数字转移表
	          状态       0      1      2       3           
     符号              

	 blank               0      
   [_a-zA-Z]             1
     [0-9]               4             4 
      [-+]               2             3 
 除了+-的其他符号        3      

								       [并做判断]
*/

/*
	数字的合法表示:
	a.[-]![0-9]![0-9]*   普通的数字,例如  123, -123
	b.各种进制n   n      进制数字表示法，二进制: 0b111  八进制: 017,-05 十六进制:0x13 -0x3A
	c.科学表示法         1.0e5  [±][1-9].[0-9]+E[±][0-9]+

	数字状态转移表:
			  
			  -1 状态直接返回, 10和12状态如果错误，后退一步返回结果

			  -1 = 错误状态  
			   0 = 初始状态   1 = 带有正负号的初始状态   
			   2 = 初始位接受一个除0之外的其他数字的状态    3 = 初始位接收一个0 （八进制等待状态/0）         4 = 0b 二进制	                      5 = 0x 十六进制                     6 = 0 八进制确认状态            
			   7 = 状态2接受一个[0-9] 12 -14                8 = 状态2/3/6/7接受一个小数点  1. 0.             9 = 状态7接受[0-9] 小数常态          10 = 0x2e 进入科学计数法等待状态   11 = 科学技术状态 0x2e0
			  12 = 科学计数负数准备状态 0x2e-              13 = 科学计数状态 0x2e-4

			  非接受状态 = -1  1   4    5    
			  


			输入符号     [+]       [-]        [0]        [1]        [2-7]      [8-9]    [.]      [xX]       [aA]        [bB]     [eE]      [cdCD]     [eE]     [fF]      其他
	状态

	 0                    1         1          3          2           2          2
	 1                                         3          2           2          2
	 2                                         7          7           7          7       8
	 3                                         3          6           6          6       8         5
	 4                                         4          4
	 5                                         5          5           5          5       5                   5           5        5          5         5        5
	 6                                         6          6           6                  8
	 7                                         7          7           7          7       8
	 8                                         9          9           9          9
	 9                                         9          9           9          9
	10                              12        11         11          11         11
	11                                        11         11          11         11
	12                                        11         11          11         11
*/



int main()
{
	string file_path("D:test.txt");
	auto resautlt = analyze_word(file_path);
	out_Cword(resautlt);

    std::cout << "Hello World!\n";
}

