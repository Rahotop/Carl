#ifndef FNARRAY_HPP_INCLUDED
#define FNARRAY_HPP_INCLUDED

#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stack>

class FnArray
{
	public:

	FnArray();
	FnArray(const FnArray& fn);
	FnArray(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset);
	~FnArray();

	inline unsigned int size() const { return m_size; }
	inline unsigned int getN() const { return m_n; }
	float evaluate(bool *s) const;
	inline void setScore(float s) { m_score = s; }
	inline float getScore() const { return m_score; }

	void crossover(const FnArray& f1, const FnArray& f2);
	void addRandom();
	void deleteRandom();

	inline bool operator<(const FnArray& fn) { return m_score < fn.m_score; }
	inline bool operator>(const FnArray& fn) { return m_score > fn.m_score; }
	FnArray& operator=(const FnArray& fn);
	friend std::ostream& operator<<(std::ostream& o, const FnArray& fn);

	private:

	void deleteTree(unsigned int index);
	void deleteAll();

	std::vector<unsigned int> m_fnset;
	unsigned int m_n;

	unsigned int m_maxsize;	//number of sub trees
	unsigned int m_size;
	unsigned int m_width;	//size of sub trees
	unsigned int *m_trees;
	bool *m_not;
	float *m_weights;

	float m_score;
};

std::ostream& operator<<(std::ostream& os, const FnArray& fn);

class FnArrayInc
{
	public:

	FnArrayInc();
	FnArrayInc(const FnArrayInc& fn);
	FnArrayInc(unsigned int size, unsigned int width, unsigned int n, const std::vector<unsigned int>& fnset);
	~FnArrayInc();

	inline unsigned int size() const { return m_size; }
	inline unsigned int getN() const { return m_n; }
	float evaluate(bool *s) const;
	float evaluateInc(bool *s, unsigned int changed) const;
	inline void setScore(float s) { m_score = s; }
	inline float getScore() const { return m_score; }

	void crossover(const FnArrayInc& f1, const FnArrayInc& f2);
	void addRandom();
	void deleteRandom();

	inline bool operator<(const FnArrayInc& fn) { return m_score < fn.m_score; }
	inline bool operator>(const FnArrayInc& fn) { return m_score > fn.m_score; }
	FnArrayInc& operator=(const FnArrayInc& fn);
	friend std::ostream& operator<<(std::ostream& o, const FnArrayInc& fn);

	private:

	void deleteTree(unsigned int index);
	void deleteAll();

	std::vector<unsigned int> m_fnset;
	unsigned int m_n;

	unsigned int m_maxsize;	//number of sub trees
	unsigned int m_size;
	unsigned int m_width;	//size of sub trees
	unsigned int *m_trees;
	bool *m_not;
	bool *m_prec;
	float *m_weights;

	float m_score;
};

std::ostream& operator<<(std::ostream& os, const FnArrayInc& fn);

#endif