#pragma once
#include "RegularElement.h"
#include <set>

using namespace std;

/*  RE正则表达式类  via nfa版本

	1.职责 
		1.1 提供不重复的状态index
		1.2 提供状态转移表
		1.3 根据输入内容生成状态转移表
		1.4 根据声明输入内容，生成状态转移表
		1.5 根据输入给出是否成立
	2.接口
		2.1 使用字符串初始化
		2.2 提供借口判断正则表达式是否成立
		2.3 提供字符串输入，返回是否符合正表达式

	3.内部构造
		3.1 由一个控制器，根据 初始化字符串,首先判断 是否符合 正则规则 （本类学习用，仅支持 连接，闭包，选择三种基本规则），如果符合，则控制器根据 正则规则，生成一个状态转移图。
			3.1.1 控制器处理初始规则方法: a.遍历查看是否符合规则
										  b.如果符合,首先将所有字符，生成单独的具有  【开始】-（无条件转移）-待接受字符-（正确字符转移）-接受字符-（无条件转移）-结束状态    每个基本字符共4个状态
										  c.遍历处理所有的*号，将*之前的单度状态字符进行处理-将开始状态也设置为接受，并将星号去掉
										  d.遍历现有串，开始处理  
											-连接的处理方法: 将前一个状态图的end 无条件转移到后面的begin
											-或的处理方法：将前一个状态图的end 无条件转移连接到 【或】符号后面的状态图的begng
	4.处理一个字符串判断是否符合正则表达式的过程：
		a.首先判断正则规则是否成立，如果成立将状态设置为初始状态，并且遍历所有【初始状态】的所有可以 通过【无条件转移】到达的状态，获得状态集合set
		b.根据输入字符，将 状态集合set 中的每个状态拿出来，尝试进行转移，如果可以转移到新的合法状态，并将新的合法状态的【无条件转移】 状态也加入到 新状态集合中
		c.如果没有状态则表示不成立，如果有状态且，其中有 接受状态， 则表示该次输入符合正则表达式规则
*/

class RegularElement;
class ReTransferControl;
class RegularCharGraph;

class RE
{
public:
	RE() = delete;
	RE(const string& str);

	// 以下为接口
	bool isValidRe() const;						      // 判断正则表达式初始化的内容是否合法，该正则是否符合正则语法   0=不符合 1=符合
	bool isStrValidFromRe(const string& str) const;	  // 输入字符串判断该字符串是否符合正则规则   0=不符合 1=符合

private:
	ReTransferControl*								m_pReControl;
};


/* 单例 状态转移控制机，负责分发状态编号,记录状态编号的元素,以及接受状态编号的询问
   
*/
class ReTransferControl
{
	friend class  RE;
public:
	~ReTransferControl();

protected:
	ReTransferControl() = delete;												 					// 不允许默认初始化
	ReTransferControl(const string& str);															// 初始化使用 正则规则 初始化控制器
	bool isValideRe() const;																		// 给出初始化正则规则是否合法的结果 0=不合法 1=合法
	bool isStrValidFromRe(const string& str) const;													// 给出的字符串是否符合正则规则的接口 0=不符合 1=符合

private:
	// 以下为函数
	RegularCharGraph* getReGraphByChar(char* c);											        // 输入一个符号，获得一个已经连好的符号内的状态转移图
	set<unsigned long> getTransferSet(const set<unsigned long>& curStateSet, const char* c);		// 根据 现在状态集合，通过转移字符c进行转移，返回之后可以到达的集合 c如果为NULL,则表示无条件转移
	bool registeElement(RegularElement* element, const unsigned long& index);

private:
	map<unsigned long, RegularElement*>				m_mapStateIndex2ReElement;	// 状态编号记录表
	RegularCharGraph*								m_pBeginGraph;              // 状态转移初始前置
	bool											m_bIsReValid;				// 初始化规则是否合法
};
