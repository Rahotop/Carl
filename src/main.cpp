#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "maxsat.hpp"
#include "localsearch.hpp"
#include "algogen.hpp"
#include "msmemory.hpp"
#include "nk.hpp"
#include "hyperheuristic.hpp"
#include "onemax.hpp"
#include "royalroad.hpp"

void ressemblance(unsigned int n, unsigned int size, unsigned int step, unsigned int sol, unsigned int func, unsigned int distance);
void ressemblance(unsigned int n, unsigned int size, unsigned int step, unsigned int sol, unsigned int func);
void ressemblancenk(unsigned int n, unsigned int k, unsigned int step, unsigned int sol, unsigned int func, unsigned int distance);
void ressemblancenk(unsigned int n, unsigned int k, unsigned int step, unsigned int sol, unsigned int func);

int main(int argc, char **argv)
{
	INIT tabinit[] = {&HyperHeuritic::initrand, &HyperHeuritic::initall};
	unsigned int init = 0;

	bool hh = false;
	bool ag = false;
	bool sls = false;
	bool ilsp = false;
	bool tabou = false;
	bool ms = false;
	bool nk = false;
	bool om = false;
	bool rr = false;
	unsigned int omn = 64;
	unsigned int rrk = 2;
	std::string path;
	unsigned int nbind = 100;
	unsigned int size = 100;
	unsigned int width = 15;
	unsigned int newsize = 5;
	unsigned int iteration = 1000;
	unsigned int cycle = 1000;
	std::string out = "data";
	bool output = true;
	unsigned int pc = 15;
	unsigned int pm = 15;
	unsigned int id = 0;
	unsigned int repeat = 1;
	unsigned int seed = time(nullptr);
	std::vector<unsigned int> fnset = {1,7,9,11,13};
	//std::vector<unsigned int> fnset = {1,9};
	std::vector<bool*> nptsol;

	for(int i(1); i < argc; i+= 2)
	{
		if("-ms" == std::string(argv[i]))
		{
			ms = true;
			path = argv[i+1];
		}
		else if("-nk" == std::string(argv[i]))
		{
			nk = true;
			path = argv[i+1];
		}
		else if("-om" == std::string(argv[i]))
		{
			om = true;
			omn = std::stoi(argv[i+1]);
		}
		else if("-rr" == std::string(argv[i]))
		{
			rr = true;
			omn = std::stoi(argv[i+1]);
			rrk = std::stoi(argv[i+2]);
			++i;
		}
		else if("-hh" == std::string(argv[i]))
		{
			hh = true;
			cycle = std::stoi(argv[i+1]);
		}
		else if("-ag" == std::string(argv[i]))
		{
			ag = true;
			--i;
		}
		else if("-ils" == std::string(argv[i]))
		{
			sls = true;
			--i;
		}
		else if("-ilsp" == std::string(argv[i]))
		{
			ilsp = true;
			cycle = std::stoi(argv[i+1]);
		}
		else if("-tabu" == std::string(argv[i]))
		{
			tabou = true;
			cycle = std::stoi(argv[i+1]);
		}
		else if("-nbind" == std::string(argv[i]))
			nbind = std::stoi(argv[i+1]);
		else if("-size" == std::string(argv[i]))
			size = std::stoi(argv[i+1]);
		else if("-width" == std::string(argv[i]))
			width = std::stoi(argv[i+1]);
		else if("-newsize" == std::string(argv[i]))
			newsize = std::stoi(argv[i+1]);
		else if("-it" == std::string(argv[i]))
			iteration = std::stoi(argv[i+1]);
		else if("-out" == std::string(argv[i]))
			out = argv[i+1];
		else if("-pc" == std::string(argv[i]))
			pc = std::stoi(argv[i+1]);
		else if("-pm" == std::string(argv[i]))
			pm = std::stoi(argv[i+1]);
		else if("-id" == std::string(argv[i]))
			id = std::stoi(argv[i+1]);
		else if("-repeat" == std::string(argv[i]))
			repeat = std::stoi(argv[i+1]);
		else if("-seed" == std::string(argv[i]))
			seed = std::stoi(argv[i+1]);
		else if("-initrand" == std::string(argv[i]))
		{
			init = 0;
			--i;
		}
		else if("-initall" == std::string(argv[i]))
		{
			init = 1;
			--i;
		}
		else if("-fnset" == std::string(argv[i]))
		{
			fnset.clear();
			unsigned int setsize = std::stoi(argv[i+1]);
			for(unsigned int j(0); j < setsize; ++j)
				fnset.push_back(std::stoi(argv[i+2+j]));
		}
		else if("-nooutput" == std::string(argv[i]) ||
				"-noout" == std::string(argv[i]) ||
				"-nout" == std::string(argv[i]) ||
				"-npt" == std::string(argv[i]))
		{
			output = false;
			--i;
		}
	}

	srand(seed);

	for(unsigned int rp(0); rp < repeat; ++rp)
	{
		//Algo Gen
		if(ms && ag)
		{
			MaxSat inst(path);
			AlgoGen algo(nbind,size,width);
			algo.run(inst, newsize, iteration, out+"-"+std::to_string(id+rp), pc, pm);
		}
		
		else if(nk && ag)
		{
			NK inst(path);
			AlgoGen algo(nbind,size,width);
			algo.run(inst, newsize, iteration, out+"-"+std::to_string(id+rp), pc, pm);
		}
		
		else if(om && ag)
		{
			OneMax inst(omn);
			AlgoGen algo(nbind,size,width);
			algo.run(inst, newsize, iteration, out+"-"+std::to_string(id+rp), pc, pm);
		}
		
		else if(rr && ag)
		{
			RoyalRoad inst(omn, rrk);
			AlgoGen algo(nbind,size,width);
			algo.run(inst, newsize, iteration, out+"-"+std::to_string(id+rp), pc, pm);
		}

		//Hyper Heuristic
		else if(ms && hh)
		{
			MaxSat inst(path);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], newsize, iteration, cycle, out+"-"+std::to_string(id+rp));
		}
		
		else if(nk && hh)
		{
			NK inst(path);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], newsize, iteration, cycle, out+"-"+std::to_string(id+rp));
		}
		
		else if(om && hh)
		{
			OneMax inst(omn);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], newsize, iteration, cycle, out+"-"+std::to_string(id+rp));
		}
		
		else if(rr && hh)
		{
			RoyalRoad inst(omn, rrk);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], newsize, iteration, cycle, out+"-"+std::to_string(id+rp));
		}

		//ILS
		else if(sls && nk)
		{
			NK inst(path);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] ils(ilsout, inst, iteration, &nbeval);
			else
				nptsol.push_back(ils(ilsout, inst, iteration, &nbeval));
		}

		else if(sls && ms)
		{
			MaxSat inst(path);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] ils(ilsout, inst, iteration, &nbeval);
			else
				nptsol.push_back(ils(ilsout, inst, iteration, &nbeval));
		}

		else if(sls && rr)
		{
			RoyalRoad inst(omn, rrk);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] ils(ilsout, inst, iteration, &nbeval);
			else
				nptsol.push_back(ils(ilsout, inst, iteration, &nbeval));
		}

		//ILSP
		else if(ilsp && nk)
		{
			NK inst(path);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] ils(ilsout, inst, iteration, cycle, &nbeval);
			else
				nptsol.push_back(ils(ilsout, inst, iteration, cycle, &nbeval));
		}

		else if(ilsp && ms)
		{
			MaxSat inst(path);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] ils(ilsout, inst, iteration, cycle, &nbeval);
			else
				nptsol.push_back(ils(ilsout, inst, iteration, cycle, &nbeval));
		}

		else if(ilsp && rr)
		{
			RoyalRoad inst(omn, rrk);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] ils(ilsout, inst, iteration, cycle, &nbeval);
			else
				nptsol.push_back(ils(ilsout, inst, iteration, cycle, &nbeval));
		}

		//TABU
		else if(tabou && nk)
		{
			NK inst(path);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] tabu(ilsout, inst, iteration, cycle, &nbeval);
			else
				nptsol.push_back(tabu(ilsout, inst, iteration, cycle, &nbeval));
		}

		else if(tabou && ms)
		{
			MaxSat inst(path);
			std::ofstream ilsout(out+"-"+std::to_string((id+rp)*output));
			unsigned int nbeval = 0;
			if(output)
				delete[] tabu(ilsout, inst, iteration, cycle, &nbeval);
			else
				nptsol.push_back(tabu(ilsout, inst, iteration, cycle, &nbeval));
		}


		else
		{
			std::cout << "Combinaison inconnue" << std::endl;
		}
	}

	if(!output && nk)
	{
		std::ofstream ilsout(out+"-0.txt");
		NK inst(path);
		double sum = 0.;
		for(unsigned int i(0); i < nptsol.size(); ++i)
		{
			for(unsigned int j(0); j < inst.getN(); ++j)
				ilsout << nptsol[i][j];
			float tmp = inst.evaluate(nptsol[i]);
			sum += tmp;
			ilsout << std::endl << tmp << std::endl;
			delete[] nptsol[i];
		}
		ilsout << std::endl << std::setprecision(9) << sum/(double)repeat;
	}

	if(!output && ms)
	{
		std::ofstream ilsout(out+"-0.txt");
		MaxSat inst(path);
		double sum = 0.;
		for(unsigned int i(0); i < nptsol.size(); ++i)
		{
			for(unsigned int j(0); j < inst.getN(); ++j)
				ilsout << nptsol[i][j];
			float tmp = inst.evaluate(nptsol[i]);
			sum += tmp;
			ilsout << std::endl << tmp << std::endl;
			delete[] nptsol[i];
		}
		ilsout << std::endl << std::setprecision(9) << sum/(double)repeat;
	}

	//ressemblance(256,150,1,60,20,1);
	//ressemblancenk(256,100,1,120,40,1);
