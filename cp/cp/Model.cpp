#include "Model.h"

namespace cp
{

IntVariable::IntVariable(const int id, const int * values, const int size) :
	Base(id),
	size_(size),
	cur_size_(size)
{
	vals_ = new int[size];
	absent_ = new int[size];
	next_ = new int[size];
	prev_ = new int[size];
	prev_absent_ = new int[size];

	for (int i = 0; i < size_; ++i)
	{
		vals_[i] = values[i];
		absent_[i] = -1;
		next_[i] = i + 1;
		prev_[i] = i - 1;
		prev_absent_[i] = -1;
	}

	next_[size_ - 1] = -1;
	prev_[0] = -1;
	tail_ = size_ - 1;
	lmt_ = vals_[size_ - 1];
}

IntVariable::~IntVariable()
{
	delete[] vals_;
	delete[] absent_;
	delete[] next_;
	delete[] prev_;
	delete[] prev_absent_;

	vals_ = NULL;
	absent_ = NULL;
	next_ = NULL;
	prev_ = NULL;
	prev_absent_ = NULL;

}

void IntVariable::RemoveValue(const int a, const int p)
{
	--cur_size_;
	absent_[a] = p;
	prev_absent_[a] = tail_absent_;
	tail_absent_ = a;

	(prev_[a] == -1) ? head_ = next_[a] : next_[prev_[a]] = next_[a];
	(next_[a] == -1) ? tail_ = prev_[a] : prev_[next_[a]] = prev_[a];
}

void IntVariable::ReduceTo(const int a, const int p)
{
	int b = head_;
	assigned_ = true;

	while (b != -1)
	{
		if (b != a)
			RemoveValue(b, p);
		b = next_[b];
	}
}

void IntVariable::AddValue(const int a)
{
	absent_[a] = -1;
	tail_absent_ = prev_absent_[a];

	(prev_[a] == -1) ? head_ = a : next_[prev_[a]] = a;
	(next_[a] == -1) ? tail_ = a : prev_[next_[a]] = a;
}

void IntVariable::RestoreUpTo(const int p)
{
	int b = tail_absent_;
	assigned_ = false;

	while (b != -1 && absent_[b] >= p)
	{
		AddValue(b);
		b = prev_absent_[b];
	}
}

int* IntVariable::begin()
{
	ptr_ = &head_;
	return ptr_;
}

int* IntVariable::next()
{
	ptr_ = &next_[*ptr_];
	return ptr_;
}

int* IntVariable::end()
{
	return &tail_;
}

int IntVariable::GetValueByIndex(const int idx) const
{
	return vals_[idx];
}

int IntVariable::size() const
{
	return cur_size_;
}

int IntVariable::capacity() const
{
	return size_;
}

int IntVariable::assigned() const
{
	return assigned_;
}

int IntVariable::next(int a) const
{
	return next_[a];
}

int IntVariable::prev(int a) const
{
	return prev_[a];
}

bool IntVariable::have(int a) const
{
	return absent_[a] == -1;
}

int IntVariable::head() const
{
	return head_;
}

int IntVariable::tail()const
{
	return tail_;
}

bool IntVariable::faild() const
{
	return cur_size_ == 1;
}

void Constraint::GetFirstValidTuple(v_value_int & v_a, IntTuple & t)
{
	for (size_t i = 0; i < arity_; ++i)
		if (scope_[i] != v_a.v())
			t[i] = scope_[i]->head();
		else
			t[i] = v_a.a();
}

void Constraint::GetNextValidTuple(v_value_int& v_a, IntTuple&t)
{
	for (int i = arity_ - 1; i >= 0; --i)
		if (scope_[i] != v_a.v())
			if (scope_[i]->next(t[i]) == -1)
				t[i] = scope_[i]->head();
			else
			{
				t[i] = scope_[i]->next(t[i]);
				return;
			}

	t.exclude();
}

Tabular::Tabular(const int id, const std::vector<IntVariable*>& scope, int **  ts, const int len) :
	Constraint(id, scope, CT_EXT)
{
	for (int i = 0; i < len; ++i)
		ts_ << IntTuple(ts[i], arity_);
}

bool Tabular::sat(IntTuple &t)
{
	return ts_.have(t);
}

void Network::MakeVar(const int id, const int * values, const int size)
{
	IntVariable* v = new IntVariable(id, values, size);
	vars_.push_back(v);
}

void Network::MakeTab(const int id, const std::vector<IntVariable *>& scope, int** ts, const int len)
{
	Tabular* tb = new Tabular(id, scope, ts, len);
	cons_.push_back(tb);

	for (IntVariable* v : scope)
		v->subscribe(tb);
}

void Network::GetFirstValidTuple(c_value_int & c_val, IntTuple& t)
{
	v_value_int v_a(c_val.v(), c_val.a());
	c_val.c()->GetFirstValidTuple(v_a, t);
}

void Network::GetNextValidTuple(c_value_int & c_val, IntTuple& t)
{
	v_value_int v_a(c_val.v(), c_val.a());
	c_val.c()->GetNextValidTuple(v_a, t);
}

const cp::c_value_int& c_value_int::operator=(const c_value_int& rhs)
{
	c_ = rhs.c_;
	v_ = rhs.v_;
	a_ = rhs.a_;

	return *this;
}

}/*namespace cp*/