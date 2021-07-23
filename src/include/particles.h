#pragma once

namespace m3d
{
    class StripOnePS :  public ParticleSystem
    {
    public:
        virtual int Render(struct CMatrix const *,ParticlesList *);
        StripOnePS();
        static void __fastcall ReleaseIb();
        virtual bool IsLocal();
        static void __fastcall CreateIb();
    protected:
    private:
    };
}

namespace m3d
{
    class ParticlesList
    {
    public:
        ~ParticlesList();
        ParticlesList();
        void SetAutoMeshEmitterPoints(int,int,float,struct CVector,struct CVector);
        void SetMeshEmitterPoints(struct CMatrix * *,int,void * *,int *,class std::vector<rend::VertexType,class std::allocator<rend::VertexType> > &,class std::vector<unsigned int,class std::allocator<unsigned int> > &,int);
        void Step(float);
        void SetMeshEmitterInds(int * *,int *);
    protected:
    private:
        unsigned int m_maxParticles;
        unsigned int m_numParticles;
        Particle *m_particles;
        CMatrix m_curXFormToWorld;
        float m_time;
        float m_spriteAngle;
        bool m_meshAutoEmitted;
        bool m_renderCalled;
        bool m_updateCalled;
        CMatrix **m_local;
        void **m_meshEmitterVerts;
        std::vector<rend::VertexType> m_VertexTypes;
        std::vector<unsigned int> m_VertexTypeSizes;
        int **m_meshEmitterInds;
        int *m_numMeshEmitterVerts;
        int *m_numMeshEmitterInds;
        int m_numMeshes;
        int m_numSkinMesh;
        rend::IbHandle *m_skinIb;
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
        TraceLineManager m_TLM;
        float m_maxSize;
    };
}

namespace m3d
{
    class PhysicModelPS :  public ParticleSystem
    {
    public:
        virtual int Update(ParticlesList *,float,float);
        virtual cmn::XmlNode * Write(fs::FileStream &,cmn::XmlFile *,cmn::XmlNode *,class CStr);
        virtual void WriteToProtos(struct PSProps &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > &);
        virtual int Read(fs::FileStream &,cmn::XmlFile *,cmn::XmlNode *);
        virtual int Render(struct CMatrix const *,ParticlesList *);
        virtual void ReadFromProtos(struct PSProps const &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > const &);
    protected:
    private:
        CStr m_modelName;
    };
}

namespace m3d
{
    class PolyPS :  public ParticleSystem
    {
    public:
        static void __fastcall ReleaseIb();
        static void __fastcall CreateIb();
        virtual int Render(struct CMatrix const *,ParticlesList *);
    protected:
    private:
    };
}

namespace m3d
{
    class SkinPS :  public ParticleSystem
    {
    public:
        virtual ~SkinPS();
        virtual void Reset(ParticlesList *);
        virtual int Update(ParticlesList *,float,float);
        virtual int Render(struct CMatrix const *,ParticlesList *);
        SkinPS();
    protected:
    private:
        unsigned __int16 **m_buffIndices;
    };
}

namespace m3d
{
    class Poly1PS :  public ParticleSystem
    {
    public:
        static void __fastcall ReleaseIb();
        static void __fastcall CreateIb();
        virtual int Render(struct CMatrix const *,ParticlesList *);
    protected:
    private:
    };
}

namespace m3d
{
    class StripAllPS :  public ParticleSystem
    {
    public:
        virtual int Render(struct CMatrix const *,ParticlesList *);
        StripAllPS();
        static void __fastcall CreateIb();
        virtual bool IsLocal();
        static void __fastcall ReleaseIb();
    protected:
    private:
    };
}

namespace m3d
{
    class LandSpritePS :  public ParticleSystem
    {
    public:
        virtual float GetBoundRadius() const ;
        virtual bool IsLocal();
        virtual int Render(struct CMatrix const *,ParticlesList *);
    protected:
    private:
    };
}

namespace m3d
{
    class SpritePS :  public ParticleSystem
    {
    public:
        virtual int Render(struct CMatrix const *,ParticlesList *);
        static void __fastcall CreateIb();
        virtual float GetBoundRadius() const ;
        static void __fastcall ReleaseIb();
    protected:
    private:
    };
}