/*
	NK inst("inst/nk_256_1.0");
	std::vector<float> scores;
	float min = 1;
	float max = 0;
	for(unsigned int i(0); i < 100; ++i)
	{
		bool *s = localsearch(inst);
		scores.push_back(inst.evaluate(s));
		delete[] s;

		min = (min < scores.back()) ? min : scores.back();
		max = (max > scores.back()) ? max : scores.back();
	}
	float diff = (max-min)/5.;
	unsigned int freq[5];
	for(unsigned int i(0); i < 5; ++i)
	{
		freq[i] = 0;
		for(unsigned int j(0); j < 100; ++j)
		{
			freq[i] += ((scores[j] >= (min+diff*i)) && (scores[j] <= (min+diff*(i+1))));
		}
	}
	std::cout << min << " 0" << std::endl;
	for(unsigned int i(0); i < 5; ++i)
	{
		std::cout << min+diff/2+diff*i << " " << freq[i]/100. << std::endl;
	}
	std::cout << max << " 0" << std::endl << std::endl;
	for(unsigned int i(0); i < 100; ++i)
	{
		std::cout << scores[i] << std::endl;
	}
	std::cout << std::endl;
	for(unsigned int i(0); i < 6; ++i)
	{
		std::cout << min+diff*i << " 0.75" << std::endl;
		std::cout << min+diff*i << " 1.25" << std::endl;
		std::cout << min+diff*i << " 0.75" << std::endl;
	}
	for(unsigned int i(0); i < 5; ++i)
	{
		std::cout << max << " " << 0.75+i*0.125 << std::endl;
		std::cout << min << " " << 0.75+i*0.125 << std::endl;
		std::cout << max << " " << 0.75+i*0.125 << std::endl;
	}
	std::cout << std::endl;
	for(unsigned int i(0); i < 6; ++i)
	{
		std::cout << min+i*diff << " 0.73 " << std::setprecision(3) << min+i*diff << std::endl;
	}*/

