#include "AC3.h"

namespace cp
{

AC3::AC3(Network *nt) :
	AC(nt)
{
	inital_Q_arc();
	cur_tp_.reserve(nt_->max_arity());
}


bool AC3::EnforceGAC_arc(VarEvt* x_evt, const int level)
{
	lvl(level);
	delete_count = 0;
	arc c_x;

	for (int i = 0; i < x_evt->size(); ++i)
		for (Constraint* c : x_evt->at(i)->subscribe())
			for (IntVar* x : c->scope())
				if (x != x_evt->at(i))
					Q.push(arc(c, x));

	//for (Constraint* c : nt_->cons_)
	//	for (IntVar* v : c->scope())
	//		if ((!v->assigned()) && v->propagated(lvl_))
	//			Q.push(arc(c, v));

	while (!Q.empty())
	{
		c_x = Q.pop();
		//std::cout << c_x << std::endl;

		if (revise(c_x))
		{
			if (c_x.v()->faild())
				return false;

			for (Constraint* c : c_x.v()->subscribe())
				if (c != c_x.c())
					for (IntVar* v : c->scope())
						if ((v != c_x.v()) && (!v->assigned()))
							Q.push(arc(c, v));
		}
	}

	return true;
}

bool AC3::revise(arc & c_x)
{
	int num_elements = c_x.v()->size();
	int a = c_x.v()->head();

	while (a != Limits::INDEX_OVERFLOW)
	{
		if (!seek_support(IntConVar(c_x, a)))
		{
			c_x.v()->RemoveValue(a, lvl_);
			//std::cout << "(" << c_x.v_id() << ", " << a << ")" << std::endl;
			++delete_count;
		}
		a = c_x.v()->next(a);
	}

	return num_elements != c_x.v()->size();
}

bool AC3::seek_support(IntConVar & c_val)
{
	nt_->GetFirstValidTuple(c_val, cur_tp_);

	while (cur_tp_.existed())
		if (((Tabular*)c_val.c())->sat(cur_tp_))
			return true;
		else
			nt_->GetNextValidTuple(c_val, cur_tp_);

	return false;
}

void AC3::inital_Q_arc()
{
	Q.MakeQue(nt_->cons_size(), nt_->max_arity());
}

}