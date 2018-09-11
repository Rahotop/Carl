#include "localsearch.hpp"

bool* localsearch(MaxSat& ms, unsigned int *nbEval)
{
	bool *s = new bool[ms.getN()];
	for(unsigned int i(0); i < ms.getN(); ++i) s[i] = rand()%2;
	float score = ms.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < ms.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = ms.getnbeval();
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

	if(nbEval)
		*nbEval += ms.getnbeval() - tmpnbeval;

	return s;
}

bool* localsearch(MSmemory& mm, unsigned int *nbEval)
{
	bool *s = new bool[mm.getN()];
	for(unsigned int i(0); i < mm.getN(); ++i) s[i] = rand()%2;
	float score = mm.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < mm.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = mm.getnbeval();
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

	if(nbEval)
		*nbEval += mm.getnbeval() - tmpnbeval;

	return s;
}

bool* localsearch(const FnArray& fn, unsigned int *nbEval)
{
	bool *s = new bool[fn.getN()];
	for(unsigned int i(0); i < fn.getN(); ++i) s[i] = rand()%2;
	float score = fn.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < fn.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = 1;
	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < fn.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = fn.evaluate(s);
			++tmpnbeval;

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

	if(nbEval)
		*nbEval += tmpnbeval;

	return s;
}

bool* localsearch(FnArrayInc& fn, unsigned int *nbEval)
{
	bool *s = new bool[fn.getN()];
	for(unsigned int i(0); i < fn.getN(); ++i) s[i] = rand()%2;
	float score = fn.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < fn.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = 1;
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

	if(nbEval)
		*nbEval += tmpnbeval;

	return s;
}

bool *ils(MaxSat& ms, unsigned int cycles, unsigned int *nbEval)
{
	std::ofstream out("ils");
	bool *best = localsearch(ms, nbEval);
	float score = ms.evaluate(best);
	float scoreinit = score;
	out << "0 " << score << " 0\n";

	for(unsigned int i(0); i < cycles; ++i)
	{
		bool *s = localsearch(ms, nbEval);
		float tmp = ms.evaluate(s);

		if(tmp > score)
		{
			delete[] best;
			best = s;
			score = tmp;
			out << i+1 << " " << score << " " << *nbEval << "\n";
		}
		else
		{
			out << i+1 << " " << scoreinit << " " << *nbEval << "\n";
		}
	}

	out << cycles << " " << score << "\n";

	return best;
}

bool *ils(MSmemory& mm, unsigned int cycles, unsigned int *nbEval)
{
	std::ofstream out("ils");
	bool *best = localsearch(mm, nbEval);
	float score = mm.evaluate(best);
	float scoreinit = score;
	out << "0 " << score << " 0\n";

	for(unsigned int i(0); i < cycles; ++i)
	{
		bool *s = localsearch(mm, nbEval);
		float tmp = mm.evaluate(s);

		if(tmp > score)
		{
			delete[] best;
			best = s;
			score = tmp;
			out << i+1 << " " << score << " " << *nbEval << "\n";
		}
		else
		{
			out << i+1 << " " << scoreinit << " " << *nbEval << "\n";
		}
	}

	out << cycles << " " << score << "\n";

	return best;
}




