#pragma once
#ifdef RETRUXX_DLL
#include "stl/xmemory.hpp"

namespace retruxx
{
    void deallocate_impl(void* ptr, size_t count);
    void* allocate_impl(size_t count);

    template<class _Ty>
    class allocator
        : public oldstd::_Allocator_base<_Ty>
    {	// generic allocator for objects of class _Ty
    public:
        typedef oldstd::_Allocator_base<_Ty> _Mybase;
        typedef typename _Mybase::value_type value_type;


        typedef value_type _FARQ* pointer;
        typedef value_type _FARQ& reference;
        typedef const value_type _FARQ* const_pointer;
        typedef const value_type _FARQ& const_reference;

        typedef _SIZT size_type;
        typedef _PDFT difference_type;

        template<class _Other>
        struct rebind
        {	// convert an allocator<_Ty> to an allocator <_Other>
            typedef allocator<_Other> other;
        };

        pointer address(reference _Val) const
        {	// return address of mutable _Val
            return (&_Val);
        }

        const_pointer address(const_reference _Val) const
        {	// return address of nonmutable _Val
            return (&_Val);
        }

        allocator()
        {	// construct default allocator (do nothing)
        }

        allocator(const allocator<_Ty>&)
        {	// construct by copying (do nothing)
        }

        template<class _Other>
        allocator(const allocator<_Other>&)
        {	// construct from a related allocator (do nothing)
        }

        template<class _Other>
        allocator<_Ty>& operator=(const allocator<_Other>&)
        {	// assign from a related allocator (do nothing)
            return (*this);
        }

        void deallocate(pointer _Ptr, size_type)
        {	// deallocate object at _Ptr, ignore size
            operator delete(_Ptr);
        }

        pointer allocate(size_type _Count)
        {	// allocate array of _Count elements
            return (oldstd::_Allocate(_Count, (pointer)0));
        }


        //void deallocate(pointer _Ptr, size_type _Count)
        //{	// deallocate object at _Ptr, ignore size
        //    deallocate_impl(_Ptr, _Count);
        //}
        //
        //pointer allocate(size_type _Count)
        //{	// allocate array of _Count elements
        //    return (pointer)allocate_impl(_Count);
        //}

        pointer allocate(size_type _Count, const void _FARQ*)
        {	// allocate array of _Count elements, ignore hint
            return (allocate(_Count));
        }

        void construct(pointer _Ptr, const _Ty& _Val)
        {	// construct object at _Ptr with value _Val
            //if (_Ptr)
            //{
            //    (_Ptr)-> _Ty(_Val);
            //}
            oldstd::_Construct(_Ptr, _Val);
        }

        void destroy(pointer _Ptr)
        {	// destroy object at _Ptr
            oldstd::_Destroy(_Ptr);
            //(_Ptr)->~_Ty();

        }

        _SIZT max_size() const
        {	// estimate maximum array size
            _SIZT _Count = (_SIZT)(-1) / sizeof(_Ty);
            return (0 < _Count ? _Count : 1);
        }
       
};

    template<class _Ty>
    bool operator==(const allocator<_Ty>&, const allocator<_Ty>&)
    {
        return true;
    }
}

#else //RETRUXX_DLL

#endif //RETRUXX_DLL
