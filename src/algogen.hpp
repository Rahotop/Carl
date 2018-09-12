#ifndef ALGOGEN_HPP_INCLUDED
#define ALGOGEN_HPP_INCLUDED

#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include "localsearch.hpp"
#include "maxsat.hpp"
#include "fnarray.hpp"
#include "msmemory.hpp"

unsigned int distance(bool *s1, bool *s2, unsigned int n);

class AlgoGen
{
	public:

	AlgoGen(unsigned int popsize, unsigned int maxsize, unsigned int width);
	~AlgoGen();

	private:

	void initPop(unsigned int newSize);
	void newPop();
	
	void crossover(unsigned int p1, unsigned int p2, unsigned int next);

	void addRandom(unsigned int index);
	void addRandom(unsigned int index, unsigned int next);
	void add(unsigned int index, unsigned int tree, unsigned int next);

	void deleteRandom(unsigned int index, unsigned int next);
	void deleteTree(unsigned int index, unsigned int next, unsigned int tree);

	void copy(unsigned int index, unsigned int next);
	void copy(unsigned int index, unsigned int next, unsigned int tree);

	float evaluate(unsigned int index, bool *s);
	float evaluateinc(unsigned int index, bool *s, unsigned int changed);
	void acceptCurr(unsigned int ind);
	bool* localsearchind(unsigned int index, unsigned int *nbEval = nullptr);
	
	bool* getActivation(unsigned int index) const;

	void show(unsigned int index, std::ostream& out);

	// PARAM
	std::vector<unsigned int> m_fnset;
	unsigned int m_n;

	// SIZE
	unsigned int m_popsize; // number of ind
	unsigned int m_maxsize; // number max of subtrees
	unsigned int m_width; // size of subtrees

	// POPULATION
	unsigned int *m_sort;
	unsigned int *m_size; // m_size[i] is the number of subtrees in the i-th ind 
	unsigned int *m_trees; // m_trees[i*maxsize*width+j*width+k] is the k-th element of the j-th subtree of i-th ind
	bool *m_not; // m_not[i*maxsize*width+j*width+k] is the sign of the k-th element of the j-th subtree of i-th ind
	bool *m_prec;
	bool *m_curr;
	bool *m_in;
	float *m_weights; // m_weights[i*maxsize+j] is the weight of the j-th subtree of the i-th ind
	float *m_fitness; // m_fitness[i] is the fitness of the i-th ind

	// NEXT GENERATION
	unsigned int *m_Gsize;
	unsigned int *m_Gtrees;
	bool *m_Gnot;
	bool *m_Gin;
	float *m_Gweights;


	public:

	template<class PB>
	void run(PB& pb, unsigned int newSize, unsigned int nbIt, const std::string& file)
	{
		// PARAM
		m_n = pb.getN();
		m_in = new bool[m_popsize*m_maxsize*m_n];
		m_Gin = new bool[m_popsize*m_maxsize*m_n];


		// OUTPUT
		std::ofstream data(file);
		std::ofstream res(file+".txt");
		unsigned int nbEval = 0;


		// INIT POP
		initPop(newSize);
		nbEval = evalPop(pb, 10);


		// AlgoGen
		for(unsigned int it(0); it < nbIt; ++it)
		{
			// NEW POP
			newPop();


			// EVAL
			unsigned int meanNbEval = evalPop(pb, 10);
			nbEval += meanNbEval;
			meanNbEval /= (m_popsize*10);


			// SAVE
			data << it << " " << m_fitness[m_sort[m_popsize-1]] << " " << m_size[m_sort[m_popsize-1]] << " " << meanNbEval << "\n";
		}

		// RES
		res << "nb eval (solutions) : " << nbEval << "\nnb eval (f obj) : " << pb.getnbeval() << "\n\n#ILS :\n#Fitness max : ";


		// ILS
		unsigned int nbeval = 0;
		data << "\n";
		bool *bestils = ils(data, pb, 5000, &nbeval);

		data << "\n0 " << pb.evaluate(bestils) << "\n" << nbIt << " " << pb.evaluate(bestils) << "\n";

		res << pb.evaluate(bestils) << "\n#nb eval : " << nbeval;


		// CORRELATION
		correlation(pb, data, bestils);

		// FREQ
		freq(pb, res, bestils);

		// BEST
		res << "\n\nFitness max : " << m_fitness[m_sort[m_popsize-1]] << "\n";
		show(m_sort[m_popsize-1], res);

		delete[] bestils;
	}

