#include "RegularElement.h"

//void RegularElement::addTransferState(const string& key, const vector<unsigned long>& states)
//{
//	for (auto i : states)
//	{
//		m_mapTransferMap[key].push_back(i);
//	}
//}
//
//vector<unsigned long> RegularElement::getTransferStateTable(const string& key) const
//{
//	if (m_mapTransferMap.find(key) != m_mapTransferMap.end())
//		return m_mapTransferMap.at(key);
//	return vector<unsigned long>();
//}

unsigned long RegularElement::getIndex() const
{
	return m_ulStateIndex;
}

void RegularElement::setIndex(const unsigned long& index)
{
	m_ulStateIndex = index;
}

bool RegularElement::isAccept() const
{
	return m_bIsAccept;
}

void RegularElement::setAccept(const bool& b)
{
	m_ulStateIndex = b;
}

RegularCharGraph::RegularCharGraph(const char& c)
{

}
