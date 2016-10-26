#include "MAC.h"

namespace cp
{

MAC::MAC(Network * nt, ACAlgorithm ac_algzm) :
	nt_(nt),
	ac_algzm_(ac_algzm)
{
	x_evt_ = new VarEvt(nt_);
	I = new AssignedStack(nt_);

	switch (ac_algzm_) {
	case AC_3:
		ac_ = new AC3(nt_);
		break;
	default:
		break;
	}
}

void MAC::enforce()
{
	consistent_ = ac_->EnforceGAC_arc(x_evt_);
	x_evt_->clear();
	IntVal v_a;
	if (!consistent_)
		return;

	while (!finished_)
	{
		v_a = select_v_value();
		I->push(v_a);
		v_a.v()->ReduceTo(v_a.a(), I->size());
		x_evt_->push_back(v_a.v());
		consistent_ = ac_->EnforceGAC_arc(x_evt_, I->size());
		x_evt_->clear();

		if (consistent_&&I->full())
		{
			std::cout << I << std::endl;
			++sol_count_;
			consistent_ = false;
		}

		while (!consistent_ && !I->empty())
		{
			v_a = I->pop();

			for (IntVar* v : nt_->vars_)
				if (!v->assigned())
					v->RestoreUpTo(I->size() + 1);

			v_a.v()->RemoveValue(v_a.a(), I->size());
			x_evt_->push_back(v_a.v());
			consistent_ = v_a.v()->size() && ac_->EnforceGAC_arc(x_evt_, I->size());
			x_evt_->clear();
		}

		if (!consistent_)
			finished_ = true;
	}

}

MAC::~MAC()
{
	delete ac_;
	delete x_evt_;
	delete I;
}

cp::IntVal MAC::select_v_value()
{
	IntVar* v = nt_->vars_[I->size()];
	return IntVal(v, v->head());
}

}