	private:

	template<class PB>
	unsigned int evalPop(PB& pb, unsigned int prec)
	{
		unsigned int nbEval = 0;
		//#pragma omp parallel for
		for(unsigned int i = 0; i < m_popsize; ++i)
		{/*
			bool **act = new bool*[prec];
			float *score = new float[prec];
			float *scoreobj = new float[prec];

			for(unsigned int j(0); j < prec; ++j)
			{
				bool *s = localsearchind(i, &nbEval);
				score[j] = evaluate(i, s);
				act[j] = getActivation(i);
				scoreobj[j] = pb.evaluate(s);
				delete[] s;
			}


			float *adj = new float[m_size[i]];
			for(unsigned int j(0); j < m_size[i]; ++j) adj[j] = 0.;
			unsigned int count = 0;

			for(unsigned int j(0); j < prec; ++j)
			{
				for(unsigned int k(j+1); k < prec; ++k)
				{
					++count;
					float delta = score[j]/score[k] - scoreobj[j]/scoreobj[k];
					delta /= 2;

					unsigned int jnk = 0;
					unsigned int njk = 0;
					for(unsigned int l(0); l < m_size[i]; ++l)
					{
						jnk += (act[j][l] && !act[k][l]);
						njk += (!act[j][l] && act[k][l]);
					}
					if(jnk) for(unsigned int l(0); l < m_size[i]; ++l) adj[l] += delta/jnk * score[j] * (act[j][l] && !act[k][l]);
					if(njk) for(unsigned int l(0); l < m_size[i]; ++l) adj[l] -= delta/njk * score[j] * (!act[j][l] && act[k][l]);
				}
			}

			for(unsigned int j(0); j < m_size[i]; ++j) m_weights[i*m_maxsize + j] += adj[j]/count;
			for(unsigned int j(0); j < m_size[i]; ++j) m_weights[i*m_maxsize + j] = (m_weights[i*m_maxsize + j] > 1) ? 1 : m_weights[i*m_maxsize + j];
			for(unsigned int j(0); j < m_size[i]; ++j) m_weights[i*m_maxsize + j] = (m_weights[i*m_maxsize + j] < -1) ? -1 : m_weights[i*m_maxsize + j];

			delete[] adj;


			delete[] scoreobj;
			delete[] score;
			delete[] act;*/

			float tmp = 0;
			for(unsigned int j(0); j < prec; ++j)
			{
				bool *s = localsearchind(i, &nbEval);
				tmp += pb.evaluate(s);
				delete s;
			}
			m_fitness[i] = tmp/prec;
		}

		// SORT
		bool tmp = true;
		while(tmp)
		{
			tmp = false;
			for(unsigned int i(1); i < m_popsize; ++i)
			{
				if(m_fitness[m_sort[i-1]] > m_fitness[m_sort[i]])
				{
					std::swap(m_sort[i-1], m_sort[i]);
					tmp = true;
				}
			}
		}

		return nbEval;
	}

	template<class PB>
	void correlation(PB& pb, std::ostream& data, bool* bestils)
	{
		data << "\n";
		for(unsigned int i(0); i < 1000; ++i) //rand
		{
			bool *s = new bool[m_n];
			for(unsigned int j(0); j < m_n; ++j) s[j] = rand()%2;
			data << evaluate(m_sort[m_popsize-1], s) << " " << pb.evaluate(s) << "\n";
			delete[] s;
		}
		data << "\n";
		for(unsigned int i(0); i < 1000; ++i) //ls f obj
		{
			bool *s = localsearch(pb);
			data << evaluate(m_sort[m_popsize-1], s) << " " << pb.evaluate(s) << "\n";
		}
		data << "\n";
		for(unsigned int i(0); i < 1000; ++i) //ls f ind
		{
			bool *s = localsearchind(m_sort[m_popsize-1]);
			data << evaluate(m_sort[m_popsize-1], s) << " " << pb.evaluate(s) << "\n";
		}
		data << "\n";

		data << evaluate(m_sort[m_popsize-1], bestils) << " " << pb.evaluate(bestils) << "\n\n"; //opt glb
	}

