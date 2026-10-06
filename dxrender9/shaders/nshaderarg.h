#pragma once
// Ported from the original dxrender9/shaders/nshaderarg.h and nshaderparams.h: a typed effect
// parameter value and the per-effect set of them (EffectImpl::m_curParams). Both classes are
// header-only in the original (every member the PDB places in nshaderarg.h / nshaderparams.h); the members
// that reached effect.obj out of line carry their original rva.
#include <cstring>

#include <math/matrix.h>
#include <math/vector4.h>
#include <renderer/i_renderer.h>

class nShaderArg
{
public:
    enum Type
    {
        Void = 0,
        Bool = 1,
        Int = 2,
        Float = 3,
        Float4 = 4,
        Matrix44 = 5,
        Texture = 6,
    };

    /* 0x0000 */ Type m_type;
    union
    {
        /* 0x0004 */ bool b;
        /* 0x0004 */ int i;
        /* 0x0004 */ float f;
        /* 0x0004 */ m3d::rend::TexHandle* tex;
        /* 0x0004 */ nFloat4 f4;
        /* 0x0004 */ float m[4][4];
    };

    // orig 0x6452b0 nshaderarg.h:97
    nShaderArg() : m_type(Void)
    {
        i = 0;
    }

    // orig 0x6452c0 nshaderarg.h:115
    ~nShaderArg()
    {
    }

    // orig 0x6452d0 nshaderarg.h:123
    bool operator==(nShaderArg const& rhs) const
    {
        if (m_type == rhs.m_type)
        {
            switch (m_type)
            {
            case Void:
                return true;
            case Bool:
                return b == rhs.b;
            case Int:
                return i == rhs.i;
            case Float:
                return f == rhs.f;
            case Float4:
                return f4.x == rhs.f4.x && f4.y == rhs.f4.y && f4.z == rhs.f4.z && f4.w == rhs.f4.w;
            case Matrix44:
            {
                bool res = true;
                for (int r = 0; r < 4; r++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        if (m[r][c] != rhs.m[r][c])
                            res = false;
                    }
                }
                return res;
            }
            case Texture:
                return tex == rhs.tex;
            }
        }
        return false;
    }

    // orig 0x645410 nshaderarg.h:180 (returns nothing: eax is not `this` on any path)
    void operator=(nShaderArg const& rhs)
    {
        m_type = rhs.m_type;
        switch (m_type)
        {
        case Bool:
            b = rhs.b;
            break;
        case Int:
            i = rhs.i;
            break;
        case Float:
            f = rhs.f;
            break;
        case Float4:
            f4 = rhs.f4;
            break;
        case Matrix44:
            memcpy(m, rhs.m, sizeof(m));
            break;
        case Texture:
            tex = rhs.tex;
            break;
        }
    }

    // orig 0x645490 nshaderarg.h:222
    void SetType(Type t)
    {
        m_type = t;
    }

    // orig 0x6454a0 nshaderarg.h:230
    Type GetType() const
    {
        return m_type;
    }

    // no original instantiation (never called out of line); written after SetInt/GetInt
    void SetBool(bool val)
    {
        m_type = Bool;
        b = val;
    }

    // no original instantiation (never called out of line); written after SetInt/GetInt
    bool GetBool() const
    {
        return b;
    }

    // orig 0x6454b0 nshaderarg.h:255
    void SetInt(int val)
    {
        m_type = Int;
        i = val;
    }

    // orig 0x6454c0 nshaderarg.h:264
    int GetInt() const
    {
        return i;
    }

    // orig 0x6454d0 nshaderarg.h:272
    void SetFloat(float val)
    {
        m_type = Float;
        f = val;
    }

    // orig 0x6454f0 nshaderarg.h:281
    float GetFloat() const
    {
        return f;
    }

    // orig 0x645500 nshaderarg.h:289
    void SetFloat4(nFloat4 const& val)
    {
        m_type = Float4;
        f4 = val;
    }

    // orig 0x645530 nshaderarg.h:298
    nFloat4 const& GetFloat4() const
    {
        return f4;
    }

    // no original instantiation (never called out of line); written after the Matrix44 case of operator=
    void SetMatrix44(CMatrix const* val)
    {
        m_type = Matrix44;
        memcpy(m, val, sizeof(m));
    }

    // orig 0x645540 nshaderarg.h:324
    CMatrix const* GetMatrix44() const
    {
        return reinterpret_cast<CMatrix const*>(m);
    }

    // no original instantiation (never called out of line); written after SetInt/GetTexture
    void SetTexture(m3d::rend::TexHandle* val)
    {
        m_type = Texture;
        tex = val;
    }

    // orig 0x645550 nshaderarg.h:341
    m3d::rend::TexHandle* GetTexture() const
    {
        return tex;
    }
}; /* size: 0x0044 */

