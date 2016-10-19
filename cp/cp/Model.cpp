#include "Model.h"

namespace cp
{

IntVariable::IntVariable(const int id, const int * values, const int size) :
	Base(id),
	size_(size)
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
	return ptr_ = &tail_;
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
	return cur_size_ == 1;
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

void Constraint::GetFirstValidTuple(v_value_int & v_a, IntTuple & t)
{
	int i = 0;
	for (IntVariable* v : scope_)
	{
		if (v != v_a.v())
			t[i] = v_a.v()->head();
		else
			t[i] = v_a.a();

		++i;
	}
}

Tabular::Tabular(const int id, const std::vector<IntVariable*>& scope, const int ** const ts, const int len) :
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

void Network::MakeTab(const int id, const std::vector<IntVariable *>& scope, const int** const ts, const int len)
{
	Tabular* tb = new Tabular(id, scope, ts, len);
	cons_.push_back(tb);

	for (IntVariable* v : scope)
		v->subscribe(tb);
}


}/*namespace cp*/