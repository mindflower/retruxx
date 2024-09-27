#pragma once
#ifdef RETRUXX_DLL

#include "functionscrapper.h"
#include <windows.h>
#include <cstdint>


namespace inject
{
    bool injectFunctionCall(uint32_t address, uint32_t newFunctionAddress);
    bool injectJmpShort(uint32_t from, uint32_t to);
    bool replaceVtableEntry(uint32_t entryAddress, uint32_t newFunctionAddress);
    bool replaceCondJmpToJmp(uint32_t address);

    template <class Out, class In>
    Out cast(In x)
    {
        union
        {
            In a;
            Out b;
        };
        a = x;
        return b;
    };
}
#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#define RETRUXX_DLL_FRIEND_CLASS(cl) public: template<typename Tag> \
                                     static inline uint32_t accessPrivate()  \
                                     {                      \
                                         return 0;    \
                                     } private:

#define RETRUXX_DLL_DEFINE_ACCESSOR(cls, method) namespace { struct CONCAT(_Accessor, __LINE__) {}; } template<> static inline uint32_t cls::accessPrivate<CONCAT(_Accessor, __LINE__)>() { return inject::cast<uint32_t>(&cls::method); };
#define RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method) cls::accessPrivate<CONCAT(_Accessor, __LINE__)>()

#define RETRUXX_DLL_INJECT_CLASS_METHOD(address, cls, method) RETRUXX_DLL_DEFINE_ACCESSOR(cls, method); namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method)); }
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(address, cls, method) RETRUXX_DLL_DEFINE_ACCESSOR(cls, method); namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method), address); }

#define RETRUXX_DLL_INJECT_FUNCTION(address, function) namespace {bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(&function)); }
#define RETRUXX_DLL_INJECT_FUNCTION_TYPED(address, function, type) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(static_cast<type>(&function))); }
#define RETRUXX_DLL_INJECT_VIRTUAL_FUNCTION(address, function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(inject::FunctionScrapper::addressOfVirtual(&function))); }
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(address, function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(inject::cast<uint32_t>(&function), address); }
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(address, function, type) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(inject::cast<uint32_t>(static_cast<type>(&function)), address); }
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_VIRTUAL_FUNCTION(address, function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(inject::cast<uint32_t>(inject::FunctionScrapper::addressOfVirtual(&function)), address); }

#else //RETRUXX_DLL

#define RETRUXX_DLL_INJECT_FUNCTION(address, funcName)
#define RETRUXX_DLL_INJECT_FUNCTION_TYPED(address, function, type)
#define RETRUXX_DLL_INJECT_VIRTUAL_FUNCTION(address, function)
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(address, function)
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(address, function, type)
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_VIRTUAL_FUNCTION(address, function)

#endif //RETRUXX_DLL
