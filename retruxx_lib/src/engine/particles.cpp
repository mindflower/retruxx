#include "particles.h"
#include <m3dapp.h>

namespace m3d
{
    void Particle::Step(float)
    {
        throw std::logic_error("Not implemented");
    }

    Particle::Particle()
    {
        throw std::logic_error("Not implemented");
    }

    Particle::~Particle()
    {
        throw std::logic_error("Not implemented");
    }

    ParticlesList::~ParticlesList()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void ParticlesList::SetMeshEmitterPoints(CMatrix**, int, void**, int*, std::vector<rend::VertexType>&, std::vector<unsigned>&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesList::Step(float)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesList::SetMeshEmitterInds(int**, int*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsVolume(int, float, float, ForceType, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsVolume(CoordinatesSystemType)
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSystem* ParticleSystem::New(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetScaleParts(float)
    {
        throw std::logic_error("Not implemented");
    }

    cmn::XmlNode* ParticleSystem::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSystem* ParticleSystem::Factory(PSProps const&, std::vector<AttrProps> const&)
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSystem* ParticleSystem::Factory(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::ReadFromProtos(PSProps const&, std::vector<AttrProps> const&)
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSystem* ParticleSystem::CreateCopy(ParticleSystem&)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsColor(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::GetTimeDiscretion() const
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetParticleColor(Particle*, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsTrailLen(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::MoveParticles(ParticlesList*, std::vector<CVector> const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::InterpolateColors()
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::ReadRenderParams(ref_ptr<cmn::XmlNode>&)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsBlendMode(PBlendMode)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::setParticleSize(Particle*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::WriteToProtos(PSProps&, std::vector<AttrProps>&)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsMesh(float, int)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetInterpolateColorsOnLoad(bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool ParticleSystem::IsLocal()
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSystem::ParticleSystem()
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::ReadVolumeParams(ref_ptr<cmn::XmlNode>&)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetPsSize(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::ReadMeshParams(ref_ptr<cmn::XmlNode>&)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float ParticleSystem::GetBoundRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSystem::~ParticleSystem()
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::SetTextureName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::Reset(ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::AddParticles(ParticlesList*, std::vector<CVector> const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::AddParticle(ParticlesList*, CVector const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSystem::ApplyBlending()
    {
        throw std::logic_error("Not implemented");
    }

    int ParticleSystem::Update(ParticlesList*, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    int StripOnePS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    StripOnePS::StripOnePS()
    {
        throw std::logic_error("Not implemented");
    }

    void StripOnePS::ReleaseIb()
    {
        throw std::logic_error("Not implemented");
    }

    bool StripOnePS::IsLocal()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    cmn::XmlNode* PhysicModelPS::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicModelPS::WriteToProtos(PSProps&, std::vector<AttrProps>&)
    {
        throw std::logic_error("Not implemented");
    }

    int PhysicModelPS::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int PhysicModelPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicModelPS::ReadFromProtos(PSProps const&, std::vector<AttrProps> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PolyPS::ReleaseIb()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    SkinPS::~SkinPS()
    {
        throw std::logic_error("Not implemented");
    }

    void SkinPS::Reset(ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    int SkinPS::Update(ParticlesList*, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    int SkinPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    SkinPS::SkinPS()
    {
        throw std::logic_error("Not implemented");
    }

    rend::IbPoolField Poly1PS::m_IbPoolField;

    void Poly1PS::ReleaseIb()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    int StripAllPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    StripAllPS::StripAllPS()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void StripAllPS::ReleaseIb()
    {
        throw std::logic_error("Not implemented");
    }

    float LandSpritePS::GetBoundRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    bool LandSpritePS::IsLocal()
    {
        throw std::logic_error("Not implemented");
    }

    int LandSpritePS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritePS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void SpritePS::ReleaseIb()
    {
        throw std::logic_error("Not implemented");
    }

    ModelPS::~ModelPS()
    {
        throw std::logic_error("Not implemented");
    }

    int ModelPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    cmn::XmlNode* ModelPS::Write(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*, CStr)
    {
        throw std::logic_error("Not implemented");
    }

    int ModelPS::Read(fs::FileStream&, cmn::XmlFile*, cmn::XmlNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void ModelPS::WriteToProtos(PSProps&, std::vector<AttrProps>&)
    {
        throw std::logic_error("Not implemented");
    }

    void ModelPS::ReadFromProtos(PSProps const&, std::vector<AttrProps> const&)
    {
        throw std::logic_error("Not implemented");
    }

    ModelPS::ModelPS()
    {
        throw std::logic_error("Not implemented");
    }

    int GlowQuadPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void GlowQuadPS::ReleaseIb()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    int RainPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
    }

    rend::IbPoolField QuadPS::m_IbPoolField;

    int QuadPS::Render(CMatrix const*, ParticlesList*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    float QuadPS::GetBoundRadius() const
    {
        throw std::logic_error("Not implemented");
    }
}
