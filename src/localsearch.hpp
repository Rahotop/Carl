#ifndef LOCALSEARCH_INCLUDED_HPP
#define LOCALSEARCH_INCLUDED_HPP

#include <iostream>
#include <list>
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
bool* localsearch(PB& pb, bool *s, unsigned int *nbEval = nullptr)
{
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
		*nbEval += pb.getnbeval() - tmpnbeval -1;

	return s;
}

template <class PB>
bool* ils(std::ostream& out, PB& pb, unsigned int evalmax, unsigned int *nbEval = nullptr)
{
	bool *best = localsearch(pb, nbEval);
	float score = pb.evaluate(best);
	float scoreinit = score;
	out << "0 " << score << " 0\n";

	for(unsigned int i(0); *nbEval < evalmax; ++i)
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

	return best;
}

template <class PB>
bool* ils(std::ostream& out, PB& pb, unsigned int evalmax, unsigned int perturb, unsigned int *nbEval = nullptr)
{
	bool *s = localsearch(pb, nbEval);
	float score = pb.evaluate(s);
	out << "0 " << score << " 0\n";

	bool *best = new bool[pb.getN()];
	for(unsigned int i(0); i < pb.getN(); ++i) best[i] = s[i];

	for(unsigned int i(0); *nbEval < evalmax; ++i)
	{
		for(unsigned int j(0); j < perturb; ++j)
		{
			unsigned int tmp = rand()%pb.getN();
			s[tmp] = !s[tmp];
		}

		localsearch(pb, s, nbEval);
		float tmp = pb.evaluate(s);

		if(tmp > score)
		{
			for(unsigned int j(0); j < pb.getN(); ++j) best[j] = s[j];
			score = tmp;
		}
		out << i+1 << " " << tmp << " " << *nbEval << " " << score << "\n";
	}

	return best;
}

template <class PB>
bool* tabu(std::ostream& out, PB& pb, unsigned int evalmax, unsigned int size, unsigned int *nbEval = nullptr)
{
	bool *s = new bool[pb.getN()];
	bool *best = new bool[pb.getN()];
	float score = 0.;
	float scorebest = 0.;

	for(unsigned int i(0); i < pb.getN(); ++i)
	{
		s[i] = rand()%2;
		best[i] = s[i];
	}
	score = pb.evaluate(s);
	scorebest = score;


	std::list<unsigned int> tabou;
	std::vector<unsigned int> next;
	for(unsigned int i(0); i < pb.getN(); ++i) next.push_back(i);

	unsigned int tmpnbeval = 1;

	for(unsigned int i(0); tmpnbeval < evalmax; ++i)
	{
		if(tabou.size() > size)
		{
			next.push_back(tabou.front());
			tabou.pop_front();
		}
		std::random_shuffle(next.begin(), next.end());

		unsigned int tmp = 0;
		s[next[tmp]] = !s[next[tmp]];
		score = pb.evaluate(s);
		s[next[tmp]] = !s[next[tmp]];

		for(unsigned int j(1); j < next.size(); ++j)
		{
			s[next[j]] = !s[next[j]];
			float tmpscore = pb.evaluate(s);
			s[next[j]] = !s[next[j]];

			if(tmpscore > score)
			{
				score = tmpscore;
				tmp = j;
			}
		}
		tmpnbeval += next.size();


		s[next[tmp]] = !s[next[tmp]];
		tabou.push_back(next[tmp]);
		next[tmp] = next.back();
		next.pop_back();

		if(score > scorebest)
		{
			scorebest = score;
			for(unsigned int j(0); j < pb.getN(); ++j) best[j] = s[j];
		}

		out << i << " " << score << " " << scorebest << std::endl;
	}

	if(nbEval)
		*nbEval += tmpnbeval;

	delete[] s;
	return best;
}

#endif