/*
	NK inst("inst/nk_256_1.0");
	for(unsigned int i(0); i < 100; ++i)
	{
		bitflip(inst, "bitflip-"+std::to_string(i)+".txt");
	}*/
/*
	NK inst("inst/nk_256_1.0");
	for(unsigned int i(0); i < 100; ++i)
	{
		proportions(inst, "proportions-"+std::to_string(i)+".txt");
	}
*/
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////   Ressemblance   ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////


void ressemblance(unsigned int n, unsigned int size, unsigned int step, unsigned int sol, unsigned int func, unsigned int distance)
{
	std::vector<unsigned int> fnset = {1,7,9,11,13};
	double **results = new double*[size];
	for(unsigned int i(0); i < size; ++i)
	{
		results[i] = new double[size];
		for(unsigned int j(0); j < size; ++j)
		{
			results[i][j] = 0.;
		}
	}


	//#pragma omp parallel for collapse(2)
	for(unsigned int testsize = 1; testsize <= size; testsize+=step)
	{
		std::cout << testsize << "    \r" << std::flush;
		for(unsigned int testwidth = 1; testwidth <= size; testwidth+=step)
		{
			std::vector<bool*> setsol;
			for(unsigned int i(0); i < sol; ++i)
			{
				bool *s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = rand()%2;
				}
				setsol.push_back(s);
				s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = setsol.back()[j];
				}
				for(unsigned int j(0); j < distance; ++j)
				{
					unsigned int tmp = rand()%n;
					s[tmp] = !s[tmp];
				}
				setsol.push_back(s);
			}


			unsigned long long mean = 0;
			for(unsigned int i(0); i < func; ++i)
			{
				FnArray fn1(testsize, testwidth, n);
				for(unsigned int j(0); j < testsize; ++j)
				{
					fn1.addRandom(fnset);
				}
				FnArray fn2(fn1);
				fn2.mutate(fnset);

				for(unsigned int j(0); j < sol*2; j+=2)
				{
					mean += (fn1.evaluate(setsol[j]) < fn1.evaluate(setsol[j+1])) == (fn2.evaluate(setsol[j]) < fn2.evaluate(setsol[j+1]));
				}
			}
			results[testsize-1][testwidth-1] = (double)mean/(double)(sol*func);


			for(unsigned int i(0); i < setsol.size(); ++i)
			{
				delete[] setsol[i];
			}
		}
	}


	std::ofstream datatest("data-0");

	for(unsigned int i(0); i < size; i+=step)
	{
		for(unsigned int j(0); j < size; j+=step)
		{
			datatest << i << " " << j << " " << results[i][j] << std::endl;
		}
		datatest << std::endl;
	}

	std::ofstream datatest2("data-1");

	for(unsigned int i(0); i < size; i+=step)
	{
		for(unsigned int j(0); j < size; j+=step)
		{
			double sum = results[i][j];
			double summean = 1;
			if(i > step-1)
			{
				sum += results[i-step][j];
				++summean;
			}
			if(j > step-1)
			{
				sum += results[i][j-step];
				++summean;
			}
			if(i > step-1 && j > step-1)
			{
				sum += results[i-step][j-step];
				++summean;
			}

			if(i < size-step)
			{
				sum += results[i+step][j];
				++summean;
			}
			if(j < size-step)
			{
				sum += results[i][j+step];
				++summean;
			}
			if(i < size-step && j != size-step)
			{
				sum += results[i+step][j+step];
				++summean;
			}

			if(i > step-1 && j < size-step)
			{
				sum += results[i-step][j+step];
				++summean;
			}
			if(i < size-step && j > step-1)
			{
				sum += results[i+step][j-step];
				++summean;
			}

			datatest2 << i << " " << j << " " << sum/summean << std::endl;
		}
		datatest2 << std::endl;
	}

	for(unsigned int i(0); i < size; ++i)
	{
		delete[] results[i];
	}
	delete[] results;
}


