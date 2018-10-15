#ifndef ROYALROAD_HPP_INCLUDED
#define ROYALROAD_HPP_INCLUDED

class RoyalRoad
{
	public:

	RoyalRoad(unsigned int n, unsigned int k);
	~RoyalRoad();

	float evaluate(bool *s);
	bool islocopt(bool *s);

	inline unsigned int getnbeval() const { return m_nbeval; }
	inline unsigned int getN() const { return m_n; }

	private:

	unsigned int m_n;
	unsigned int m_k;
	unsigned int m_nbeval;
};

#endif