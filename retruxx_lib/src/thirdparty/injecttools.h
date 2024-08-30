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

#define RETRUXX_DLL_INJECT_FUNCTION(address, function) namespace { bool CONCAT(_injected, __LINE__) = inject::injectFunctionCall(address, inject::cast<uint32_t>(&function)); }
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
#define RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_VIRTUAL_FUNCTION(address, function)

#endif //RETRUXX_DLL
