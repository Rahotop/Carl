#include "onemax.hpp"

OneMax::OneMax(unsigned int n) : m_n(n), m_nbeval(0)
{
	
}

OneMax::~OneMax()
{
	
}

float OneMax::evaluate(bool *s)
{
	++m_nbeval;
	float tmp = 0.;
	for(unsigned int i(0); i < m_n; ++i)
	{
		tmp += s[i];
	}
	return tmp;
}

bool OneMax::islocopt(bool *s)
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