namespace m3d
{
    class ParticleSystem
    {
    public:
        void SetPsVolume(int,float,float,enum ForceType,float);
        void SetPsVolume(enum CoordinatesSystemType);
        static class ParticleSystem * __fastcall New(class CStr const &);
        virtual int Render(struct CMatrix const *,ParticlesList *);
        void SetScaleParts(float);
        virtual cmn::XmlNode * Write(fs::FileStream &,cmn::XmlFile *,cmn::XmlNode *,class CStr);
        static class ParticleSystem * __fastcall Factory(struct PSProps const &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > const &);
        static class ParticleSystem * __fastcall Factory(char const *);
        virtual void ReadFromProtos(struct PSProps const &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > const &);
        static class ParticleSystem * __fastcall CreateCopy(class ParticleSystem &);
        void SetPsColor(float,unsigned int);
        int GetTimeDiscretion() const ;
        void SetParticleColor(Particle *,float);
        void SetPsTrailLen(int);
        void MoveParticles(ParticlesList *,class std::vector<struct CVector,class std::allocator<struct CVector> > const *);
        void InterpolateColors();
        int ReadRenderParams(class ref_ptr<cmn::XmlNode> &);
        void SetPsBlendMode(enum PBlendMode);
        void setParticleSize(Particle *);
        virtual void WriteToProtos(struct PSProps &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > &);
        void SetPsMesh(float,int);
        static void __fastcall SetInterpolateColorsOnLoad(bool);
        virtual bool IsLocal();
        ParticleSystem();
        int ReadVolumeParams(class ref_ptr<cmn::XmlNode> &);
        void SetPsSize(float,float);
        int ReadMeshParams(class ref_ptr<cmn::XmlNode> &);
        virtual int Read(fs::FileStream &,cmn::XmlFile *,cmn::XmlNode *);
        virtual float GetBoundRadius() const ;
        virtual ~ParticleSystem();
        void SetTextureName(class CStr const &);
        virtual void Reset(ParticlesList *);
        void AddParticles(ParticlesList *,class std::vector<struct CVector,class std::allocator<struct CVector> > const *);
        void AddParticle(ParticlesList *,struct CVector const *);
        void ApplyBlending();
        virtual int Update(ParticlesList *,float,float);
    protected:
    private:
        ParticleSystem_vtbl *__vftable /*VFT*/;
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
        rend::IEffect *m_shader;
        Emitter m_Emitter;
        Force m_x0[3];
        CoordinatesSystemType m_x0Cst;
        Force m_pos[3];
        std::vector<Attr *> m_Attractors;
        float m_parentDependency;
        Aabb m_bBox;
        int m_numvert;
        bool m_backflag;
    };
}

namespace m3d
{
    class ModelPS :  public ParticleSystem
    {
    public:
        virtual ~ModelPS();
        virtual int Render(struct CMatrix const *,ParticlesList *);
        virtual cmn::XmlNode * Write(fs::FileStream &,cmn::XmlFile *,cmn::XmlNode *,class CStr);
        virtual int Read(fs::FileStream &,cmn::XmlFile *,cmn::XmlNode *);
        virtual void WriteToProtos(struct PSProps &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > &);
        virtual void ReadFromProtos(struct PSProps const &,class std::vector<struct AttrProps,class std::allocator<struct AttrProps> > const &);
        ModelPS();
    protected:
    private:
        AnimatedModel *m_PartsModel;
        AnimInfo *m_Anim;
        CStr m_PartsModelName;
    };
}

namespace m3d
{
    class GlowQuadPS :  public ParticleSystem
    {
    public:
        virtual int Render(struct CMatrix const *,ParticlesList *);
        static void __fastcall CreateIb();
        virtual float GetBoundRadius() const ;
        static void __fastcall ReleaseIb();
    protected:
    private:
    };
}

namespace m3d
{
    class RainPS :  public ParticleSystem
    {
    public:
        static void __fastcall CreateIb();
        static void __fastcall ReleaseIb();
        virtual int Render(struct CMatrix const *,ParticlesList *);
    protected:
    private:
    };
}

namespace m3d
{
    class Particle :  public ParticleBase
    {
    public:
        void Step(float);
        Particle();
        ~Particle();
    protected:
    private:
        int m_trailSize;
        ParticleBase *m_trail;
        float m_time0;
        float m_fade;
        float m_ttl;
        rend::BlendMode m_blendMode;
        Particle *m_next;
    };
}

namespace m3d
{
    class QuadPS :  public ParticleSystem
    {
    public:
        virtual int Render(struct CMatrix const *,ParticlesList *);
        static void __fastcall CreateIb();
        static void __fastcall ReleaseIb();
        virtual float GetBoundRadius() const ;
    protected:
    private:
    };
}
