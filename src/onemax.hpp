#ifndef ONEMAX_HPP_INCLUDED
#define ONEMAX_HPP_INCLUDED

class OneMax
{
	public:

	OneMax(unsigned int n);
	~OneMax();

	float evaluate(bool *s);
	bool islocopt(bool *s);

	inline unsigned int getnbeval() const { return m_nbeval; }
	inline unsigned int getN() const { return m_n; }

	private:

	unsigned int m_n;
	unsigned int m_nbeval;
};

#endif