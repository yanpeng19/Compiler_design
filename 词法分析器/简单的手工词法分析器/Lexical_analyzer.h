/*
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
			  12 = 科学计数负数准备状态 0x2e-


			  状态		0		1		2		3		4		5		6		7		8		9     10      11       12

	符号

	[+] 				1
	[-]                 1                                                                             12
	[0]                 3       3       7       3       4       5       6       7       9       9     11      11       11
	[1]                 2       2       7       6       4       5       6       7       9		9     11      11       11
	[2-7]               2       2       7       6               5       6       7       9       9     11      11       11
	[8-9]               2       2       7       6               5               7       9       9     11      11       11
	[.]                                 8       8               5       8       8
	[xX]			                            5
	[aA]                                                        5
	[bB]                                        4               5
	[cdCD]                                                      5
	[eE]                                                        5                              10
	[fF]                                                        5
	[g-zG-Z 除了x]
*/


#pragma once
#include "word_struct.h"
#include <vector>
#include <fstream>
#include <unordered_set>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// 查询的hash_set
static unordered_set<char> symbol_set;
static unordered_set<char> word_set;

// 前置函数声明
C_word do_process(const std::string& word, int& index, const int& type);
int get_word_type(const std::string& word, const int& index);
void init_set();
C_word process_word(const std::string& word, int& index);
C_word process_symbol(const std::string& word, int& index);
C_word process_number(const std::string& word, int& index);

vector<C_word> analyze_word(ifstream& file);

// 对某个文件名路径进行分析，返回一个vector<C_word> 关键字组
vector<C_word> analyze_word(const string& file_name)
{
	ifstream file_stream;
	file_stream.open(file_name, ios_base::in);
	return analyze_word(file_stream);
}

// 对文件分析，返回C语言关键字vector
vector<C_word> analyze_word(ifstream& file)
{
	// 输入 文件流，返回一个 词序列
	vector<C_word> words;
	if (!file.is_open())
		return words;

	if (symbol_set.empty())
		init_set();

	if (symbol_map.empty())
		init_symbol_map();

	// 获得符号

	// 循环从文件中读取内容
	while (!file.eof())
	{
		string line;

		// 循环读取每行
		while (getline(file, line))
		{
			stringstream ss(line);
			string word;
			while (ss >> word)
			{
				if (word.empty())
					continue;

				int index = 0;
				while (index < word.size())
				{
					char c = word[index];
					// 判断 这个词是 ‘符号词’ 还是 ‘字符词’ 还是数字   0 = 字符词 1 = 符号词   2 = 数字词   -1 错误
					int flag = get_word_type(word, index);

					if (flag == -1)
					{
						std::cout << "Error the word is not Symbol or Number" << std::endl; exit(1);
					}

					// 正式处理流程，如果返回结果为空，说明出现了错误
					C_word tmp = do_process(word, index, flag);
					if (tmp.empty())
					{
						std::cout << "Error: process word error" << std::endl; exit(1);
					}
					words.push_back(tmp);
				}
			}
		}
	}
	return words;
}

// 输出 一个词的序列
void out_Cword(const vector<C_word>& words)
{
	int max_row = 5;
	for (const C_word& word : words)
		std::cout << word << std::endl;
}

