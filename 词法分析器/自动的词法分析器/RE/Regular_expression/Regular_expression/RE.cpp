#include "RE.h"

using namespace std;

vector<vector<int> > getReRuleStateTabel()
{
	/*          
		 0=初始状态    1=接受了一个字符之后的状态 2=字符之后有一个符号*  3=字符之后跟了一个|符号  -1=非法状态
		 输入内容为 0-9a-zA-Z则 进入index0 ,如果为*进入index 1,如果为|则进入index2

				输入			0-9a-ZA-Z               *                |
	     状态
		  
		  0							1					-1				 -1
		  1							1					2                3
		  2							1					-1              -1
		  3				            1                   -1				-1

		  0,1,2都为接受状态 
	*/

	//获得RE的状态转移表
	vector<vector<int> > stateTable
	{
	{1,-1,-1},
	{1,2,3},
	{1,-1,-1}
	};

	return stateTable;
}

RE::RE(const string& str):m_pReControl(new ReTransferControl(str))
{
}

bool RE::isValidRe() const
{
	if (m_pReControl != NULL)
	{
		return m_pReControl->isValideRe();
	}

	return false;
}

bool RE::isStrValidFromRe(const string& str) const
{
	if (this->isValidRe() && m_pReControl->isStrValidFromRe(str))
		return true;
	return false;
}

ReTransferControl::~ReTransferControl()
{
}

ReTransferControl::ReTransferControl(const string& str) :
	m_bIsReValid(false)
{
	/* 正则控制初始化部分,步骤:
	   1.判断转移字符串是否合法
	   2.如果合法 根据规则，生成状态转移结合
	   */

	 // 首先判断状态转移输入字符串是否合法
	vector<vector<int> > stateTable = getReRuleStateTabel();
	int state = 0;

	for (char c : str)
	{
		// 遍历字符进行状态转移
		if (state == -1)
		{
			m_bIsReValid = false;
			break;
		}

		int index = -1;
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			index = 0;
		}
		else if (c == '*')
		{
			index = 1;
		}
		else if (c == '|')
		{
			index = 2;
		}

		if (index == -1)
		{
			state = -1;
			break;
		}
		else
		{
			state = stateTable[state][index];
		}
	}

	if (state >= 0 && state <= 2)
	{
		m_bIsReValid = true;
	}

	if (m_bIsReValid == true)
	{
		// 2.到此处说明正则表达式规则合法，根据规则生成转移图

		// 2.0 为RE生成自己得 最初不接受状态，方便后续连接进行
		m_pBeginGraph = this->getReGraphByChar(NULL);
		m_pBeginGraph->m_pBegin->setAccept(str.empty());
		m_pBeginGraph->m_pAcceptStateElement->setAccept(str.empty());
		m_pBeginGraph->m_pEnd->setAccept(str.empty());

		// 2.1 根据输入为每个字符生成单独字符图，遇到符号则跳过
		vector< RegularCharGraph*> vecReCharGraphs(str.size(),NULL);
		
		for (int i = 0; i< str.size();i++)
		{
			char c = str[i];
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				// 是字符
				RegularCharGraph* charGraph = getReGraphByChar(&c);
				vecReCharGraphs[i] = charGraph;
			}
		}

		// 2.2 处理符号和进行连接，

		long index = -1;

		for (int i = 1; i < str.size(); i++)
		{
			// 判断内容，如果为字符，则和pre进行无条件转移，将pre的end连接到该字符的begin；
			//           如果为*，则将pre的状态进行修改，将他的begin改为接受状态
		    //           如果为|, 则将下一个字符的begin连接到pre的end，进行无条件转移
			char c = str[i];

			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				// 字符情况,将pre得end和当前图得begin进行无条件转移 连接

				
			}
			else if (c == '|')
			{
				// 选择情况,将pre和后一个elemnet连接
			}
			else if (c == '*')
			{
				// 闭包情况，将前一个element得开始状态设置为 接受

			}

		}


		// 2.3 按照顺序连接每个字符图

		// 2.4 处理特殊情况 空串
	}
}

bool ReTransferControl::isValideRe() const
{
	return false;
}

bool ReTransferControl::isStrValidFromRe(const string& str) const
{
	return false;
}

RegularCharGraph* ReTransferControl::getReGraphByChar(char* c)
{
	// 根据字符生成一个 单独字符得转移内容，自动分配状态index
	RegularCharGraph* graph = NULL;

	unsigned long index = m_mapStateIndex2ReElement.size();



	return nullptr;
}

set<unsigned long> ReTransferControl::getTransferSet(const set<unsigned long>& curStateSet, const char* c)
{
	return set<unsigned long>();
}

bool ReTransferControl::registeElement(RegularElement* element, const unsigned long& index)
{
	if (m_mapStateIndex2ReElement.find(index) == m_mapStateIndex2ReElement.end() && element!=NULL)
	{
		element->setIndex(index);
		m_mapStateIndex2ReElement[index] = element;
		return true;
	}
	return false;
}

