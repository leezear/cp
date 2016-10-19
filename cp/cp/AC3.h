#pragma once
#include "CPAlgorithm.h"
namespace cp {
class AC3 :public AC
{
public:
	AC3(Network *nt);
	virtual ~AC3() {}
	virtual bool EnforceGAC_arc(VarEvt& evt, const int level);

protected:
	arc_que Q;
private:
	bool revise(arc& c_x);
	bool seek_support(c_value_int& c_val);
	void inital_Q_arc();
};

}
