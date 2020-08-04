/*
�ֹ��ʷ�������������һ���ַ����������������ݣ��жϸôʵ�����Լ�����洢��
����: �ַ���
���: vector<C_word> �ʼ���

���̣�
1.ѭ�������ж�ȡ����,���Է�Ϊ�����ࣺ a.�ؼ���/���� b.����
	1.0 �����һ���ַ��ǿո�������
		1.1.0 �����һ���ַ��ǷǷ���: ����������ʽ [_a-zA-Z]{1} [_a-zA-Z0-9]* ֱ����ȡ�����Ϸ����ַ���
		1.1.1 �жϽ�ȡ���ַ��� s ��ͨ����ϣ���㣬�ж��Ƿ��� �ؼ��֣����������vectgor ��pushһ���ؼ��ֵĽṹ��
			  ������ǣ�����vector��pushһ�� �����Ľṹ��
		1.1.2 �����һ���ַ��Ƿ��ţ���ô��ȡ��һ���ַ�������ϲ���һ��ͨ������hash �ж��Ƿ� �Ϸ����š�
			  ����ǣ�������һ�����Ա��� �˷��ŵ� �ṹ��,push��vector�У�
			  ������ǣ��򽫺���һ���ֽڣ����� ��һ���ַ���ɵķ��Žṹ�壬push ��voctor��
		1.1.3 �����һ���ַ������֣���ô������һ�����֣��Ϸ������ֱ�ʾ��Ϊ:
			  a.[-]![0-9]![0-9]*   ��ͨ������,����  123, -123
			  b.���ֽ���n   n      �������ֱ�ʾ����������: 0b111  �˽���: 017,-05 ʮ������:0x13 -0x3A
			  c.��ѧ��ʾ��         1.0e5f
2.�������һ��vector<C_word> ������

���ʷ������������ж�ĳ���ַ����ĺ�����ʲô�������﷨����ȷ�Բ����ж�
*/

// ״̬ת�Ʊ�

/*
	 �����ж������������ַ����ͣ�����,ת�Ʊ��ؽ��ֱ�������ָ�״̬0 ��������һ�������ж�:

	 0 = ����ʼ״̬   1 = ��һ���ؼ���Ϊword��/���� ���� �������ת�Ʊ�   2 = ��Ϊ����/�����м�״̬ ������������˻ص���λ�ã���Ϊ���ţ��������ת�Ʊ�
					  3 = ��Ϊ����״̬   �������ת�Ʊ�                   4 = ��Ϊ����״̬������������ת�Ʊ�
			  ״̬       0      1      2       3
	 ����

	 blank               0
   [_a-zA-Z]             1
	 [0-9]               4             4
	  [-+]               2             3
 ����+-����������        3

									   [�����ж�]
*/

