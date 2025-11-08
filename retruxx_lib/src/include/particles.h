#pragma once
#include "retruxx/common.h"
#include <psprops.h>
#include <ps_attractors.h>
#include <tracelinemanager.h>
#include <core/ref_ptr.h>
#include <math/aabb.h>
#include <math/matrix.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace fs
    {
        class FileStream;
    }

    class AnimInfo;
    class AnimatedModel;

    struct ParticleBase
    {
        CVector m_locorigin;
        CVector m_origin;
        CVector m_rotvel;
        CVector m_rotaccel;
        CVector m_vel;
        CVector m_accel;
        CVector m_dir;
        /* 0x0054 */ float m_size;
        /* 0x0058 */ unsigned int m_curClr;
        CVector m_forigin;
    }; /* size: 0x0068 */

    struct Particle : public m3d::ParticleBase
    {
        m3d::ParticleBase m3d::ParticleBase;
        /* 0x0068 */ int m_trailSize;
        /* 0x006c */ m3d::ParticleBase* m_trail;
        /* 0x0070 */ float m_time0;
        /* 0x0074 */ float m_fade;
        /* 0x0078 */ float m_ttl;
        /* 0x007c */ m3d::rend::BlendMode m_blendMode;
        /* 0x0080 */ m3d::Particle* m_next;
        void Step(float dt);
        Particle(const m3d::Particle&);
        Particle();
        ~Particle();
    }; /* size: 0x0084 */

    class ParticlesList
    {
    public:
        ParticlesList(const m3d::ParticlesList&);
        ParticlesList();
        ~ParticlesList();
        void SetMeshEmitterPoints(CMatrix** mat, int numMeshes, void** verts, int* numVerts, retruxx::vector<m3d::rend::VertexType>& VertexTypes, retruxx::vector<unsigned int>& VertexTypeSizes, int numSkinMesh);
        void SetMeshEmitterInds(int** inds, int* numinds);
        void SetAutoMeshEmitterPoints(int mode, int numVerts, float radius, CVector point1, CVector point2);
        void SetCurrentXForm(const CMatrix&);
        void Step(float dt);
        /* 0x0000 */ unsigned int m_maxParticles;
        /* 0x0004 */ unsigned int m_numParticles;
        /* 0x0008 */ m3d::Particle* m_particles;
        CMatrix m_curXFormToWorld;
        /* 0x004c */ float m_time;
        /* 0x0050 */ float m_spriteAngle;
        /* 0x0054 */ bool m_meshAutoEmitted;
        /* 0x0055 */ bool m_renderCalled;
        /* 0x0056 */ bool m_updateCalled;
        /* 0x0057 */ char Padding_33;
        /* 0x0058 */ CMatrix** m_local;
        /* 0x005c */ void** m_meshEmitterVerts;
        retruxx::vector<enum m3d::rend::VertexType, retruxx::allocator<enum m3d::rend::VertexType> > m_VertexTypes;
        retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > m_VertexTypeSizes;
        /* 0x0080 */ int** m_meshEmitterInds;
        /* 0x0084 */ int* m_numMeshEmitterVerts;
        /* 0x0088 */ int* m_numMeshEmitterInds;
        /* 0x008c */ int m_numMeshes;
        /* 0x0090 */ int m_numSkinMesh;
        /* 0x0094 */ m3d::rend::IbHandle* m_skinIb;
        /* 0x0098 */ unsigned int m_numIb;
        CVector m_vel;
        CVector m_accel;
        CVector m_origin;
        CVector m_rotvel;
        CVector m_mrotvel;
        CVector m_rotaccel;
        CVector m_worldVel;
        /* 0x00f0 */ float m_RandShader;
        /* 0x00f4 */ float m_PhaseTime;
        /* 0x00f8 */ float m_start1;
        CVector m_point2Max;
        m3d::TraceLineManager m_TLM;
        /* 0x011c */ float m_maxSize;
    }; /* size: 0x0120 */

    class ParticleSystem
    {
    public:
        CStr m_Name;
        CStr m_ClassName;
        /* 0x001c */ bool m_Specific;
        /* 0x001d */ bool m_HaveTrail;
        /* 0x001e */ char Padding_68[2];
        /* 0x0020 */ int m_trailLen;
        /* 0x0024 */ bool m_CreateOne;
        /* 0x0025 */ bool m_autoMeshEmitter;
        /* 0x0026 */ char Padding_69[2];
        /* 0x0028 */ float m_meshradius;
        /* 0x002c */ int m_points;
        CVector m_point1;
        CVector m_point2;
        CVector m_point2Max;
        /* 0x0054 */ int m_colors[20];
        /* 0x00a4 */ float m_sizes[20];
        /* 0x00f4 */ PBlendMode m_blendMode;
        /* 0x00f8 */ bool m_forv;
        /* 0x00f9 */ bool m_back;
        /* 0x00fa */ bool m_orient;
        /* 0x00fb */ char Padding_70;
        /* 0x00fc */ float m_scaleparts;
        /* 0x0100 */ bool m_updateXForm;
        /* 0x0101 */ char Padding_71[3];
        /* 0x0104 */ float m_SpriteAngle;
        /* 0x0108 */ int m_TexTiling;
        m3d::rend::TexHandle m_texAdd;
        CStr m_texName;
        /* 0x011c */ PsShaderType m_shaderType;
        /* 0x0120 */ m3d::rend::IEffect* m_shader;
        m3d::Emitter m_Emitter;
        /* 0x0150 */ Force m_x0[3];
        /* 0x0180 */ CoordinatesSystemType m_x0Cst;
        /* 0x0184 */ Force m_pos[3];
        retruxx::vector<m3d::Attr*, retruxx::allocator<m3d::Attr*> > m_Attractors;
        /* 0x01c4 */ float m_parentDependency;
        Aabb m_bBox;
        /* 0x01e0 */ int m_numvert;
        /* 0x01e4 */ bool m_backflag;
        static m3d::ParticleSystem* __fastcall New(const CStr& ClassName);
        static m3d::ParticleSystem* __fastcall Factory(const PSProps& psprops, const retruxx::vector<AttrProps, retruxx::allocator<AttrProps> >& AttrProtos);
        static m3d::ParticleSystem* __fastcall Factory(const char* fileName);
        static m3d::ParticleSystem* __fastcall CreateCopy(m3d::ParticleSystem& CopyPS);
        static void __fastcall SetInterpolateColorsOnLoad(bool c);
        ParticleSystem(const m3d::ParticleSystem&);
        ParticleSystem();
        virtual  ~ParticleSystem() /* 0x00 */;
        virtual int Update(m3d::ParticlesList* parts, float lastFrameSecs, float fader) /* 0x04 */;
        virtual int Render(const CMatrix* local, m3d::ParticlesList* parts) /* 0x08 */;
        virtual void Reset(m3d::ParticlesList* parts) /* 0x0c */;

        using CVectorVector = retruxx::vector<CVector, retruxx::allocator<CVector> >;

    public:
        void AddParticle(m3d::ParticlesList* parts, const CVector* pos);
        void AddParticles(m3d::ParticlesList* parts, const retruxx::vector<CVector, retruxx::allocator<CVector> >* newPoses);
        void MoveParticles(m3d::ParticlesList* parts, const retruxx::vector<CVector, retruxx::allocator<CVector> >* newPoses);
        virtual m3d::cmn::XmlNode* Write(m3d::fs::FileStream& out, m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* root, CStr name) /* 0x10 */;
        virtual int Read(m3d::fs::FileStream& in, m3d::cmn::XmlFile* m_file, m3d::cmn::XmlNode* psroot) /* 0x14 */;
        int ReadVolumeParams(ref_ptr<m3d::cmn::XmlNode>& force);
        int ReadRenderParams(ref_ptr<m3d::cmn::XmlNode>& vis);
        int ReadMeshParams(ref_ptr<m3d::cmn::XmlNode>& mesh);
        virtual void ReadFromProtos(const PSProps& psprops, const retruxx::vector<AttrProps, retruxx::allocator<AttrProps> >& AttrProtos) /* 0x18 */;
        virtual void WriteToProtos(PSProps& psprops, retruxx::vector<AttrProps, retruxx::allocator<AttrProps> >& AttrProtos) /* 0x1c */;
        void InterpolateColors();
        void SetParticleColor(m3d::Particle* pParticle, float fader);
        void setParticleSize(m3d::Particle* p);
        void ApplyBlending();
        void SetName(CStr);
        void SetPsVolume(CoordinatesSystemType mode);
        void SetPsVolume(int axis, float forceMin, float forceMax, ForceType ft, float period);
        void SetPsTrailLen(int traillen);
        void SetScaleParts(float tt);
        void SetPsBlendMode(PBlendMode blendmode);
        void SetPsSize(float time, float value);
        void SetPsColor(float time, unsigned int value);
        void SetPsMesh(float radius, int points);
        void SetTextureName(const CStr& name);
        CStr GetName();
        CoordinatesSystemType GetPsVolume() const;
        void GetPsVolume(int, float&, float&, ForceType&, float&) const;
        void GetPsRender(int, void*) const;
        PBlendMode GetPsBlendMode() const;
        float GetScaleParts() const;
        int GetPsTrailLen() const;
        unsigned int GetPsColor(float) const;
        float GetPsSize(float) const;
        int GetTimeDiscretion() const;
        void GetPsMesh(float&, int&) const;
        const CStr& GetTextureName() const;
        virtual float GetBoundRadius() const /* 0x20 */;
        virtual bool IsLocal() /* 0x24 */;
    }; /* size: 0x01e8 */

    class StripOnePS : public ParticleSystem
    {
    public:
        virtual int Render(CMatrix const *,ParticlesList *);
        StripOnePS();
        static void ReleaseIb();
        virtual bool IsLocal();
        static void CreateIb();

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class PhysicModelPS : public ParticleSystem
    {
    public:
        virtual int Update(ParticlesList*, float, float);
        virtual cmn::XmlNode* Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr);
        virtual void WriteToProtos(PSProps&, retruxx::vector<AttrProps>&);
        virtual int Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*);
        virtual int Render(CMatrix const*, ParticlesList*);
        virtual void ReadFromProtos(PSProps const&, retruxx::vector<AttrProps> const&);

    private:
        CStr m_modelName;
    };

    class PolyPS : public ParticleSystem
    {
    public:
        static void ReleaseIb();
        static void CreateIb();
        virtual int Render(CMatrix const*, ParticlesList*);

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class SkinPS : public ParticleSystem
    {
    public:
        virtual ~SkinPS();
        virtual void Reset(ParticlesList*);
        virtual int Update(ParticlesList*, float, float);
        virtual int Render(CMatrix const*, ParticlesList*);
        SkinPS();

    private:
        unsigned __int16** m_buffIndices;
    };

    class Poly1PS : public ParticleSystem
    {
    public:
        static void ReleaseIb();
        static void CreateIb();
        virtual int Render(CMatrix const*, ParticlesList*);

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class StripAllPS : public ParticleSystem
    {
    public:
        virtual int Render(CMatrix const*, ParticlesList*);
        StripAllPS();
        static void CreateIb();
        virtual bool IsLocal();
        static void ReleaseIb();

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class LandSpritePS : public ParticleSystem
    {
    public:
        virtual float GetBoundRadius() const;
        virtual bool IsLocal();
        virtual int Render(CMatrix const*, ParticlesList*);
    };

    class SpritePS : public ParticleSystem
    {
    public:
        virtual int Render(CMatrix const*, ParticlesList*);
        static void CreateIb();
        virtual float GetBoundRadius() const;
        static void ReleaseIb();

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class ModelPS : public ParticleSystem
    {
    public:
        virtual ~ModelPS();
        virtual int Render(CMatrix const*, ParticlesList*);
        virtual cmn::XmlNode* Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr);
        virtual int Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*);
        virtual void WriteToProtos(PSProps&, retruxx::vector<AttrProps>&);
        virtual void ReadFromProtos(PSProps const&, retruxx::vector<AttrProps> const&);
        ModelPS();

    private:
        AnimatedModel* m_PartsModel;
        AnimInfo* m_Anim;
        CStr m_PartsModelName;
    };

    class GlowQuadPS : public ParticleSystem
    {
    public:
        virtual int Render(CMatrix const*, ParticlesList*);
        static void CreateIb();
        virtual float GetBoundRadius() const;
        static void ReleaseIb();

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class RainPS : public ParticleSystem
    {
    public:
        static void CreateIb();
        static void ReleaseIb();
        virtual int Render(CMatrix const*, ParticlesList*);

    private:
        static rend::IbPoolField m_IbPoolField;
    };

    class QuadPS : public ParticleSystem
    {
    public:
        virtual int Render(CMatrix const*, ParticlesList*);
        static void CreateIb();
        static void ReleaseIb();
        virtual float GetBoundRadius() const;

    private:
        static rend::IbPoolField m_IbPoolField;
    };
}