void ressemblance(unsigned int n, unsigned int size, unsigned int step, unsigned int sol, unsigned int func)
{
	std::vector<unsigned int> fnset = {1,7,9,11,13};
	double **results = new double*[size];
	for(unsigned int i(0); i < size; ++i)
	{
		results[i] = new double[size];
		for(unsigned int j(0); j < size; ++j)
		{
			results[i][j] = 0.;
		}
	}


	#pragma omp parallel for collapse(2)
	for(unsigned int testsize = 1; testsize <= size; testsize+=step)
	{
		for(unsigned int testwidth = 1; testwidth <= size; testwidth+=step)
		{
			std::vector<bool*> setsol;
			for(unsigned int i(0); i < sol; ++i)
			{
				bool *s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = rand()%2;
				}
				setsol.push_back(s);
				s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = rand()%2;
				}
				setsol.push_back(s);
			}


			unsigned long long mean = 0;
			for(unsigned int i(0); i < func; ++i)
			{
				FnArray fn1(testsize, testwidth, n);
				for(unsigned int j(0); j < testsize; ++j)
				{
					fn1.addRandom(fnset);
				}
				FnArray fn2(fn1);
				fn2.mutate(fnset);

				for(unsigned int j(0); j < sol*2; j+=2)
				{
					mean += (fn1.evaluate(setsol[j]) < fn1.evaluate(setsol[j+1])) == (fn2.evaluate(setsol[j]) < fn2.evaluate(setsol[j+1]));
				}
			}
			results[testsize-1][testwidth-1] = (double)mean/(double)(sol*func);


			for(unsigned int i(0); i < setsol.size(); ++i)
			{
				delete[] setsol[i];
			}
		}
	}


	std::ofstream datatest("data-0");

	for(unsigned int i(0); i < size; i+=step)
	{
		for(unsigned int j(0); j < size; j+=step)
		{
			datatest << i << " " << j << " " << results[i][j] << std::endl;
		}
		datatest << std::endl;
	}

	std::ofstream datatest2("data-1");

	for(unsigned int i(0); i < size; i+=step)
	{
		for(unsigned int j(0); j < size; j+=step)
		{
			double sum = results[i][j];
			double summean = 1;
			if(i > step-1)
			{
				sum += results[i-step][j];
				++summean;
			}
			if(j > step-1)
			{
				sum += results[i][j-step];
				++summean;
			}
			if(i > step-1 && j > step-1)
			{
				sum += results[i-step][j-step];
				++summean;
			}

			if(i < size-step)
			{
				sum += results[i+step][j];
				++summean;
			}
			if(j < size-step)
			{
				sum += results[i][j+step];
				++summean;
			}
			if(i < size-step && j != size-step)
			{
				sum += results[i+step][j+step];
				++summean;
			}

			if(i > step-1 && j < size-step)
			{
				sum += results[i-step][j+step];
				++summean;
			}
			if(i < size-step && j > step-1)
			{
				sum += results[i+step][j-step];
				++summean;
			}

			datatest2 << i << " " << j << " " << sum/summean << std::endl;
		}
		datatest2 << std::endl;
	}

	for(unsigned int i(0); i < size; ++i)
	{
		delete[] results[i];
	}
	delete[] results;
}



