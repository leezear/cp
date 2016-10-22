#pragma once
#include "Model.h"
namespace cp
{

enum PropagateScheme
{
	ARC_ORIENTED,
	VAR_ORIENTED
};
//template<class T>
//class pro_que
//{
//public:
//#define  have(a) vid_set_[a.c_id() * arity_ + a.c()->index(a.v())]
//	pro_que() {}
//	pro_que(const int cons_size, const int max_arity);
//	virtual ~pro_que();
//
//	virtual void MakeQue(const size_t cons_size, const size_t max_arity) = 0;
//	virtual bool empty() const;
//	virtual bool full() const;
//	virtual bool push(T& ele) throw(std::bad_exception) = 0;
//	virtual T pop() throw(std::bad_exception) = 0;
//
//private:
//	T* m_data_;
//	int* vid_set_;
//	size_t arity_;
//	size_t m_size_;
//	int m_front_;
//	int m_rear_;
//
//};

template<class T>
class pro_que
{
public:
	pro_que();
	virtual ~pro_que();

private:

};

}


