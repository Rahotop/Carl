#include "royalroad.hpp"

RoyalRoad::RoyalRoad(unsigned int n, unsigned int k) : m_n(n), m_k(k), m_nbeval(0)
{
	
}

RoyalRoad::~RoyalRoad()
{
	
}

float RoyalRoad::evaluate(bool *s)
{
	++m_nbeval;
	float score = 0.;
	for(unsigned int i(0); i < (m_n/m_k); ++i)
	{
		float tmp = 1.;
		for(unsigned int j(0); j < m_k; ++j)
		{
			tmp = (s[i*m_k + j]) ? tmp : 0.;
		}
		score += tmp*m_k;
	}
	return score;
}

bool RoyalRoad::islocopt(bool *s)
{
	float score = evaluate(s);
	for(unsigned int i(0); i < m_n; ++i)
	{
		s[i] = !s[i];
		bool tmp = false;
		if(score < evaluate(s))
			tmp = true;
		s[i] = !s[i];
		if(tmp)
			return false;
	}
	return true;
}











