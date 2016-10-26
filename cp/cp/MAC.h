#pragma once
#include "Model.h"
#include "CPAlgorithm.h"
#include "AC3.h"

namespace cp
{
class MAC
{
public:
	MAC(Network *nt, ACAlgorithm ac_algzm);
	void enforce();
	virtual ~MAC();
	int sol_count() const { return sol_count_; }
	void sol_count(int val) { sol_count_ = val; }
private:
	int sol_count_ = 0;
	Network *nt_;
	AC* ac_;
	VarEvt* x_evt_;
	ACAlgorithm ac_algzm_;
	AssignedStack* I;
	IntVal select_v_value();
	bool consistent_;
	bool finished_ = false;
};
}