/*
	���ֵĺϷ���ʾ:
	a.[-]![0-9]![0-9]*   ��ͨ������,����  123, -123
	b.���ֽ���n   n      �������ֱ�ʾ����������: 0b111  �˽���: 017,-05 ʮ������:0x13 -0x3A
	c.��ѧ��ʾ��         1.0e5  [��][1-9].[0-9]+E[��][0-9]+

	����״̬ת�Ʊ�:

			  -1 ״ֱ̬�ӷ���, 10��12״̬������󣬺���һ�����ؽ��

			  -1 = ����״̬
			   0 = ��ʼ״̬   1 = ���������ŵĳ�ʼ״̬
			   2 = ��ʼλ����һ����0֮����������ֵ�״̬    3 = ��ʼλ����һ��0 ���˽��Ƶȴ�״̬/0��         4 = 0b ������	                      5 = 0x ʮ������                     6 = 0 �˽���ȷ��״̬
			   7 = ״̬2����һ��[0-9] 12 -14                8 = ״̬2/3/6/7����һ��С����  1. 0.             9 = ״̬7����[0-9] С����̬          10 = 0x2e �����ѧ�������ȴ�״̬   11 = ��ѧ����״̬ 0x2e0
			  12 = ��ѧ��������׼��״̬ 0x2e-


			  ״̬		0		1		2		3		4		5		6		7		8		9     10      11       12

	����

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
	[g-zG-Z ����x]
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

// ��ѯ��hash_set
static unordered_set<char> symbol_set;
static unordered_set<char> word_set;

// ǰ�ú�������
C_word do_process(const std::string& word, int& index, const int& type);
int get_word_type(const std::string& word, const int& index);
void init_set();
C_word process_word(const std::string& word, int& index);
C_word process_symbol(const std::string& word, int& index);
C_word process_number(const std::string& word, int& index);

vector<C_word> analyze_word(ifstream& file);

// ��ĳ���ļ���·�����з���������һ��vector<C_word> �ؼ�����
vector<C_word> analyze_word(const string& file_name)
{
	ifstream file_stream;
	file_stream.open(file_name, ios_base::in);
	return analyze_word(file_stream);
}

// ���ļ�����������C���Թؼ���vector
vector<C_word> analyze_word(ifstream& file)
{
	// ���� �ļ���������һ�� ������
	vector<C_word> words;
	if (!file.is_open())
		return words;

	if (symbol_set.empty())
		init_set();

	if (symbol_map.empty())
		init_symbol_map();

	// ��÷���

	// ѭ�����ļ��ж�ȡ����
	while (!file.eof())
	{
		string line;

		// ѭ����ȡÿ��
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
					// �ж� ������� �����Ŵʡ� ���� ���ַ��ʡ� ��������   0 = �ַ��� 1 = ���Ŵ�   2 = ���ִ�   -1 ����
					int flag = get_word_type(word, index);

					if (flag == -1)
					{
						std::cout << "Error the word is not Symbol or Number" << std::endl; exit(1);
					}

					// ��ʽ�������̣�������ؽ��Ϊ�գ�˵�������˴���
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

// ��� һ���ʵ�����
void out_Cword(const vector<C_word>& words)
{
	int max_row = 5;
	for (const C_word& word : words)
		std::cout << word << std::endl;
}

// �������״̬ת�Ʊ�
/*

			�������     [+]       [-]        [0]        [1]        [2-7]      [8-9]    [.]      [xX]       [aA]        [bB]     [eE]      [cdCD]     [eE]     [fF]     
	״̬

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

//ͨ��һ���ַ���� ����ת�Ʊ��ת���±�
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
	// ��ʼ������set��

	// ����set
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

	// �ؼ���set
}

// �����н�����һ��C_word�ṹ�� ���û�У��򷵻ؿ�����
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
	//[_0-9a-zA-Z��
	if (c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && 'c' <= 'Z'))
		return true;
	return false;
}

//�������ֵķ�ʽ���� һ�����ӣ��ƶ�index����������

/*
	���ֵĺϷ���ʾ:
	a.[-]![0-9]![0-9]*   ��ͨ������,����  123, -123
	b.���ֽ���n   n      �������ֱ�ʾ����������: 0b111  �˽���: 017,-05 ʮ������:0x13 -0x3A
	c.��ѧ��ʾ��         1.0e5  [��][1-9].[0-9]+E[��][0-9]+


			  -1 ״ֱ̬�ӷ���, 10��12״̬������󣬺���һ�����ؽ��

			  -1 = ����״̬
			   0 = ��ʼ״̬   1 = ���������ŵĳ�ʼ״̬
			   2 = ��ʼλ����һ����0֮����������ֵ�״̬    3 = ��ʼλ����һ��0 ���˽��Ƶȴ�״̬/0��         4 = 0b ������	                      5 = 0x ʮ������                     6 = 0 �˽���ȷ��״̬
			   7 = ״̬2����һ��[0-9] 12 -14                8 = ״̬2/3/6/7����һ��С����  1. 0.             9 = ״̬7����[0-9] С����̬          10 = 0x2e �����ѧ�������ȴ�״̬   11 = ��ѧ����״̬ 0x2e0
			  12 = ��ѧ��������׼��״̬ 0x2e-              13 = ��ѧ����״̬ 0x2e-4

			  �ǽ���״̬ =  -1   1   4    5   


			�������     [+]       [-]        [0]        [1]        [2-7]      [8-9]    [.]      [xX]       [aA]        [bB]     [eE]      [cdCD]     [eE]     [fF]      ����      
	״̬       

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

// ����һ��״̬���鿴�Ƿ��ǺϷ�״̬
inline bool is_valid_state(const int& state)
{
	if (state == -1 || state == 1 || state == 4 || state == 5)
		return false;
	return true;
}

C_word process_number(const std::string& word, int& index)
{
	// ���ַ�ʽ���д�����Ҫʹ��ת�Ʊ�

	// ��ȡת�Ʊ�
	static vector<vector<int>> number_state;
	if(number_state.empty())
		number_state = get_num_table_instance();

	int state = 0;

	// ��¼��һ�κϷ�������,���������ս������
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

// ���շ��ŵķ�ʽ������ӣ������͵��ַ�������һ��C_word �ؼ���
C_word process_symbol(const std::string& word, int& index)
{
	// ����
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

// ���չؼ���/�����ķ�ʽ��������ӣ��͵��ַ�������һ��C_word �ؼ���
C_word process_word(const std::string& word, int& index)
{
	// �ؼ���/����
	string key;
	while (index < word.size() && is_valid_word(word.at(index)))
		key += word.at(index++);

	if (symbol_map.count(key) == 0)
		symbol_map[key] = C_word(key, MEMBER);

	return symbol_map.at(key);
}

// �жϴʴ�indexλ�ÿ�ʼ����һ��������ʲô���͵ģ� 0 = �ؼ���/����  1 = ����  2 = ����  -1 = ����
int get_word_type(const std::string& word, const int& index)
{
	if (word.empty())
		return -1;
	int flag = -1;
	char c = word.at(index);
	if (word.size() > 1)
	{
		// ���������ַ�
		if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			flag = 0;
		else if (symbol_set.count(c) != 0)
		{
			// ���ǰ������ [+-][0-9]�������֣������Ƿ���
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