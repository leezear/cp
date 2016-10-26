#pragma once
#include <vector>
#include <algorithm>
#include <xstddef>
#include <bitset>
#include <deque>
#include <limits>
#include <iostream>

namespace cp
{

namespace Limits
{
/**
* \brief 取值范围
*/
const int MIN_INTVAR_ID = 0x7fff7000;
const int MAX_INTVAR_ID = INT_MAX - 1;
const int MAX_OPT = INT_MIN & 0xffff7000 - 1;
const int MIN_OPT = INT_MIN + 1;
const int UNSIGNED_VAL = INT_MIN & 0xffff7000;
const int MIN_VAL = UNSIGNED_VAL + 1;
const int MAX_VAL = MIN_INTVAR_ID - 1;
const int INDEX_OVERFLOW = -1;
const int PRESENT = -1;
const int ABSENT = 0;
}

enum IntVariableType
{
	CONTI_ZERO,
	CONTI_NON_ZERO,
	DES_ZERO,
	DER_NON_ZERO
};

enum ConstraintType
{
	CT_EXT,
	CT_INT,
	CT_SOFT,
	CT_AD
};

class Base
{
public:
	Base() {}
	Base(const int id) : id_(id) {}
	virtual ~Base() {}
	int id() { return id_; };
private:
	int id_ = 0;
};

class Constraint;
class IntVal;

class IntVar :public Base
{
public:
	IntVar() {};
	IntVar(const int id) :Base(id) {}
	IntVar(const int id, const int *values, const int size);
	virtual ~IntVar();

	void RemoveValue(const int a, const int p = 0);
	void ReduceTo(const int a, const int p = 0);
	void AddValue(const int a);
	void RestoreUpTo(const int p);
	int* begin();
	int* next();
	int* end();
	int value(const int idx) const;
	int size() const;
	int capacity() const;
	int assigned() const;
	void assigned(const bool a);
	int next(const int a) const;
	int prev(const int a) const;
	bool have(const int a) const;
	int head() const;
	int tail() const;
	bool faild() const;

	std::vector<Constraint *>& subscribe() { return cs_; }
	void subscribe(Constraint* c) { cs_.push_back(c); }

	//bool propagated(const int level) const { return (level == propagated_); }
	int stamp() const { return stamp_; }
	void stamp(int val) { stamp_ = val; }
	//int propagated() const { return propagated_; }
	//void propagated(int val) { propagated_ = val; }
private:
	int* vals_;

	int* absent_;
	int* next_;
	int* prev_;
	int* prev_absent_;

	int asin_idx_ = -1;
	int size_;
	int cur_size_;

	int head_ = 0;
	int tail_;
	int tail_absent_ = -1;
	bool assigned_ = false;
	int* ptr_;
	int lmt_;
	//int propagated_ = 0;
	int stamp_;
	std::vector<Constraint* > cs_;

	//IntVariableType type_;
};

class IntTuple
{
public:
	IntTuple() {}
	IntTuple(const size_t size) :arity_(size), cur_size_(size)
	{
		tuple_ = new int[arity_]();
	}

	IntTuple(const int* t, const size_t size) :arity_(size), cur_size_(size)
	{
		//if (arity_ != size)
		//{
		//	delete[] tuple_;
		//	tuple_ = NULL;
		//}
		tuple_ = new int[arity_];

		for (size_t i = 0; i < size; ++i)
			tuple_[i] = t[i];
	}

	IntTuple(const IntTuple& rhs) :arity_(rhs.arity_), cur_size_(rhs.arity_)
	{
		//if (arity_ != rhs.arity_)
		//{
		//	delete[] tuple_;
		//	tuple_ = NULL;
		//}
		tuple_ = new int[arity_];

		for (size_t i = 0; i < arity_; ++i)
			tuple_[i] = rhs.tuple_[i];
	}

	void reserve(const int size)
	{
		if (arity_ == 0)
		{
			tuple_ = new int[size];
			arity_ = size;
			cur_size_ = size;
		}
		else
		{
			cur_size_ = size;
		}
	}

	void resize(const int size)
	{
		cur_size_ = size;
	}

	int& operator[](const int i) const
	{
		return tuple_[i];
	}

