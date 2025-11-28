#include "particles.h"

#include <algorithm>
#include <m3dapp.h>
#include <scene/servers/serverparticles.h>
#include <core/log.h>
#include <skelmodel.h>

bool interpolateColorsOnLoad = false;

namespace m3d
{
    namespace
    {

        void addStripePart(
            const CVector& gorg0,
            const CVector& gorg1,
            const CVector& org0,
            const CVector& org1,
            float sz0,
            float sz1,
            unsigned int clr0,
            unsigned int clr1,
            const CVector& camOrg,
            unsigned int u0,
            unsigned int v0,
            float u1,
            float v1,
            int ptIdx)
        {
            // TODO: generated code addStripePart
            static CVector prevUp;
            // Calculate direction vector between the two segment points
            CVector segmentDir = org1 - org0;

            // Calculate vector from camera to second global origin
            CVector camToGorg1 = gorg1 - camOrg;

            // Calculate up vector for the second point using cross product
            CVector up1;
            up1.x = (segmentDir.z * camToGorg1.y) - (segmentDir.y * camToGorg1.z);
            up1.y = (segmentDir.x * camToGorg1.z) - (segmentDir.z * camToGorg1.x);
            up1.z = (segmentDir.y * camToGorg1.x) - (segmentDir.x * camToGorg1.y);

            // Normalize and scale the up vector for second point
            float invLength1 = 1.0f / sqrtf(up1.x * up1.x + up1.y * up1.y + up1.z * up1.z + 1.1920929e-7f);
            up1 = up1 * invLength1 * sz1;

            CVector up0;

            // Calculate up vector for first point
            if (ptIdx != 0)
            {
                // Use previous up vector (stored in global)
                up0 = prevUp;
            }
            else
            {
                // Calculate up vector for first point from scratch
                CVector camToGorg0 = gorg0 - camOrg;

                up0.x = (camToGorg0.z * segmentDir.y) - (camToGorg0.y * segmentDir.z);
                up0.y = (camToGorg0.x * segmentDir.z) - (camToGorg0.z * segmentDir.x);
                up0.z = (camToGorg0.y * segmentDir.x) - (camToGorg0.x * segmentDir.y);

                // Normalize and scale the up vector for first point
                float invLength0 = 1.0f / sqrtf(up0.x * up0.x + up0.y * up0.y + up0.z * up0.z + 1.1920929e-7f);
                up0 = up0 * invLength0 * sz0;
            }

            // Store current up vector for next iteration
            prevUp = up1;

            // Calculate quad vertices
            CVector quadVertices[4];

            // First point - left side
            quadVertices[0] = org0 - up0;
            // First point - right side
            quadVertices[1] = org0 + up0;
            // Second point - right side
            quadVertices[2] = org1 + up1;
            // Second point - left side
            quadVertices[3] = org1 - up1;

            // Get render buffer and set up quad
            m3d::rend::VertexXYZCT1* vertices = M3D_APP->RenderQuadXyzct1GetNextPtr();

            // Vertex 0: First point, left side
            vertices[0].x = quadVertices[0].x;
            vertices[0].y = quadVertices[0].y;
            vertices[0].z = quadVertices[0].z;
            vertices[0].c = clr0;
            vertices[0].tu = static_cast<float>(u0);
            vertices[0].tv = static_cast<float>(v0);

            // Vertex 1: First point, right side
            vertices[1].x = quadVertices[1].x;
            vertices[1].y = quadVertices[1].y;
            vertices[1].z = quadVertices[1].z;
            vertices[1].c = clr0;
            vertices[1].tu = u1;
            vertices[1].tv = static_cast<float>(v0);

            // Vertex 2: Second point, right side
            vertices[2].x = quadVertices[2].x;
            vertices[2].y = quadVertices[2].y;
            vertices[2].z = quadVertices[2].z;
            vertices[2].c = clr1;
            vertices[2].tu = u1;
            vertices[2].tv = v1;

            // Vertex 3: Second point, left side
            vertices[3].x = quadVertices[3].x;
            vertices[3].y = quadVertices[3].y;
            vertices[3].z = quadVertices[3].z;
            vertices[3].c = clr1;
            vertices[3].tu = static_cast<float>(u0);
            vertices[3].tv = v1;
        }
    }
    void Particle::Step(float dt)
    {
        // TODO: generated code Particle::Step
        // Update velocity with acceleration
        m_vel.x += m_accel.x * dt;
        m_vel.y += m_accel.y * dt;
        m_vel.z += m_accel.z * dt;

        // Update position with velocity
        m_locorigin.x += m_vel.x * dt;
        m_locorigin.y += m_vel.y * dt;
        m_locorigin.z += m_vel.z * dt;

        // Update rotational velocity with rotational acceleration
        m_rotvel.x += m_rotaccel.x * dt;
        m_rotvel.y += m_rotaccel.y * dt;
        m_rotvel.z += m_rotaccel.z * dt;

        // Apply rotation if rotational velocity is significant
        float rotSpeedSquared = m_rotvel.x * m_rotvel.x + m_rotvel.y * m_rotvel.y + m_rotvel.z * m_rotvel.z;

        if (rotSpeedSquared > 0.0000001f)
        {
            CMatrix matX, matY, matZ;

            // Initialize matrices as identity
            std::memset(&matX, 0, sizeof(matX));
            std::memset(&matY, 0, sizeof(matY));
            std::memset(&matZ, 0, sizeof(matZ));

            matX._11 = matX._22 = matX._33 = matX._44 = 1.0f;
            matY._11 = matY._22 = matY._33 = matY._44 = 1.0f;
            matZ._11 = matZ._22 = matZ._33 = matZ._44 = 1.0f;

            // Create rotation matrices for each axis
            float rotX = dt * m_rotvel.x;
            float rotY = dt * m_rotvel.y;
            float rotZ = dt * m_rotvel.z;

            float sinX = std::sin(rotX);
            float cosX = std::cos(rotX);
            float sinY = std::sin(rotY);
            float cosY = std::cos(rotY);
            float sinZ = std::sin(rotZ);
            float cosZ = std::cos(rotZ);

            // X-axis rotation matrix
            matX._22 = cosX;
            matX._23 = -sinX;
            matX._32 = sinX;
            matX._33 = cosX;

            // Y-axis rotation matrix
            matY._11 = cosY;
            matY._13 = sinY;
            matY._31 = -sinY;
            matY._33 = cosY;

            // Z-axis rotation matrix
            matZ._11 = cosZ;
            matZ._12 = -sinZ;
            matZ._21 = sinZ;
            matZ._22 = cosZ;

            // Combine rotations: result = matZ * matY * matX
            CMatrix result;

            result._11 = matZ._11 * matY._11 * matX._11 + matZ._12 * matY._21 * matX._11 + matZ._13 * matY._31 * matX._11;
            result._12 = matZ._11 * matY._11 * matX._12 + matZ._12 * matY._21 * matX._12 + matZ._13 * matY._31 * matX._12;
            result._13 = matZ._11 * matY._11 * matX._13 + matZ._12 * matY._21 * matX._13 + matZ._13 * matY._31 * matX._13;
            result._14 = matZ._11 * matY._11 * matX._14 + matZ._12 * matY._21 * matX._14 + matZ._13 * matY._31 * matX._14;

            result._21 = matZ._21 * matY._11 * matX._11 + matZ._22 * matY._21 * matX._11 + matZ._23 * matY._31 * matX._11;
            result._22 = matZ._21 * matY._11 * matX._12 + matZ._22 * matY._21 * matX._12 + matZ._23 * matY._31 * matX._12;
            result._23 = matZ._21 * matY._11 * matX._13 + matZ._22 * matY._21 * matX._13 + matZ._23 * matY._31 * matX._13;
            result._24 = matZ._21 * matY._11 * matX._14 + matZ._22 * matY._21 * matX._14 + matZ._23 * matY._31 * matX._14;

            result._31 = matZ._31 * matY._11 * matX._11 + matZ._32 * matY._21 * matX._11 + matZ._33 * matY._31 * matX._11;
            result._32 = matZ._31 * matY._11 * matX._12 + matZ._32 * matY._21 * matX._12 + matZ._33 * matY._31 * matX._12;
            result._33 = matZ._31 * matY._11 * matX._13 + matZ._32 * matY._21 * matX._13 + matZ._33 * matY._31 * matX._13;
            result._34 = matZ._31 * matY._11 * matX._14 + matZ._32 * matY._21 * matX._14 + matZ._33 * matY._31 * matX._14;

            result._41 = matZ._41 * matY._11 * matX._11 + matZ._42 * matY._21 * matX._11 + matZ._43 * matY._31 * matX._11;
            result._42 = matZ._41 * matY._11 * matX._12 + matZ._42 * matY._21 * matX._12 + matZ._43 * matY._31 * matX._12;
            result._43 = matZ._41 * matY._11 * matX._13 + matZ._42 * matY._21 * matX._13 + matZ._43 * matY._31 * matX._13;
            result._44 = matZ._41 * matY._11 * matX._14 + matZ._42 * matY._21 * matX._14 + matZ._43 * matY._31 * matX._14;

            // Apply rotation to position
            float newX = result._11 * m_locorigin.x + result._21 * m_locorigin.y + result._31 * m_locorigin.z;
            float newY = result._12 * m_locorigin.x + result._22 * m_locorigin.y + result._32 * m_locorigin.z;
            float newZ = result._13 * m_locorigin.x + result._23 * m_locorigin.y + result._33 * m_locorigin.z;

            m_locorigin.x = newX;
            m_locorigin.y = newY;
            m_locorigin.z = newZ;
        }

        // Reset accelerations for next frame
        m_accel.x = 0.0f;
        m_accel.y = 0.0f;
        m_accel.z = 0.0f;
        m_rotaccel.x = 0.0f;
        m_rotaccel.y = 0.0f;
        m_rotaccel.z = 0.0f;
    }

