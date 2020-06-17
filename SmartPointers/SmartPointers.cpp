/*
 * SmartPointers.cpp
 *
 *  Created on: Jun. 15, 2020
 *      Author: ckirsch
 */
#include "SmartPointers.h"
#include <iostream>

using namespace std;

class Person
{
    string name;
    int age;

public:
    Person(const string &name, int age) : name(name), age(age) {} // constructor
    Person(const Person &) = default; // Copy constructor
    Person(Person &&) noexcept = default; // Move constructor

    Person& operator=(const Person &) = default; // Copy Assignment
    Person& operator=(Person &&) noexcept = default; // Move assignment
    ~Person() noexcept = default; // Destructor
};


template <class T>
class UniquePointer
{
    T* pointer;

public:
    void swap(UniquePointer &src) noexcept
    {
        std::swap(pointer, src.pointer);
    }

    UniquePointer() : pointer(nullptr) {}

    // Explicit Constructor
    explicit UniquePointer(T* pointer) : pointer(pointer) {}
    ~UniquePointer() { if (pointer != nullptr) delete pointer; }

    /* Constructor/Assignment that binds to nullptr
     * This makes usage with nullptr cleaner */
    UniquePointer(nullptr_t) : pointer(nullptr) {}
    UniquePointer& operator=(nullptr_t)
    {
        reset();
        return *this;
    }

    /* Constructor/Assignment that allows move semantics */
    UniquePointer(UniquePointer &&moving) noexcept
    {
        moving.swap(*this);
    }
    UniquePointer& operator=(UniquePointer &&moving) noexcept
    {
        moving.swap(*this);
        return *this;
    }

   /* // Constructor/Assignment for use with types derived from T
    template<typename U>
    UniquePointer(UniquePointer<U> &&moving)
    {
        UniquePointer<T> tmp(moving.release());
        tmp.swap(*this);
    }
    template<typename U>
    UniquePointer& operator=(UniquePointer<U>&& moving)
    {
        UniquePointer<T> tmp(moving.release());
        moving.swap(*this);
        return *this;
    }*/

    // Remove comipler generated copy semantics
    UniquePointer(const UniquePointer &)            = delete;
    UniquePointer& operator=(const UniquePointer &) = delete;

    // const correct access owned object
    T* operator->() const { return pointer; }
    T& operator*() const { return *pointer; }

    // Access smart pointer state
    T* get()                 const {return pointer;}
    explicit operator bool() const { return pointer; } // For conditional expression of is set

    // Modify object state
    T* release() noexcept
    {
        T* releasePointer = nullptr;
        std::swap(releasePointer, pointer);
        return releasePointer;
    }
    void reset()
    {
        T* tmp = release();
        delete tmp;
    }
};
/*template<typename T>
void swap(UniquePointer<T> &lhs, UniquePointer<T> &rhs)
{
    lhs.swap(rhs);
}*/


template <class T>
class SmartPointer
{
    T *pointer;
    int *refCount;

public:
    SmartPointer() : pointer(nullptr), refCount(nullptr) {}
    SmartPointer(nullptr_t) : pointer(nullptr), refCount(nullptr) {}

    explicit SmartPointer(T *pointer) : pointer(pointer), refCount(new (std::nothrow) int(1))
    {
        // Check if pointer correctly allocated
        if (refCount == nullptr)
        {
            // If we failed then delete the pointer and manually throw an exception
            delete pointer;
            throw std::bad_alloc();
        }
    }

    ~SmartPointer()
    {
        if (refCount != nullptr)
        {
            --(*refCount);
            if (*refCount <= 0)
            {
                delete pointer;
                delete refCount;
            }
        }
    }

    // Implement rule of three destructor for memory manage => copy and assignment
    SmartPointer(const SmartPointer &copy) : pointer(copy.pointer), refCount(copy.refCount)
    {
        ++(*refCount);
    }

