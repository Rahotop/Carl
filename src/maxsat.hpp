#ifndef MAXSAT_HPP_INCLUDED
#define MAXSAT_HPP_INCLUDED

#include <string>
#include <fstream>

class MaxSat
{
	public:

	MaxSat(const std::string& path);
	~MaxSat();

	float evaluate(bool *s) const;

	inline unsigned int getnbeval() const { return m_nbeval; }
	inline unsigned int getN() const { return m_n; }

	private:

	unsigned int m_n;
	unsigned int m_nbclauses;
	unsigned int m_size;
	int *m_clauses;
	unsigned int m_nbeval;
};

#endif