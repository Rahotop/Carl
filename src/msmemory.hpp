#ifndef MSMEMORY_HPP_INCLUDED
#define MSMEMORY_HPP_INCLUDED

#include <algorithm>
#include <string>
#include "maxsat.hpp"

class MSmemory
{
	public:

	MSmemory(const std::string& path, unsigned int memsize);
	~MSmemory();

	float evaluate(bool *s);
	bool islocopt(bool *s);

	inline unsigned int getnbeval() const { return m_ms.getnbeval(); }
	inline unsigned int getN() const { return m_ms.getN(); }

	private:

	MaxSat m_ms;
	unsigned int m_memsize;
	unsigned int m_saved;
	bool *m_memory;
	unsigned int *m_freq;
	float *m_result;
};

#endif