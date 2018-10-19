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

int main(int argc, char **argv)
{
	INIT tabinit[] = {&HyperHeuritic::initrand, &HyperHeuritic::initall};
	NEXT tabnext[] = {&HyperHeuritic::mut1, &HyperHeuritic::mut2, &HyperHeuritic::mut3, &HyperHeuritic::mut1add, &HyperHeuritic::mutadd, &HyperHeuritic::mutnew};
	COND tabcond[] = {&HyperHeuritic::sup, &HyperHeuritic::supeq};
	unsigned int init = 0;
	unsigned int next = 0;
	unsigned int cond = 0;

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
	unsigned int pc = 15;
	unsigned int pm = 15;
	unsigned int id = 0;
	unsigned int repeat = 1;
	unsigned int seed = time(nullptr);
	std::vector<unsigned int> fnset = {1,7,9,11,13};

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
		else if("-mut1" == std::string(argv[i]))
		{
			next = 0;
			--i;
		}
		else if("-mut2" == std::string(argv[i]))
		{
			next = 1;
			--i;
		}
		else if("-mut3" == std::string(argv[i]))
		{
			next = 2;
			--i;
		}
		else if("-mut1add" == std::string(argv[i]))
		{
			next = 3;
			--i;
		}
		else if("-mutadd" == std::string(argv[i]))
		{
			next = 4;
			--i;
		}
		else if("-mutnew" == std::string(argv[i]))
		{
			next = 5;
			--i;
		}
		else if("-sup" == std::string(argv[i]))
		{
			cond = 0;
			--i;
		}
		else if("-supeq" == std::string(argv[i]))
		{
			cond = 1;
			--i;
		}
		else if("-fnset" == std::string(argv[i]))
		{
			fnset.clear();
			unsigned int setsize = std::stoi(argv[i+1]);
			for(unsigned int j(0); j < setsize; ++j)
				fnset.push_back(std::stoi(argv[i+2+j]));
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
			hyper.run(inst, tabinit[init], tabcond[cond], tabnext[next], newsize, iteration, cycle, out+"-"+std::to_string(id+rp), !cond);
		}
		
		else if(nk && hh)
		{
			NK inst(path);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], tabcond[cond], tabnext[next], newsize, iteration, cycle, out+"-"+std::to_string(id+rp), !cond);
		}
		
		else if(om && hh)
		{
			OneMax inst(omn);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], tabcond[cond], tabnext[next], newsize, iteration, cycle, out+"-"+std::to_string(id+rp), !cond);
		}
		
		else if(rr && hh)
		{
			RoyalRoad inst(omn, rrk);
			HyperHeuritic hyper(size,width,fnset);
			hyper.run(inst, tabinit[init], tabcond[cond], tabnext[next], newsize, iteration, cycle, out+"-"+std::to_string(id+rp), !cond);
		}

		//ILS
		else if(sls && nk)
		{
			NK inst(path);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] ils(ilsout, inst, iteration, &nbeval);
		}

		else if(sls && ms)
		{
			MaxSat inst(path);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] ils(ilsout, inst, iteration, &nbeval);
		}

		else if(sls && rr)
		{
			RoyalRoad inst(omn, rrk);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] ils(ilsout, inst, iteration, &nbeval);
		}

		//ILSP
		else if(ilsp && nk)
		{
			NK inst(path);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] ils(ilsout, inst, iteration, cycle, &nbeval);
		}

		else if(ilsp && ms)
		{
			MaxSat inst(path);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] ils(ilsout, inst, iteration, cycle, &nbeval);
		}

		else if(ilsp && rr)
		{
			RoyalRoad inst(omn, rrk);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] ils(ilsout, inst, iteration, cycle, &nbeval);
		}

		//TABU
		else if(tabou && nk)
		{
			NK inst(path);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] tabu(ilsout, inst, iteration, cycle, &nbeval);
		}

		else if(tabou && ms)
		{
			MaxSat inst(path);
			std::ofstream ilsout(out+"-"+std::to_string(id+rp));
			unsigned int nbeval = 0;
			delete[] tabu(ilsout, inst, iteration, cycle, &nbeval);
		}


		else
		{
			std::cout << "Combinaison inconnue" << std::endl;
		}
	}

	return 0;
}


















