#include "maxsat.hpp"

MaxSat::MaxSat(const std::string& path) : m_n(0), m_nbclauses(0), m_size(0), m_clauses(nullptr), m_nbeval(0)
{
	std::ifstream in(path.c_str());

	while(true)
	{
		std::string tmp;
		in >> tmp;
		if(tmp == "c")
		{
			char buffer[256];
			in.getline(buffer,256);
		}
		else if(tmp == "p")
		{
			in >> tmp >> m_n >> m_nbclauses;
			break;
		}
	}

	int savepoint = in.tellg();

	for(unsigned int i(0); i < m_nbclauses; ++i)
	{
		int tmp(0);
		do{
			in >> tmp;
			++m_size;
		} while(tmp != 0);
	}

	in.seekg(savepoint);

	m_clauses = new int[m_size];

	for(unsigned int i(0); i < m_size; ++i)
	{
		in >> m_clauses[i];
	}
}

MaxSat::~MaxSat()
{
	if(m_clauses)
		delete[] m_clauses;
}

float MaxSat::evaluate(bool *s) const
{
	//++m_nbeval;

	float score = 0;
	bool sat = false;
	for(unsigned int i(0); i < m_size; ++i)
	{
		int form = m_clauses[i];
		bool tmp = false;
		if(form == 0)
		{
			score += sat;
			sat = false;
			tmp = false;
		}
		else
		{
			tmp = (form > 0) ? s[form-1] : !s[-form-1];
		}
		sat = sat || tmp;
	}
	return score;
}
