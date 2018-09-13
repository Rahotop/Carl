#include <iostream>
#include <cstdlib>
#include <ctime>
#include "maxsat.hpp"
#include "localsearch.hpp"
#include "algogen.hpp"
#include "msmemory.hpp"
#include "nk.hpp"

void plot(std::string out, unsigned int id);

int main(int argc, char **argv)
{
	srand(time(nullptr));

	bool ms = false;
	bool nk = false;
	std::string path;
	unsigned int nbind = 100;
	unsigned int size = 100;
	unsigned int width = 15;
	unsigned int newsize = 5;
	unsigned int iteration = 1000;
	std::string out = "data";
	unsigned int pc = 15;
	unsigned int pm = 15;
	unsigned int id = 0;

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
	}

	if(ms)
	{
		MaxSat inst(path);
		AlgoGen algo(nbind,size,width);
		algo.run(inst, newsize, iteration, out+"-"+std::to_string(id), pc, pm);
	}
	
	if(nk)
	{
		NK inst(path);
		AlgoGen algo(nbind,size,width);
		algo.run(inst, newsize, iteration, out+"-"+std::to_string(id), pc, pm);
	}

	if(ms||nk)
		plot(out,id);

	return 0;
}

void plot(std::string path, unsigned int id)
{
	std::ifstream in("plot");
	std::ofstream out(path+"-"+std::to_string(id)+"-plot");

	while(!in.eof())
	{
		std::string tmp;
		in >> tmp;

		if(tmp == "'fitness.jpg'")
			out << "'" << path << "-" << id << "-fitness.jpg'\n";
		else if(tmp == "'nbEval.jpg'")
			out << "'" << path << "-" << id << "-nbEval.jpg'\n";
		else if(tmp == "'ils.jpg'")
			out << "'" << path << "-" << id << "-ils.jpg'\n";
		else if(tmp == "'correlation.jpg'")
			out << "'" << path << "-" << id << "-correlation.jpg'\n";
		else if(tmp == "'data'")
			out << "'" << path << "-" << id << "' ";
		else if(tmp == ",\\" || tmp == "x1y2" || tmp == "y2tics" || tmp == "1000")
			out << tmp << "\n";
		else
			out << tmp << " ";
	}
}

















