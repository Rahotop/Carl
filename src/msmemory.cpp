#include "msmemory.hpp"

MSmemory::MSmemory(const std::string& path, unsigned int memsize) : m_ms(path), m_memsize(memsize), m_saved(0), m_memory(new bool[m_memsize*m_ms.getN()]), m_freq(new unsigned int[m_memsize]), m_result(new float[m_memsize])
{
	
}

MSmemory::~MSmemory()
{
	delete[] m_memory;
	delete[] m_freq;
	delete[] m_result;
}

float MSmemory::evaluate(bool *s)
{
	for(unsigned int i(0); i < m_saved; ++i)
	{
		for(unsigned int j(0); j < m_ms.getN(); ++j)
		{
			if(s[j] != m_memory[i*m_ms.getN()+j])
			{
				break;
			}
			else if(j+1 == m_ms.getN())
			{
				++m_freq[i];
				return m_result[i];
			}
		}
	}


	unsigned int replace = 0;
	if(m_saved < m_memory)
	{
		replace = m_saved;
		++m_saved;
	}
	else
	{
		for(unsigned int i(0); i < m_saved; ++i)
		{
			if(m_freq[replace] > m_freq[i])
			{
				replace = i;
			}
		}
	}


	for(unsigned int i(0); i < m_ms.getN(); ++i)
	{
		m_memory[replace*m_ms.getN()+i] = s[i];
	}
	m_freq[replace] = 1;
	m_result[replace] = m_ms.evaluate(s);

	return m_result[replace];
}

