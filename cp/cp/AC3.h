#pragma once
#include "CPAlgorithm.h"


namespace cp {

class AC3 :public AC
{
public:
	AC3(Network *nt);
	virtual ~AC3() {}
	virtual bool EnforceGAC_arc(VarEvt* x_evt, const int level = 0);

protected:
	arc_que Q;
	//pro_que<T> q;
	virtual bool revise(arc& c_x);
	virtual bool seek_support(IntConVar& c_val);
private:
	void inital_Q_arc();
	IntTuple cur_tp_;
};

}
