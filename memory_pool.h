
#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <new>

#ifndef ASSERT
#include <assert.h>
#define ASSERT assert
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef DEBUG
#include <typeinfo>
#include <iostream>
using std::cout;
#endif

template<typename _Tp, size_t N>
class MyPool 
{
public:
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
    typedef const _Tp* const_pointer;
    typedef const _Tp& const_reference;
    typedef size_t size_type;

    template<typename _Tp1>
    struct rebind { 
        typedef MyPool<_Tp1, N> other;
    };

    MyPool(): _list(NULL), _root(NULL) { 
#ifdef DEBUG
        cout<<"MyPool<"<<typeid(_Tp).name()<<", N> Constructed!\n";
#endif
    }

    MyPool(const MyPool&) {}

    // When construct rebinded allocator, only the methods are used. 
    // To avoid invalid free, initialized new object to NULL memory.
    template<typename _Tp1>
    MyPool(const MyPool<_Tp1, N>& pool): _list(NULL), _root(NULL) { 
#ifdef DEBUG
        cout<<"Rebind MyPool<"<<typeid(_Tp).name()<<", N> Constructed!\n";
#endif
    }

    ~MyPool() {
        if (_root != NULL)
            ::operator delete[] (_root); 
    }

    pointer allocate(size_type _n, const void*);

    void deallocate(pointer _p, size_type);

    void construct(pointer _p, const _Tp& _val);

    void destroy(pointer _p);

private:
    template<typename _Tp1>
    struct _Node {
        _Tp1 data; // place the first in the structure
        _Node<_Tp1> * _next;
        _Node() { this->_next = (this + 1); }
    };

    _Node<_Tp> * _list;
    _Node<_Tp> * _root;
};

template<typename _Tp, size_t N>
_Tp * MyPool<_Tp,N>::allocate(size_type _n, const void* =0)
{
    ASSERT(_n == 1); // could only allocate one obj perl time
    if (_list == NULL) {
        ASSERT(_root == NULL);
        // Note the following new statement will construct _Node.data, too, 
        // which is in fact not expected.
        _root = ::new _Node<_Tp>[N];
#if 1
cout<<"Allocated "<<N<<" elements, with total memory "<<sizeof( _Node<_Tp> )*N<<" Bytes\n\n";
#endif
        //_root = static_cast<_Node<_Tp> *>(::operator new(N*sizeof(_Node<_Tp>)));
        ASSERT(_root != NULL);
        _list = _root;
        _Node<_Tp> *_tmp = static_cast<_Node<_Tp> *>(_list + N - 1);
        _tmp->_next = NULL;
    }
    _Node<_Tp> *_p = _list;
    _list = static_cast<_Node<_Tp> *>(_p->_next);

    return static_cast<pointer>(&(_p->data));
}

template<typename _Tp, size_t N>
void MyPool<_Tp,N>::deallocate(pointer _p, size_type)
{
    _Node<_Tp> *_tmp = reinterpret_cast<_Node<_Tp> *>(_p);
    _tmp->_next = _list;
    _list = _tmp;
}

template<typename _Tp, size_t N>
void MyPool<_Tp,N>::construct(pointer _p, const _Tp& _val)
{
    ::new((void *)_p) _Tp(_val);
}

template<typename _Tp, size_t N>
void MyPool<_Tp,N>::destroy(pointer _p)
{
    _p->~_Tp();
}

#endif
