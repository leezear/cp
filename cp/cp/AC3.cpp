#include "AC3.h"

namespace cp
{
AC3::AC3(Network *nt) :
	AC(nt)
{
	inital_Q_arc();
}

bool AC3::EnforceGAC_arc(VarEvt& evt, const int level)
{
	lvl(level);
	delete_count = 0;
	return true;
	arc c_x;

	for (Constraint* c : nt_->cons_)
	{
		for (IntVariable* v : c->scope())
		{
			if (evt.Assigned(v) && evt.NeedModi(v))
				return false;
		}
	}

	while (!Q.empty())
	{
		c_x = Q.pop();


	}
}

bool AC3::revise(arc & c_x)
{
	int num_elements = c_x.v()->size();
	int* ptr = c_x.v()->begin();
	int* end = c_x.v()->end();

	while (ptr != end)
	{
		if (seek_support(c_value_int(c_x, (*ptr))))
		{
			c_x.v()->RemoveValue((*ptr), lvl_);
			++delete_count;
		}
		ptr = c_x.v()->next();
	}

	return num_elements != c_x.v()->size();
}

bool AC3::seek_support(c_value_int & c_val)
{
	return false;
}

void AC3::inital_Q_arc()
{
	Q.MakeQue(nt_->cons_size(), nt_->max_arity());
}

}