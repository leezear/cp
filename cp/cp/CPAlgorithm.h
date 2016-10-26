#pragma once
#pragma warning (disable:4290)

#include "Model.h"
#include "pro_que.h"

namespace cp
{

//class VarEvt
//{
//public:
//	VarEvt(const VarEvt& ve);
//	VarEvt(Network *nt);
//
//	bool Assigned(IntVar* const v) const { return (sig_[v->id()] & 0x80000000) != 0; }
//	bool Assigned(const int idx) const { return (sig_[idx] & 0x80000000) != 0; }
//	void Assign(const int idx) { sig_[idx] |= 0x80000000; }
//	void Assign(IntVar* const v) { sig_[v->id()] |= 0x80000000; }
//	void Modi(const int idx) { sig_[idx] |= 1; }
//	void Modi(IntVar* const v) { sig_[v->id()] |= 1; }
//	bool NeedModi(const int idx) const { return (sig_[idx] & 1) != 0; }
//	bool NeedModi(IntVar* const v) const { return (sig_[v->id()] & 1) != 0; }
//protected:
//	//////////////////////////////////////////////////////////////////////////
//	//1xxx	变量已赋值
//	//0xxx	变量未赋值
//	//xxx1  【主线程】变量需要传播
//	//xxx0	【主线程】变量不需要传播
//	//////////////////////////////////////////////////////////////////////////
//	std::vector<unsigned> sig_;
//};

enum ACAlgorithm {
	AC_1, AC_2, AC_3, AC_4, AC_6, AC_7, AC_2001, AC3_bit, AC_3rm, STR_1, STR_2, STR_3
};

class VarEvt
{
public:
	VarEvt(Network* nt_);
	virtual ~VarEvt();

	IntVar* operator[](const int i);
	void push_back(IntVar* v);
	void clear();
	int size() const;
	IntVar* at(const int i);

private:
	IntVar** vars_;
	int size_;
	int cur_size_;
};

template<class T>
class LinkedNode
{
public:
	int prev;
	T data;
	int absent;
	int next;
};

template<class T>
class VarList
{
public:
	VarList(Network* nt_);
	virtual ~VarList();

	const T operator[](const int i);
	void push_back(T t);
	T pop_back();
	T pop_front();
	const bool full();
	const bool empty();
	void clear();
	T top() const;
	void del(IntVal& v_a);
	//int size() const;
	T at(const int i);

protected:
	LinkedNode<T>* nodes_;
	int size_;
	int cur_size_;
	int head_ = 0;
	int tail_ = 0;
	int top_;
};

class arc_que
{
public:
#define  have(a) vid_set_[a.c_id() * arity_ + a.c()->index(a.v())]
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

class AssignedStack
{
public:
	AssignedStack(Network *nt);
	virtual ~AssignedStack();

	void push(IntVal& v_a);
	IntVal pop();
	IntVal top() const;
	int size() const { return lvl_; }
	int capacity() const { return size_; }
	bool full() const { return lvl_ == size_; }
	bool empty() const { return lvl_ == 0; }
	IntVal operator[](const int i) const;
	IntVal at(const int i) const;
	friend std::ostream& operator<< (std::ostream &os, AssignedStack &I);
	friend std::ostream& operator<< (std::ostream &os, AssignedStack* I);
private:
	Network *nt_;
	IntVar** vars_;
	int* a_;
	int lvl_ = 0;
	int size_;
};


class AC
{
public:
	AC() {}
	AC(Network *nt) : nt_(nt) {}
	virtual ~AC() {}
	virtual bool EnforceGAC_arc(VarEvt* x_evt, const int level = 0) = 0;

	int DeletedCount()
	{
		int count = 0;
		for (IntVar* v : nt_->vars_)
			count += (v->capacity() - v->size());
		return count;
	}


	int lvl() const { return lvl_; }
	void lvl(int val) { lvl_ = val; }

protected:

	//virtual bool revise(arc &c_x) = 0;
	//virtual bool seek_support(c_value_int& c_val) = 0;
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
