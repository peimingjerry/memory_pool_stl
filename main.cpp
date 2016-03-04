
#ifdef USE_STL_LIST
#include <list>
using std::list;
#else
#include "list.h"
#endif

#ifdef USE_STD_ALLOC
#include <new>
#else
#include "memory_pool.h"
#endif

#ifdef USE_STL_ALGO
#include <algorithm>
using std::find;
#else
#include "iterator_algo.h"
#endif

#include <iostream>
using std::cout;

#define MAX_SIZE 30

int main(void)
{
#ifdef USE_STD_ALLOC
    list<int> myList;
#define ITERATOR_TYPE list<int>::iterator
#else
    list<int, MyPool<int, MAX_SIZE> > myList;
#define ITERATOR_TYPE list<int, MyPool<int, MAX_SIZE> >::iterator
#endif

#define DISPLAY {\
    ITERATOR_TYPE iter;\
    cout<<"Totally "<<myList.size()<<" elements in the list:\n";\
    for (iter=myList.begin();iter!=myList.end();++iter)\
        cout<<*iter<<"\t";\
    cout<<"\n\n";\
}

    int i, capacity;
    for (i=0;i<MAX_SIZE;i++) {
        myList.push_front(i);
    }
    DISPLAY

    capacity = 0;
    for (i=0;i<MAX_SIZE;i++) {
        if (i%10 == 0) {
            ITERATOR_TYPE iter = find(myList.begin(), myList.end(), i);
            myList.erase(iter);
            capacity++;
        }
    }
    DISPLAY

    for (i=0;i<capacity;i++) {
        myList.push_front(i + 1000);
    }
    DISPLAY

    return 0;
}
