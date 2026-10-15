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
        float m_size;
        unsigned int m_curClr;
        CVector m_forigin;
    };

    struct Particle : public m3d::ParticleBase
    {
        int m_trailSize;
        m3d::ParticleBase* m_trail;
        float m_time0;
        float m_fade;
        float m_ttl;
        m3d::rend::BlendMode m_blendMode;
        m3d::Particle* m_next;
        void Step(float dt);
        Particle(m3d::Particle const&);
        Particle();
        ~Particle();
    };

    class ParticlesList
    {
    public:
        ParticlesList(m3d::ParticlesList const&);
        ParticlesList();
        ~ParticlesList();
        void SetMeshEmitterPoints(
            CMatrix** mat,
            int numMeshes,
            void** verts,
            int* numVerts,
            retruxx::vector<m3d::rend::VertexType>& VertexTypes,
            retruxx::vector<unsigned int>& VertexTypeSizes,
            int numSkinMesh);
        void SetMeshEmitterInds(int** inds, int* numinds);
        void SetAutoMeshEmitterPoints(int mode, int numVerts, float radius, CVector point1, CVector point2);
        void SetCurrentXForm(CMatrix const&);
        void Step(float dt);
        unsigned int m_maxParticles;
        unsigned int m_numParticles;
        m3d::Particle* m_particles;
        CMatrix m_curXFormToWorld;
        float m_time;
        float m_spriteAngle;
        bool m_meshAutoEmitted;
        bool m_renderCalled;
        bool m_updateCalled;
        CMatrix** m_local;
        void** m_meshEmitterVerts;
        retruxx::vector<enum m3d::rend::VertexType, retruxx::allocator<enum m3d::rend::VertexType>> m_VertexTypes;
        retruxx::vector<unsigned int, retruxx::allocator<unsigned int>> m_VertexTypeSizes;
        int** m_meshEmitterInds;
        int* m_numMeshEmitterVerts;
        int* m_numMeshEmitterInds;
        int m_numMeshes;
        int m_numSkinMesh;
        m3d::rend::IbHandle* m_skinIb;
        unsigned int m_numIb;
        CVector m_vel;
        CVector m_accel;
        CVector m_origin;
        CVector m_rotvel;
        CVector m_mrotvel;
        CVector m_rotaccel;
        CVector m_worldVel;
        float m_RandShader;
        float m_PhaseTime;
        float m_start1;
        CVector m_point2Max;
        m3d::TraceLineManager m_TLM;
        float m_maxSize;
    };

    class ParticleSystem
    {
    public:
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
        m3d::rend::TexHandle m_texAdd;
        CStr m_texName;
        PsShaderType m_shaderType;
        m3d::rend::IEffect* m_shader;
        m3d::Emitter m_Emitter;
        Force m_x0[3];
        CoordinatesSystemType m_x0Cst;
        Force m_pos[3];
        retruxx::vector<m3d::Attr*, retruxx::allocator<m3d::Attr*>> m_Attractors;
        float m_parentDependency;
        Aabb m_bBox;
        int m_numvert;
        bool m_backflag;
        static m3d::ParticleSystem* New(CStr const& ClassName);
        static m3d::ParticleSystem* Factory(
            PSProps const& psprops,
            retruxx::vector<AttrProps, retruxx::allocator<AttrProps>> const& AttrProtos);
        static m3d::ParticleSystem* Factory(char const* fileName);
        static m3d::ParticleSystem* CreateCopy(m3d::ParticleSystem& CopyPS);
        static void SetInterpolateColorsOnLoad(bool c);
        ParticleSystem(m3d::ParticleSystem const&);
        ParticleSystem();
        virtual ~ParticleSystem();
        virtual int Update(m3d::ParticlesList* parts, float lastFrameSecs, float fader);
        virtual int Render(CMatrix const* local, m3d::ParticlesList* parts);
        virtual void Reset(m3d::ParticlesList* parts);

        using CVectorVector = retruxx::vector<CVector, retruxx::allocator<CVector>>;

    public:
        void AddParticle(m3d::ParticlesList* parts, CVector const* pos);
        void AddParticles(
            m3d::ParticlesList* parts,
            retruxx::vector<CVector, retruxx::allocator<CVector>> const* newPoses);
        void MoveParticles(
            m3d::ParticlesList* parts,
            retruxx::vector<CVector, retruxx::allocator<CVector>> const* newPoses);
        virtual m3d::cmn::XmlNode*
            Write(m3d::fs::FileStream& out, m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* root, CStr name);
        virtual int Read(m3d::fs::FileStream& in, m3d::cmn::XmlFile* m_file, m3d::cmn::XmlNode* psroot);
        int ReadVolumeParams(ref_ptr<m3d::cmn::XmlNode>& force);
        int ReadRenderParams(ref_ptr<m3d::cmn::XmlNode>& vis);
        int ReadMeshParams(ref_ptr<m3d::cmn::XmlNode>& mesh);
        virtual void ReadFromProtos(
            PSProps const& psprops,
            retruxx::vector<AttrProps, retruxx::allocator<AttrProps>> const& AttrProtos);
        virtual void WriteToProtos(
            PSProps& psprops,
            retruxx::vector<AttrProps, retruxx::allocator<AttrProps>>& AttrProtos);
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
        void SetTextureName(CStr const& name);
        // Not in the original: the shader lookup is shared by ReadFromProtos and
        // ReadRenderParams, which both inline the same switch in the shipped build.
        void CreateShaderForType(PsShaderType type);
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
        CStr const& GetTextureName() const;
        virtual float GetBoundRadius() const;
        virtual bool IsLocal();
    };

    class StripOnePS : public ParticleSystem
    {
    public:
        virtual int Render(CMatrix const*, ParticlesList*);
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
        // Not in the original: Read and ReadFromProtos both inline this.
        void LoadPartsModel();

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
}  // namespace m3d
