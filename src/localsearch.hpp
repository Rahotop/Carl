#ifndef LOCALSEARCH_INCLUDED_HPP
#define LOCALSEARCH_INCLUDED_HPP

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "maxsat.hpp"
#include "msmemory.hpp"
#include "nk.hpp"
#include "onemax.hpp"

template <class PB>
bool* localsearch(PB& pb, unsigned int *nbEval = nullptr)
{
	bool *s = new bool[pb.getN()];
	for(unsigned int i(0); i < pb.getN(); ++i) s[i] = rand()%2;
	float score = pb.evaluate(s);

	std::vector<unsigned int> next;
	for(unsigned int i(0); i < pb.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = pb.getnbeval();
	bool improved = true;

	while(improved)
	{
		std::random_shuffle(next.begin(), next.end());
		improved = false;

		for(unsigned int i(0); i < pb.getN(); ++i)
		{
			s[next[i]] = !s[next[i]];
			float tmp = pb.evaluate(s);

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
		*nbEval += pb.getnbeval() - tmpnbeval;

	return s;
}

template <class PB>
bool* ils(std::ostream& out, PB& pb, unsigned int cycles, unsigned int *nbEval = nullptr)
{
	bool *best = localsearch(pb, nbEval);
	float score = pb.evaluate(best);
	float scoreinit = score;
	out << "0 " << score << " 0\n";

	for(unsigned int i(0); i < cycles; ++i)
	{
		bool *s = localsearch(pb, nbEval);
		float tmp = pb.evaluate(s);

		if(tmp > score)
		{
			delete[] best;
			best = s;
			score = tmp;
			out << i+1 << " " << score << " " << *nbEval << " " << score << "\n";
		}
		else
		{
			delete[] s;
			out << i+1 << " " << scoreinit << " " << *nbEval << " " << score << "\n";
		}
	}

	out << cycles << " " << score << " " << *nbEval << " " << score << "\n";

	return best;
}

#endif