#pragma once
#pragma warning (disable:4290)

#include "Model.h"
namespace cp
{

class VarEvt
{
public:
	VarEvt(const VarEvt& ve);
	VarEvt(Network *nt);

	bool Assigned(IntVariable* const v) const { return (sig_[v->id()] & 0x80000000) != 0; }
	bool Assigned(const int idx) const { return (sig_[idx] & 0x80000000) != 0; }
	void Assign(const int idx) { sig_[idx] |= 0x80000000; }
	void Assign(IntVariable* const v) { sig_[v->id()] |= 0x80000000; }
	void Modi(const int idx) { sig_[idx] |= 1; }
	void Modi(IntVariable* const v) { sig_[v->id()] |= 1; }
	bool NeedModi(const int idx) const { return (sig_[idx] & 1) != 0; }
	bool NeedModi(IntVariable* const v) const { return (sig_[v->id()] & 1) != 0; }
protected:
	//////////////////////////////////////////////////////////////////////////
	//1xxx	变量已赋值
	//0xxx	变量未赋值
	//xxx1  【主线程】变量需要传播
	//xxx0	【主线程】变量不需要传播
	//////////////////////////////////////////////////////////////////////////
	std::vector<unsigned> sig_;
};


class arc_que
{
public:
#define  have(a) vid_set_[a.c_id()*arity_+a.c()->index(a.v())]
	arc_que() {}
	arc_que(const int cons_size, const int max_arity);
	virtual ~arc_que();

	void MakeQue(const size_t cons_size, const size_t max_arity);
	bool empty() const;
	bool full() const;
	bool push(arc& ele) throw(std::bad_exception);
	arc pop() throw(std::bad_exception);

private:
	arc* m_data_;
	int* vid_set_;
	size_t arity_;
	size_t m_size_;
	int m_front_;
	int m_rear_;
};

class AC
{
public:
	AC() {}
	AC(Network *nt);
	virtual ~AC() {}
	virtual bool EnforceGAC_arc(VarEvt& evt, const int level) = 0;
	virtual bool Revise(arc &c_x) = 0;
	virtual bool SeekSupport(c_value_int& c_val) = 0;
	int DeletedCount();

	arc_que Q;

	int lvl() const { return lvl_; }
	void lvl(int val) { lvl_ = val; }

protected:
	Network* nt_;
	int delete_count = 0;
	int lvl_;
};

}

//class arc_que
//{
//public:
//	arc_que() {}
//	arc_que(const int vars_size, const int cons_size) :arity_(cons_size)
//	{
//		resize(vars_size, cons_size);
//	}
//
//	arc_que(Network* nt) :arity_(nt->max_arity())
//	{
//		vid_set.resize(nt->cons_size(), std::vector<int>(nt->max_arity(), 0));
//	}
//
//#define  in(a) vid_set[a.c_id()][a.c()->index(a.v())]
//
//	virtual ~arc_que() {}
//
//	void resize(const int vars_size, const int cons_size)
//	{
//		vid_set.resize(cons_size, std::vector<int>(vars_size, 0));
//	}
//
//	void reset()
//	{
//
//	}
//
//	bool push(const arc& a)
//	{
//		if (in(a))
//		{
//			aq_.push_back(a);
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//		++current_total_size_;
//	}
//
//	arc front()
//	{
//		return aq_.front();
//	}
//
//	void pop_front()
//	{
//		aq_.pop_front();
//		arc a = aq_.front();
//		in(a) = 0;
//		--current_total_size_;
//	}
//
//	bool empty() const
//	{
//		return aq_.empty();
//	}
//
//	std::deque<arc> aq_;
//	std::vector<std::vector<int>> vid_set;
//private:
//	int arity_;
//	int current_total_size_ = 0;
//};
