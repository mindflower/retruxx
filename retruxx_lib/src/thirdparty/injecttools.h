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

#define STRINGIFY(Token) #Token

#define MSVC_MANGLE_VTABLE(Class) STRINGIFY(??_7##Class##@@6B@)
#define MSVC_VTABLE(Class) (__pragma(warning(suppress: 4483)) __identifier(MSVC_MANGLE_VTABLE(Class)))

#define MSVC_MANGLE_VTABLE_NAMESPACED(Namespace, Class) STRINGIFY(??_7##Class##@##Namespace##@@6B@)
#define MSVC_VTABLE_NAMESPACED(Namespace, Class) (__pragma(warning(suppress: 4483)) __identifier(MSVC_MANGLE_VTABLE_NAMESPACED(Namespace, Class)))


#define RETRUXX_DLL_FRIEND_CLASS(cl) template<typename Tag> \
                                     friend uint32_t accessPrivate(); friend class inject::FunctionScrapper;

#define RETRUXX_DLL_DEFINE_ACCESSOR(cls, method) namespace { struct CONCAT(_Accessor, __LINE__) {}; } template<typename Tag> uint32_t accessPrivate(); template<> uint32_t accessPrivate<CONCAT(_Accessor, __LINE__)>() { return inject::cast<uint32_t>(&cls::method); };
#define RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method) accessPrivate<CONCAT(_Accessor, __LINE__)>()

#define RETRUXX_DLL_INJECT_CLASS_METHOD(address, cls, method) RETRUXX_DLL_DEFINE_ACCESSOR(cls, method); namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method)); }
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(address, cls, method) RETRUXX_DLL_DEFINE_ACCESSOR(cls, method); namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method), address); }

#define RETRUXX_DLL_INJECT_FUNCTION(address, function) namespace {bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(&function)); }
#define RETRUXX_DLL_INJECT_FUNCTION_TYPED(address, function, type) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(static_cast<type>(&function))); }
//#define RETRUXX_DLL_INJECT_VIRTUAL_FUNCTION(address, function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(inject::FunctionScrapper::addressOfVirtual(&function))); }
#define RETRUXX_DLL_INJECT_VIRTUAL_FUNCITON_NAMESPACED(address, Namespace, Class, Function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(inject::FunctionScrapper::addressOfVirtual((void**)MSVC_VTABLE_NAMESPACED(Namespace, Class), &Namespace::Class::Function))); }

#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(address, function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(inject::cast<uint32_t>(&function), address); }
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(address, function, type) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(inject::cast<uint32_t>(static_cast<type>(&function)), address); }

#define RETRUXX_DLL_INJECT_CTOR(address, cls) namespace {void __fastcall  CONCAT(cls, _)ctor(cls* self) { self->cls::cls(); }}  RETRUXX_DLL_INJECT_FUNCTION(address, CONCAT(cls, _)ctor);
#define RETRUXX_DLL_INJECT_DTOR(address, cls) namespace {void __fastcall  CONCAT(cls, _)dtor(cls* self) { self->cls::~cls(); }} RETRUXX_DLL_INJECT_FUNCTION(address, CONCAT(cls, _)dtor);

#define RETUXX_DLL_JMP_TO_FUNCTION(address, function, ...) return (this->*inject::cast<decltype(&function)>(address))(__VA_ARGS__)

#else //RETRUXX_DLL

#define RETRUXX_DLL_FRIEND_CLASS(cl)
#define RETRUXX_DLL_DEFINE_ACCESSOR(cls, method)
#define RETRUXX_DLL_ACCESS_PRIVATE_METHOD(cls, method)

#define RETRUXX_DLL_INJECT_CLASS_METHOD(address, cls, method)
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_CLASS_METHOD(address, cls, method)

#define RETRUXX_DLL_INJECT_FUNCTION(address, funcName)
#define RETRUXX_DLL_INJECT_FUNCTION_TYPED(address, function, type)
//#define RETRUXX_DLL_INJECT_VIRTUAL_FUNCTION(address, function)
#define RETRUXX_DLL_INJECT_VIRTUAL_FUNCITON_NAMESPACED(address, Namespace, Class, Function)

#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(address, function)
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION_TYPED(address, function, type)

#define RETRUXX_DLL_INJECT_CTOR(address, cls)
#define RETRUXX_DLL_INJECT_DTOR(address, cls)

#endif //RETRUXX_DLL
