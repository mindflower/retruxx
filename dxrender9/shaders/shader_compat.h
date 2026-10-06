#pragma once
// retruxx adaptation (no original counterpart): compiling the original shaders with a current D3DX.
//
// The original compiled its shaders with the D3DX of its own SDK. d3dx9_43's compiler no
// longer accepts the ps_1_x profiles the shaders use ("error X3539"), and its backwards
// compatibility mode compiles them as ps_2_0 with different semantics: texture coordinates read
// as values are no longer clamped to [0,1], compares are exact instead of ps_1_x's cnd, and
// samplers are bound in order of first use (an unused sampler is dropped) instead of declaration
// order - lsdetailedshadows.fx then draws its shadow overlay outside the shadow texture (stretched
// edge) and landscapefp_ps11.ps samples the wrong stages.
//
// d3dx9_43 can delegate compilation to the April 2006 compiler, d3dx9_31.dll, with
// D3DXSHADER_USE_LEGACY_D3DX9_31_DLL; that DLL is part of the DirectX end-user runtime the game
// installs, and its output is the original ps_1_x / vs_1_x code (verified against the 2005
// compiler's: identical pixel shader instruction streams), which the D3D9 runtime still executes.
// When d3dx9_31.dll is not installed the fallback is the compatibility mode with the preprocessed
// source rewritten in memory so that samplers get their registers in declaration order. The
// shader files are never touched.
#include <windows.h>

#include <cstring>
#include <string>

namespace hlsl_compat
{
    // True when the April 2006 compiler (d3dx9_31.dll) is installed, so the shaders can be compiled
    // with D3DXSHADER_USE_LEGACY_D3DX9_31_DLL.
    inline bool LegacyCompilerAvailable()
    {
        static int state = -1;
        if (state < 0)
        {
            HMODULE h = LoadLibraryA("d3dx9_31.dll");
            state = h ? 1 : 0;
            if (h)
                FreeLibrary(h);
        }
        return state == 1;
    }

    inline bool isIdentChar(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
    }

    inline bool isSpace(char c)
    {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    // One `sampler[1D|2D|3D|CUBE] <name>` declaration at statement level: the position right after the
    // name (where `: register(sN)` would go), whether it already has one, and which register.
    struct SamplerDecl
    {
        size_t afterName;
        bool hasRegister;
        int reg;
    };

    // Finds the next sampler declaration at or after `pos`; returns false when there is none.
    inline bool findSamplerDecl(std::string const& src, size_t pos, SamplerDecl& decl, size_t& next)
    {
        while ((pos = src.find("sampler", pos)) != std::string::npos)
        {
            size_t p = pos + 7;
            if (pos > 0 && isIdentChar(src[pos - 1]))
            {
                pos = p;
                continue;
            }
            // the type suffix (1D, 2D, 3D, CUBE); `sampler_state` has none and fails the whitespace test
            while (p < src.size() && isIdentChar(src[p]) && src[p] != '_')
                ++p;
            if (p >= src.size() || !isSpace(src[p]))
            {
                pos = p;
                continue;
            }
            while (p < src.size() && isSpace(src[p]))
                ++p;
            size_t nameStart = p;
            while (p < src.size() && isIdentChar(src[p]))
                ++p;
            if (p == nameStart)
            {
                pos = p;
                continue;
            }
            size_t afterName = p;
            while (p < src.size() && isSpace(src[p]))
                ++p;
            if (p >= src.size())
                return false;
            char c = src[p];
            if (c == ';' || c == '=')
            {
                decl.afterName = afterName;
                decl.hasRegister = false;
                decl.reg = -1;
                next = p;
                return true;
            }
            if (c == ':')
            {
                // `: register(sN)` (any other semantic is left alone)
                size_t q = p + 1;
                while (q < src.size() && isSpace(src[q]))
                    ++q;
                decl.afterName = afterName;
                decl.hasRegister = true;
                decl.reg = -1;
                if (src.compare(q, 8, "register") == 0)
                {
                    q = src.find('(', q);
                    if (q != std::string::npos)
                    {
                        ++q;
                        while (q < src.size() && isSpace(src[q]))
                            ++q;
                        if (q < src.size() && src[q] == 's')
                            decl.reg = atoi(src.c_str() + q + 1);
                    }
                }
                next = p;
                return true;
            }
            // a function parameter or something else; not a declaration
            pos = p;
        }
        return false;
    }

    // Gives every sampler declaration without a register binding the lowest free sampler register in
    // declaration order. Returns the number of declarations rewritten.
    inline int AssignSamplerRegistersInDeclarationOrder(std::string& src)
    {
        bool used[16] = {};
        size_t pos = 0;
        SamplerDecl decl;
        size_t next;
        while (findSamplerDecl(src, pos, decl, next))
        {
            if (decl.hasRegister && decl.reg >= 0 && decl.reg < 16)
                used[decl.reg] = true;
            pos = next;
        }

        int rewritten = 0;
        pos = 0;
        while (findSamplerDecl(src, pos, decl, next))
        {
            if (!decl.hasRegister)
            {
                int reg = 0;
                while (reg < 16 && used[reg])
                    ++reg;
                if (reg == 16)
                    break;
                used[reg] = true;
                char binding[32];
                sprintf(binding, " : register(s%d)", reg);
                src.insert(decl.afterName, binding);
                next += strlen(binding);
                ++rewritten;
            }
            pos = next;
        }
        return rewritten;
    }
}  // namespace hlsl_compat
