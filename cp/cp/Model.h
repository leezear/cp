#pragma once
#include <vector>
#include <algorithm>
#include <xstddef>
#include <bitset>
#include <deque>
#include <limits>

namespace cp
{
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
class v_value_int;

class IntVariable :public Base
{
public:
	IntVariable() {};
	IntVariable(const int id) :Base(id) {}
	//IntVariable(const int id, const int min, const int interval, const int size);
	//IntVariable(const int id, const int min, const int max);
	//IntVariable(const int id, std::vector<int> const &values);
	IntVariable(const int id, const int *values, const int size);
	virtual ~IntVariable();

	void RemoveValue(const int a, const int p = 0);

	void ReduceTo(const int a, const int p = 0);

	void AddValue(const int a);

	void RestoreUpTo(const int p);

	int* begin();

	int* next();

	int* end();

	int GetValueByIndex(const int idx) const;

	int size() const;

	int capacity() const;

	int assigned() const;

	int next(int a) const;

	int prev(int a) const;

	bool have(int a) const;

	int head() const;

	int tail()const;


	std::vector<Constraint *>& subscribe() { return cs_; }
	void subscribe(Constraint* c) { cs_.push_back(c); }

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
	int tail_absent_;
	bool assigned_;
	int* ptr_;
	int lmt_;
	std::vector<Constraint* > cs_;

	//IntVariableType type_;
};

class IntTuple
{
public:
	IntTuple() {}
	IntTuple(const size_t size) :arity_(size)
	{
		tuple_ = new int[arity_];
	}

	IntTuple(const int* t, const size_t size) :arity_(size)
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

	IntTuple(const IntTuple& rhs) :arity_(rhs.arity_)
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

	int& operator[](const int i)
	{
		return tuple_[i];
	}

	bool operator==(const IntTuple& rhs) const
	{
		if (arity_ == rhs.arity_)
			for (size_t i = 0; i < rhs.arity_; ++i)
				if (tuple_[i] != rhs.tuple_[i])
					return false;

		return true;
	}

	bool operator!=(const IntTuple& rhs) const
	{
		if (arity_ != rhs.arity_)
			return false;
		else
			for (size_t i = 0; i < rhs.arity_; ++i)
				if (tuple_[i] != rhs.tuple_[i])
					return false;
		return true;
	}

	bool operator<(const IntTuple& rhs) const
	{
		for (size_t i = 0; i < arity_; ++i)
			if (tuple_[i] > rhs.tuple_[i])
				return false;
			else if (tuple_[i] < rhs.tuple_[i])
				return true;

		return false;
	}

	bool operator>(const IntTuple& rhs) const
	{
		for (size_t i = 0; i < arity_; ++i)
			if (tuple_[i] < rhs.tuple_[i])
				return false;
			else if (tuple_[i] > rhs.tuple_[i])
				return true;

		return false;
	}

	const IntTuple& operator=(const IntTuple& rhs)
	{
		if (arity_ != rhs.arity_)
		{
			delete[] tuple_;
			tuple_ = NULL;
			arity_ = rhs.arity_;
			tuple_ = new int[rhs.arity_];
		}

		for (size_t i = 0; i < rhs.arity_; ++i)
			tuple_[i] = rhs.tuple_[i];

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

private:
	int *tuple_;
	size_t arity_;
};

struct CmpFun
	:public std::binary_function < IntTuple, IntTuple, bool>
{
	bool operator()(const IntTuple &lhs, const IntTuple &rhs) const  //这里如果没有const，程序还是能运行
	{
		return lhs < rhs;
	}

};

class IntTupleArray :public Base
{
public:
	IntTupleArray() {}

	IntTupleArray(const int arity) :arity_(arity)
	{
		tuples_.resize(arity_);
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
		auto p = equal_range(tuples_.begin(), tuples_.end(), t, CmpFun());
		if (p.first != p.second)
			return true;
		else
			return false;
	}

	virtual ~IntTupleArray() {}


private:
	int arity_;
	std::vector<IntTuple> tuples_;
};

class Constraint :public Base
{
public:
	Constraint() {}
	Constraint(const int id) :Base(id) {}
	Constraint(const int id, const std::vector<IntVariable *>& scope, const ConstraintType type) :
		Base(id),
		scope_(scope),
		type_(type),
		arity_(scope.size()) {}
	virtual bool sat(IntTuple &t) = 0;

	std::vector<IntVariable *>& scope()
	{
		return scope_;
	}

	int index(IntVariable* v) const
	{
		for (size_t i = 0; i > scope_.size(); ++i)
			if (scope_[i]->id() == v->id())
				return i;

		return -1;
	}

	virtual void GetFirstValidTuple(v_value_int& v_a, IntTuple&t);

	virtual ~Constraint() {}
protected:
	size_t arity_;
	std::vector<IntVariable *>scope_;
	ConstraintType type_;
};

class Tabular :public Constraint
{
public:
	Tabular() {}
	Tabular(const int id, const std::vector<IntVariable *>& scope, const int** const ts, const int len);
	virtual bool sat(IntTuple &t) override;
	virtual ~Tabular() {}

private:
	IntTupleArray ts_;
};

class arc
{
public:
	arc() {}
	arc(Constraint* c, IntVariable* v) :c_(c), v_(v) {}
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

	IntVariable* v() const { return v_; }
	void v(IntVariable* val) { v_ = val; }
private:
	Constraint* c_;
	IntVariable* v_;
};

class v_value_int
{
public:
	v_value_int() {}
	v_value_int(IntVariable *v, const int a) :v_(v), a_(a) {}
	~v_value_int() {}

	IntVariable* v() const { return v_; }
	void v(IntVariable* val) { v_ = val; }

	int a() const { return a_; }
	void a(int val) { a_ = val; }
private:
	IntVariable* v_;
	int a_;
};

class c_value_int
{
public:
	c_value_int() {}
	c_value_int(Constraint* c, IntVariable *v, const  int a) : c_(c), v_(v), a_(a) {}
	c_value_int(Constraint* c, v_value_int& va) :c_(c), v_(va.v()), a_(va.a()) {}
	c_value_int(arc& rc, const int a) :c_(rc.c()), v_(rc.v()), a_(a) {}

	virtual ~c_value_int() {}

	Constraint* c() const { return c_; }
	void c(Constraint* c) { c_ = c; }


	cp::IntVariable* v() const { return v_; }
	void v(cp::IntVariable* val) { v_ = val; }

	int a() const { return a_; }
	void a(int val) { a_ = val; }

	arc get_arc() const { return arc(c_, v_); }
	v_value_int get_v_value() const { return v_value_int(v_, a_); }

	const c_value_int& operator=(const c_value_int& rhs)
	{
		c_ = rhs.c_;
		v_ = rhs.v_;
		a_ = rhs.a_;

		return *this;
	}

private:
	Constraint* c_;
	IntVariable* v_;
	int a_;
};

class Network
{
public:
	Network(const int max_arity) :max_arity_(max_arity) {}

	virtual ~Network() {}

	void MakeVar(const int id, const int *values, const int size);

	void MakeTab(const int id, const std::vector<IntVariable *>& scope, const int** const ts, const int len);

	size_t vars_size() { return vars_.size(); }

	size_t cons_size() { return cons_.size(); }

	std::vector<IntVariable*> vars_;
	std::vector<Constraint*> cons_;

	size_t max_arity() const { return max_arity_; }
	void max_arity(int val) { max_arity_ = val; }
private:
	size_t max_arity_ = 0;
};

}/*namespace cp*/
