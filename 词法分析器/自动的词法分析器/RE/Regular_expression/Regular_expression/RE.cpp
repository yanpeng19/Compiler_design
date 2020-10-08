#include "RE.h"

using namespace std;

vector<vector<int> > getReRuleStateTabel()
{
	/*          
		 0=��ʼ״̬    1=������һ���ַ�֮���״̬ 2=�ַ�֮����һ������*  3=�ַ�֮�����һ��|����  -1=�Ƿ�״̬
		 ��������Ϊ 0-9a-zA-Z�� ����index0 ,���Ϊ*����index 1,���Ϊ|�����index2

				����			0-9a-ZA-Z               *                |
	     ״̬
		  
		  0							1					-1				 -1
		  1							1					2                3
		  2							1					-1              -1
		  3				            1                   -1				-1

		  0,1,2��Ϊ����״̬ 
	*/

	//���RE��״̬ת�Ʊ�
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
	/* ������Ƴ�ʼ������,����:
	   1.�ж�ת���ַ����Ƿ�Ϸ�
	   2.����Ϸ� ���ݹ�������״̬ת�ƽ��
	   */

	 // �����ж�״̬ת�������ַ����Ƿ�Ϸ�
	vector<vector<int> > stateTable = getReRuleStateTabel();
	int state = 0;

	for (char c : str)
	{
		// �����ַ�����״̬ת��
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
		// 2.���˴�˵��������ʽ����Ϸ������ݹ�������ת��ͼ

		// 2.0 ΪRE�����Լ��� ���������״̬������������ӽ���
		m_pBeginGraph = this->getReGraphByChar(NULL);
		m_pBeginGraph->m_pBegin->setAccept(str.empty());
		m_pBeginGraph->m_pAcceptStateElement->setAccept(str.empty());
		m_pBeginGraph->m_pEnd->setAccept(str.empty());

		// 2.1 ��������Ϊÿ���ַ����ɵ����ַ�ͼ����������������
		vector< RegularCharGraph*> vecReCharGraphs(str.size(),NULL);
		
		for (int i = 0; i< str.size();i++)
		{
			char c = str[i];
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				// ���ַ�
				RegularCharGraph* charGraph = getReGraphByChar(&c);
				vecReCharGraphs[i] = charGraph;
			}
		}

		// 2.2 ������źͽ������ӣ�

		long index = -1;

		for (int i = 1; i < str.size(); i++)
		{
			// �ж����ݣ����Ϊ�ַ������pre����������ת�ƣ���pre��end���ӵ����ַ���begin��
			//           ���Ϊ*����pre��״̬�����޸ģ�������begin��Ϊ����״̬
		    //           ���Ϊ|, ����һ���ַ���begin���ӵ�pre��end������������ת��
			char c = str[i];

			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				// �ַ����,��pre��end�͵�ǰͼ��begin����������ת�� ����

				
			}
			else if (c == '|')
			{
				// ѡ�����,��pre�ͺ�һ��elemnet����
			}
			else if (c == '*')
			{
				// �հ��������ǰһ��element�ÿ�ʼ״̬����Ϊ ����

			}

		}


		// 2.3 ����˳������ÿ���ַ�ͼ

		// 2.4 ����������� �մ�
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
	// �����ַ�����һ�� �����ַ���ת�����ݣ��Զ�����״̬index
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

