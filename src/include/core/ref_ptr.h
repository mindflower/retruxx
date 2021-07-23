#pragma once

template<class T>
class ref_ptr
{
public:
    ref_ptr(T*);
    ref_ptr();
    ~ref_ptr();

    bool operator!() const;
    operator T* ();
    bool operator==(ref_ptr<T> const&) const;
    T* operator->();

private:
    T* m_ptr;
};