void ressemblancenk(unsigned int n, unsigned int k, unsigned int step, unsigned int sol, unsigned int func, unsigned int distance)
{
	double **results = new double*[k+1];
	for(unsigned int i(0); i <= k; ++i)
	{
		results[i] = new double[101];
		for(unsigned int j(0); j < 101; ++j)
		{
			results[i][j] = 0.;
		}
	}

	for(unsigned int testk = 0; testk <= k; ++testk)
	{
		std::cout << testk << "    \r" << std::flush;
		for(unsigned int testprop = 0; testprop <= 100; testprop+=step)
		{
			std::vector<bool*> setsol;
			for(unsigned int i(0); i < sol; ++i)
			{
				bool *s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = rand()%2;
				}
				setsol.push_back(s);
				s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = setsol.back()[j];
				}
				for(unsigned int j(0); j < distance; ++j)
				{
					unsigned int tmp = rand()%n;
					s[tmp] = !s[tmp];
				}
				setsol.push_back(s);
			}


			unsigned long long mean = 0;
			for(unsigned int i(0); i < func; ++i)
			{
				NK fn1(n,4);
				//NK fn2(n,4);
				NK fn2(fn1);
				fn2.mutate(testprop, testk);
				//fn2.mutate(100, 1000);

				for(unsigned int j(0); j < sol*2; j+=2)
				{
					mean += (fn1.evaluate(setsol[j]) <= fn1.evaluate(setsol[j+1])) == (fn2.evaluate(setsol[j]) <= fn2.evaluate(setsol[j+1]));
				}
			}
			results[testk][testprop] = (double)mean/(double)(sol*func);


			for(unsigned int i(0); i < setsol.size(); ++i)
			{
				delete[] setsol[i];
			}
		}
	}


	std::ofstream datatest("data-0");

	for(unsigned int i(0); i <= k; ++i)
	{
		for(unsigned int j(0); j < 101; j+=step)
		{
			datatest << i << " " << j << " " << results[i][j] << std::endl;
		}
		datatest << std::endl;
	}

	std::ofstream datatest2("data-1");

	for(unsigned int i(0); i <= k; ++i)
	{
		for(unsigned int j(0); j < 101; j+=step)
		{
			double sum = results[i][j];
			double summean = 1;
			if(i > 0)
			{
				sum += results[i-1][j];
				++summean;
			}
			if(j > step-1)
			{
				sum += results[i][j-step];
				++summean;
			}
			if(i > 0 && j > step-1)
			{
				sum += results[i-1][j-step];
				++summean;
			}

			if(i < k-1)
			{
				sum += results[i+1][j];
				++summean;
			}
			if(j < 101-step)
			{
				sum += results[i][j+step];
				++summean;
			}
			if(i < k-1 && j != 101-step)
			{
				sum += results[i+1][j+step];
				++summean;
			}

			if(i > 0 && j < 101-step)
			{
				sum += results[i-1][j+step];
				++summean;
			}
			if(i < k-1 && j > step-1)
			{
				sum += results[i+1][j-step];
				++summean;
			}

			datatest2 << i << " " << j << " " << sum/summean << std::endl;
		}
		datatest2 << std::endl;
	}

	for(unsigned int i(0); i <= k; ++i)
	{
		delete[] results[i];
	}
	delete[] results;
}


