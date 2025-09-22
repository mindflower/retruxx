#include "particles.h"
#include <m3dapp.h>
#include <scene/servers/serverparticles.h>
#include <core/log.h>

namespace m3d
{
    void Particle::Step(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Particle::Particle()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Particle::~Particle()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesList::SetMeshEmitterPoints(CMatrix** mat, int numMeshes, void** verts, int* numVerts, retruxx::vector<m3d::rend::VertexType>& VertexTypes, retruxx::vector<unsigned int>& VertexTypeSizes, int numSkinMesh)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesList::Step(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesList::SetMeshEmitterInds(int**, int*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsVolume(int, float, float, ForceType, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsVolume(CoordinatesSystemType)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetScaleParts(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    cmn::XmlNode* ParticleSystem::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ParticleSystem* ParticleSystem::Factory(PSProps const& psprops, retruxx::vector<AttrProps> const& AttrProtos)
    {
        auto* system = New(psprops.m_ClassName);
        system->ReadFromProtos(psprops, AttrProtos);
        return system;
    }

    ParticleSystem* ParticleSystem::Factory(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::ReadFromProtos(PSProps const&, retruxx::vector<AttrProps> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ParticleSystem* ParticleSystem::CreateCopy(ParticleSystem&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsColor(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticleSystem::GetTimeDiscretion() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetParticleColor(Particle*, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsTrailLen(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::MoveParticles(ParticlesList*, retruxx::vector<CVector> const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::InterpolateColors()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticleSystem::ReadRenderParams(ref_ptr<cmn::XmlNode>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsBlendMode(PBlendMode)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::setParticleSize(Particle*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::WriteToProtos(PSProps&, retruxx::vector<AttrProps>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsMesh(float, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetInterpolateColorsOnLoad(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool ParticleSystem::IsLocal()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsSize(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticleSystem::ReadMeshParams(ref_ptr<cmn::XmlNode>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticleSystem::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float ParticleSystem::GetBoundRadius() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    ParticleSystem::~ParticleSystem()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::SetTextureName(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::Reset(ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::AddParticles(ParticlesList*, retruxx::vector<CVector> const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::AddParticle(ParticlesList*, CVector const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticleSystem::ApplyBlending()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticleSystem::Update(ParticlesList*, float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StripOnePS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    StripOnePS::StripOnePS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StripOnePS::ReleaseIb()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool StripOnePS::IsLocal()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    cmn::XmlNode* PhysicModelPS::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicModelPS::WriteToProtos(PSProps&, retruxx::vector<AttrProps>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int PhysicModelPS::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int PhysicModelPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicModelPS::ReadFromProtos(PSProps const&, retruxx::vector<AttrProps> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PolyPS::ReleaseIb()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    SkinPS::~SkinPS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SkinPS::Reset(ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SkinPS::Update(ParticlesList*, float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SkinPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SkinPS::SkinPS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::IbPoolField Poly1PS::m_IbPoolField;

    void Poly1PS::ReleaseIb()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int StripAllPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    StripAllPS::StripAllPS()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void StripAllPS::ReleaseIb()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float LandSpritePS::GetBoundRadius() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool LandSpritePS::IsLocal()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LandSpritePS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SpritePS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void SpritePS::ReleaseIb()
    {
        throw retruxx::logic_error("Not implemented");
    }

    ModelPS::~ModelPS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ModelPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    cmn::XmlNode* ModelPS::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ModelPS::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ModelPS::WriteToProtos(PSProps&, retruxx::vector<AttrProps>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ModelPS::ReadFromProtos(PSProps const&, retruxx::vector<AttrProps> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ModelPS::ModelPS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int GlowQuadPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void GlowQuadPS::ReleaseIb()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int RainPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::IbPoolField QuadPS::m_IbPoolField;

    int QuadPS::Render(CMatrix const*, ParticlesList*)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    float QuadPS::GetBoundRadius() const
    {
        throw retruxx::logic_error("Not implemented");
    }
}
