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
private:
	Network *nt_;
	AC* ac_;
	VarEvt* x_evt_;
	ACAlgorithm ac_algzm_;
	AssignedStack* I;
	v_value_int select_v_value();
	bool consistent_;
	bool finished_ = false;
};
}

