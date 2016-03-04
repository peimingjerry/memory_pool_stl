
#ifndef ITERATOR_TRAITS_H
#define ITERATOR_TRAITS_H

template <typename _Iterator>
struct Iterator_Traits {
    typedef typename _Iterator::value_type value_type;
};

template <typename _Tp>
struct Iterator_Traits<_Tp*> {
    typedef _Tp value_type;
};

template <typename _Tp>
struct Iterator_Traits<const _Tp*> {
    typedef _Tp value_type;
};

#endif