    /* Copy and Swap Idiom
     * SmartPointer& operator=(const SmartPointer &rhs)
    {
        // Keep a copy of old data
        T* oldPointer = pointer;
        int* oldRefCount = refCount;

        // Now do an exception safe transfer
        pointer = rhs.pointer;
        refCount = rhs.refCount;

        // update counters
        --(*oldRefCount);
        ++(*refCount);

        // Finally delete old pointer if required
        if (*oldRefCount <= 0)
        {
            delete oldPointer;
            delete oldRefCount;
        }
    }*/
    SmartPointer& operator=(SmartPointer rhs) // Implicate copy due to pass by value
    {
        rhs.swap(*this);
        return *this;
    }
    SmartPointer& operator=(T* pointer)
    {
        SmartPointer tmp(pointer);
        tmp.swap(*this);
        return *this;
    }

    // Always good to have a swap function
    // Make sure it is noexcept
    void swap(SmartPointer &other) noexcept
    {
        std::swap(pointer, other.pointer);
        std::swap(refCount, other.refCount);
    }

    // Const correct access owned object
    T* operator->() const { return pointer; }
    T& operator*()  const { return *pointer; }

    // Access to smart pointer state
    T* get()                 const { return pointer; }
    explicit operator bool() const { return pointer; }
};


class RC
{
    int count;

public:
    void AddRef()
    {
        ++count;
    }

    int Release()
    {
        return --count;
    }
};

template <class T>
class SP
{
    T* ptr;
    RC *reference;

public:
    SP(T *ptr = nullptr) : ptr(ptr), reference(new (nothrow) RC())
    {
        if (nullptr == reference)
        {
            throw std::bad_alloc();
        }
        reference->AddRef();
    }
    ~SP()
    {
        if (nullptr != reference && reference->Release() == 0)
        {
            if (nullptr != ptr) delete ptr;
            delete reference;
        }
    }

    T& operator*() const
    {
        if (nullptr == ptr) throw;
       return *ptr;
    }

    T* operator->() const
    {
        if (nullptr == ptr) throw;
        return ptr;
    }

    SP<T>& operator=(const SP<T> &sp)
    {
        if (this == &sp) // avoid self assignment
        {
            if (reference->Release() == 0)
            {
                delete ptr;
                delete reference;
            }

            ptr = sp.ptr;
            reference = sp.reference;
            reference->AddRef();
        }

        return *this;
    }
};


// Book Implementation
template <class T>
class SmartPointerBook
{
    /* The smart pointer class needs pointers to both the object itself and to the reference count.
     * These must be pointers rather than actual objects or ref count value, since the goal of a
     * smart pointer is that the reference count is tracked across multiple smart pointers to one
     * object */

    T* obj;
    unsigned int *refCount;

protected:
    void remove()
    {
        --(*refCount);
        if (*refCount == 0)
        {
            delete refCount;
            delete obj;

            obj = nullptr;
            refCount = nullptr;
        }
    }

public:
    /* We want to set the value of T* object, and set the reference counter to 1 */
    SmartPointerBook(T* object) : obj(object), refCount(new (nothrow) unsigned int(1)) {}

    /* This constructor creates a new smart pointer that points to an existing object. We will need
     * to first set object and ref count to pointer to sptr's obj and ref count. Then, because we
     * created a new reference to obj, we need to increment ref_count. */
    SmartPointerBook(SmartPointerBook<T> &sptr)
    {
        refCount = sptr.refCount;
        obj = sptr.obj;
        ++(*refCount);
    }

    /* Override the equal operator, so that when you set one smart pointer equal to another
     * the old smart pointer has its reference count decremented and then new smart pointer has
     * its reference count increamented */
    SmartPointerBook<T>& operator=(SmartPointerBook<T> &sptr)
    {
        if (this == &sptr) return *this;

        /* If already assigned to an object, remove one reference */
        if (*refCount > 0)
        {
           remove();
        }

        obj = sptr.obj;
        refCount = sptr.refCount;
        ++(*refCount);
        return *this;
    }

    /* We are destroying a reference to the object. Decrement ref count. If ref count is 0, then free
     * the memory created by the integer and destroy the object */
    ~SmartPointerBook()
    {
        remove();
    }

    T getValue() { return *obj; }

    T& operator*() {return *obj;}
    T* operator->() {return obj;}
};


void SmartPointersMain()
{
    UniquePointer<int> x(new int());
    UniquePointer<int> y(nullptr);

    y = std::move(x);
    UniquePointer<int> z(std::move(y));

    SP<int> a(new int());
    {
        SP<int> q = a;
        SP<int> r;
        r = a;
    }
}

