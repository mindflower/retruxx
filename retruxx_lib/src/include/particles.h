#pragma once
#include <psprops.h>
#include <ps_attractors.h>
#include <tracelinemanager.h>
#include <vector>
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
        void SetMeshEmitterPoints(CMatrix** mat, int numMeshes, void** verts, int* numVerts, std::vector<enum m3d::rend::VertexType, std::allocator<enum m3d::rend::VertexType> >& VertexTypes, std::vector<unsigned int, std::allocator<unsigned int> >& VertexTypeSizes, int numSkinMesh);
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
        std::vector<enum m3d::rend::VertexType, std::allocator<enum m3d::rend::VertexType> > m_VertexTypes;
        std::vector<unsigned int, std::allocator<unsigned int> > m_VertexTypeSizes;
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
        void SetPsVolume(int, float, float, enum ForceType, float);
        void SetPsVolume(enum CoordinatesSystemType);
        static ParticleSystem* New(CStr const&);
        virtual int Render(CMatrix const*, ParticlesList*);
        void SetScaleParts(float);
        virtual cmn::XmlNode* Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr);
        static ParticleSystem* Factory(PSProps const&, std::vector<AttrProps> const&);
        static ParticleSystem* Factory(char const*);
        virtual void ReadFromProtos(PSProps const&, std::vector<AttrProps> const&);
        static ParticleSystem* CreateCopy(ParticleSystem&);
        void SetPsColor(float, unsigned int);
        int GetTimeDiscretion() const;
        void SetParticleColor(Particle*, float);
        void SetPsTrailLen(int);
        void MoveParticles(ParticlesList*, std::vector<CVector> const*);
        void InterpolateColors();
        int ReadRenderParams(ref_ptr<cmn::XmlNode>&);
        void SetPsBlendMode(PBlendMode);
        void setParticleSize(Particle*);
        virtual void WriteToProtos(PSProps&, std::vector<AttrProps>&);
        void SetPsMesh(float, int);
        static void SetInterpolateColorsOnLoad(bool);
        virtual bool IsLocal();
        ParticleSystem();
        int ReadVolumeParams(ref_ptr<cmn::XmlNode>&);
        void SetPsSize(float, float);
        int ReadMeshParams(ref_ptr<cmn::XmlNode>&);
        virtual int Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*);
        virtual float GetBoundRadius() const;
        virtual ~ParticleSystem();
        void SetTextureName(CStr const&);
        virtual void Reset(ParticlesList*);
        void AddParticles(ParticlesList*, std::vector<CVector> const*);
        void AddParticle(ParticlesList*, CVector const*);
        void ApplyBlending();
        virtual int Update(ParticlesList*, float, float);

    private:
        CStr m_Name;
        CStr m_ClassName;
        bool m_Specific;
        bool m_HaveTrail;
        int m_trailLen;
        bool m_CreateOne;
        bool m_autoMeshEmitter;
        float m_meshradius;
        int m_points;
        CVector m_point1;
        CVector m_point2;
        CVector m_point2Max;
        int m_colors[20];
        float m_sizes[20];
        PBlendMode m_blendMode;
        bool m_forv;
        bool m_back;
        bool m_orient;
        float m_scaleparts;
        bool m_updateXForm;
        float m_SpriteAngle;
        int m_TexTiling;
        rend::TexHandle m_texAdd;
        CStr m_texName;
        PsShaderType m_shaderType;
        rend::IEffect* m_shader;
        Emitter m_Emitter;
        Force m_x0[3];
        CoordinatesSystemType m_x0Cst;
        Force m_pos[3];
        std::vector<Attr*> m_Attractors;
        float m_parentDependency;
        Aabb m_bBox;
        int m_numvert;
        bool m_backflag;
    };

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
        virtual void WriteToProtos(PSProps&, std::vector<AttrProps>&);
        virtual int Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*);
        virtual int Render(CMatrix const*, ParticlesList*);
        virtual void ReadFromProtos(PSProps const&, std::vector<AttrProps> const&);

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
        virtual void WriteToProtos(PSProps&, std::vector<AttrProps>&);
        virtual void ReadFromProtos(PSProps const&, std::vector<AttrProps> const&);
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
