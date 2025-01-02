#pragma once
#ifdef RETRUXX_DLL
#include "stl/xmemory.hpp"
#include "core/kernel.h"

namespace retruxx
{
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
            M3D_KERNEL->g_mar.FreeMem(_Ptr, 0, 0);
        }

        pointer allocate(size_type _Count)
        {	// allocate array of _Count elements
            return (pointer)M3D_KERNEL->g_mar.AllocMem(_Count, 0, 0);
        }

        pointer allocate(size_type _Count, const void _FARQ*)
        {	// allocate array of _Count elements, ignore hint
            return (allocate(_Count));
        }

        //void construct(pointer _Ptr, const _Ty& _Val)
        //{	// construct object at _Ptr with value _Val
        //    oldstd::_Construct(_Ptr, _Val);
        //}

        //void destroy(pointer _Ptr)
        //{	// destroy object at _Ptr
        //    _Destroy(_Ptr);
        //}

        _SIZT max_size() const
        {	// estimate maximum array size
            _SIZT _Count = (_SIZT)(-1) / sizeof(_Ty);
            return (0 < _Count ? _Count : 1);
        }
};
}

#else //RETRUXX_DLL

#endif //RETRUXX_DLL
