#include "CPAlgorithm.h"

namespace cp
{

VarEvt::VarEvt(const VarEvt& ve) :
	sig_(std::vector <unsigned>(ve.sig_))
{}

VarEvt::VarEvt(Network *nt)
{
	sig_.resize(nt->vars_size(), 1);
}

arc_que::arc_que(const int cons_size, const int max_arity) :
	arity_(max_arity),
	m_size_(max_arity*cons_size + 1),
	m_front_(0),
	m_rear_(0)
{
	m_data_ = new arc[m_size_];
	vid_set_ = new int[m_size_]();
}

arc_que::~arc_que()
{
	delete[] m_data_;
	delete[] vid_set_;
	m_data_ = NULL;
	vid_set_ = NULL;
	arity_ = 0;
}

void arc_que::MakeQue(const size_t cons_size, const size_t max_arity)
{
	arity_ = max_arity;
	m_size_ = max_arity*cons_size + 1;
	m_front_ = 0;
	m_rear_ = 0;

	m_data_ = new arc[m_size_];
	vid_set_ = new int[m_size_]();
}

bool arc_que::empty() const
{
	return m_front_ == m_rear_;
}

bool arc_que::full() const
{
	return m_front_ == (m_rear_ + 1) % m_size_;
}

bool arc_que::push(arc& ele) throw(std::bad_exception)
{
	if (full())
		throw std::bad_exception();

	if (have(ele))
		return false;

	m_data_[m_rear_] = ele;
	m_rear_ = (m_rear_ + 1) % m_size_;
	have(ele) = 1;

	return true;
}

arc arc_que::pop() throw(std::bad_exception)
{
	if (empty())
		throw std::bad_exception();

	arc tmp = m_data_[m_front_];
	m_front_ = (m_front_ + 1) % m_size_;
	have(tmp) = 0;

	return tmp;
}

AC::AC(Network *nt) :
	nt_(nt)
{

}

int AC::DeletedCount()
{
	int count = 0;

	for (IntVariable* v : nt_->vars_)
		count += (v->capacity() - v->size());

	return count;
}

}