// 获得数字状态转移表
/*

			输入符号     [+]       [-]        [0]        [1]        [2-7]      [8-9]    [.]      [xX]       [aA]        [bB]     [eE]      [cdCD]     [eE]     [fF]     
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
std::vector<std::vector<int>> get_num_table_instance()
{
	static std::vector<std::vector<int>> num_state_transition_table;

	if (num_state_transition_table.empty())
	{
		num_state_transition_table = std::vector<std::vector<int>>
		{
			{ 1, 1, 3, 2, 2,   2,-1,-1,-1,-1,  -1,-1,-1,-1},
		    {-1,-1, 3, 2, 2,   2,-1,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1, 3, 2, 2,   2,-1,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1, 7, 7, 7,   7, 8,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1, 3, 6, 6,   6, 8, 5,-1,-1,  -1,-1,-1,-1},
		    {-1,-1, 4, 4,-1,  -1,-1,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1, 5, 5, 5,   5, 5,-1, 5, 5,   5, 5, 5, 5},
	     	{-1,-1, 6, 6, 6,  -1, 8,-1,-1,-1,  -1,-1,-1,-1},
		    {-1,-1, 7, 7, 7,   7, 8,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1, 9, 9, 9,   9,-1,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1, 9, 9, 9,   9,-1,-1,-1,-1,  -1,-1,-1,-1},
			{-1,12,11,11,11,  11,-1,-1,-1,-1,  -1,-1,-1,-1},
		    {-1,-1,11,11,11,  11,-1,-1,-1,-1,  -1,-1,-1,-1},
			{-1,-1,11,11,11,  11,-1,-1,-1,-1,  -1,-1,-1,-1}
		};
	}

	return num_state_transition_table;
}

//通过一个字符获得 数字转移表的转移下标
inline int get_num_state_table_index(const char& c)
{
	if (c == '+')
		return 0;
	else if (c == '-')
		return 1;
	else if (c == '0')
		return 2;
	else if (c == '1')
		return 3;
	else if (c >= '2' && c <= '7')
		return 4;
	else if (c >= '8' && c <= '9')
		return 5;
	else if (c == '.')
		return 6;
	else if (c == 'x' || c == 'X')
		return 7;
	else if (c == 'a' || c == 'A')
		return 8;
	else if (c == 'b' || c == 'B')
		return 9;
	else if (c == 'c' || c == 'd' || c == 'C' || c == 'D')
		return 10;
	else if (c == 'e' || c == 'E')
		return 11;
	else if (c == 'f' || c == 'F')
		return 12;
}


//vector<vector<int>> state_table
//{
//	{1,5,3,3},
//	{1,1,5,5},
//	{2,5,4,4}
//};

void init_set()
{
	// 初始化两个set表

	// 符号set
	symbol_set.insert('+');
	symbol_set.insert('-');
	symbol_set.insert('*');
	symbol_set.insert('/');
	symbol_set.insert('\\');
	symbol_set.insert('!');
	symbol_set.insert('=');
	symbol_set.insert('~');
	symbol_set.insert('|');
	symbol_set.insert('&');
	symbol_set.insert('%');
	symbol_set.insert('^');
	symbol_set.insert('(');
	symbol_set.insert(')');
	symbol_set.insert('[');
	symbol_set.insert(']');
	symbol_set.insert('{');
	symbol_set.insert('}');
	symbol_set.insert('\'');
	symbol_set.insert('"');
	symbol_set.insert('#');
	symbol_set.insert('<');
	symbol_set.insert('>');
	symbol_set.insert('?');
	symbol_set.insert(',');
	symbol_set.insert('.');
	symbol_set.insert(';');

	// 关键字set
}

// 从流中解析出一个C_word结构体 如果没有，则返回空内容
C_word do_process(const std::string& word, int& index, const int& type)
{
	switch (type)
	{
	case 0:
		return process_word(word, index);
	case 1:
		return process_symbol(word, index);
	case 2:
		return process_number(word, index);
	}
}

bool is_valid_word(const char& c)
{
	//[_0-9a-zA-Z】
	if (c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && 'c' <= 'Z'))
		return true;
	return false;
}

//按照数字的方式处理 一个句子，移动index，返回内容

/*
	数字的合法表示:
	a.[-]![0-9]![0-9]*   普通的数字,例如  123, -123
	b.各种进制n   n      进制数字表示法，二进制: 0b111  八进制: 017,-05 十六进制:0x13 -0x3A
	c.科学表示法         1.0e5  [±][1-9].[0-9]+E[±][0-9]+


			  -1 状态直接返回, 10和12状态如果错误，后退一步返回结果

			  -1 = 错误状态
			   0 = 初始状态   1 = 带有正负号的初始状态
			   2 = 初始位接受一个除0之外的其他数字的状态    3 = 初始位接收一个0 （八进制等待状态/0）         4 = 0b 二进制	                      5 = 0x 十六进制                     6 = 0 八进制确认状态
			   7 = 状态2接受一个[0-9] 12 -14                8 = 状态2/3/6/7接受一个小数点  1. 0.             9 = 状态7接受[0-9] 小数常态          10 = 0x2e 进入科学计数法等待状态   11 = 科学技术状态 0x2e0
			  12 = 科学计数负数准备状态 0x2e-              13 = 科学计数状态 0x2e-4

			  非接受状态 =  -1   1   4    5   


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

// 返回一个状态，查看是否是合法状态
inline bool is_valid_state(const int& state)
{
	if (state == -1 || state == 1 || state == 4 || state == 5)
		return false;
	return true;
}

C_word process_number(const std::string& word, int& index)
{
	// 数字方式进行处理，需要使用转移表

	// 获取转移表
	static vector<vector<int>> number_state;
	if(number_state.empty())
		number_state = get_num_table_instance();

	int state = 0;

	// 记录上一次合法的内容,并且以最终结果返回
	string last_valid_num;
	string num;

	while (index < word.size())
	{
		int rank = get_num_state_table_index(word.at(index));
		num += word.at(index);
		index++;

		state = number_state[state][rank];
		if (state == -1)
			break;
		if (is_valid_state(state) == true)
			last_valid_num = num;
	}

	return C_word(last_valid_num,NUMBER);
}

// 按照符号的方式处理句子，从中屯掉字符，返回一个C_word 关键字
C_word process_symbol(const std::string& word, int& index)
{
	// 符号
	string key;
	if (word.size()>1 && (index < (word.size() - 2)) && symbol_map.count(string(word.cbegin() + index, word.cbegin() + index + 2))!=0)
	{
		key = string(word.cbegin() + index, word.cbegin() + index + 2);
		index += 2;
		
	}
	else
	{
		key = string(word.cbegin() + index, word.cbegin() + index + 1);
		index++;
	}

	return symbol_map.at(key);
}

// 按照关键字/变量的方式来处理句子，屯掉字符，返回一个C_word 关键字
C_word process_word(const std::string& word, int& index)
{
	// 关键字/变量
	string key;
	while (index < word.size() && is_valid_word(word.at(index)))
		key += word.at(index++);

	if (symbol_map.count(key) == 0)
		symbol_map[key] = C_word(key, MEMBER);

	return symbol_map.at(key);
}

// 判断词从index位置开始，下一个内容是什么类型的： 0 = 关键字/变量  1 = 符号  2 = 数字  -1 = 错误
int get_word_type(const std::string& word, const int& index)
{
	if (word.empty())
		return -1;
	int flag = -1;
	char c = word.at(index);
	if (word.size() > 1)
	{
		// 至少两个字符
		if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			flag = 0;
		else if (symbol_set.count(c) != 0)
		{
			// 如果前两个是 [+-][0-9]则是数字，否则是符号
			if ((c == '+' || c == '-') && (word[1] >= 0 && word[1] <= 9))
				flag = 2;
			else flag = 1;
		}
		else if (c >= '0' && c <= '9')
			flag = 2;
	}
	else
		if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			flag = 0;
		else if (symbol_set.count(word[0]) != 0)
			flag = 1;
		else
			flag = 2;
	return flag;
}