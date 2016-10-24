#pragma once
#include "Model.h"
#include "XMLModel.h"

using namespace cp;
using namespace cp::model;

namespace cp
{
namespace parse
{

inline void Equal(int* lh, const int* rh, const int& len)
{
	for (int i = 0; i < len; ++i)
		lh[i] = rh[i];
}

//************************************
// Method:    IsEqual
// FullName:  IsEqual
// Access:    public 
// Returns:   bool
// Qualifier: 判断元组是否相等
// Parameter: const int * lh
// Parameter: const int * rh
// Parameter: const int & len	元组长度
//************************************
inline bool IsEqual(const int* lh, const int* rh, const int& len)
{
	for (int i = 0; i < len; ++i)
		if (lh[i] != rh[i])
			return false;

	return true;
}

//************************************
// Method:    GetIntTuple
// FullName:  GetIntTuple
// Access:    public 
// Returns:   void
// Qualifier: 从序号获得约束元组
// Parameter: int idx	序号
// Parameter: int * t	元组	
// Parameter: const XMLDomain * * ds	scope(c)
//************************************
inline void GetIntTuple(int idx, int *t, const int& len, XMLDomain **ds)
{
	for (int i = (len - 1); i >= 0; --i)
	{
		t[i] = ds[i]->values[(idx % (ds[i]->size))];
		idx /= (ds[i]->size);
	}
}

void BuildModel(const XMLModel *xmodel, Network* nt_)
{
	XMLConstraint* c;
	XMLRelation* r;
	XMLVariable* v;
	XMLDomain* d;
	nt_->max_arity(xmodel->features.arity);
	for (int i = 0; i < xmodel->vs_size; ++i)
	{
		d = &(xmodel->domains[xmodel->variables[i].dm_id]);
		nt_->MakeVar(i, d->values, d->size);
	}

	if (xmodel->features.rel_type == REL_TABULAR)
	{
		int* tpl = new int[xmodel->features.arity];
		XMLDomain** ds = new XMLDomain*[xmodel->features.arity];
		int num_tuples, sup_size_;

		for (int i = 0; i < xmodel->cs_size; ++i)
		{
			int k = 0;
			c = &xmodel->constraints[i];
			r = &xmodel->relations[c->re_id];
			std::vector<IntVar *> scope;
			for (int j = 0; j < c->arity; ++j)
			{
				scope.push_back(nt_->vars_[c->scope[j]]);
			}

			if (r->semantices == SEM_SUPPORT)
			{
				nt_->MakeTab(i, scope, r->tuples, r->size);
				//for (int j = 0; j < r->size; ++j)
				//	ts.add(IntArgs(c->arity, r->tuples[j]));

				//ts.finalize();
			}
			else
			{
				int** ts;
				num_tuples = 1;

				for (int j = 0; j < r->arity; ++j)
				{
					v = &xmodel->variables[j];
					d = &xmodel->domains[v->dm_id];
					ds[j] = d;
					num_tuples *= d->size;
				}

				sup_size_ = num_tuples - r->size;
				ts = new int*[sup_size_];

				for (int j = 0; j < num_tuples; ++j)
				{
					ts[k] = new int[r->arity];
					GetIntTuple(j, tpl, r->arity, ds);
					//支持
					if (k < r->size)
					{
						std::cout << r->tuples[k][0] << ", " << r->tuples[k][1] << "_" << tpl[0] << ", " << tpl[1] << std::endl;
						if (!IsEqual(r->tuples[k], tpl, r->arity))
						{
							Equal(ts[k], tpl, r->arity);
						}
						else
							++k;
					}
					else
						Equal(ts[k], tpl, r->arity);
				}

				nt_->MakeTab(i, scope, ts, sup_size_);

				for (int k = 0; k < sup_size_; ++k)
				{
					delete[] ts[k];
					ts[k] = NULL;
				}

				delete[] ts;
				ts = NULL;

			}
		}

		delete[] tpl;
		delete[] ds;

		tpl = NULL;
		ds = NULL;
	}
	else
	{
	}
}



}
}