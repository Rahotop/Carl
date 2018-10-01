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

bool* localsearch(NK& nk, unsigned int *nbEval)
{
	bool *s = new bool[nk.getN()];
	for(unsigned int i(0); i < nk.getN(); ++i) s[i] = rand()%2;
	float score = nk.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < nk.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = nk.getnbeval();
	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < nk.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = nk.evaluate(s);

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
		*nbEval += nk.getnbeval() - tmpnbeval;

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

bool* localsearch(OneMax& om, unsigned int *nbEval)
{
	bool *s = new bool[om.getN()];
	for(unsigned int i(0); i < om.getN(); ++i) s[i] = rand()%2;
	float score = om.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < om.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = om.getnbeval();
	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < om.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = om.evaluate(s);

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
		*nbEval += om.getnbeval() - tmpnbeval;

	return s;
}

bool *ils(std::ostream& out, MaxSat& ms, unsigned int cycles, unsigned int *nbEval)
{
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
			delete[] s;
			out << i+1 << " " << scoreinit << " " << *nbEval << "\n";
		}
	}

	out << cycles << " " << score << "\n";

	return best;
}

bool *ils(std::ostream& out, NK& nk, unsigned int cycles, unsigned int *nbEval)
{
	bool *best = localsearch(nk, nbEval);
	float score = nk.evaluate(best);
	float scoreinit = score;
	out << "0 " << score << " 0\n";

	for(unsigned int i(0); i < cycles; ++i)
	{
		bool *s = localsearch(nk, nbEval);
		float tmp = nk.evaluate(s);

		if(tmp > score)
		{
			delete[] best;
			best = s;
			score = tmp;
			out << i+1 << " " << score << " " << *nbEval << "\n";
		}
		else
		{
			delete[] s;
			out << i+1 << " " << scoreinit << " " << *nbEval << "\n";
		}
	}

	out << cycles << " " << score << "\n";

	return best;
}

bool *ils(std::ostream& out, MSmemory& mm, unsigned int cycles, unsigned int *nbEval)
{
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
			delete[] s;
			out << i+1 << " " << scoreinit << " " << *nbEval << "\n";
		}
	}

	out << cycles << " " << score << "\n";

	return best;
}

bool *ils(std::ostream& out, OneMax& om, unsigned int cycles, unsigned int *nbEval)
{
	bool *best = localsearch(om, nbEval);
	float score = om.evaluate(best);
	float scoreinit = score;
	out << "0 " << score << " 0\n";

	for(unsigned int i(0); i < cycles; ++i)
	{
		bool *s = localsearch(om, nbEval);
		float tmp = om.evaluate(s);

		if(tmp > score)
		{
			delete[] best;
			best = s;
			score = tmp;
			out << i+1 << " " << score << " " << *nbEval << "\n";
		}
		else
		{
			delete[] s;
			out << i+1 << " " << scoreinit << " " << *nbEval << "\n";
		}
	}

	out << cycles << " " << score << "\n";

	return best;
}




