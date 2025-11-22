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
        m_trail = 0;
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

    void ParticlesList::Step(float dt)
    {
        // TODO: generated code ParticlesList::Step
        // Linear motion integration
        CVector linearAcceleration = m_accel;
        m_vel += linearAcceleration * dt;
        m_origin += m_vel * dt;

        // Rotational motion integration
        CVector angularAcceleration = m_rotaccel;
        m_mrotvel += angularAcceleration * dt;
        m_rotvel += m_mrotvel * dt;

        // Clear accumulated forces for next frame
        // (forces will be re-applied based on physics simulation)
        m_accel = ZeroVector;
        m_rotaccel = ZeroVector;
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

    void ParticleSystem::Reset(ParticlesList* parts)
    {
        // TODO: generated code ParticleSystem::Reset
        parts->m_vel = ZeroVector;
        parts->m_rotvel = ZeroVector;
        parts->m_mrotvel = ZeroVector;
        parts->m_accel = ZeroVector;
        parts->m_rotaccel = ZeroVector;
        parts->m_origin = ZeroVector;
        for (int i = 0; i < parts->m_numParticles; ++i)
        {
            ParticlesPool.Delete((&parts->m_particles)[i]);
        }
        parts->m_numParticles = 0;

        // Generate random point2Max using linear congruential generator
        float randomX = static_cast<float>(rndGet() >> 16) * 0.000015259022f;  // 1.0f / 65536.0f
        parts->m_point2Max.x = (m_point2Max.x - m_point2.x) * randomX + m_point2.x;

        float randomY = static_cast<float>(rndGet() >> 16) * 0.000015259022f;
        parts->m_point2Max.y = (m_point2Max.y - m_point2.y) * randomY + m_point2.y;

        float randomZ = static_cast<float>(rndGet() >> 16) * 0.000015259022f;
        parts->m_point2Max.z = (m_point2Max.z - m_point2.z) * randomZ + m_point2.z;

        // Setup auto mesh emitter if enabled
        if (m_autoMeshEmitter)
        {
            parts->SetAutoMeshEmitterPoints(0, m_points, m_meshradius, m_point1, parts->m_point2Max);
        }

        parts->m_time = 0.0f;

        // Calculate initial forces
        CVector dest;
        m3d::CalcForcesCarthesian(dest, m_pos, 0.0f);

        // Handle mesh emitter vertex selection
        int* numMeshVerts = parts->m_numMeshEmitterVerts;
        if (numMeshVerts)
        {
            if (!m_forv && !m_back)
            {
                m_numvert = numMeshVerts[parts->m_numSkinMesh] - 1;
                m_backflag = true;
            }
            else if (m_forv)
            {
                m_numvert = 0;
                m_backflag = false;
            }
            else if (m_back)
            {
                m_numvert = numMeshVerts[parts->m_numSkinMesh] - 1;
                m_backflag = true;
            }
        }

        // Calculate random start time
        float randomStart = static_cast<float>(rndGet() >> 16) * 0.000015259022f;
        parts->m_start1 = (m_Emitter.m_wtime.m_start - m_Emitter.m_start) * randomStart + m_Emitter.m_start;

        // Initialize transformation matrix
        CMatrix Local(parts->m_curXFormToWorld);
        Local.m[3][0] = 0.0f;
        Local.m[3][1] = 0.0f;
        Local.m[3][2] = 0.0f;

        // Initialize attractors
        for (size_t i = 0; i < m_Attractors.size(); ++i)
            {
                m_Attractors[i]->InitParticlesList(parts, Local, m_orient, m_scaleparts);
            }

        // Apply initial forces to origin
        parts->m_origin += dest;
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

    int ParticleSystem::Update(ParticlesList* parts, float lastFrameSecs, float fader)
    {
        // TODO: generated code ParticleSystem::Update
        // Early return if phase time not reached
        if (parts->m_start1 > parts->m_PhaseTime)
        {
            parts->m_updateCalled = true;
            parts->m_PhaseTime += lastFrameSecs;
            return 1;
        }

        parts->m_updateCalled = true;

        // Handle reset timing
        if (m_Emitter.m_resettime != 0.0f)
        {
            if (parts->m_time <= m_Emitter.m_resettime)
            {
                if (parts->m_time == 0.0f)
                {
                    Reset(parts);
                    parts->m_RandShader = m3d::rnd(0.0f, 1.0f);
                }
            }
            else
            {
                Reset(parts);
                parts->m_PhaseTime = 0.0f;
                parts->m_RandShader = rndGet();
            }
        }

        // Set up local coordinate system
        CMatrix Local = parts->m_curXFormToWorld;
        Local.m[3][0] = 0.0f;
        Local.m[3][1] = 0.0f;
        Local.m[3][2] = 0.0f;

        // Update attractors
        for (unsigned int i = 0; i < this->m_Attractors.size(); i++)
        {
            m_Attractors[i]->SetState(parts->m_time);
        }

        // Process existing particles
        m3d::Particle** ppParticle = &parts->m_particles;

        while (*ppParticle)
        {
            m3d::Particle* current = *ppParticle;

            // Apply attractors to current particle
            for (unsigned int j = 0; j < this->m_Attractors.size(); j++)
                {
                    this->m_Attractors[j]->AffectParticle(current, parts->m_time, Local, this->m_orient, this->m_scaleparts);
                }

            // Apply emitter stop conditions
            this->m_Emitter.LocalStop(current, parts->m_time);

            // Update fade time
            current->m_fade -= lastFrameSecs;

            if (current->m_fade > 0.0f)
            {
                // Handle trail updates
                if (this->m_HaveTrail)
                {
                    int trailIndex = (int)((1.0f - (current->m_fade / current->m_ttl)) * (this->m_trailLen - 1));
                    if (trailIndex < 0)
                        trailIndex = 0;
                    if (trailIndex > this->m_trailLen - 1)
                        trailIndex = this->m_trailLen - 1;

                    if (trailIndex >= current->m_trailSize)
                    {
                        // Update trail point
                        m3d::ParticleBase* trailPoint = &current->m_trail[current->m_trailSize];
                        trailPoint->m_origin = current->m_origin;
                        trailPoint->m_locorigin = current->m_locorigin;
                        trailPoint->m_size = current->m_size;
                        trailPoint->m_curClr = current->m_curClr;
                        current->m_trailSize++;
                    }
                }

                // Step particle physics
                current->Step(lastFrameSecs);

                // Update world position if needed
                if (this->m_updateXForm)
                {
                    current->m_origin.x = parts->m_curXFormToWorld._41;
                    current->m_origin.y = parts->m_curXFormToWorld._42;
                    current->m_origin.z = parts->m_curXFormToWorld._43;
                }

                ppParticle = &current->m_next;
            }
            else
            {
                // Remove dead particle
                *ppParticle = current->m_next;
                ParticlesPool.Delete(current);

                // Return particle to pool
                --parts->m_numParticles;
            }
        }

        // Affect particles list with attractors
        for (unsigned int k = 0; k < this->m_Attractors.size(); k++)
            {
                this->m_Attractors[k]->AffectParticlesList(parts, Local, this->m_orient, this->m_scaleparts);
            }

        // Step particle list and emit new particles
        parts->Step(lastFrameSecs);
        unsigned int particlesToEmit = m_Emitter.Emit(parts->m_time, lastFrameSecs);
        unsigned int dwParticlesEmit = parts->m_numParticles + particlesToEmit;

        // Emit new particles
        if (parts->m_numParticles < parts->m_maxParticles)
        {
            do
            {
                if (parts->m_numParticles >= dwParticlesEmit)
                    break;

                // Handle mesh emitter vertex traversal
                if (parts->m_numMeshEmitterVerts)
                {
                    int numVerts = parts->m_numMeshEmitterVerts[parts->m_numSkinMesh];
                    int maxIndex = numVerts - 1;

                    // Complex vertex index update logic (matches assembly)
                    if (this->m_numvert > maxIndex && this->m_forv && !this->m_back)
                    {
                        this->m_numvert = 0;
                        this->m_backflag = 0;
                    }
                    else if (this->m_numvert < 0 && !this->m_forv && !this->m_back)
                    {
                        this->m_numvert = maxIndex;
                        this->m_backflag = 1;
                    }
                    else if (this->m_numvert < 0 && !this->m_forv && this->m_back)
                    {
                        this->m_numvert = 1;
                        this->m_backflag = 0;
                    }
                    else if (this->m_numvert > maxIndex && this->m_forv && this->m_back)
                    {
                        this->m_numvert = maxIndex - 1;
                        this->m_backflag = 1;
                    }
                    else if (this->m_numvert < 0 && this->m_forv && this->m_back)
                    {
                        this->m_numvert = 1;
                        this->m_backflag = 0;
                    }
                    else if (this->m_numvert > maxIndex && !this->m_forv && this->m_back)
                    {
                        this->m_numvert = maxIndex - 1;
                        this->m_backflag = 0;
                    }
                }

                // Get new particle from pool
                m3d::Particle* newParticle = ParticlesPool.New();

                // Initialize particle linked list
                newParticle->m_next = parts->m_particles;
                parts->m_particles = newParticle;
                ++parts->m_numParticles;

                // Set particle lifetime
                unsigned int seed = rndGet();
                newParticle->m_fade =
                    ((this->m_Emitter.m_ttlMax - this->m_Emitter.m_ttlMin) * (float)(seed >> 16) * 0.000015259022f) + this->m_Emitter.m_ttlMin;
                newParticle->m_time0 = parts->m_time;

                // Calculate initial forces and direction
                CVector dest;
                if (this->m_x0Cst != PS_CST_CARTHESIAN)
                {
                    m3d::CalcForcesPolar(dest, m_x0, parts->m_time);
                }
                else
                {
                    m3d::CalcForcesCarthesian(dest, m_x0, parts->m_time);
                }

                // Calculate normalized direction
                CVector org;
                org.x = parts->m_origin.x + dest.x;
                org.y = parts->m_origin.y + dest.y;
                org.z = parts->m_origin.z + dest.z;

                float invLength = 1.0f / sqrtf(org.z * org.z + org.y * org.y + org.x * org.x + 1.1920929e-7f);
                newParticle->m_dir.x = invLength * org.x;
                newParticle->m_dir.y = invLength * org.y;
                newParticle->m_dir.z = invLength * org.z;

                // Calculate initial position
                float posX = parts->m_origin.x;
                float posY = parts->m_origin.y;
                float posZ = parts->m_origin.z;

                if (parts->m_meshEmitterVerts)
                {
                    float* vertexData = (float*)((char*)parts->m_meshEmitterVerts[parts->m_numSkinMesh] +
                                                 parts->m_VertexTypeSizes[parts->m_numSkinMesh] * this->m_numvert);
                    posX += vertexData[0];
                    posY += vertexData[1];
                    posZ += vertexData[2];
                }

                // Set scaled positions
                newParticle->m_locorigin.x = this->m_scaleparts * posX;
                newParticle->m_locorigin.y = this->m_scaleparts * posY;
                newParticle->m_locorigin.z = this->m_scaleparts * posZ;
                newParticle->m_forigin = newParticle->m_locorigin;

                // Initialize velocities and accelerations to zero
                newParticle->m_vel.x = 0.0f;
                newParticle->m_vel.y = 0.0f;
                newParticle->m_vel.z = 0.0f;
                newParticle->m_accel.x = 0.0f;
                newParticle->m_accel.y = 0.0f;
                newParticle->m_accel.z = 0.0f;
                newParticle->m_rotvel.x = 0.0f;
                newParticle->m_rotvel.y = 0.0f;
                newParticle->m_rotvel.z = 0.0f;
                newParticle->m_rotaccel.x = 0.0f;
                newParticle->m_rotaccel.y = 0.0f;
                newParticle->m_rotaccel.z = 0.0f;

                // Apply attractor initialization
                for (unsigned int m = 0; m < this->m_Attractors.size(); m++)
                    {
                        this->m_Attractors[m]->InitParticle(newParticle, parts->m_time, Local, this->m_orient, this->m_scaleparts);
                    }

                // Apply parent velocity if not updating transform
                if (!this->m_updateXForm)
                {
                    newParticle->m_vel.x += parts->m_worldVel.x * this->m_parentDependency;
                    newParticle->m_vel.y += parts->m_worldVel.y * this->m_parentDependency;
                    newParticle->m_vel.z += parts->m_worldVel.z * this->m_parentDependency;
                }

                // Set initial world position
                newParticle->m_origin.x = parts->m_curXFormToWorld._41;
                newParticle->m_origin.y = parts->m_curXFormToWorld._42;
                newParticle->m_origin.z = parts->m_curXFormToWorld._43;

                // Initialize trail if enabled
                if (this->m_HaveTrail)
                {
                    newParticle->m_trail = (m3d::ParticleBase*)TrailsPool.New();
                    newParticle->m_trailSize = 0;
                }

                // Update total lifetime
                newParticle->m_ttl = newParticle->m_fade;

                // Update vertex index
                if (this->m_backflag)
                {
                    --this->m_numvert;
                }
                else
                {
                    ++this->m_numvert;
                }
            } while ((!this->m_CreateOne || this->m_Emitter.m_wtime.m_length == 0.1f) && parts->m_numParticles < parts->m_maxParticles);
        }

        // Update particle colors and sizes
        for (m3d::Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            this->SetParticleColor(particle, fader);

            // Interpolate particle size based on lifetime
            float lifeRatio = (particle->m_ttl - particle->m_fade) / particle->m_ttl;
            int sizeIndex = (int)(lifeRatio * 20.0f);
            float interpolationFactor = (lifeRatio - ((float)sizeIndex * (1.0f / 20.0f))) * 20.0f;

            if (sizeIndex >= 0)
            {
                if (sizeIndex > 18)
                {
                    particle->m_size = this->m_sizes[19];
                }
                else
                {
                    particle->m_size = (this->m_sizes[sizeIndex + 1] - this->m_sizes[sizeIndex]) * interpolationFactor + this->m_sizes[sizeIndex];
                }
            }
            else
            {
                sizeIndex = 0;
                particle->m_size = (this->m_sizes[sizeIndex + 1] - this->m_sizes[sizeIndex]) * interpolationFactor + this->m_sizes[sizeIndex];
            }

            particle->m_size *= this->m_scaleparts;
        }

        // Update timing
        parts->m_time += lastFrameSecs;
        parts->m_spriteAngle += this->m_SpriteAngle * lastFrameSecs;

        return 1;
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

    int QuadPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        // TODO: generated code QuadPS::Render
        parts->m_renderCalled = true;
        // Check if particles should be rendered based on timing
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0)
        {
            return 1;
        }

        // Apply transformation if needed
        if (m_updateXForm)
        {
            M3D_RENDERER->MatPush(*local);
        }

        // Get camera basis vectors for billboarding
        CVector right, up, forward;
        Application::g_pApp->m_renderer->MatGetBasis(right, up, forward);

        // Create quad vertices for billboarded particles
        CVector quad[4];

        // Initialize quad vertices
        quad[0] = CVector(-up.x - right.x, -up.y - right.y, -up.z - right.z);
        quad[1] = CVector(-up.x + right.x, -up.y + right.y, -up.z + right.z);
        quad[2] = CVector(right.x + up.x, right.y + up.y, right.z + up.z);
        quad[3] = CVector(up.x - right.x, up.y - right.y, up.z - right.z);

        // Set up rendering states
        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);

        // Get vertex buffer for streaming particle data
        m3d::rend::VbHandle vbHandle = Application::g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1);

        int vertexCount = 4 * parts->m_numParticles;
        int lockOffset;
        char* vertexData = (char*)M3D_RENDERER->LockVbStreaming(vbHandle, vertexCount, lockOffset, 0);

        // Fill vertex buffer with particle data
        Particle* currentParticle = parts->m_particles;
        if (currentParticle)
        {
            float* vertexPtr = (float*)(vertexData + 32);  // Start after some header

            do
            {
                float orgX, orgY, orgZ;

                // Calculate particle position
                if (this->m_updateXForm)
                {
                    orgX = currentParticle->m_locorigin.x;
                    orgY = currentParticle->m_locorigin.y;
                    orgZ = currentParticle->m_locorigin.z;
                }
                else
                {
                    orgX = currentParticle->m_origin.x + currentParticle->m_locorigin.x;
                    orgY = currentParticle->m_origin.y + currentParticle->m_locorigin.y;
                    orgZ = currentParticle->m_origin.z + currentParticle->m_locorigin.z;
                }

                float particleSize = currentParticle->m_size;
                uint32_t particleColor = currentParticle->m_curClr;

                // Vertex 0 (bottom-left)
                vertexPtr[0] = orgX + quad[0].x * particleSize;  // x
                vertexPtr[1] = orgY + quad[0].y * particleSize;  // y
                vertexPtr[2] = orgZ + quad[0].z * particleSize;  // z
                vertexPtr[3] = *(float*)&particleColor;          // color
                vertexPtr[4] = 0.0f;                             // tu
                vertexPtr[5] = 0.0f;                             // tv

                // Vertex 1 (bottom-right)
                vertexPtr[6] = orgX + quad[1].x * particleSize;  // x
                vertexPtr[7] = orgY + quad[1].y * particleSize;  // y
                vertexPtr[8] = orgZ + quad[1].z * particleSize;  // z
                vertexPtr[9] = *(float*)&particleColor;          // color
                vertexPtr[10] = 1.0f;                            // tu
                vertexPtr[11] = 0.0f;                            // tv

                // Vertex 2 (top-right)
                vertexPtr[12] = orgX + quad[2].x * particleSize;  // x
                vertexPtr[13] = orgY + quad[2].y * particleSize;  // y
                vertexPtr[14] = orgZ + quad[2].z * particleSize;  // z
                vertexPtr[15] = *(float*)&particleColor;          // color
                vertexPtr[16] = 1.0f;                             // tu
                vertexPtr[17] = 1.0f;                             // tv

                // Vertex 3 (top-left)
                vertexPtr[18] = orgX + quad[3].x * particleSize;  // x
                vertexPtr[19] = orgY + quad[3].y * particleSize;  // y
                vertexPtr[20] = orgZ + quad[3].z * particleSize;  // z
                vertexPtr[21] = *(float*)&particleColor;          // color
                vertexPtr[22] = 0.0f;                             // tu
                vertexPtr[23] = 1.0f;                             // tv

                currentParticle = currentParticle->m_next;
                vertexPtr += 24;  // Advance 24 floats (6 per vertex * 4 vertices)

            } while (currentParticle);
        }

        // Unlock and render the vertex buffer
        M3D_RENDERER->UnlockVb(vbHandle);
        M3D_RENDERER->SetToStream0(vbHandle);

        // Draw the particles using indexed primitives
        M3D_RENDERER->SetIndices(m_IbPoolField, lockOffset);

        M3D_RENDERER->DrawIndexedPrimitiveEffect(
            rend::M3DPT_TRIANGLELIST,
            m_shader,
            0,                                 // start vertex
            vertexCount,                       // vertex count
            m_IbPoolField.RealOffset,  // start index
            2 * parts->m_numParticles  // primitive count (2 triangles per quad)
        );

        // Restore transformation if needed
        if (m_updateXForm)
        {
            M3D_RENDERER->MatPop(true);
        }

        return 1;
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