class nShaderParams
{
public:
    /* 0x0000 */ bool m_valid[m3d::rend::IEffect::NumParameters];
    /* ...... */ nShaderArg m_args[m3d::rend::IEffect::NumParameters];

    // orig 0x646080 nshaderparams.h:80
    nShaderParams()
    {
        memset(m_valid, 0, sizeof(m_valid));
    }

    // orig 0x645560 nshaderparams.h:88
    ~nShaderParams()
    {
    }

    // orig 0x645570 nshaderparams.h:96
    bool IsParameterValid(m3d::rend::IEffect::Parameter p) const
    {
        return m_valid[p];
    }

    // orig 0x645580 nshaderparams.h:105
    void SetArg(m3d::rend::IEffect::Parameter p, nShaderArg const& arg)
    {
        m_args[p] = arg;
    }

    // orig 0x6455a0 nshaderparams.h:114
    nShaderArg const& GetArg(m3d::rend::IEffect::Parameter p) const
    {
        return m_args[p];
    }

    // orig 0x6455c0 nshaderparams.h:123
    void SetInt(m3d::rend::IEffect::Parameter p, int val)
    {
        m_valid[p] = true;
        m_args[p].SetInt(val);
    }

    // no original instantiation (never called out of line); written after SetInt
    int GetInt(m3d::rend::IEffect::Parameter p) const
    {
        return m_args[p].GetInt();
    }

    // orig 0x6455f0 nshaderparams.h:142
    void SetFloat(m3d::rend::IEffect::Parameter p, float val)
    {
        m_valid[p] = true;
        m_args[p].SetFloat(val);
    }

    // no original instantiation (never called out of line); written after SetFloat
    float GetFloat(m3d::rend::IEffect::Parameter p) const
    {
        return m_args[p].GetFloat();
    }

    // orig 0x645620 nshaderparams.h:161
    void SetFloat4(m3d::rend::IEffect::Parameter p, nFloat4 const& val)
    {
        m_valid[p] = true;
        m_args[p].SetFloat4(val);
    }

    // no original instantiation (never called out of line); written after SetFloat4
    nFloat4 const& GetFloat4(m3d::rend::IEffect::Parameter p) const
    {
        return m_args[p].GetFloat4();
    }

    // no original instantiation (never called out of line); written after SetFloat4
    void SetMatrix44(m3d::rend::IEffect::Parameter p, CMatrix const* val)
    {
        m_valid[p] = true;
        m_args[p].SetMatrix44(val);
    }

    // no original instantiation (never called out of line); written after GetFloat4
    CMatrix const* GetMatrix44(m3d::rend::IEffect::Parameter p) const
    {
        return m_args[p].GetMatrix44();
    }

    // no original instantiation (never called out of line); written after SetFloat4
    void SetTexture(m3d::rend::IEffect::Parameter p, m3d::rend::TexHandle* val)
    {
        m_valid[p] = true;
        m_args[p].SetTexture(val);
    }

    // no original instantiation (never called out of line); written after GetFloat4
    m3d::rend::TexHandle* GetTexture(m3d::rend::IEffect::Parameter p) const
    {
        return m_args[p].GetTexture();
    }

    // inlined into EffectImpl::SetVector4 (orig 0x6457f0): valid flag + Float4 payload
    void SetVector4(m3d::rend::IEffect::Parameter p, CVector4 const& val)
    {
        m_valid[p] = true;
        m_args[p].SetFloat4(reinterpret_cast<nFloat4 const&>(val));
    }

    // no original instantiation (never called out of line); written after GetFloat4
    CVector4 GetVector4(m3d::rend::IEffect::Parameter p) const
    {
        nFloat4 const& f4 = m_args[p].GetFloat4();
        return CVector4(f4.x, f4.y, f4.z, f4.w);
    }

    // orig 0x645660 nshaderparams.h:239
    void Reset()
    {
        memset(m_valid, 0, sizeof(m_valid));
        for (int i = 0; i < m3d::rend::IEffect::NumParameters; i++)
            m_args[i].SetType(nShaderArg::Void);
    }
};
