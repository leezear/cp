#pragma once
#include "Model.h"
#include "pro_que.h"
namespace cp
{
template<class T>
class GAC
{
public:
	GAC() {}
	GAC(Network* nt) {}
	virtual ~GAC() {}

	virtual bool enforce(const int level = 0) {}
	int level() const { return lvl_; }
	void level(int val) { lvl_ = val; }
protected:
	Network *nt_;
	pro_que<T> Q;

	virtual bool revise(arc &c_x) {}
	virtual bool seek_support(IntConVar& c_val) {}
	virtual void inital_queue() {}
	int lvl_;
};

}