	template<class PB>
	void freq(PB& pb, std::ostream& res, bool* bestils)
	{
		std::vector<bool*> lsobj;
		std::vector<bool*> lsind;
		std::vector<unsigned int> fobj;
		std::vector<unsigned int> find;

		for(unsigned int i(0); i < 100; ++i)
		{
			bool *s = localsearch(pb);
			bool tmp = true;
			for(unsigned int j(0); j < lsobj.size(); ++j)
			{
				if(!distance(s,lsobj[j],m_n))
				{
					delete[] s;
					++fobj[j];
					tmp = false;
					break;
				}
			}
			if(tmp)
			{
				lsobj.push_back(s);
				fobj.push_back(1);
			}
		}
		for(unsigned int i(0); i < 100; ++i)
		{
			bool *s = localsearchind(m_sort[m_popsize-1]);
			bool tmp = true;
			for(unsigned int j(0); j < lsind.size(); ++j)
			{
				if(!distance(s,lsind[j],m_n))
				{
					delete[] s;
					++find[j];
					tmp = false;
					break;
				}
			}
			if(tmp)
			{
				lsind.push_back(s);
				find.push_back(1);
			}
		}

		// SORT
		unsigned int *sortobj = new unsigned int[lsobj.size()];
		for(unsigned int i(0); i < lsobj.size(); ++i) sortobj[i] = i;
		unsigned int *sortind = new unsigned int[lsind.size()];
		for(unsigned int i(0); i < lsind.size(); ++i) sortind[i] = i;

		bool tmp = true;
		while(tmp)
		{
			tmp = false;
			for(unsigned int i(1); i < lsobj.size(); ++i)
			{
				if(pb.evaluate(lsobj[sortobj[i-1]]) < pb.evaluate(lsobj[sortobj[i]]))
				{
					std::swap(sortobj[i-1], sortobj[i]);
					tmp = true;
				}
			}
		}
		tmp = true;
		while(tmp)
		{
			tmp = false;
			for(unsigned int i(1); i < lsind.size(); ++i)
			{
				if(pb.evaluate(lsind[sortind[i-1]]) < pb.evaluate(lsind[sortind[i]]) || (pb.evaluate(lsind[sortind[i-1]]) == pb.evaluate(lsind[sortind[i]]) && find[sortind[i-1]] < find[sortind[i]]))
				{
					std::swap(sortind[i-1], sortind[i]);
					tmp = true;
				}
			}
		}

		// FREQ RES
		res << "\n\nFreq :\n";
		res << std::setw(21) << "Fonction objectif" << " " << std::setw(55) << "Fonction trouvée" << "\n\n";
		res << std::setw(21) << "Fitness moyenne :" << " " << std::setw(54) << "Fitness moyenne :" << "\n";
		float moyennescore = 0;
		for(unsigned int i(0); i < lsobj.size(); ++i) moyennescore += pb.evaluate(lsobj[i]);
		res << std::setw(21) << moyennescore/lsobj.size() << " ";
		moyennescore = 0;
		for(unsigned int i(0); i < lsind.size(); ++i) moyennescore += pb.evaluate(lsind[i]);
		res << std::setw(54) << moyennescore/lsind.size() << "\n\n";
		res << std::setw(10) << "Fitness" << " " << std::setw(10) << "Frequence" << " " << std::setw(10) << "Fitness" << " " << std::setw(10) << "Frequence" << " ";
		res << std::setw(10) << "opt loc" << " " << std::setw(10) << "Dist ils" << " " << std::setw(10) << "Dist best" << "\n";


		for(unsigned int i(0); i < lsobj.size(); ++i)
		{
			res << std::setw(10) << pb.evaluate(lsobj[sortobj[i]]) << " " << std::setw(10)  << fobj[sortobj[i]] << " ";
			if(i < lsind.size())
			{
				res << std::setw(10)  << pb.evaluate(lsind[sortind[i]]) << " " << std::setw(10)  << find[sortind[i]] << " " << std::setw(10) << pb.islocopt(lsind[sortind[i]]);
				res << " " << std::setw(10) << distance(bestils,lsind[sortind[i]],m_n) << " " << std::setw(10) << distance(lsind[sortind[0]],lsind[sortind[i]],m_n);
			}
			res << "\n";
		}
	}

};

#endif