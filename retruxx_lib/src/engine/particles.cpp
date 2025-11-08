#include "particles.h"
#include <m3dapp.h>
#include <scene/servers/serverparticles.h>
#include <core/log.h>
#include <skelmodel.h>

bool interpolateColorsOnLoad = false;

namespace m3d
{
    void Particle::Step(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Particle::Particle()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Particle::~Particle()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticlesList::~ParticlesList()
    {
        for (auto particle = m_particles; particle != nullptr; particle = particle->m_next)
        {
            ParticlesPool.Delete(particle);
        }
        if (this->m_skinIb)
        {
            for (unsigned i = 0; i < this->m_numIb; ++i)
            {
                if (m_skinIb[i].IsValid())
                    m3d::Application::g_pApp->m_renderer->ReleaseIb(m_skinIb[i]);
            }
            delete[] m_skinIb;
        }
        if (m_meshAutoEmitted)
        {
            delete[] m_numMeshEmitterVerts;
            delete[] m_meshEmitterVerts;
        }
    }

    ParticlesList::ParticlesList() : m_TLM(0x1F4)
    {
        this->m_renderCalled = 0;
        this->m_updateCalled = 0;
        this->m_numParticles = 0;
        this->m_particles = 0;
        this->m_maxParticles = 200;
        this->m_time = 0.0;

        memset(&this->m_curXFormToWorld, 0, sizeof(this->m_curXFormToWorld));
        this->m_curXFormToWorld._44 = 1.0;
        this->m_curXFormToWorld._33 = 1.0;
        this->m_curXFormToWorld._22 = 1.0;
        this->m_curXFormToWorld._11 = 1.0;
        this->m_meshEmitterVerts = 0;
        this->m_numMeshEmitterVerts = 0;
        this->m_rotvel.y = 0LL;
        this->m_rotvel.x = 0.0;
        this->m_rotaccel.y = 0LL;
        this->m_rotaccel.x = 0.0;
        this->m_vel.y = 0LL;
        this->m_vel.x = 0.0;
        this->m_rotvel.y = 0LL;
        this->m_rotvel.x = 0.0;
        this->m_accel.y = 0LL;
        this->m_accel.x = 0.0;
        this->m_rotaccel.y = 0LL;
        this->m_rotaccel.x = 0.0;
        this->m_origin.y = 0LL;
        this->m_origin.x = 0.0;
        this->m_worldVel.y = 0LL;
        this->m_worldVel.x = 0.0;
        this->m_skinIb = 0;
        this->m_numIb = 0;
        this->m_numSkinMesh = 0;
        this->m_meshAutoEmitted = 0;
        this->m_spriteAngle = 0.0;
        this->m_RandShader = 0.0;
        this->m_PhaseTime = 0.0;
        this->m_start1 = 0.0;
    }

    void ParticlesList::SetAutoMeshEmitterPoints(int, int, float, CVector, CVector)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesList::SetMeshEmitterPoints(CMatrix** mat, int numMeshes, void** verts, int* numVerts, retruxx::vector<m3d::rend::VertexType>& VertexTypes, retruxx::vector<unsigned int>& VertexTypeSizes, int numSkinMesh)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesList::Step(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticlesList::SetMeshEmitterInds(int**, int*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsVolume(int, float, float, ForceType, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsVolume(CoordinatesSystemType)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticleSystem* ParticleSystem::New(CStr const& className)
    {
        if (className == "Quad")
        {
            return new QuadPS;
        }
        if (className == "GlowQuad")
        {
            return new GlowQuadPS;
        }
        if (className == "Sprite")
        {
            return new SpritePS;
        }
        if (className == "Poly")
        {
            return new PolyPS;
        }
        if (className == "StripAll")
        {
            return new StripAllPS;
        }
        if (className == "Poly1")
        {
            return new Poly1PS;
        }
        if (className == "StripOne")
        {
            return new StripOnePS;
        }
        if (className == "Skin")
        {
            return new SkinPS;
        }
        if (className == "LandSprite")
        {
            return new LandSpritePS;
        }
        if (className == "Rain")
        {
            return new RainPS;
        }
        if (className == "Model")
        {
            return new ModelPS;
        }
        if (className == "PhysicModel")
        {
            return new PhysicModelPS;
        }
        M3D_LOG_INFO("New: wrong class name" + className);
        return new ParticleSystem;
    }

    int ParticleSystem::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetScaleParts(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    cmn::XmlNode* ParticleSystem::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticleSystem* ParticleSystem::Factory(PSProps const& psprops, retruxx::vector<AttrProps> const& AttrProtos)
    {
        auto* system = New(psprops.m_ClassName);
        system->ReadFromProtos(psprops, AttrProtos);
        return system;
    }

    ParticleSystem* ParticleSystem::Factory(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::ReadFromProtos(PSProps const& psprops, retruxx::vector<AttrProps> const& AttrProtos)
    {
        // Copy basic properties
        m_Name = psprops.m_Name;
        m_ClassName = psprops.m_ClassName;

        m_Specific = psprops.m_Specific;
        m_HaveTrail = psprops.m_HaveTrail;
        m_trailLen = psprops.m_trailLen;
        m_CreateOne = psprops.m_CreateOne;
        m_autoMeshEmitter = psprops.m_autoMeshEmitter;
        m_meshradius = psprops.m_meshradius;
        m_points = psprops.m_points;
        m_point1 = psprops.m_point1;
        m_point2 = psprops.m_point2;
        m_point2Max = psprops.m_point2Max;

        // Copy size arrays (optimized copy for 20 elements)
        memcpy(m_sizes, psprops.m_sizes, sizeof(m_sizes));

        if (interpolateColorsOnLoad)
        {
            InterpolateColors();
        }

        m_blendMode = psprops.m_blendMode;
        m_forv = psprops.m_forv;
        m_back = psprops.m_back;
        m_orient = psprops.m_orient;
        m_scaleparts = psprops.m_scaleparts;
        m_updateXForm = psprops.m_updateXForm;
        m_SpriteAngle = psprops.m_SpriteAngle;
        m_TexTiling = psprops.m_TexTiling;

        SetTextureName(psprops.m_texName);

        // Set up shader based on type
        m_shaderType = psprops.m_shaderType;
        switch (m_shaderType) {
        case PSST_DUST:
            m_shader = M3D_RENDERER->NewEffect(
                "data/shaders/ps_dust.fx", true);
            break;
        case PSST_LIGHT:
            m_shader = M3D_RENDERER->NewEffect(
                "data/shaders/ps_light.fx", true);
            break;
        case PSST_NOFOG_DUST:
            m_shader = M3D_RENDERER->NewEffect(
                "data/shaders/ps_nofog_dust.fx", true);
            break;
        case PSST_NOFOG_LIGHT:
            m_shader = M3D_RENDERER->NewEffect(
                "data/shaders/ps_nofog_light.fx", true);
            break;
        default:
            M3D_ASSERT(false);
            break;
        }

        M3D_ASSERT(m_shader);
        m_shader->SetDefaultTechnique(true);

        // Copy emitter properties
        m_Emitter.m_emitAtPeriod = psprops.m_emitAtPeriod;
        m_Emitter.m_wtime = psprops.m_wtime;
        m_Emitter.m_maxParticles = psprops.m_maxParticles;
        m_Emitter.m_ttlMin = psprops.m_ttlMin;
        m_Emitter.m_ttlMax = psprops.m_ttlMax;
        m_Emitter.m_resettime = psprops.m_resettime;
        m_Emitter.m_localStop = psprops.m_localStop;
        m_Emitter.m_stopTime = psprops.m_stopTime;
        m_Emitter.m_start = psprops.m_start;

        // Copy position and initial position properties
        for (int i = 0; i < 3; ++i)
        {
            m_x0[i] = psprops.m_x0[i];
            m_pos[i] = psprops.m_pos[i];
        }
        m_x0Cst = psprops.m_x0Cst;

        m_parentDependency = psprops.m_parentDependency;

        // Copy bounding box
        m_bBox.Create(psprops.m_bBoxMin, psprops.m_bBoxMax);

        // Clear existing attractors
        for (auto* attractor : m_Attractors)
        {
            if (attractor)
            {
                delete attractor;
            }
        }
        m_Attractors.clear();

        // Create new attractors from prototypes
        m_Attractors.reserve(AttrProtos.size());

        for (const auto& attrProto : AttrProtos)
        {
            // Create new attractor based on class name
            auto* newAttractor = m3d::Attr::New(attrProto.m_ClassName);
            newAttractor->ReadFromProto(attrProto);
            m_Attractors.push_back(newAttractor);
        }
    }

    ParticleSystem* ParticleSystem::CreateCopy(ParticleSystem&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsColor(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticleSystem::GetTimeDiscretion() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetParticleColor(Particle*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsTrailLen(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::MoveParticles(ParticlesList*, retruxx::vector<CVector> const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::InterpolateColors()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticleSystem::ReadRenderParams(ref_ptr<cmn::XmlNode>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsBlendMode(PBlendMode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::setParticleSize(Particle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::WriteToProtos(PSProps&, retruxx::vector<AttrProps>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsMesh(float, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetInterpolateColorsOnLoad(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ParticleSystem::IsLocal()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticleSystem::ParticleSystem()
    {
        for (int i = 0; i < 20; ++i)
        {
            auto v5 = (float)i * 0.050000001;
            auto v6 = v5;
            if (v5 >= 0.0)
            {
                if (v5 > 1.0)
                    v6 = 1.0;
            }
            else
            {
                v6 = 0.0;
            }
            auto v7 = (int)(float)(v6 * 20.0);
            if (v7 >= 0)
            {
                if (v7 > 19)
                    v7 = 19;
            }
            else
            {
                v7 = 0;
            }
            this->m_colors[v7] = -1;
            if (v5 >= 0.0)
            {
                if (v5 > 1.0)
                    v5 = 1.0;
            }
            else
            {
                v5 = 0.0;
            }
            auto v8 = (int)(float)(v5 * 20.0);
            if (v8 >= 0)
            {
                if (v8 > 19)
                    v8 = 19;
            }
            else
            {
                v8 = 0;
            }
            this->m_sizes[v8] = 0.30000001;
        }

        this->m_Emitter.m_emitAtPeriod = 10.0;
        this->m_Emitter.m_wtime.m_start = 0.0;
        this->m_Emitter.m_wtime.m_length = 1.0;
        this->m_Emitter.m_wtime.m_repeat = 1.0;
        this->m_Emitter.m_resettime = 0.0;
        this->m_Emitter.m_localStop = 0;
        this->m_Emitter.m_stopTime = 0.0;
        this->m_trailLen = 15;
        this->m_HaveTrail = 0;
        this->m_CreateOne = 0;
        this->m_scaleparts = 1.0;
        this->m_blendMode = PS_ADDSMOOTH;
        this->m_shader = 0;
        this->m_shaderType = PSST_DUST;
        this->m_meshradius = 5.0;
        this->m_autoMeshEmitter = 1;
        this->m_points = 20;
        this->m_numvert = 0;
        this->m_forv = 1;
        this->m_back = 1;
        this->m_backflag = 0;
        this->m_Specific = 0;
        
        m_Attractors.reserve(3);
        this->m_parentDependency = 0.0;
        this->m_bBox.m_box[0] = -20.0;
        this->m_bBox.m_box[1] = -20.0;
        this->m_bBox.m_box[2] = -20.0;
        this->m_bBox.m_box[3] = 20.0;
        this->m_bBox.m_box[4] = 20.0;
        this->m_bBox.m_box[5] = 20.0;
    }

    int ParticleSystem::ReadVolumeParams(ref_ptr<cmn::XmlNode>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetPsSize(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticleSystem::ReadMeshParams(ref_ptr<cmn::XmlNode>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticleSystem::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float ParticleSystem::GetBoundRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticleSystem::~ParticleSystem()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::SetTextureName(CStr const& name)
    {
        M3D_RENDERER->ReleaseTexture(this->m_texAdd);
        if (!name.empty())
        {
                this->m_texAdd = M3D_RENDERER->AddTexture(
                    name,
                    0);
                M3D_RENDERER->SetTextureParameter(
                    this->m_texAdd,
                    m3d::rend::TexParam::TM_WRAP_S,
                    3u);
                M3D_RENDERER->SetTextureParameter(
                    this->m_texAdd,
                    m3d::rend::TexParam::TM_WRAP_T,
                    3u);
                M3D_RENDERER->SetTextureParameter(
                    this->m_texAdd,
                    m3d::rend::TexParam::TM_TEX_FILTER,
                    2u);
        }
        this->m_texName = name;
    }

    void ParticleSystem::Reset(ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::AddParticles(ParticlesList*, retruxx::vector<CVector> const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::AddParticle(ParticlesList*, CVector const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::ApplyBlending()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ParticleSystem::Update(ParticlesList*, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StripOnePS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    StripOnePS::StripOnePS()
    {
        this->m_CreateOne = 1;
    }

    void StripOnePS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool StripOnePS::IsLocal()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField StripOnePS::m_IbPoolField;

    void StripOnePS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc;
            *(base - 2) = inc - 2;
            base[2] = inc + 1;
            base[3] = inc - 2;
            inc += 4;
            base += 6;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    int PhysicModelPS::Update(ParticlesList*, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    cmn::XmlNode* PhysicModelPS::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicModelPS::WriteToProtos(PSProps&, retruxx::vector<AttrProps>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PhysicModelPS::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PhysicModelPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicModelPS::ReadFromProtos(PSProps const& Prototype, retruxx::vector<AttrProps> const& AttrProtos)
    {
        ParticleSystem::ReadFromProtos(Prototype, AttrProtos);
        m_modelName = Prototype.m_PartsModelName;
    }

    void PolyPS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField PolyPS::m_IbPoolField;

    void PolyPS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 2) = inc - 2;
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc + 1;
            inc += 4;
            base += 4;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    int PolyPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SkinPS::~SkinPS()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SkinPS::Reset(ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SkinPS::Update(ParticlesList*, float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SkinPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SkinPS::SkinPS()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField Poly1PS::m_IbPoolField;

    void Poly1PS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Poly1PS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 2) = inc - 2;
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc + 1;
            inc += 4;
            base += 4;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    int Poly1PS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StripAllPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    StripAllPS::StripAllPS()
    {
        this->m_HaveTrail = 1;
    }

    rend::IbPoolField StripAllPS::m_IbPoolField;

    void StripAllPS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc;
            *(base - 2) = inc - 2;
            base[2] = inc + 1;
            base[3] = inc - 2;
            inc += 4;
            base += 6;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    bool StripAllPS::IsLocal()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StripAllPS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float LandSpritePS::GetBoundRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool LandSpritePS::IsLocal()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LandSpritePS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SpritePS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField SpritePS::m_IbPoolField;

    void SpritePS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc;
            *(base - 2) = inc - 2;
            base[2] = inc + 1;
            base[3] = inc - 2;
            inc += 4;
            base += 6;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    float SpritePS::GetBoundRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SpritePS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ModelPS::~ModelPS()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ModelPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    cmn::XmlNode* ModelPS::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int ModelPS::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ModelPS::WriteToProtos(PSProps&, retruxx::vector<AttrProps>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ModelPS::ReadFromProtos(PSProps const& Prototype, retruxx::vector<AttrProps> const& AttrProtos)
    {
        ParticleSystem::ReadFromProtos(Prototype, AttrProtos);
        m_PartsModelName = Prototype.m_PartsModelName;
        
        delete m_PartsModel;
        m_PartsModel = nullptr;

        delete m_Anim;
        m_Anim = nullptr;

        m_PartsModel = new AnimatedModel;
        m_PartsModel->Load(m_PartsModelName, true);

        m_Anim = new AnimInfo;
        m_Anim->CreateFor(m_PartsModel);
    }

    ModelPS::ModelPS()
    {
        this->m_PartsModel = 0;
        this->m_Anim = 0;
    }

    int GlowQuadPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField GlowQuadPS::m_IbPoolField;

    void GlowQuadPS::CreateIb()
    {
        //TODO: recreate normal logic 
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc;
            *(base - 2) = inc - 2;
            base[2] = inc + 1;
            base[3] = inc - 2;
            inc += 4;
            base += 6;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    float GlowQuadPS::GetBoundRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GlowQuadPS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField RainPS::m_IbPoolField;

    void RainPS::CreateIb()
    {
        //TODO: recreate normal logic 
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<int*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 0;
        do
        {
            *ptr = inc;
            ptr[1] = inc + 1;
            inc += 2;
            ptr += 2;
        } while (inc < 0x190);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    void RainPS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RainPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    rend::IbPoolField QuadPS::m_IbPoolField;

    int QuadPS::Render(CMatrix const*, ParticlesList*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void QuadPS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<int*>(ptr + 4);
        do
        {
            *(base - 1) = inc - 1;
            *base = inc;
            base[1] = inc;
            *(base - 2) = inc - 2;
            base[2] = inc + 1;
            base[3] = inc - 2;
            inc += 4;
            base += 6;
        } while (inc < 0x322);
        Application::g_pApp->m_renderer->UnlockIbPoolField(m_IbPoolField);
    }

    void QuadPS::ReleaseIb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float QuadPS::GetBoundRadius() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
