#ifndef NK_HPP_INCLUDED
#define NK_HPP_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

class NK
{
	public:

	NK(const std::string& path);
	NK(unsigned int n, unsigned int k);
	NK(const NK& nk);
	~NK();

	float evaluate(bool *s);
	bool islocopt(bool *s);
	void showvar();
	void copylinks(const NK& nk);
	void copymat(const NK& nk);

	inline unsigned int getnbeval() const { return m_nbeval; }
	inline unsigned int getN() const { return m_n; }

	////
	inline void setfitness(float fit) { m_fit = fit; }
	inline float getfitness() const { return m_fit; }
	void mutate(unsigned int prop, unsigned int nb);
	void mutate(unsigned int prop, unsigned int nb, bool *s, unsigned int loc);
	inline unsigned int size() const { return 0; }
	inline unsigned int getsteps() const { return m_nbstep; }
	inline bool* getSol() const { return m_s; }

	bool* ls(bool *s, unsigned int *nbEval = nullptr)
	{
		for(unsigned int i(0); i < m_n; ++i) m_s[i] = s[i];
		float score = evaluate(m_s);
		unsigned int tmpEval = 1;

		std::vector<unsigned int> next(m_n,0);
		for(unsigned int i(0); i < m_n; ++i) //next.push_back(i);
			next[i] = i;

		bool improved = true;
		m_nbstep = 0;

		while(improved)
		{
			std::random_shuffle(next.begin(), next.end());
			improved = false;

			for(unsigned int i(0); i < m_n; ++i)
			{
				m_s[next[i]] = !m_s[next[i]];
			//	float tmp = evaluateinc(m_s, next[i]);
				float tmp = evaluate(m_s);
				++tmpEval;

				if(tmp > score)
				{
					score = tmp;
					improved = true;
					++m_nbstep;
					break;
				}
				else
				{
					m_s[next[i]] = !m_s[next[i]];
				}
			}
		}

		if(nbEval)
			*nbEval += tmpEval;

		return m_s;
	}

	private:

	unsigned int m_n;
	unsigned int m_k1;
	unsigned int m_2k1;

	float *m_mat;
	unsigned int *m_var;
	unsigned long long m_nbeval;

	float m_fit;
	bool *m_s;
	unsigned int m_nbstep;
};

#endif