
#ifndef LIST_H
#define LIST_H

#ifndef ASSERT
#include <assert.h>
#define ASSERT assert
#endif

#include <iostream>
using std::cout;

template<typename _Tp>
struct ListItem {
        _Tp _data;
        ListItem<_Tp> *_next;
};

template<typename _Tp>
struct List_Iterator {
    typedef std::forward_iterator_tag iterator_category;
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;

    typedef ListItem<_Tp> Node;

    Node *ptr;

    List_Iterator(Node *p=0): ptr(p) {}

    reference operator*() const { return ptr->_data; }

    pointer operator->() { return &(operator *()); }

    List_Iterator& operator++ () {
        ptr = ptr->_next;
        return *this;
    }

    bool operator== (const List_Iterator& i) {
        return ptr == i.ptr;
    }

    bool operator!= (const List_Iterator& i) {
        return ptr != i.ptr;
    }
};

template <typename _Tp, typename _Alloc=std::allocator<_Tp> >
class list {
public:
    typedef List_Iterator<_Tp> iterator;
    typedef ListItem<_Tp> Node;

    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
    typedef size_t size_type;

    typedef typename _Alloc::template rebind<ListItem<_Tp> >::other _Node_alloc_type;
    typedef typename _Alloc::template rebind<_Tp>::other  _Tp_alloc_type;

    _Node_alloc_type _M_node_alloc;

    _Tp_alloc_type _M_get_Tp_allocator() const { 
        return _Tp_alloc_type(_M_node_alloc); 
    }

    list(): _M_node_alloc(), _front(0), _end(0), _size(0) {}

    ~list();

    iterator begin() const { return iterator(_front); }

    iterator end() const { return iterator(_end); }

    void push_front(_Tp _value);

    value_type pop_front();

    iterator erase(iterator _position);

    void empty();

    size_type size() const { return _size; }

private:
    Node * _front;
    Node * _end;
    size_type _size;
};

// TODO: return type cannot be cite as internal defined: such as pointer, iterator
template<typename _Tp, typename _Alloc>
List_Iterator<_Tp> list<_Tp,_Alloc>::erase(iterator _position) {
    ASSERT(_position!=NULL && _position.ptr!=NULL);
    iterator _ret = iterator(_position.ptr->_next);
    Node *_tmp = _front, *_last;

    while (_tmp!=NULL && _tmp!=_position.ptr) {
        _last = _tmp;
        _tmp = _tmp->_next;
    }
    if (_tmp!=NULL) {
        _last->_next = _tmp->_next;
        _size--;
        _M_get_Tp_allocator().destroy(&(*_position));
        _M_node_alloc.deallocate(_position.ptr,1);
        return _ret;
    } else {
        return NULL;
    }
}

template<typename _Tp, typename _Alloc>
void list<_Tp,_Alloc>::empty() {
    while (_front != _end) {
        Node *_tmp = _front;
        _front = _front->_next;
        _M_get_Tp_allocator().destroy(&(_tmp->_data));
        _M_node_alloc.deallocate(_tmp,1);
    }
    _size = 0;
}

template<typename _Tp, typename _Alloc>
list<_Tp,_Alloc>::~list() {
    empty();
}

template<typename _Tp, typename _Alloc>
void list<_Tp,_Alloc>::push_front(_Tp _value) {
    Node *tmp = _M_node_alloc.allocate(1);
    _M_get_Tp_allocator().construct(&(tmp->_data), _value);
    tmp->_next = _front;
    _front = tmp;
    _size++;
}

template<typename _Tp, typename _Alloc>
_Tp list<_Tp,_Alloc>::pop_front() {
    value_type value = _front->_data;
    Node *_tmp = _front;
    _front = _front->_next;
    _size--;
    _M_get_Tp_allocator().destroy(&(_tmp->_data));
    _M_node_alloc.deallocate(_tmp,1);
    return value;
}

#endif