	bool operator==(const IntTuple& rhs) const
	{
		if (cur_size_ == rhs.cur_size_)
		{
			for (size_t i = 0; i < cur_size_; ++i)
			{
				if (tuple_[i] != rhs.tuple_[i])
					return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator!=(const IntTuple& rhs) const
	{
		if (cur_size_ != rhs.cur_size_)
			return false;
		else
			for (size_t i = 0; i < rhs.cur_size_; ++i)
				if (tuple_[i] != rhs.tuple_[i])
					return false;
		return true;
	}

	bool operator<(const IntTuple& rhs) const
	{
		std::cout << "---------------------" << std::endl;
		for (size_t i = 0; i < cur_size_; ++i)
		{
			std::cout << tuple_[i] << "::" << rhs.tuple_[i] << std::endl;
			if (tuple_[i] > rhs.tuple_[i])
				return false;
			else if (tuple_[i] < rhs.tuple_[i])
				return true;
		}


		return false;
	}

	bool operator>(const IntTuple& rhs) const
	{
		for (size_t i = 0; i < cur_size_; ++i)
			if (tuple_[i] < rhs.tuple_[i])
				return false;
			else if (tuple_[i] > rhs.tuple_[i])
				return true;

		return false;
	}

	const IntTuple& operator=(const IntTuple& rhs)
	{
		//if (arity_ != rhs.arity_)
		//{
		//	delete[] tuple_;
		//	tuple_ = NULL;
		//	arity_ = rhs.arity_;
		//	tuple_ = new int[rhs.arity_];
		//}

		for (size_t i = 0; i < rhs.cur_size_; ++i)
			tuple_[i] = rhs.tuple_[i];
		cur_size_ = cur_size_;

		return *this;
	}

	virtual ~IntTuple()
	{
		delete[] tuple_;
		tuple_ = NULL;
	}

	size_t arity() const
	{
		return arity_;
	}

	size_t size() const
	{
		return cur_size_;
	}

	bool existed()
	{
		return tuple_[0] != INT_MAX;
	}

	void exclude()
	{
		tuple_[0] = INT_MAX;
	}

	//friend std::ostream& operator<< (std::ostream &os, IntTuple &t)
	//{
	//	os << "(";
	//	int i;
	//	for (i = 0; i < size() - 1; ++i)
	//	{
	//		os << t.tuple_[i] << ", ";
	//	}
	//	os << t.tuple_[i] << ")";

	//	return os;
	//}

private:
	int *tuple_;
	size_t cur_size_ = 0;
	size_t arity_ = 0;
};

struct CmpFun
	:public std::binary_function < IntTuple, IntTuple, bool>
{
	bool operator()(const IntTuple &lhs, const IntTuple &rhs) const  //这里如果没有const，程序还是能运行
	{
		std::cout << lhs[0] << ", " << lhs[1] << ":" << rhs[0] << ", " << rhs[1] << "=" << (lhs < rhs) << std::endl;
		return lhs < rhs;
	}

};

class IntTupleArray
{
public:
	IntTupleArray() {}

	//IntTupleArray(const int arity) :arity_(arity)
	//{
	//}

	int arity() const
	{
		if (size() != 0)
			return tuples_[0].size();
		else
			return 0;
	}

	IntTuple& operator[](const int i)
	{
		return tuples_[i];
	}

	void operator<<(const IntTuple& rhs)
	{
		add(rhs);
	}

	void add(const IntTuple& t)
	{
		tuples_.push_back(IntTuple(t));
	}

	size_t size() const
	{
		return tuples_.size();
	}

	virtual bool have(const IntTuple& t)
	{
		//auto p = std::equal_range(tuples_.begin(), tuples_.end(), t);
		//if (p.first != p.second)
		//	return true;
		//else
		//	return false;
		for (size_t i = 0; i < tuples_.size(); ++i)
			if (tuples_[i] == t)
				return true;

		return false;
	}

	virtual ~IntTupleArray() {}


private:
	std::vector<IntTuple> tuples_;
};

class Constraint :public Base
{
public:
	Constraint() {}
	Constraint(const int id) :Base(id) {}
	Constraint(const int id, const std::vector<IntVar *>& scope, const ConstraintType type) :
		Base(id),
		scope_(scope),
		type_(type),
		arity_(scope.size()) {}
	virtual bool sat(IntTuple &t) = 0;

	std::vector<IntVar *>& scope()
	{
		return scope_;
	}

	int index(IntVar* v) const
	{
		for (int i = scope_.size() - 1; i >= 0; --i)
			if (scope_[i]->id() == v->id())
				return i;

		return -1;
	}

	virtual void GetFirstValidTuple(IntVal& v_a, IntTuple&t);
	virtual void GetNextValidTuple(IntVal& v_a, IntTuple&t);

	virtual ~Constraint() {}
protected:
	size_t arity_;
	std::vector<IntVar *>scope_;
	ConstraintType type_;
};

class Tabular :public Constraint
{
public:
	Tabular() {}
	Tabular(const int id, const std::vector<IntVar *>& scope, int**  ts, const int len);
	virtual bool sat(IntTuple &t) override;
	virtual ~Tabular() {}

private:
	IntTupleArray ts_;
};

class arc
{
public:
	arc() {}
	arc(Constraint* c, IntVar* v) :c_(c), v_(v) {}
	virtual ~arc() {}

	Constraint* c() const { return c_; }
	void c(Constraint* val) { c_ = val; }

	int c_id() const { return c_->id(); }
	int v_id() const { return v_->id(); }

	const arc& operator=(arc& rhs)
	{
		c_ = rhs.c_;
		v_ = rhs.v_;

		return *this;
	}

	friend std::ostream& operator<< (std::ostream &os, arc &c_x)
	{
		os << "(" << c_x.c_->id() << ", " << c_x.v_->id() << ")";
		return os;
	}

	IntVar* v() const { return v_; }
	void v(IntVar* val) { v_ = val; }
private:
	Constraint* c_;
	IntVar* v_;
};

class IntVal
{
public:
	IntVal() {}
	IntVal(IntVar *v, const int a) :v_(v), a_(a) {}
	~IntVal() {}

	IntVar* v() const { return v_; }
	void v(IntVar* val) { v_ = val; }
	const IntVal& operator=(const IntVal& rhs);
	//const int id() { return v_->id(); }
	int a() const { return a_; }
	void a(int val) { a_ = val; }
	friend std::ostream& operator<< (std::ostream &os, IntVal &v_val);
private:
	IntVar* v_;
	int a_;
};

class IntConVar
{
public:
	IntConVar() {}
	IntConVar(Constraint* c, IntVar *v, const  int a) : c_(c), v_(v), a_(a) {}
	IntConVar(Constraint* c, IntVal& va) :c_(c), v_(va.v()), a_(va.a()) {}
	IntConVar(arc& rc, const int a) :c_(rc.c()), v_(rc.v()), a_(a) {}

	virtual ~IntConVar() {}

	Constraint* c() const { return c_; }
	void c(Constraint* c) { c_ = c; }


	cp::IntVar* v() const { return v_; }
	void v(cp::IntVar* val) { v_ = val; }

	int a() const { return a_; }
	void a(int val) { a_ = val; }

	arc get_arc() const { return arc(c_, v_); }
	IntVal get_v_value() const { return IntVal(v_, a_); }

	const IntConVar& operator=(const IntConVar& rhs);

	friend std::ostream& operator<< (std::ostream &os, IntConVar &c_val)
	{
		os << "(" << c_val.c_->id() << ", " << c_val.v_->id() << ", " << c_val.a_ << ")";
		return os;
	}

private:
	Constraint* c_;
	IntVar* v_;
	int a_;
};

class Network
{
public:
	Network() {}
	Network(const int max_arity) :max_arity_(max_arity) {}

	virtual ~Network() {}

	void MakeVar(const int id, const int *values, const int size);
	void MakeTab(const int id, const std::vector<IntVar *>& scope, int** ts, const int len);

	size_t vars_size() { return vars_.size(); }
	size_t cons_size() { return cons_.size(); }

	void GetFirstValidTuple(IntConVar & c_val, IntTuple& t);
	void GetNextValidTuple(IntConVar & c_val, IntTuple& t);

	std::vector<IntVar*> vars_;
	std::vector<Constraint*> cons_;

	size_t max_arity() const { return max_arity_; }
	void max_arity(int val) { max_arity_ = val; }
private:
	size_t max_arity_ = 0;
};

}/*namespace cp*/