void ressemblancenk(unsigned int n, unsigned int k, unsigned int step, unsigned int sol, unsigned int func)
{
	double **results = new double*[k+1];
	for(unsigned int i(0); i <= k; ++i)
	{
		results[i] = new double[101];
		for(unsigned int j(0); j < 101; ++j)
		{
			results[i][j] = 0.;
		}
	}

	for(unsigned int testk = 0; testk <= k; ++testk)
	{
		std::cout << testk << "    \r" << std::flush;
		for(unsigned int testprop = 0; testprop <= 100; testprop+=step)
		{
			std::cout << testk << "    \r" << std::flush;
			std::vector<bool*> setsol;
			for(unsigned int i(0); i < sol; ++i)
			{
				bool *s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = rand()%2;
				}
				setsol.push_back(s);
				s = new bool[n];
				for(unsigned int j(0); j < n; ++j)
				{
					s[j] = rand()%2;
				}
				setsol.push_back(s);
			}


			unsigned long long mean = 0;
			for(unsigned int i(0); i < func; ++i)
			{
				NK fn1(n,4);
				NK fn2(fn1);
				fn2.mutate(testprop,testk);

				for(unsigned int j(0); j < sol*2; j+=2)
				{
					mean += (fn1.evaluate(setsol[j]) < fn1.evaluate(setsol[j+1])) == (fn2.evaluate(setsol[j]) < fn2.evaluate(setsol[j+1]));
				}
			}
			results[testk][testprop] = (double)mean/(double)(sol*func);


			for(unsigned int i(0); i < setsol.size(); ++i)
			{
				delete[] setsol[i];
			}
/*
			unsigned long long mean = 0;
			for(unsigned int i(0); i < func; ++i)
			{
				NK fn1(n,1);
				NK fn2(fn1);
				fn2.mutate(testprop, testk);

				bool *s = localsearch(fn1);
				float score = fn2.evaluate(s);
				for(unsigned int j(0); j < n; ++j)
				{
					s[i] = !s[i];
					mean += fn2.evaluate(s) > score;
					s[i] = !s[i];
				}
				delete[] s;
			}
			results[testk][testprop] = (double)mean/(double)(n*func);*/
		}
	}


	std::ofstream datatest("data-0");

	for(unsigned int i(0); i <= k; ++i)
	{
		for(unsigned int j(0); j < 101; j+=step)
		{
			datatest << i << " " << j << " " << results[i][j] << std::endl;
		}
		datatest << std::endl;
	}

	std::ofstream datatest2("data-1");

	for(unsigned int i(0); i <= k; ++i)
	{
		for(unsigned int j(0); j < 101; j+=step)
		{
			double sum = results[i][j];
			double summean = 1;
			if(i > 0)
			{
				sum += results[i-1][j];
				++summean;
			}
			if(j > step-1)
			{
				sum += results[i][j-step];
				++summean;
			}
			if(i > 0 && j > step-1)
			{
				sum += results[i-1][j-step];
				++summean;
			}

			if(i < k-1)
			{
				sum += results[i+1][j];
				++summean;
			}
			if(j < 101-step)
			{
				sum += results[i][j+step];
				++summean;
			}
			if(i < k-1 && j != 101-step)
			{
				sum += results[i+1][j+step];
				++summean;
			}

			if(i > 0 && j < 101-step)
			{
				sum += results[i-1][j+step];
				++summean;
			}
			if(i < k-1 && j > step-1)
			{
				sum += results[i+1][j-step];
				++summean;
			}

			datatest2 << i << " " << j << " " << sum/summean << std::endl;
		}
		datatest2 << std::endl;
	}

	for(unsigned int i(0); i <= k; ++i)
	{
		delete[] results[i];
	}
	delete[] results;
}












