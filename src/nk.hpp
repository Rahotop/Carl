#ifndef NK_HPP_INCLUDED
#define NK_HPP_INCLUDED

#include <string>
#include <fstream>
#include <vector>
#include <cmath>

class NK
{
	public:

	NK(const std::string& path);
	~NK();

	float evaluate(bool *s);
	bool islocopt(bool *s);

	inline unsigned int getnbeval() const { return m_nbeval; }
	inline unsigned int getN() const { return m_n; }

	private:

	unsigned int m_n;
	unsigned int m_k1;
	unsigned int m_2k1;

	float *m_mat;
	unsigned int *m_var;
	unsigned long long m_nbeval;
};

#endif