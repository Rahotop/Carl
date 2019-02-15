#include "nk.hpp"

NK::NK(const std::string& path) : m_n(0), m_k1(0), m_2k1(0), m_mat(nullptr), m_var(nullptr), m_nbeval(0), m_fit(0.), m_s(new bool[1024])
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

NK::NK(unsigned int n, unsigned int k) : m_n(n), m_k1(k+1), m_2k1(pow(2,(k+1))), m_mat(new float[(unsigned int)(n*pow(2,(k+1)))]), m_var(new unsigned int[n*(k+1)]), m_nbeval(0), m_fit(0.), m_s(new bool[n])
{
	for(unsigned int i(0); i < m_n*m_k1; ++i)
	{
		m_var[i] = rand()%n;/*
		if(i%m_k1 == 0)
			m_var[i] = i/m_k1;*/
	}

	for(unsigned int i(0); i < m_n*m_2k1; ++i)
	{
		m_mat[i] = ((float)(rand()%10000))/(float)10000.;
	}
}

NK::NK(const NK& nk) : m_n(nk.m_n), m_k1(nk.m_k1), m_2k1(nk.m_2k1), m_mat(new float[nk.m_n*nk.m_2k1]), m_var(new unsigned int[nk.m_n*nk.m_k1]), m_nbeval(0), m_fit(0.), m_s(new bool[nk.m_n])
{
	for(unsigned int i(0); i < m_n*m_k1; ++i)
	{
		m_var[i] = nk.m_var[i];
	}

	for(unsigned int i(0); i < m_n*m_2k1; ++i)
	{
		m_mat[i] = nk.m_mat[i];
	}
}

NK::~NK()
{
	delete[] m_mat;
	delete[] m_var;
	if(m_s)
		delete[] m_s;
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
			tmp <<= 1;
			if(m_var[m_k1*i+j] < m_n)
				tmp += s[m_var[m_k1*i+j]];
			else if(m_var[m_k1*i+j] == m_n)
				tmp += 1;
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

void NK::showvar()
{
	unsigned int *tmp = new unsigned int[m_n+2];
	for(unsigned int i(0); i < m_n+2; ++i)
	{
		tmp[i] = 0;
	}
	for(unsigned int i(0); i < m_n*m_k1; ++i)
	{
		++tmp[m_var[i]];
	}
	for(unsigned int i(0); i < m_n+2; ++i)
	{
		//if(tmp[i] >= 5)
		std::cout << i << " : " << tmp[i] << std::endl;
	}
	std::cout << std::endl;
}

void NK::copylinks(const NK& nk)
{
	for(unsigned int i(0); i < m_n*m_k1; ++i)
	{
		m_var[i] = nk.m_var[i];
	}
}

void NK::copymat(const NK& nk)
{
	for(unsigned int i(0); i < m_n*m_2k1; ++i)
	{
		m_mat[i] = nk.m_mat[i];
	}
}

void NK::mutate(unsigned int prop, unsigned int nb)
{
	for(unsigned int i(0); i < m_n*m_2k1; ++i)
	{
		m_mat[i] = ((unsigned int)(rand()%1000) < prop) ? ((float)(rand()%10000))/(float)10000. : m_mat[i];
	}
	for(unsigned int i(0); i < nb; ++i)
	{
		m_var[rand()%(m_n*m_k1)] = rand()%m_n;
	}
}

void NK::mutate(unsigned int prop, unsigned int nb, bool *s, unsigned int loc)
{
	unsigned int *act = new unsigned int[m_n];
	for(unsigned int i(0); i < m_n; ++i)
	{
		unsigned int tmp = 0;
		for(unsigned int j(0); j < m_k1; ++j)
		{
			tmp <<= 1;
			tmp += s[m_var[m_k1*i+j]];
		}
		act[i] = tmp;
	}

/*
	for(unsigned int i(0); i < m_n; ++i)
	{
		for(unsigned int j(0); j < m_2k1; ++i)
		{
			m_mat[i*m_2k1+j] = ((unsigned int)(rand()%1000) < 
				prop+loc*(act[i]==j))
				 ? ((float)(rand()%10000))/(float)10000. : m_mat[i*m_2k1+j];
		}
	}*/
	for(unsigned int i(0); i < m_n*m_2k1; ++i)
	{
		m_mat[i] = ((unsigned int)(rand()%1000) < prop+loc*(act[i/m_2k1]==i%m_2k1)) ? ((float)(rand()%10000))/(float)10000. : m_mat[i];
	}
	for(unsigned int i(0); i < nb; ++i)
	{
		m_var[rand()%(m_n*m_k1)] = rand()%(m_n+2);
	}
/*
	for(unsigned int i(0); i < m_n*m_k1; ++i)
	{
		if(i%m_k1 == 0)
			m_var[i] = i/m_k1;
	}*/

	delete[] act;
}