    Particle::Particle()
    {
        m_trail = 0;
    }

    Particle::~Particle()
    {
        if (m_trail)
        {
            auto* bases = (ParticleBases*)m_trail;
            TrailsPool.Delete(bases);
            m_trail = nullptr;
        }
    }

    ParticlesList::~ParticlesList()
    {
        auto particle = m_particles;
        while (particle != nullptr)
        {
            auto particleNext = particle->m_next;
            ParticlesPool.Delete(particle);
            particle = particleNext;
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

    void ParticlesList::SetAutoMeshEmitterPoints(int mode, int numVerts, float radius, CVector point1, CVector point2)
    {
        // TODO: generated code ParticlesList::SetAutoMeshEmitterPoints check this!!!!
        // Clean up existing mesh emitter data if auto-emitted
        if (this->m_meshAutoEmitted)
        {
            delete m_numMeshEmitterVerts;

            if (this->m_meshEmitterVerts && *this->m_meshEmitterVerts)
            {
                delete[] (float*)*m_meshEmitterVerts;
            }

            delete m_meshEmitterVerts;

            this->m_numMeshEmitterVerts = nullptr;
            this->m_meshEmitterVerts = nullptr;
        }

        // Mark as auto-emitted and allocate new memory
        this->m_meshAutoEmitted = true;
        this->m_numMeshes = 1;

        // Allocate memory for vertex count and vertex data
        this->m_numMeshEmitterVerts = new int;
        *m_numMeshEmitterVerts = numVerts;
        this->m_meshEmitterVerts = new void*;

        // Allocate memory for vertices (32 bytes per vertex)
        int vertexSize = 32;
        *this->m_meshEmitterVerts = new float[numVerts * 8];
        *this->m_numMeshEmitterVerts = numVerts;

        // Set up vertex types
        this->m_VertexTypes.clear();

        rend::VertexType vertexType = rend::VERTEX_XYZNT1;
        this->m_VertexTypes.push_back(vertexType);

        // Set up vertex type sizes
        this->m_VertexTypeSizes.clear();

        unsigned int typeSize = vertexSize;
        this->m_VertexTypeSizes.push_back(typeSize);

        // Generate vertices based on mode and radius
        if (radius == 0.0f)
        {
            // Linear interpolation between point1 and point2
            CVector* vertices = static_cast<CVector*>(*this->m_meshEmitterVerts);

            // Set first vertex to point1
            vertices[0] = point1;

            if (numVerts > 1)
            {
                // Calculate step size for interpolation
                float step = 1.0f / static_cast<float>(numVerts - 1);
                CVector direction = point2 - point1;
                CVector stepVector = direction * step;

                // Generate intermediate vertices
                for (int i = 1; i < numVerts - 1; i++)
                {
                    float t = static_cast<float>(i) * step;
                    vertices[i] = point1 + (direction * t);
                }

                // Set last vertex to point2
                vertices[numVerts - 1] = point2;
            }
        }
        else
        {
            // Circular arrangement around origin
            CVector* vertices = static_cast<CVector*>(*this->m_meshEmitterVerts);

            for (int i = 0; i < numVerts; i++)
            {
                float angle = static_cast<float>(i) * (2.0f * 3.14159265f) / static_cast<float>(numVerts);
                vertices[i].x = std::cos(angle) * radius;
                vertices[i].y = 0.0f;
                vertices[i].z = std::sin(angle) * radius;
            }
        }
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

    void ParticleSystem::SetParticleColor(Particle* pParticle, float fader)
    {
        // TODO: generated code ParticleSystem::SetParticleColor
        // Calculate normalized lifetime (0.0 = just born, 1.0 = about to die)
        float normalizedLifetime = (pParticle->m_ttl - pParticle->m_fade) / pParticle->m_ttl;

        // Convert to discrete color table index (0-19)
        int colorIndex = static_cast<int>(normalizedLifetime * 20.0f);
        colorIndex = std::clamp(colorIndex, 0, 19);

        // Check if the color at this index is defined (not -1)
        uint32_t colorValue = m_colors[colorIndex];

        if (colorValue == 0xFFFFFFFF)  // Undefined color
        {
            // Find the nearest defined colors before and after this index
            int prevIndex = colorIndex;
            int nextIndex = colorIndex;

            // Search backwards for defined color
            while (prevIndex >= 0 && m_colors[prevIndex] == 0xFFFFFFFF)
            {
                --prevIndex;
            }

            // Search forwards for defined color
            while (nextIndex <= 19 && m_colors[nextIndex] == 0xFFFFFFFF)
            {
                ++nextIndex;
            }

            // If we found valid color boundaries, interpolate between them
            if (prevIndex >= 0 && nextIndex <= 19)
            {
                const float INV_DISCRETION = 1.0f / 20.0f;  // Assuming this constant

                float prevTime = static_cast<float>(prevIndex) * INV_DISCRETION;
                float nextTime = static_cast<float>(nextIndex) * INV_DISCRETION;

                // Calculate interpolation factor between the two colors
                float t = (normalizedLifetime - prevTime) / (nextTime - prevTime);
                t = std::clamp(t, 0.0f, 1.0f);

                // Get the two colors to interpolate between
                uint32_t prevColor = m_colors[prevIndex];
                uint32_t nextColor = m_colors[nextIndex];

                // Interpolate RGBA components
                m3d::rend::Colori interpolatedColor;

                interpolatedColor.r = static_cast<uint8_t>(
                    (static_cast<float>((nextColor >> 0) & 0xFF) - static_cast<float>((prevColor >> 0) & 0xFF)) * t +
                    static_cast<float>((prevColor >> 0) & 0xFF) * fader);

                interpolatedColor.g = static_cast<uint8_t>(
                    (static_cast<float>((nextColor >> 8) & 0xFF) - static_cast<float>((prevColor >> 8) & 0xFF)) * t +
                    static_cast<float>((prevColor >> 8) & 0xFF) * fader);

                interpolatedColor.b = static_cast<uint8_t>(
                    (static_cast<float>((nextColor >> 16) & 0xFF) - static_cast<float>((prevColor >> 16) & 0xFF)) * t +
                    static_cast<float>((prevColor >> 16) & 0xFF) * fader);

                interpolatedColor.a = static_cast<uint8_t>(
                    (static_cast<float>((nextColor >> 24) & 0xFF) - static_cast<float>((prevColor >> 24) & 0xFF)) * t +
                    static_cast<float>((prevColor >> 24) & 0xFF) * fader);

                pParticle->m_curClr = interpolatedColor.rgba;
            }
            else
            {
                // Fallback: use the original color index if interpolation fails
                pParticle->m_curClr = colorValue;
            }
        }
        else
        {
            // Use the exact color from the table
            pParticle->m_curClr = colorValue;
        }
    }

    void ParticleSystem::SetPsTrailLen(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSystem::MoveParticles(ParticlesList* parts, retruxx::vector<CVector> const* newPoses)
    {
        // TODO: generated code ParticleSystem::MoveParticles
        if (!parts || newPoses->empty())
        {
            return;
        }

        m3d::Particle* currentParticle = parts->m_particles;
        unsigned int particleIndex = 0;

        if (this->m_updateXForm)
        {
            // Use inverse transformation
            CMatrix fromWorld = parts->m_curXFormToWorld.getInverse();

            while (currentParticle && particleIndex < newPoses->size())
            {
                CVector const& newPos = (*newPoses)[particleIndex];

                // Transform the new position from world space to local space
                float x = (newPos.x * fromWorld._11 + newPos.y * fromWorld._21 + newPos.z * fromWorld._31) + fromWorld._41;
                float y = (newPos.x * fromWorld._12 + newPos.y * fromWorld._22 + newPos.z * fromWorld._32) + fromWorld._42;
                float z = (newPos.x * fromWorld._13 + newPos.y * fromWorld._23 + newPos.z * fromWorld._33) + fromWorld._43;

                // Update particle's local origin
                currentParticle->m_locorigin.x = x;
                currentParticle->m_locorigin.y = y;
                currentParticle->m_locorigin.z = z;

                currentParticle = currentParticle->m_next;
                particleIndex++;
            }
        }
        else
        {
            // Direct position update (relative to origin)
            while (currentParticle && particleIndex < newPoses->size())
            {
                CVector const& newPos = (*newPoses)[particleIndex];

                // Calculate relative position from origin
                CVector relativePos;
                relativePos.x = newPos.x - currentParticle->m_origin.x;
                relativePos.y = newPos.y - currentParticle->m_origin.y;
                relativePos.z = newPos.z - currentParticle->m_origin.z;

                // Update particle's local origin
                currentParticle->m_locorigin = relativePos;

                currentParticle = currentParticle->m_next;
                particleIndex++;
            }
        }
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
            // TODO: check this!!!
            auto temp = parts->m_particles;
            parts->m_particles = temp->m_next;
            ParticlesPool.Delete(temp);
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

    void ParticleSystem::AddParticle(ParticlesList* parts, CVector const* pos)
    {
        // TODO: generated code ParticleSystem::AddParticle
        // Get a free particle from the pool
        m3d::Particle* particle = ParticlesPool.New();

        // Add particle to the list
        particle->m_next = parts->m_particles;
        parts->m_particles = particle;
        ++parts->m_numParticles;

        // Generate random TTL (time to live)
        float randomFactor = static_cast<float>(HIWORD(rndGet())) * 0.000015259022f;
        particle->m_fade = (this->m_Emitter.m_ttlMax - this->m_Emitter.m_ttlMin) * randomFactor + this->m_Emitter.m_ttlMin;
        particle->m_time0 = parts->m_time;

        // Calculate particle direction and origin
        if (!this->m_updateXForm)
        {
            // Simple world space calculation
            CVector diff = *pos - CVector(parts->m_curXFormToWorld._41, parts->m_curXFormToWorld._42, parts->m_curXFormToWorld._43);
            float invLength = 1.0f / sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z + 1.1920929e-7f);

            particle->m_dir = diff * invLength;
            particle->m_locorigin = diff;
            particle->m_forigin = diff;
        }
        else
        {
            // Transform position to local space
            CMatrix inverse = parts->m_curXFormToWorld.getInverse();
            CVector localPos;
            localPos.x = inverse._11 * pos->x + inverse._21 * pos->y + inverse._31 * pos->z + inverse._41;
            localPos.y = inverse._12 * pos->x + inverse._22 * pos->y + inverse._32 * pos->z + inverse._42;
            localPos.z = inverse._13 * pos->x + inverse._23 * pos->y + inverse._33 * pos->z + inverse._43;

            float invLength = 1.0f / sqrtf(localPos.x * localPos.x + localPos.y * localPos.y + localPos.z * localPos.z + 1.1920929e-7f);

            particle->m_dir = localPos * invLength;
            particle->m_locorigin = localPos;
            particle->m_forigin = localPos;
        }

        // Initialize particle physics properties
        CMatrix Local(parts->m_curXFormToWorld);
        memset(Local.m[3], 0, 12);  // Zero out translation

        particle->m_vel = CVector(0.0f, 0.0f, 0.0f);
        particle->m_rotvel = CVector(0.0f, 0.0f, 0.0f);

        // Apply attractors
        for (size_t i = 0; i < this->m_Attractors.size(); ++i)
        {
            this->m_Attractors[i]->InitParticle(particle, parts->m_time, Local, this->m_orient, this->m_scaleparts);
        }

        // Add world velocity if not updating transform
        if (!this->m_updateXForm)
        {
            CVector worldVelContribution = parts->m_worldVel * this->m_parentDependency;
            particle->m_vel += worldVelContribution;
        }

        // Initialize remaining properties
        particle->m_accel = CVector(0.0f, 0.0f, 0.0f);
        particle->m_rotaccel = CVector(0.0f, 0.0f, 0.0f);
        particle->m_ttl = particle->m_fade;

        // Initialize trail if needed
        if (this->m_HaveTrail)
        {
            particle->m_trail = (m3d::ParticleBase*)(TrailsPool.New());
            particle->m_trailSize = 0;
        }

        // Set initial position
        particle->m_origin = CVector(parts->m_curXFormToWorld._41, parts->m_curXFormToWorld._42, parts->m_curXFormToWorld._43);

        // Calculate initial size based on lifetime
        float lifeRatio = (particle->m_ttl - particle->m_fade) / particle->m_ttl;
        int sizeIndex = static_cast<int>(lifeRatio * 20.0f);
        float interpolationFactor = (lifeRatio - static_cast<float>(sizeIndex) * 0.05f) * 20.0f;

        if (sizeIndex < 0)
        {
            sizeIndex = 0;
        }
        else if (sizeIndex > 18)
        {
            particle->m_size = this->m_sizes[19];
        }
        else
        {
            particle->m_size = this->m_sizes[sizeIndex] + (this->m_sizes[sizeIndex + 1] - this->m_sizes[sizeIndex]) * interpolationFactor;
        }

        // Apply scale
        particle->m_size *= this->m_scaleparts;

        // Set initial color
        SetParticleColor(particle, 1.0f);
    }

    void ParticleSystem::ApplyBlending()
    {
        if (this->m_blendMode == PS_ADD)
        {
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->SetBlend(rend::BM_1_1, 0);
        }
        if (this->m_blendMode == PS_ADDSMOOTH)
        {
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->SetBlend(rend::BM_1_ISCOLOR, 0);
        }
        if (this->m_blendMode == PS_ALPHA)
        {
            M3D_RENDERER->SetAlphaTest(1);
            M3D_RENDERER->SetBlend(rend::BM_ALPHA, 0);
        }
        if (this->m_blendMode == PS_ADDSIGNED)
        {
            M3D_RENDERER->SetAlphaTest(0);
            M3D_RENDERER->SetBlend(rend::BM_SCOLOR_1, 0);
        }
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

    int StripOnePS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = 1;

        if (parts->m_start1 <= parts->m_PhaseTime && parts->m_numParticles > 0)
        {
            // Apply blending and setup rendering state
            ApplyBlending();
            M3D_APP->SetFlushQuadsShader(m_shader);
            M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
            M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);
            M3D_APP->StartQuads(rend::VERTEX_XYZCT1);

            // Initialize rendering variables
            int numSegments = parts->m_numParticles - 1;
            float tu = 0.0f;
            Particle* currentParticle = parts->m_particles;
            float tadd = 1.0f / static_cast<float>(parts->m_numParticles);

            // Extract transformation matrix components
            CMatrix rmat(*local);
            float local_11 = local->_11;
            float local_21 = local->_21;
            float local_31 = local->_31;
            float local_41 = local->_41;
            float local_42 = local->_42;
            float local_43 = local->_43;

            // Calculate scale factor
            float scx = std::sqrt(local_31 * local_31 + local_21 * local_21 + local_11 * local_11);

            // Get camera origin for billboarding
            CVector camOrg = M3D_RENDERER->MatGetOrgInv();

            // Render each particle segment
            for (int segmentIndex = 0; segmentIndex < numSegments; segmentIndex++)
            {
                Particle* nextParticle = currentParticle->m_next;
                CVector org, org1;

                if (this->m_updateXForm)
                {
                    // Transform current particle position
                    org.x =
                        (rmat._11 * currentParticle->m_locorigin.x + rmat._21 * currentParticle->m_locorigin.y + rmat._31 * currentParticle->m_locorigin.z) +
                        local_41;

                    org.y =
                        (rmat._12 * currentParticle->m_locorigin.x + rmat._22 * currentParticle->m_locorigin.y + rmat._32 * currentParticle->m_locorigin.z) +
                        local_42;

                    org.z =
                        (rmat._13 * currentParticle->m_locorigin.x + rmat._23 * currentParticle->m_locorigin.y + rmat._33 * currentParticle->m_locorigin.z) +
                        local_43;

                    // Transform next particle position
                    org1.x =
                        (rmat._11 * nextParticle->m_locorigin.x + rmat._21 * nextParticle->m_locorigin.y + rmat._31 * nextParticle->m_locorigin.z) + local_41;

                    org1.y =
                        (rmat._12 * nextParticle->m_locorigin.x + rmat._22 * nextParticle->m_locorigin.y + rmat._32 * nextParticle->m_locorigin.z) + local_42;

                    org1.z =
                        (rmat._13 * nextParticle->m_locorigin.x + rmat._23 * nextParticle->m_locorigin.y + rmat._33 * nextParticle->m_locorigin.z) + local_43;
                }
                else
                {
                    // Use local origin directly (no transformation)
                    org.x = currentParticle->m_origin.x + currentParticle->m_locorigin.x;
                    org.y = currentParticle->m_origin.y + currentParticle->m_locorigin.y;
                    org.z = currentParticle->m_origin.z + currentParticle->m_locorigin.z;

                    org1.x = nextParticle->m_origin.x + nextParticle->m_locorigin.x;
                    org1.y = nextParticle->m_origin.y + nextParticle->m_locorigin.y;
                    org1.z = nextParticle->m_origin.z + nextParticle->m_locorigin.z;
                }

                // Calculate sizes
                float currentSize = currentParticle->m_size * scx;
                float nextSize = nextParticle->m_size * scx;

                // Add stripe segment to render queue
                if (this->m_TexTiling == 0)
                {
                    // Tiled texture coordinates
                    addStripePart(
                        currentParticle->m_origin,
                        nextParticle->m_origin,
                        org,
                        org1,
                        currentSize,
                        nextSize,
                        currentParticle->m_curClr,
                        nextParticle->m_curClr,
                        camOrg,
                        0,
                        tu,
                        1.0f,
                        tu + tadd,
                        segmentIndex);
                    tu += tadd;
                }
                else
                {
                    // Non-tiled texture coordinates
                    addStripePart(
                        currentParticle->m_origin,
                        nextParticle->m_origin,
                        org,
                        org1,
                        currentSize,
                        nextSize,
                        currentParticle->m_curClr,
                        nextParticle->m_curClr,
                        camOrg,
                        0,
                        0.0f,
                        1.0f,
                        1.0f,
                        segmentIndex);
                }

                // Move to next particle
                currentParticle = currentParticle->m_next;
            }

            // Finish rendering
            M3D_APP->FinishQuads();
            M3D_APP->SetFlushQuadsShader(nullptr);
        }

        return 1;
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

    int StripAllPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        // TODO: generated code StripAllPS::Render
        m3d::rend::IRenderer* renderer = M3D_RENDERER;

        // Get camera position for billboarding
        CVector camOrg = renderer->MatGetOrgInv();

        CVector localOrigin = *(CVector*)&local->m[3][0];  // Extract position from matrix
        parts->m_renderCalled = true;

        // Early out if not in valid phase or no particles
        if (parts->m_start1 > parts->m_PhaseTime || !parts->m_numParticles)
            return 1;

        // Create local copy of transformation matrix
        CMatrix renderMatrix(*local);

        // Set up rendering state
        ApplyBlending();
        M3D_APP->SetFlushQuadsShader(m_shader);
        renderer->SetTexture(0, m_texAdd, -1.0f);
        renderer->SetCull(rend::M3DCULL_CW, 0);

        // Start quad batch rendering
        M3D_APP->StartQuads(rend::VERTEX_XYZCT1);

        if (m_trailLen == 10)
        {
            // Render particles as simple strips (no trails)
            for (m3d::Particle* particle = parts->m_particles; particle; particle = particle->m_next)
            {
                if (m_updateXForm)
                {
                    // Transform particle positions using the render matrix
                    CVector transformedPos;
                    transformedPos.x = (renderMatrix._11 * particle->m_locorigin.x) + (renderMatrix._21 * particle->m_locorigin.y) +
                        (renderMatrix._31 * particle->m_locorigin.z);
                    transformedPos.y = (renderMatrix._12 * particle->m_locorigin.x) + (renderMatrix._22 * particle->m_locorigin.y) +
                        (renderMatrix._32 * particle->m_locorigin.z);
                    transformedPos.z = (renderMatrix._13 * particle->m_locorigin.x) + (renderMatrix._23 * particle->m_locorigin.y) +
                        (renderMatrix._33 * particle->m_locorigin.z);

                    // Transform forward origin
                    CVector transformedForward;
                    transformedForward.x = (renderMatrix._11 * particle->m_forigin.x) + (renderMatrix._21 * particle->m_forigin.y) +
                        (renderMatrix._31 * particle->m_forigin.z) + localOrigin.x;
                    transformedForward.y = (renderMatrix._12 * particle->m_forigin.x) + (renderMatrix._22 * particle->m_forigin.y) +
                        (renderMatrix._32 * particle->m_forigin.z) + localOrigin.y;
                    transformedForward.z = (renderMatrix._13 * particle->m_forigin.x) + (renderMatrix._23 * particle->m_forigin.y) +
                        (renderMatrix._33 * particle->m_forigin.z) + localOrigin.z;

                    // Add transformed position
                    transformedPos.x += localOrigin.x;
                    transformedPos.y += localOrigin.y;
                    transformedPos.z += localOrigin.z;

                    // Add stripe part for this particle
                    addStripePart(
                        particle->m_origin,  // startPos1
                        particle->m_origin,  // startPos2
                        transformedPos,      // endPos1
                        transformedForward,  // endPos2
                        particle->m_size,     // startSize
                        particle->m_size,     // endSize
                        particle->m_curClr,   // startColor
                        particle->m_curClr,   // endColor
                        camOrg,              // camera position
                        0,                    // texture coordinate U
                        0,                    // texture coordinate V
                        1.0f,                 // texture scale U
                        1.0f,                 // texture scale V
                        0                     // flags
                    );
                }
                else
                {
                    // Use world coordinates directly
                    CVector worldPos;
                    worldPos.x = particle->m_origin.x + particle->m_locorigin.x;
                    worldPos.y = particle->m_origin.y + particle->m_locorigin.y;
                    worldPos.z = particle->m_origin.z + particle->m_locorigin.z;

                    CVector worldForward;
                    worldForward.x = particle->m_origin.x + particle->m_forigin.x;
                    worldForward.y = particle->m_origin.y + particle->m_forigin.y;
                    worldForward.z = particle->m_origin.z + particle->m_forigin.z;

                    // Add stripe part for this particle
                    addStripePart(
                        worldPos,           // startPos1
                        worldPos,           // startPos2
                        worldPos,           // endPos1
                        worldForward,       // endPos2
                        particle->m_size,    // startSize
                        particle->m_size,    // endSize
                        particle->m_curClr,  // startColor
                        particle->m_curClr,  // endColor
                        camOrg,             // camera position
                        0,                   // texture coordinate U
                        0,                   // texture coordinate V
                        1.0f,                // texture scale U
                        1.0f,                // texture scale V
                        0                    // flags
                    );
                }
            }
        }
        else
        {
            // Render particles with trails
            for (m3d::Particle* particle = parts->m_particles; particle; particle = particle->m_next)
            {
                int trailSize = particle->m_trailSize;
                if (trailSize >= 1)
                {
                    float textureIncrement = 1.0f / (trailSize + 1);
                    float currentTexCoord = 1.0f;

                    // Render trail segments
                    for (int segment = 0; segment < trailSize; segment++)
                    {
                        int trailIndex = trailSize - 1 - segment;
                        auto* trail = &particle->m_trail[trailIndex];

                        if (m_updateXForm)
                        {
                            // Transform trail positions
                            CVector transformedCurrent;
                            transformedCurrent.x = (renderMatrix._11 * particle->m_locorigin.x) + (renderMatrix._21 * particle->m_locorigin.y) +
                                (renderMatrix._31 * particle->m_locorigin.z) + localOrigin.x;
                            transformedCurrent.y = (renderMatrix._12 * particle->m_locorigin.x) + (renderMatrix._22 * particle->m_locorigin.y) +
                                (renderMatrix._32 * particle->m_locorigin.z) + localOrigin.y;
                            transformedCurrent.z = (renderMatrix._13 * particle->m_locorigin.x) + (renderMatrix._23 * particle->m_locorigin.y) +
                                (renderMatrix._33 * particle->m_locorigin.z) + localOrigin.z;

                            // Transform trail position
                            CVector transformedTrail;
                            transformedTrail.x = (renderMatrix._11 * trail->m_locorigin.x) + (renderMatrix._21 * trail->m_locorigin.y) +
                                (renderMatrix._31 * trail->m_locorigin.z) + localOrigin.x;
                            transformedTrail.y = (renderMatrix._12 * trail->m_locorigin.x) + (renderMatrix._22 * trail->m_locorigin.y) +
                                (renderMatrix._32 * trail->m_locorigin.z) + localOrigin.y;
                            transformedTrail.z = (renderMatrix._13 * trail->m_locorigin.x) + (renderMatrix._23 * trail->m_locorigin.y) +
                                (renderMatrix._33 * trail->m_locorigin.z) + localOrigin.z;

                            addStripePart(
                                transformedCurrent,                 // startPos1
                                transformedTrail,                   // startPos2
                                transformedCurrent,                 // endPos1
                                transformedTrail,                   // endPos2
                                particle->m_size,                    // startSize
                                trail->m_size,                         // endSize
                                particle->m_curClr,                  // startColor
                                trail->m_curClr,                        // endColor
                                camOrg,                             // camera position
                                0,                                   // texture coordinate U
                                currentTexCoord + textureIncrement,  // texture coordinate V
                                1.0f,                                // texture scale U
                                currentTexCoord,                     // texture scale V
                                segment                              // segment index
                            );
                        }
                        else
                        {
                            // Use world coordinates directly for trail
                            CVector worldCurrent;
                            worldCurrent.x = particle->m_origin.x + particle->m_locorigin.x;
                            worldCurrent.y = particle->m_origin.y + particle->m_locorigin.y;
                            worldCurrent.z = particle->m_origin.z + particle->m_locorigin.z;

                            CVector worldTrail;
                            worldTrail.x = particle->m_origin.x + trail->m_locorigin.x;
                            worldTrail.y = particle->m_origin.y + trail->m_locorigin.y;
                            worldTrail.z = particle->m_origin.z + trail->m_locorigin.z;

                            addStripePart(
                                worldCurrent,                       // startPos1
                                worldTrail,                         // startPos2
                                worldCurrent,                       // endPos1
                                worldTrail,                         // endPos2
                                particle->m_size,                    // startSize
                                trail->m_size,                         // endSize
                                particle->m_curClr,                  // startColor
                                trail->m_curClr,                        // endColor
                                camOrg,                             // camera position
                                0,                                   // texture coordinate U
                                currentTexCoord + textureIncrement,  // texture coordinate V
                                1.0f,                                // texture scale U
                                currentTexCoord,                     // texture scale V
                                segment                              // segment index
                            );
                        }

                        currentTexCoord -= textureIncrement;
                    }
                }
            }
        }

        // Finish quad batch rendering
        M3D_APP->FinishQuads();
        M3D_APP->SetFlushQuadsShader(nullptr);

        return 1;
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
        // TODO implement SpritePS::Render
        // RETRUXX_NOT_IMPLEMENTED;
        return 1;
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

    int GlowQuadPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        // TODO: generated code GlowQuadPS::Render
        parts->m_renderCalled = true;

        // Early out if not in valid phase or no particles
        if (parts->m_start1 > parts->m_PhaseTime || !parts->m_numParticles)
            return 1;

        m3d::rend::IRenderer* renderer = M3D_RENDERER;

        // Apply transformation if needed
        if (m_updateXForm)
        {
            renderer->MatPush(*local);
        }

        // Get basis vectors from current matrix
        CVector right, up, forward;
        renderer->MatGetBasis(right, up, forward);

        // Create quad vertices in local space
        CVector quad[4];

        // Calculate quad corners relative to particle position
        quad[0] = (-up - right);  // Bottom-left
        quad[1] = (-up + right);  // Bottom-right
        quad[2] = (up + right);   // Top-right
        quad[3] = (up - right);   // Top-left

        float DirScale = 1.0f;

        // Handle directional scaling for certain effects
        if (m_forv)
        {
            CVector cameraOrg = renderer->MatGetOrgInv();

            CVector viewDir = cameraOrg.getNormalized();
            DirScale = viewDir.y;  // Scale based on view direction

            if (DirScale <= 0.0f)
            {
                if (m_updateXForm)
                    renderer->MatPop(1);
                return 1;
            }
        }

        // Check if particle system is visible via line trace
        CVector localPos(local->_41, local->_42, local->_43);
        if (parts->m_TLM.TraceLine(localPos, M3D_APP->m_curCamera.m_worldOrigin))
        {
            if (m_updateXForm)
                renderer->MatPop(1);
            return 1;
        }

        // Set up rendering state
        ApplyBlending();
        renderer->SetTexture(0, m_texAdd, -1.0f);
        renderer->SetCull(rend::M3DCULL_CW, 0);
        renderer->PushZbState();

        // Lock vertex buffer for particle data
        m3d::rend::VbHandle vb = renderer->GetVbStreaming(rend::VERTEX_XYZCT1);

        int totalVertices = 4 * parts->m_numParticles;
        int vbOffset = 0;

        // Lock vertex buffer for writing
        char* vtxData = (char*)renderer->LockVbStreaming(vb, totalVertices, vbOffset, 0);

        // Fill vertex buffer with particle data
        m3d::Particle* particle = parts->m_particles;
        float* vtxPtr = (float*)(vtxData + 32);  // Skip some initial data

        while (particle)
        {
            CVector particlePos;

            if (m_updateXForm)
            {
                // Use local origin directly
                particlePos = particle->m_locorigin;
            }
            else
            {
                // Combine world and local origin
                particlePos = particle->m_origin + particle->m_locorigin;
            }

            float scaledSize = particle->m_size * DirScale;

            // Generate quad vertices for this particle
            for (int i = 0; i < 4; i++)
            {
                // Calculate vertex position
                vtxPtr[0] = particlePos.x + quad[i].x * scaledSize;  // x
                vtxPtr[1] = particlePos.y + quad[i].y * scaledSize;  // y
                vtxPtr[2] = particlePos.z + quad[i].z * scaledSize;  // z

                // Set vertex color (particle color)
                vtxPtr[3] = *(float*)&particle->m_curClr;

                // Set texture coordinates
                static const float texCoords[4][2] = {
                    {0.0f, 0.0f},  // Bottom-left
                    {1.0f, 0.0f},  // Bottom-right
                    {1.0f, 1.0f},  // Top-right
                    {0.0f, 1.0f}   // Top-left
                };

                vtxPtr[4] = texCoords[i][0];
                vtxPtr[5] = texCoords[i][1];

                vtxPtr += 6;  // Move to next vertex (XYZ + Color + TexCoord)
            }

            particle = particle->m_next;
        }

        // Unlock and render
        renderer->UnlockVb(vb);
        renderer->SetToStream0(vb);
        renderer->SetIndices(m_IbPoolField, vbOffset);

        // Draw the particles as quads (2 triangles per particle)
        renderer->DrawIndexedPrimitiveEffect(rend::M3DPT_TRIANGLELIST, m_shader, 0, totalVertices, m_IbPoolField.RealOffset, 2 * parts->m_numParticles);

        // Restore render state
        renderer->PopZbState();

        if (m_updateXForm)
        {
            renderer->MatPop(1);
        }

        return 1;
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
