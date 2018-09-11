#include "nk.hpp"

NK::NK(const std::string& path) : m_n(0), m_k1(0), m_2k1(0), m_mat(nullptr), m_var(nullptr), m_nbeval(0)
{
	std::ifstream in(path.c_str());

	in >> m_n >> m_k1;
	++m_k1;
	m_2k1 = pow(2,m_k1);

	m_mat = new float[m_n*m_2k1];
	m_var = new unsigned int[m_n*m_k1];

	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_k1; ++j)
		{
			in >> m_var[m_k1*i+j];
		}
	}

	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_2k1; ++j)
		{
			in >> m_mat[m_2k1*i+j];
		}
	}
}

NK::~NK()
{
	delete[] m_mat;
	delete[] m_var;
}

float NK::evaluate(bool *s)
{
	++m_nbeval;
	float sum = 0.;
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_k1; ++j)
		{
			tmp = tmp << 1;
			tmp += s[m_var[m_k1*i+j]];
		}

		sum += m_mat[m_2k1*i+tmp];
	}
	return sum/(float)m_n;
}

bool NK::islocopt(bool *s)
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




