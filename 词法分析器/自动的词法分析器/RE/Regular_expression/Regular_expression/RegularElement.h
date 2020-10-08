#pragma once

/* 正则表达式的基本元素 element,内容：
	1 状态编号
	2 是否结果被接受

	正则表达式的基本字母 RegularCharGraph,内容：
	1.开始elemnt, 接受状态element, 结束elemtnt,   共3个状态            begin-(接受正确字符后)-接受状态-(无条件转移)-结束状态

 */
#include <map>
#include <string>
#include <vector>
#include "RE.h"

using namespace std;

class ReTransferControl;
class RegularElement;

class RegularCharGraph
{
public:
	RegularCharGraph() = delete;
	RegularCharGraph(const char& c);
	RegularElement* begin();
	RegularElement* end();

private:
	RegularElement*  m_pBegin;
	RegularElement*  m_pEnd;
	RegularElement*  m_pAcceptStateElement;

	friend class ReTransferControl;v 

};

class   bn
{
	friend class ReTransferControl;
public:
	unsigned long getIndex() const;
	//void setIndex(const unsigned long& index);
	bool isAccept() const;

protected:
	void setIndex(const unsigned long& index);
	void setAccept(const bool& b);

private:
	unsigned long								m_ulStateIndex;
	bool										m_bIsAccept;
};

