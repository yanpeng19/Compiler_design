#pragma once

/* ������ʽ�Ļ���Ԫ�� element,���ݣ�
	1 ״̬���
	2 �Ƿ���������

	������ʽ�Ļ�����ĸ RegularCharGraph,���ݣ�
	1.��ʼelemnt, ����״̬element, ����elemtnt,   ��3��״̬            begin-(������ȷ�ַ���)-����״̬-(������ת��)-����״̬

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

