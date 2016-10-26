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

void VarEvt::push_back(IntVar* v)
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

void AssignedStack::push(IntVal & v_a)
{
	vars_[lvl_] = v_a.v();
	a_[lvl_] = v_a.a();
	vars_[lvl_]->assigned(true);
	++lvl_;
}

IntVal AssignedStack::pop()
{
	--lvl_;
	IntVal v_a(vars_[lvl_], a_[lvl_]);
	vars_[lvl_]->assigned(false);
	return v_a;
}

IntVal AssignedStack::top() const
{
	return IntVal(vars_[lvl_], a_[lvl_]);
}

IntVal AssignedStack::operator[](const int i) const
{
	return IntVal(vars_[i], a_[i]);
}

IntVal AssignedStack::at(const int i) const
{
	return IntVal(vars_[i], a_[i]);
}

std::ostream & operator<<(std::ostream & os, AssignedStack & I)
{
	for (int i = 0; i < I.size(); ++i)
		os << "(" << I[i].v()->id() << ", " << I[i].a() << ")\t";

	return os;
}

std::ostream & operator<<(std::ostream & os, AssignedStack * I)
{
	for (int i = 0; i < I->size(); ++i)
		os << "(" << I->at(i).v()->id() << ", " << I->at(i).a() << ")\t";

	return os;
}

inline VarList<IntVal>::VarList(Network * nt_)
{
	nodes_ = new LinkedNode<IntVal>[size_];

	for (IntVar* v : nt_->vars_)
	{
		nodes_[v->id()].data = IntVal(v, Limits::UNSIGNED_VAL);
		nodes_[v->id()].absent = Limits::ABSENT;
	}

	head_ = 0;
	tail_ = 0;
	nodes_[0].prev = Limits::INDEX_OVERFLOW;
	nodes_[tail_].next = Limits::INDEX_OVERFLOW;
	cur_size_ = 0;
}

inline VarList<IntVar*>::VarList(Network * nt_)
{
	nodes_ = new LinkedNode<IntVar*>[size_];

	for (IntVar* v : nt_->vars_)
	{
		nodes_[v->id()].data = v;
		nodes_[v->id()].absent = Limits::PRESENT;
		nodes_[v->id()].next = v->id() + 1;
		nodes_[v->id()].prev = v->id() - 1;
	}

	head_ = 0;
	tail_ = size_ - 1;
	nodes_[0].prev = Limits::INDEX_OVERFLOW;
	nodes_[tail_].next = Limits::INDEX_OVERFLOW;
	cur_size_ = nt_->vars_size();
}

template<typename T>
VarList<T>::~VarList()
{
	delete[] nodes_;
}

template<class T>
const T VarList<T>::operator[](const int i)
{
	return nodes_[i].data;
}

//void VarList<IntVar*>::push_back(IntVar* v)
//{
//	const int vid = v->id();
//
//	if (full())
//		++cur_size_;
//	else if (empty())
//		head_ = vid;
//	else
//		nodes_[tail_].next = vid;
//
//	nodes_[vid].prev = tail_;
//	nodes_[vid].next = Limits::INDEX_OVERFLOW;
//	tail_ = vid;
//}
template<class T>
void VarList<T>::push_back(T t)
{
	const int vid = v->id();

	if (full())
		++cur_size_;
	else if (empty())
		head_ = vid;
	else
		nodes_[tail_].next = vid;

	nodes_[vid].data = t;
	nodes_[vid].absent = Limits::PRESENT;
	nodes_[vid].prev = tail_;
	nodes_[vid].next = Limits::INDEX_OVERFLOW;
	tail_ = vid;
}

template<class T>
T VarList<T>::pop_back()
{
	--cur_size_;
	T t = nodes_[tail_].data;
	nodes_[tail_].absent = Limits::ABSENT;
	tail_ = nodes_[tail_].prev;
	nodes_[tail_].next = Limits::INDEX_OVERFLOW;
	return t;
}

template<class T>
T VarList<T>::pop_front()
{
	--cur_size_;
	T t = nodes_[head_].data;
	nodes_[head_].absent = Limits::ABSENT;
	head_ = nodes_[head_].next;
	nodes_[head_].prev = Limits::INDEX_OVERFLOW;
}

template<class T>
const bool VarList<T>::full()
{
	return cur_size_ == size_;
}

template<class T>
const bool VarList<T>::empty()
{
	return cur_size_ == 0;
}

template<class T>
void VarList<T>::clear()
{
	head_ = Limits::INDEX_OVERFLOW;
	tail_ = head_;
}

template<class T>
T VarList<T>::top() const
{
	return nodes_[tail_].data;
}

void VarList<IntVal>::del(IntVal & v_a)
{
	const int vid = v_a.v()->id();

	if (vid == head_)
		pop_front();
	else if (vid == tail_)
		pop_back();
	else
	{
		nodes_[nodes_[vid].next].prev = nodes_[vid].prev;
		nodes_[nodes_[vid].prev].next = nodes_[vid].next;
		--cur_size_;
	}

}

template<class T>
T VarList<T>::at(const int i)
{
	return nodes_[i].data;
}








//void VarList<IntVal>::generateNode()

//IntVar * VarList<IntVar>::operator[](const int i)
//{
//	//return nodes_[i].data.v();
//}

//template<typename T>
//void VarList<T>::push_back(T& v)
//{
//}
//
//template<class T>
//void VarList<T>::clear()
//{
//	cur_size_ = 0;
//}

//void VarList<IntVar>::generateNode(IntVar* const v)
//{
//	nodes_[v->id()].data = v;
//	nodes_[v->id()].absent = Limits::INDEX_ABSENT;
//	nodes_[v->id()].next = v->id() + 1;
//	nodes_[v->id()].prev = v->id() - 1;
//}

}
