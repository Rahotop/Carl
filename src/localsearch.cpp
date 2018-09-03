#include "localsearch.hpp"

bool* localsearch(MaxSat& ms)
{
	bool *s = new bool[ms.getN()];
	for(unsigned int i(0); i < ms.getN(); ++i) s[i] = rand()%2;
	float score = ms.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < ms.getN(); ++i) next.push_back(i);

	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < ms.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = ms.evaluate(s);

			if(tmp > score)
			{
				score = tmp;
				improved = true;
				break;
			}
			else
			{
				s[next[i]] = !s[next[i]];
			}
		}
	}

	return s;
}

bool* localsearch(MSmemory& mm)
{
	bool *s = new bool[mm.getN()];
	for(unsigned int i(0); i < mm.getN(); ++i) s[i] = rand()%2;
	float score = mm.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < mm.getN(); ++i) next.push_back(i);

	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < mm.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = mm.evaluate(s);

			if(tmp > score)
			{
				score = tmp;
				improved = true;
				break;
			}
			else
			{
				s[next[i]] = !s[next[i]];
			}
		}
	}

	return s;
}

bool* localsearch(const FnArray& fn)
{
	bool *s = new bool[fn.getN()];
	for(unsigned int i(0); i < fn.getN(); ++i) s[i] = rand()%2;
	float score = fn.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < fn.getN(); ++i) next.push_back(i);

	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < fn.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = fn.evaluate(s);

			if(tmp > score)
			{
				score = tmp;
				improved = true;
				break;
			}
			else
			{
				s[next[i]] = !s[next[i]];
			}
		}
	}

	return s;
}

bool* localsearch(FnArrayInc& fn)
{
	bool *s = new bool[fn.getN()];
	for(unsigned int i(0); i < fn.getN(); ++i) s[i] = rand()%2;
	float score = fn.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < fn.getN(); ++i) next.push_back(i);

	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < fn.getN(); ++i)
		{
			float tmp = fn.evaluateInc(s, next[i]);

			if(tmp > score)
			{
				s[next[i]] = !s[next[i]];
				score = fn.evaluate(s);
				improved = true;

				break;
			}
		}
	}

	return s;
}
