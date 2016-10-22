#include "CPAlgorithm.h"

namespace cp
{

//VarEvt::VarEvt(const VarEvt& ve) :
//	sig_(std::vector <unsigned>(ve.sig_))
//{}
//
//VarEvt::VarEvt(Network *nt)
//{
//	sig_.resize(nt->vars_size(), 1);
//}

//template<Network* nt_>
//VarEvt<v>::VarEvt():size_(nt_->vars_size()),cur_size_(nt_->vars_size())
//{
//
//}
//template<Network * nt_>
//inline VarEvt<nt_>::VarEvt() :
//	size_(nt_->vars_size()),
//	cur_size_(nt_->vars_size())
//{
//	vars_ = new IntVar*[size_];
//}
//
//template<Network * nt_>
//VarEvt<nt_>::~VarEvt()
//{
//	delete[] vars_;
//	delete vars_;
//}

//
//template<IntVar * v>
//VarEvt<v>::~VarEvt()
//{
//}
VarEvt::VarEvt(Network* nt_) :
	size_(nt_->vars_size()),
	cur_size_(0)
{
	vars_ = new IntVar*[size_];
	for (IntVar* v : nt_->vars_)
		push_back(v);
}

IntVar* VarEvt::operator[](const int i)
{
	return vars_[i];
}

int VarEvt::size() const
{
	return cur_size_;
}

cp::IntVar* VarEvt::at(const int i)
{
	return vars_[i];
}

VarEvt::~VarEvt()
{
	delete[] vars_;
}

void cp::VarEvt::push_back(IntVar* v)
{
	vars_[cur_size_] = v;
	++cur_size_;
}

void cp::VarEvt::clear()
{
	cur_size_ = 0;
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

AssignedStack::AssignedStack(Network * nt) :
	nt_(nt),
	size_(nt_->vars_size())
{
	vars_ = new IntVar*[size_];
	a_ = new int[size_];
}

AssignedStack::~AssignedStack()
{
	delete[] vars_;
	delete[] a_;
}

void AssignedStack::push(v_value_int & v_a)
{
	vars_[lvl_] = v_a.v();
	a_[lvl_] = v_a.a();
	++lvl_;
}

v_value_int AssignedStack::pop()
{
	--lvl_;
	v_value_int v_a(vars_[lvl_], a_[lvl_]);
	return v_a;
}

v_value_int AssignedStack::top() const
{
	return v_value_int(vars_[lvl_], a_[lvl_]);
}

v_value_int AssignedStack::operator[](const int i) const
{
	return v_value_int(vars_[i], a_[i]);
}

v_value_int AssignedStack::at(const int i) const
{
	return v_value_int(vars_[i], a_[i]);
}

std::ostream & operator<<(std::ostream & os, AssignedStack & I)
{
	for (int i = 0; i < I.size(); ++i)
		os << I[i] << "\t";
	os << std::endl;

	return os;
}

}
