#pragma once
#include "RegularElement.h"
#include <set>

using namespace std;

/*  RE������ʽ��  via nfa�汾

	1.ְ�� 
		1.1 �ṩ���ظ���״̬index
		1.2 �ṩ״̬ת�Ʊ�
		1.3 ����������������״̬ת�Ʊ�
		1.4 ���������������ݣ�����״̬ת�Ʊ�
		1.5 ������������Ƿ����
	2.�ӿ�
		2.1 ʹ���ַ�����ʼ��
		2.2 �ṩ����ж�������ʽ�Ƿ����
		2.3 �ṩ�ַ������룬�����Ƿ���������ʽ

	3.�ڲ�����
		3.1 ��һ�������������� ��ʼ���ַ���,�����ж� �Ƿ���� ������� ������ѧϰ�ã���֧�� ���ӣ��հ���ѡ�����ֻ������򣩣�������ϣ������������ �����������һ��״̬ת��ͼ��
			3.1.1 �����������ʼ���򷽷�: a.�����鿴�Ƿ���Ϲ���
										  b.�������,���Ƚ������ַ������ɵ����ľ���  ����ʼ��-��������ת�ƣ�-�������ַ�-����ȷ�ַ�ת�ƣ�-�����ַ�-��������ת�ƣ�-����״̬    ÿ�������ַ���4��״̬
										  c.�����������е�*�ţ���*֮ǰ�ĵ���״̬�ַ����д���-����ʼ״̬Ҳ����Ϊ���ܣ������Ǻ�ȥ��
										  d.�������д�����ʼ����  
											-���ӵĴ�����: ��ǰһ��״̬ͼ��end ������ת�Ƶ������begin
											-��Ĵ���������ǰһ��״̬ͼ��end ������ת�����ӵ� ���򡿷��ź����״̬ͼ��begng
	4.����һ���ַ����ж��Ƿ����������ʽ�Ĺ��̣�
		a.�����ж���������Ƿ���������������״̬����Ϊ��ʼ״̬�����ұ������С���ʼ״̬�������п��� ͨ����������ת�ơ������״̬�����״̬����set
		b.���������ַ����� ״̬����set �е�ÿ��״̬�ó��������Խ���ת�ƣ��������ת�Ƶ��µĺϷ�״̬�������µĺϷ�״̬�ġ�������ת�ơ� ״̬Ҳ���뵽 ��״̬������
		c.���û��״̬���ʾ�������������״̬�ң������� ����״̬�� ���ʾ�ô��������������ʽ����
*/

class RegularElement;
class ReTransferControl;
class RegularCharGraph;

class RE
{
public:
	RE() = delete;
	RE(const string& str);

	// ����Ϊ�ӿ�
	bool isValidRe() const;						      // �ж�������ʽ��ʼ���������Ƿ�Ϸ����������Ƿ���������﷨   0=������ 1=����
	bool isStrValidFromRe(const string& str) const;	  // �����ַ����жϸ��ַ����Ƿ�����������   0=������ 1=����

private:
	ReTransferControl*								m_pReControl;
};


/* ���� ״̬ת�ƿ��ƻ�������ַ�״̬���,��¼״̬��ŵ�Ԫ��,�Լ�����״̬��ŵ�ѯ��
   
*/
class ReTransferControl
{
	friend class  RE;
public:
	~ReTransferControl();

protected:
	ReTransferControl() = delete;												 					// ������Ĭ�ϳ�ʼ��
	ReTransferControl(const string& str);															// ��ʼ��ʹ�� ������� ��ʼ��������
	bool isValideRe() const;																		// ������ʼ����������Ƿ�Ϸ��Ľ�� 0=���Ϸ� 1=�Ϸ�
	bool isStrValidFromRe(const string& str) const;													// �������ַ����Ƿ�����������Ľӿ� 0=������ 1=����

private:
	// ����Ϊ����
	RegularCharGraph* getReGraphByChar(char* c);											        // ����һ�����ţ����һ���Ѿ����õķ����ڵ�״̬ת��ͼ
	set<unsigned long> getTransferSet(const set<unsigned long>& curStateSet, const char* c);		// ���� ����״̬���ϣ�ͨ��ת���ַ�c����ת�ƣ�����֮����Ե���ļ��� c���ΪNULL,���ʾ������ת��
	bool registeElement(RegularElement* element, const unsigned long& index);

private:
	map<unsigned long, RegularElement*>				m_mapStateIndex2ReElement;	// ״̬��ż�¼��
	RegularCharGraph*								m_pBeginGraph;              // ״̬ת�Ƴ�ʼǰ��
	bool											m_bIsReValid;				// ��ʼ�������Ƿ�Ϸ�
};
