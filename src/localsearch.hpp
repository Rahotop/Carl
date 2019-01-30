#ifndef LOCALSEARCH_INCLUDED_HPP
#define LOCALSEARCH_INCLUDED_HPP

#include <fstream>
#include <iostream>
#include <list>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "maxsat.hpp"
#include "msmemory.hpp"
#include "nk.hpp"
#include "onemax.hpp"

template<class PB>
void proportions(PB& pb, std::string path)
{
	std::ofstream out(path);

	bool *s = new bool[pb.getN()];
	for(unsigned int i(0); i < pb.getN(); ++i) s[i] = rand()%2;

	std::vector<unsigned int> proposed;
	std::vector<unsigned int> accepted;

	bool better = true;
	for(unsigned int it(0); better; ++it)
	{
		better = false;
		for(unsigned int i(0); i < pb.getN(); ++i)
		{
			bool *s2 = new bool[pb.getN()];
			for(unsigned int i(0); i < pb.getN(); ++i) s2[i] = s[i];
			unsigned int step = 0;
			unsigned int choice = (rand()%100)+1;
			/* 256-1-256-1
			step += (choice > 4);
			step += (choice > 15);
			step += (choice > 31);
			step += (choice > 48);
			step += (choice > 63);
			step += (choice > 75);
			step += (choice > 84);
			step += (choice > 90);
			step += (choice > 94);
			step += (choice > 97);
			step += (choice > 99);*/

			/* 256-1-256-12 
			step += (choice > 19);
			step += (choice > 29);
			step += (choice > 35);
			step += (choice > 40);
			step += (choice > 44);
			step += (choice > 48);
			step += (choice > 52);
			step += (choice > 56);
			step += (choice > 59);
			step += (choice > 62);
			step += (choice > 65);
			step += (choice > 68);
			step += (choice > 71);
			step += (choice > 74);
			step += (choice > 77);
			step += (choice > 79);
			step += (choice > 81);
			step += (choice > 83);
			step += (choice > 85);
			step += (choice > 87);
			step += (choice > 89);
			step += (choice > 91);
			step += (choice > 92);
			step += (choice > 93);
			step += (choice > 94);
			step += (choice > 95);
			step += (choice > 96);
			step += (choice > 97);
			step += (choice > 98);
			step += (choice > 99);*/

			/* 256-12-256-1 */
			step += (choice > 18);
			step += (choice > 44);
			step += (choice > 65);
			step += (choice > 80);
			step += (choice > 89);
			step += (choice > 94);
			step += (choice > 97);
			step += (choice > 99);

			/* 256-12-256-12
			step += (choice > 14);
			step += (choice > 22);
			step += (choice > 28);
			step += (choice > 32);
			step += (choice > 36);
			step += (choice > 40);
			step += (choice > 44);
			step += (choice > 48);
			step += (choice > 52);
			step += (choice > 55);
			step += (choice > 58);
			step += (choice > 61);
			step += (choice > 64);
			step += (choice > 67);
			step += (choice > 70);
			step += (choice > 73);
			step += (choice > 76);
			step += (choice > 78);
			step += (choice > 80);
			step += (choice > 82);
			step += (choice > 84);
			step += (choice > 86);
			step += (choice > 88);
			step += (choice > 90);
			step += (choice > 91);
			step += (choice > 92);
			step += (choice > 93);
			step += (choice > 94);
			step += (choice > 95);
			step += (choice > 96);
			step += (choice > 97);
			step += (choice > 98);
			step += (choice > 99);*/

			unsigned int flip[64];
			for(unsigned int i(0); i < step; ++i)
			{
				flip[i] = rand()%pb.getN();
				bool different = true;
				do{
					different = true;
					for(unsigned int j(0); j < i; ++j)
					{
						if(flip[i] == flip[j])
						{
							different = false;
							break;
						}
					}
					if(!different)
					{
						flip[i] = rand()%pb.getN();
					}
				}while(!different);
			}
			for(unsigned int i(0); i < step; ++i)
			{
				s2[flip[i]] = !s2[flip[i]];
			}

			while(proposed.size() <= step)
			{
				proposed.push_back(0);
				accepted.push_back(0);
			}
			++proposed[step];

			if(pb.evaluate(s) < pb.evaluate(s2))
			{
				++accepted[step];
				delete[] s;
				s = s2;
				better = true;
				break;
			}
			else
			{
				delete[] s2;
			}
		}
	}

	out << pb.evaluate(s) << std::endl << std::endl;
	delete[] s;
	unsigned int sum = 0;
	for(unsigned int i(0); i < accepted.size(); ++i)
		sum += accepted[i];
	for(unsigned int i(0); i < accepted.size(); ++i)
		out << (float)accepted[i]/(float)sum << " ";
	out << std::endl;
	sum = 0;
	for(unsigned int i(0); i < proposed.size(); ++i)
		sum += proposed[i];
	for(unsigned int i(0); i < proposed.size(); ++i)
		out << (float)proposed[i]/(float)sum << " ";
	out << std::endl;
}

template<class PB>
void bitflip(PB& pb, std::string path)
{
	std::ofstream out(path);

	bool *s = new bool[pb.getN()];
	for(unsigned int i(0); i < pb.getN(); ++i) s[i] = rand()%2;

	std::vector<unsigned int> proposed;
	std::vector<unsigned int> accepted;

	bool better = true;
	for(unsigned int it(0); better; ++it)
	{
		better = false;
		for(unsigned int i(0); i < pb.getN(); ++i)
		{
			bool *s2 = new bool[pb.getN()];
			unsigned int step = 0;
			for(unsigned int j(0); j < pb.getN(); ++j)
			{
				unsigned int tmp = rand()%pb.getN();
				s2[j] = (tmp) ? s[j] : !s[j];
				step += !tmp;
			}

			while(proposed.size() <= step)
			{
				proposed.push_back(0);
				accepted.push_back(0);
			}
			++proposed[step];

			if(pb.evaluate(s) < pb.evaluate(s2))
			{
				++accepted[step];
				delete[] s;
				s = s2;
				better = true;
				break;
			}
			else
			{
				delete[] s2;
			}
		}
	}

	out << pb.evaluate(s) << std::endl << std::endl;
	delete[] s;
	unsigned int sum = 0;
	for(unsigned int i(0); i < accepted.size(); ++i)
		sum += accepted[i];
	for(unsigned int i(0); i < accepted.size(); ++i)
		out << (float)accepted[i]/(float)sum << " ";
	out << std::endl;
	sum = 0;
	for(unsigned int i(0); i < proposed.size(); ++i)
		sum += proposed[i];
	for(unsigned int i(0); i < proposed.size(); ++i)
		out << (float)proposed[i]/(float)sum << " ";
	out << std::endl;
}

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

		out << i << " " << score << " " << tmpnbeval << " " << scorebest << std::endl;
	}

	if(nbEval)
		*nbEval += tmpnbeval;

	delete[] s;
	return best;
}

#endif