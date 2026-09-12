#include "particles.h"

#include "world.h"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <m3dapp.h>
#include <scene/servers/serverparticles.h>
#include <core/log.h>
#include <core/ini.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <skelmodel.h>
#include <client.h>
#include <level.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypemanager.h>
#include <server/objects/dummyobject.h>

bool interpolateColorsOnLoad = false;

namespace
{
    // The color/size keyframe tables hold TIME_DISCRETION entries spanning a
    // normalized particle lifetime of 0..1.
    int constexpr TIME_DISCRETION = 20;
    float constexpr INV_DISCRETION = 1.0f / 20.0f;

    // Sentinel stored in ParticleSystem::m_colors for "no keyframe here".
    unsigned int constexpr UNDEFINED_COLOR = 0xFFFFFFFFu;

    // Points held per trail by one ParticleBases block from TrailsPool.
    int constexpr MAX_TRAIL_LEN = 15;

    // Reads an integer-valued flag attribute; an absent or empty one keeps the default.
    bool ReadFlagAttrib(ref_ptr<m3d::cmn::XmlNode> const& node, char const* name, bool defaultValue)
    {
        CStr const str = node->GetAttribute(name);
        if (!str.c_str() || !*str.c_str())
        {
            return defaultValue;
        }
        int value = 0;
        sscanf(str.c_str(), "%i", &value);
        return value != 0;
    }

    // Reads a float attribute; returns false (and sets the default) when absent or empty.
    bool ReadFloatAttrib(float& dest, ref_ptr<m3d::cmn::XmlNode> const& node, char const* name, float defaultValue)
    {
        CStr const str = node->GetAttribute(name);
        if (!str.c_str() || !*str.c_str())
        {
            dest = defaultValue;
            return false;
        }
        sscanf(str.c_str(), "%f", &dest);
        return true;
    }

    // Reads a "x y z" attribute, leaving dest untouched when it is absent or empty.
    bool ReadVectorAttrib(CVector& dest, ref_ptr<m3d::cmn::XmlNode> const& node, char const* name)
    {
        CStr const str = node->GetAttribute(name);
        if (!str.c_str() || !*str.c_str())
        {
            return false;
        }
        sscanf(str.c_str(), "%f %f %f", &dest.x, &dest.y, &dest.z);
        return true;
    }

    // Copies a CStr into one of PSProps' fixed-size, NUL-terminated name buffers.
    template<size_t N>
    void CopyToFixedBuffer(char (&dest)[N], CStr const& src)
    {
        char const* text = src.c_str();
        if (!text || !*text)
        {
            dest[0] = 0;
            return;
        }
        std::strncpy(dest, text, N - 1);
        dest[N - 1] = 0;
    }
}  // namespace

namespace m3d
{
    namespace
    {

        void addStripePart(
            CVector const& gorg0,
            CVector const& gorg1,
            CVector const& org0,
            CVector const& org1,
            float sz0,
            float sz1,
            unsigned int clr0,
            unsigned int clr1,
            CVector const& camOrg,
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
    }  // namespace
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

            // Combine rotations. The shipped build folds this into one expression
            // with the stores to the three matrices elided; it multiplies in the
            // opposite order under its own transposed matrix indexing, which is
            // this order here. The previous code multiplied a single element of
            // matX through a (matZ*matY) term, which is not a matrix product at
            // all, so any particle with angular velocity was displaced instead
            // of rotated.
            CMatrix const result = matZ * matY * matX;

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

        // Generate vertices based on mode and radius. The buffer is a real
        // VERTEX_XYZNT1 array, so points go in at the 32-byte vertex stride -
        // which is what ParticleSystem::Update reads them back at. Treating it
        // as a packed CVector array writes them 12 bytes apart and every
        // emitter point after the first is then read from the wrong place.
        auto* const verts = static_cast<char*>(*this->m_meshEmitterVerts);
        unsigned const stride = this->m_VertexTypeSizes[0];

        auto vertexAt = [verts, stride](int i)
        {
            return reinterpret_cast<float*>(verts + i * stride);
        };

        if (radius == 0.0f)
        {
            // Even spacing along the segment from point1 to point2.
            float* first = vertexAt(0);
            first[0] = point1.x;
            first[1] = point1.y;
            first[2] = point1.z;

            if (numVerts > 2)
            {
                float const step = 1.0f / static_cast<float>(numVerts - 1);
                CVector const delta = (point2 - point1) * step;

                for (int i = 1; i < numVerts - 1; ++i)
                {
                    float* v = vertexAt(i);
                    v[0] = static_cast<float>(i) * delta.x + point1.x;
                    v[1] = static_cast<float>(i) * delta.y + point1.y;
                    v[2] = static_cast<float>(i) * delta.z + point1.z;
                }
            }

            float* last = vertexAt(numVerts - 1);
            last[0] = point2.x;
            last[1] = point2.y;
            last[2] = point2.z;
        }
        else
        {
            // A ring of the given radius in the x/z plane.
            for (int i = 0; i < numVerts; ++i)
            {
                float const angle = static_cast<float>(i) * 6.2831855f / static_cast<float>(numVerts);
                float* v = vertexAt(i);
                v[0] = std::cos(angle) * radius;
                v[1] = 0.0f;
                v[2] = std::sin(angle) * radius;
            }
        }
    }

    void ParticlesList::SetMeshEmitterPoints(
        CMatrix** mat,
        int numMeshes,
        void** verts,
        int* numVerts,
        retruxx::vector<m3d::rend::VertexType>& VertexTypes,
        retruxx::vector<unsigned int>& VertexTypeSizes,
        int numSkinMesh)
    {
        // The buffers stay owned by the mesh server; the list only borrows them
        // until UnregisterNode hands them back.
        m_local = mat;
        m_numMeshEmitterVerts = numVerts;
        m_meshEmitterVerts = verts;
        m_numSkinMesh = numSkinMesh;
        m_numMeshes = numMeshes;
        m_VertexTypes = VertexTypes;
        m_VertexTypeSizes = VertexTypeSizes;
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

    void ParticlesList::SetMeshEmitterInds(int** inds, int* numinds)
    {
        m_meshEmitterInds = inds;
        m_numMeshEmitterInds = numinds;
    }

    void ParticleSystem::SetPsVolume(int axis, float forceMin, float forceMax, ForceType ft, float period)
    {
        Force& force = m_x0[axis];
        force.m_max = forceMax;
        force.m_min = forceMin;
        force.m_freq = 3.1415927f / period;
        force.m_type = ft;
    }

    void ParticleSystem::SetPsVolume(CoordinatesSystemType mode)
    {
        m_x0Cst = mode;
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
        // The base system draws nothing; every concrete system overrides this.
        return 1;
    }

    void ParticleSystem::SetScaleParts(float tt)
    {
        m_scaleparts = tt;
    }

    cmn::XmlNode* ParticleSystem::Write(fs::FileStream&, cmn::XmlFile* xmlFile, cmn::XmlNode*, CStr name)
    {
        ref_ptr<cmn::XmlNode> psroot = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "PS");
        ref_ptr<cmn::XmlNode> emitter = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Emitter");
        ref_ptr<cmn::XmlNode> force = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Force");
        ref_ptr<cmn::XmlNode> mesh = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Mesh");
        ref_ptr<cmn::XmlNode> keys = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Keys");
        ref_ptr<cmn::XmlNode> color = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Color");
        ref_ptr<cmn::XmlNode> size = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Size");
        ref_ptr<cmn::XmlNode> vis = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Render");

        psroot->SetAttribute("Name", name.c_str());
        psroot->SetAttribute("Class", m_ClassName.c_str());

        force->SetAttribute("Min", CStr(CVector(m_x0[0].m_min, m_x0[1].m_min, m_x0[2].m_min)).c_str());
        force->SetAttribute("Max", CStr(CVector(m_x0[0].m_max, m_x0[1].m_max, m_x0[2].m_max)).c_str());
        force->SetAttribute("Type", m_x0Cst ? "sphere" : "box");
        force->SetAttribute("PosMin", CStr(CVector(m_pos[0].m_min, m_pos[1].m_min, m_pos[2].m_min)).c_str());
        force->SetAttribute("PosMax", CStr(CVector(m_pos[0].m_max, m_pos[1].m_max, m_pos[2].m_max)).c_str());

        m_Emitter.WriteToXmlNode(emitter);

        for (int i = 0; i < TIME_DISCRETION; ++i)
        {
            CStr const key = CStr("k:") + CStr(i);
            color->SetAttribute(key.c_str(), CStr(m_colors[i]).c_str());
            size->SetAttribute(key.c_str(), CStr(m_sizes[i]).c_str());
        }

        vis->SetAttribute("TrailLen", CStr(m_trailLen).c_str());
        vis->SetAttribute("ScaleParts", CStr(m_scaleparts).c_str());
        vis->SetAttribute("BlendMode", CStr(static_cast<int>(m_blendMode)).c_str());
        vis->SetAttribute("UpdateXForm", CStr(static_cast<int>(m_updateXForm)).c_str());
        vis->SetAttribute("SpriteAngle", CStr(m_SpriteAngle).c_str());
        vis->SetAttribute("Orient", CStr(static_cast<int>(m_orient)).c_str());
        vis->SetAttribute("Texture", m_texName.c_str());
        vis->SetAttribute("Specific", CStr(static_cast<int>(m_Specific)).c_str());
        vis->SetAttribute("TexTiling", CStr(m_TexTiling).c_str());
        vis->SetAttribute("ParentDependency", CStr(m_parentDependency).c_str());
        vis->SetAttribute("ShaderType", CStr(static_cast<int>(m_shaderType)).c_str());
        vis->SetAttribute("BBoxMin", CStr(CVector(m_bBox.m_box[0], m_bBox.m_box[1], m_bBox.m_box[2])).c_str());
        vis->SetAttribute("BBoxMax", CStr(CVector(m_bBox.m_box[3], m_bBox.m_box[4], m_bBox.m_box[5])).c_str());

        mesh->SetAttribute("Enable", CStr(static_cast<int>(m_autoMeshEmitter)).c_str());
        mesh->SetAttribute("Points", CStr(m_points).c_str());
        mesh->SetAttribute("Radius", CStr(m_meshradius).c_str());
        mesh->SetAttribute("Point1", CStr(m_point1).c_str());
        mesh->SetAttribute("Point2", CStr(m_point2).c_str());
        mesh->SetAttribute("Forv", CStr(static_cast<int>(m_forv)).c_str());
        mesh->SetAttribute("Back", CStr(static_cast<int>(m_back)).c_str());

        keys->AddChild(color.get());
        keys->AddChild(size.get());

        psroot->AddChild(vis.get());
        psroot->AddChild(keys.get());
        psroot->AddChild(emitter.get());
        psroot->AddChild(force.get());
        psroot->AddChild(mesh.get());

        for (auto* attractor : m_Attractors)
        {
            attractor->WriteToXmlNode(xmlFile, psroot);
        }

        return psroot.get();
    }

    ParticleSystem* ParticleSystem::Factory(PSProps const& psprops, retruxx::vector<AttrProps> const& AttrProtos)
    {
        auto* system = New(psprops.m_ClassName);
        system->ReadFromProtos(psprops, AttrProtos);
        return system;
    }

    ParticleSystem* ParticleSystem::Factory(char const* fileName)
    {
        scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!fileStream->Open(fileName, fs::IStream::OPEN_READ))
        {
            M3D_LOG_INFO("Error: PS: Can't open file: " + CStr(fileName));
            return new ParticleSystem();
        }

        ref_ptr<cmn::XmlFile> xmlFile = M3D_KERNEL->CreateXmlFile();
        if (!xmlFile->Read(*fileStream))
        {
            M3D_LOG_INFO("Error: PS: Error while reading file: " + CStr(fileName));
            return new ParticleSystem();
        }

        fileStream->Close();

        ref_ptr<cmn::XmlNode> psed = xmlFile->CreateNode();
        ref_ptr<cmn::XmlNode> ps = xmlFile->CreateNode();

        xmlFile->GetFirstChild(psed.get(), "PSEditor");
        psed->GetFirstChild(ps.get(), "PS");

        CStr const className = ps->GetAttribute("Class");
        auto* system = ParticleSystem::New(className);
        system->m_Name = CStr(ps->GetAttribute("Name"));
        system->m_ClassName = className;
        system->Read(*fileStream, xmlFile.get(), ps.get());

        return system;
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

        // Copy the color and size keyframe arrays (20 entries each)
        memcpy(m_colors, psprops.m_colors, sizeof(m_colors));
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
        CreateShaderForType(m_shaderType);

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

        for (auto const& attrProto : AttrProtos)
        {
            // Create new attractor based on class name
            auto* newAttractor = m3d::Attr::New(attrProto.m_ClassName);
            newAttractor->ReadFromProto(attrProto);
            m_Attractors.push_back(newAttractor);
        }
    }

    ParticleSystem* ParticleSystem::CreateCopy(ParticleSystem& CopyPS)
    {
        auto* copy = ParticleSystem::New(CopyPS.m_ClassName);

        PSProps psprops;
        retruxx::vector<AttrProps> attrProtos;
        CopyPS.WriteToProtos(psprops, attrProtos);
        copy->ReadFromProtos(psprops, attrProtos);

        return copy;
    }

    void ParticleSystem::SetPsColor(float time, unsigned value)
    {
        time = std::clamp(time, 0.0f, 1.0f);
        int slot = static_cast<int>(time * static_cast<float>(TIME_DISCRETION));
        slot = std::clamp(slot, 0, TIME_DISCRETION - 1);
        m_colors[slot] = static_cast<int>(value);
    }

    int ParticleSystem::GetTimeDiscretion() const
    {
        return TIME_DISCRETION;
    }

    void ParticleSystem::SetParticleColor(Particle* pParticle, float fader)
    {
        // Normalized lifetime (0.0 = just born, 1.0 = about to die).
        float normalizedLifetime = (pParticle->m_ttl - pParticle->m_fade) / pParticle->m_ttl;

        int colorIndex = static_cast<int>(normalizedLifetime * static_cast<float>(TIME_DISCRETION));
        colorIndex = std::clamp(colorIndex, 0, TIME_DISCRETION - 1);

        unsigned int const colorValue = static_cast<unsigned int>(m_colors[colorIndex]);
        if (colorValue != UNDEFINED_COLOR)
        {
            // A keyframe is defined at this slot, it is taken verbatim - the fader
            // only ever applies to the interpolated path.
            pParticle->m_curClr = colorValue;
            return;
        }

        // Walk outwards to the nearest defined keyframes on either side.
        int prevIndex = colorIndex;
        while (prevIndex >= 0 && static_cast<unsigned int>(m_colors[prevIndex]) == UNDEFINED_COLOR)
        {
            --prevIndex;
        }

        int nextIndex = colorIndex;
        while (nextIndex < TIME_DISCRETION && static_cast<unsigned int>(m_colors[nextIndex]) == UNDEFINED_COLOR)
        {
            ++nextIndex;
        }

        // The original reads one slot past either end of the table when the whole
        // table is undefined; clamp instead so the lookup stays in bounds.
        prevIndex = std::clamp(prevIndex, 0, TIME_DISCRETION - 1);
        nextIndex = std::clamp(nextIndex, 0, TIME_DISCRETION - 1);

        float const prevTime = static_cast<float>(prevIndex) * INV_DISCRETION;
        float const nextTime = static_cast<float>(nextIndex) * INV_DISCRETION;
        float const tt = (nextTime != prevTime) ? (normalizedLifetime - prevTime) / (nextTime - prevTime) : 0.0f;

        auto const* prevBytes = reinterpret_cast<unsigned char const*>(&m_colors[prevIndex]);
        auto const* nextBytes = reinterpret_cast<unsigned char const*>(&m_colors[nextIndex]);

        m3d::rend::Colori clr;
        for (int i = 0; i < 4; ++i)
        {
            // The fader scales the whole interpolated channel, not just its base.
            clr.clr[i] = static_cast<unsigned char>(
                ((static_cast<double>(nextBytes[i]) - static_cast<double>(prevBytes[i])) * tt +
                 static_cast<double>(prevBytes[i])) *
                fader);
        }

        pParticle->m_curClr = clr.rgba;
    }

    void ParticleSystem::SetPsTrailLen(int traillen)
    {
        // The trail pool only holds MAX_TRAIL_LEN points per particle.
        m_trailLen = (traillen >= MAX_TRAIL_LEN) ? MAX_TRAIL_LEN - 1 : traillen;
    }

    void ParticleSystem::MoveParticles(ParticlesList* parts, retruxx::vector<CVector> const* newPoses)
    {
        // RVA 0x8E6410
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
                float x =
                    (newPos.x * fromWorld._11 + newPos.y * fromWorld._21 + newPos.z * fromWorld._31) + fromWorld._41;
                float y =
                    (newPos.x * fromWorld._12 + newPos.y * fromWorld._22 + newPos.z * fromWorld._32) + fromWorld._42;
                float z =
                    (newPos.x * fromWorld._13 + newPos.y * fromWorld._23 + newPos.z * fromWorld._33) + fromWorld._43;

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
        // Fills every undefined slot by interpolating between its nearest defined
        // neighbours. Slots are filled in place and in order, so an already-filled
        // slot acts as the left neighbour for the next one.
        for (int slot = 0; slot < TIME_DISCRETION; ++slot)
        {
            if (static_cast<unsigned int>(m_colors[slot]) != UNDEFINED_COLOR)
            {
                continue;
            }

            int prevIndex = slot;
            while (prevIndex >= 0 && static_cast<unsigned int>(m_colors[prevIndex]) == UNDEFINED_COLOR)
            {
                --prevIndex;
            }

            int nextIndex = slot;
            while (nextIndex < TIME_DISCRETION && static_cast<unsigned int>(m_colors[nextIndex]) == UNDEFINED_COLOR)
            {
                ++nextIndex;
            }

            // The original reads one slot past either end of the table when no
            // keyframe bounds the gap; clamp instead so the lookup stays in bounds.
            prevIndex = std::clamp(prevIndex, 0, TIME_DISCRETION - 1);
            nextIndex = std::clamp(nextIndex, 0, TIME_DISCRETION - 1);

            double const tt = (nextIndex != prevIndex) ?
                static_cast<double>(slot - prevIndex) / static_cast<double>(nextIndex - prevIndex) :
                0.0;

            auto const* prevBytes = reinterpret_cast<unsigned char const*>(&m_colors[prevIndex]);
            auto const* nextBytes = reinterpret_cast<unsigned char const*>(&m_colors[nextIndex]);

            m3d::rend::Colori clr;
            for (int i = 0; i < 4; ++i)
            {
                clr.clr[i] = static_cast<unsigned char>(
                    (static_cast<double>(nextBytes[i]) - static_cast<double>(prevBytes[i])) * tt +
                    static_cast<double>(prevBytes[i]));
            }

            m_colors[slot] = static_cast<int>(clr.rgba);
        }
    }

    void ParticleSystem::CreateShaderForType(PsShaderType type)
    {
        switch (type)
        {
        case PSST_DUST:
            m_shader = M3D_RENDERER->NewEffect("data/shaders/ps_dust.fx", true);
            break;
        case PSST_LIGHT:
            m_shader = M3D_RENDERER->NewEffect("data/shaders/ps_light.fx", true);
            break;
        case PSST_NOFOG_DUST:
            m_shader = M3D_RENDERER->NewEffect("data/shaders/ps_nofog_dust.fx", true);
            break;
        case PSST_NOFOG_LIGHT:
            m_shader = M3D_RENDERER->NewEffect("data/shaders/ps_nofog_light.fx", true);
            break;
        default:
            M3D_ASSERT(false);
            break;
        }

        M3D_ASSERT(m_shader);
        m_shader->SetDefaultTechnique(true);
    }

    int ParticleSystem::ReadRenderParams(ref_ptr<cmn::XmlNode>& vis)
    {
        CStr str = vis->GetAttribute("Texture");
        SetTextureName(str);

        PsShaderType shaderType = PSST_DUST;
        SafeEnumAttrib(shaderType, vis.get(), "ShaderType");
        m_shaderType = shaderType;
        CreateShaderForType(shaderType);

        // ScaleParts is read unconditionally, so a missing attribute leaves
        // whatever sscanf did not overwrite.
        float scaleparts = m_scaleparts;
        str = vis->GetAttribute("ScaleParts");
        sscanf(str.c_str(), "%f", &scaleparts);
        m_scaleparts = scaleparts;

        float spriteAngle = 0.0f;
        ReadFloatAttrib(spriteAngle, vis, "SpriteAngle", 0.0f);
        m_SpriteAngle = spriteAngle;

        m_TexTiling = 1;
        SafeEnumAttrib(m_TexTiling, vis.get(), "TexTiling");

        // BlendMode and the two flags below are stored as floats in the XML.
        float blendMode = 0.0f;
        if (ReadFloatAttrib(blendMode, vis, "BlendMode", 0.0f))
        {
            m_blendMode = static_cast<PBlendMode>(static_cast<int>(blendMode));
        }
        else
        {
            m_blendMode = PS_ADDSMOOTH;
        }

        float flag = 0.0f;
        m_updateXForm = ReadFloatAttrib(flag, vis, "UpdateXForm", 0.0f) ? (flag != 0.0f) : false;
        m_orient = ReadFloatAttrib(flag, vis, "Orient", 0.0f) ? (flag != 0.0f) : true;

        str = vis->GetAttribute("TrailLen");
        if (str.c_str() && *str.c_str())
        {
            int traillen = 0;
            sscanf(str.c_str(), "%i", &traillen);
            m_trailLen = (traillen <= MAX_TRAIL_LEN - 1) ? traillen : MAX_TRAIL_LEN - 1;
        }
        else
        {
            m_trailLen = 10;
        }

        SafeBoolAttrib(m_Specific, vis.get(), "Specific");

        if (!vis->IsEmpty())
        {
            if (char const* attr = vis->GetAttribute("ParentDependency"))
            {
                m_parentDependency = static_cast<float>(atof(attr));
            }
        }

        CVector bBoxMin(-20.0f, -20.0f, -20.0f);
        SafeVectorAttrib(bBoxMin, vis.get(), "BBoxMin");

        CVector bBoxMax(20.0f, 20.0f, 20.0f);
        SafeVectorAttrib(bBoxMax, vis.get(), "BBoxMax");

        m_bBox.m_box[0] = bBoxMin.x;
        m_bBox.m_box[1] = bBoxMin.y;
        m_bBox.m_box[2] = bBoxMin.z;
        m_bBox.m_box[3] = bBoxMax.x;
        m_bBox.m_box[4] = bBoxMax.y;
        m_bBox.m_box[5] = bBoxMax.z;

        return 1;
    }

    void ParticleSystem::SetPsBlendMode(PBlendMode blendmode)
    {
        m_blendMode = blendmode;
    }

    void ParticleSystem::setParticleSize(Particle* p)
    {
        float const normalizedLifetime = (p->m_ttl - p->m_fade) / p->m_ttl;
        int slot = static_cast<int>(normalizedLifetime * static_cast<float>(TIME_DISCRETION));
        float const tt =
            (normalizedLifetime - static_cast<float>(slot) * INV_DISCRETION) * static_cast<float>(TIME_DISCRETION);

        if (slot > TIME_DISCRETION - 2)
        {
            p->m_size = m_sizes[TIME_DISCRETION - 1];
        }
        else
        {
            if (slot < 0)
            {
                slot = 0;
            }
            p->m_size = (m_sizes[slot + 1] - m_sizes[slot]) * tt + m_sizes[slot];
        }

        p->m_size = m_scaleparts * p->m_size;
    }

    void ParticleSystem::WriteToProtos(PSProps& psprops, retruxx::vector<AttrProps>& AttrProtos)
    {
        CopyToFixedBuffer(psprops.m_Name, m_Name);
        CopyToFixedBuffer(psprops.m_ClassName, m_ClassName);

        psprops.m_Specific = m_Specific;
        psprops.m_HaveTrail = m_HaveTrail;
        psprops.m_trailLen = m_trailLen;
        psprops.m_CreateOne = m_CreateOne;
        psprops.m_autoMeshEmitter = m_autoMeshEmitter;
        psprops.m_meshradius = m_meshradius;
        psprops.m_points = m_points;
        psprops.m_point1 = m_point1;
        psprops.m_point2 = m_point2;
        psprops.m_point2Max = m_point2Max;

        memcpy(psprops.m_colors, m_colors, sizeof(psprops.m_colors));
        memcpy(psprops.m_sizes, m_sizes, sizeof(psprops.m_sizes));

        psprops.m_blendMode = m_blendMode;
        psprops.m_forv = m_forv;
        psprops.m_back = m_back;
        psprops.m_orient = m_orient;
        psprops.m_scaleparts = m_scaleparts;
        psprops.m_updateXForm = m_updateXForm;
        psprops.m_SpriteAngle = m_SpriteAngle;
        psprops.m_TexTiling = m_TexTiling;

        CopyToFixedBuffer(psprops.m_texName, m_texName);
        psprops.m_shaderType = m_shaderType;

        psprops.m_emitAtPeriod = m_Emitter.m_emitAtPeriod;
        psprops.m_wtime = m_Emitter.m_wtime;
        psprops.m_maxParticles = m_Emitter.m_maxParticles;
        psprops.m_ttlMin = m_Emitter.m_ttlMin;
        psprops.m_ttlMax = m_Emitter.m_ttlMax;
        psprops.m_resettime = m_Emitter.m_resettime;
        psprops.m_localStop = m_Emitter.m_localStop;
        psprops.m_stopTime = m_Emitter.m_stopTime;
        psprops.m_start = m_Emitter.m_start;

        for (int i = 0; i < 3; ++i)
        {
            psprops.m_x0[i] = m_x0[i];
            psprops.m_pos[i] = m_pos[i];
        }
        psprops.m_x0Cst = m_x0Cst;

        psprops.m_parentDependency = m_parentDependency;
        psprops.m_bBoxMin = CVector(m_bBox.m_box[0], m_bBox.m_box[1], m_bBox.m_box[2]);
        psprops.m_bBoxMax = CVector(m_bBox.m_box[3], m_bBox.m_box[4], m_bBox.m_box[5]);

        AttrProps blank;
        AttrProtos.resize(m_Attractors.size(), blank);
        for (size_t i = 0; i < m_Attractors.size(); ++i)
        {
            m_Attractors[i]->WriteToProto(AttrProtos[i]);
        }
    }

    void ParticleSystem::SetPsMesh(float radius, int points)
    {
        m_meshradius = radius;
        m_points = points;
    }

    void ParticleSystem::SetInterpolateColorsOnLoad(bool c)
    {
        interpolateColorsOnLoad = c;
    }

    bool ParticleSystem::IsLocal()
    {
        return m_updateXForm;
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

    int ParticleSystem::ReadVolumeParams(ref_ptr<cmn::XmlNode>& force)
    {
        // "Min"/"Max" bound the initial velocity, "PosMin"/"PosMax" the spawn offset.
        //
        // NOTE: the shipped build has a defect here. It assigns m_x0 before parsing
        // PosMin, and crosses the two Min sources over, so every m_x0 minimum came
        // from uninitialized stack and every m_pos minimum came from the velocity
        // "Min" attribute. This reads the attributes the way the XML plainly means
        // them, which only affects assets loaded per-file rather than from effects.bps.
        CVector velMin = ZeroVector;
        CVector velMax = ZeroVector;
        CVector posMin = ZeroVector;
        CVector posMax = ZeroVector;

        ReadVectorAttrib(velMin, force, "Min");
        ReadVectorAttrib(velMax, force, "Max");
        ReadVectorAttrib(posMin, force, "PosMin");
        ReadVectorAttrib(posMax, force, "PosMax");

        for (int i = 0; i < 3; ++i)
        {
            m_x0[i].m_min = (&velMin.x)[i];
            m_x0[i].m_max = (&velMax.x)[i];
            m_x0[i].m_freq = 1.5707964f;
            m_x0[i].m_type = PS_FORCE_RANDOM;

            m_pos[i].m_min = (&posMin.x)[i];
            m_pos[i].m_max = (&posMax.x)[i];
        }

        CStr const type = force->GetAttribute("Type");
        m_x0Cst = (type == "box") ? PS_CST_CARTHESIAN : PS_CST_POLAR;

        return 1;
    }

    void ParticleSystem::SetPsSize(float time, float value)
    {
        value = std::clamp(value, 0.0f, 10.0f);
        time = std::clamp(time, 0.0f, 1.0f);
        int slot = static_cast<int>(time * static_cast<float>(TIME_DISCRETION));
        slot = std::clamp(slot, 0, TIME_DISCRETION - 1);
        m_sizes[slot] = value;
    }

    int ParticleSystem::ReadMeshParams(ref_ptr<cmn::XmlNode>& mesh)
    {
        // Every attribute is optional; a missing or empty one keeps the listed default.
        m_autoMeshEmitter = ReadFlagAttrib(mesh, "Enable", false);
        m_forv = ReadFlagAttrib(mesh, "Forv", true);
        m_back = ReadFlagAttrib(mesh, "Back", false);

        int points = 0;
        CStr str = mesh->GetAttribute("Points");
        sscanf(str.c_str(), "%i", &points);

        float radius = 0.0f;
        str = mesh->GetAttribute("Radius");
        sscanf(str.c_str(), "%f", &radius);

        m_points = points;
        m_meshradius = radius;

        if (!ReadVectorAttrib(m_point1, mesh, "Point1"))
        {
            m_point1 = ZeroVector;
        }
        if (!ReadVectorAttrib(m_point2, mesh, "Point2"))
        {
            m_point2 = ZeroVector;
        }
        if (!ReadVectorAttrib(m_point2Max, mesh, "Point2Max"))
        {
            // Without an explicit maximum the emitter volume degenerates to Point2.
            m_point2Max = m_point2;
        }

        return 1;
    }

    int ParticleSystem::Read(fs::FileStream&, cmn::XmlFile* m_file, cmn::XmlNode* psroot)
    {
        ref_ptr<cmn::XmlNode> force = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> emitter = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> keys = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> vis = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> mesh = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> color = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> size = m_file->CreateNode();
        ref_ptr<cmn::XmlNode> attr = m_file->CreateNode();

        psroot->GetFirstChild(force.get(), "Force");
        psroot->GetFirstChild(emitter.get(), "Emitter");
        psroot->GetFirstChild(keys.get(), "Keys");
        psroot->GetFirstChild(vis.get(), "Render");
        psroot->GetFirstChild(mesh.get(), "Mesh");
        keys->GetFirstChild(color.get(), "Color");
        keys->GetFirstChild(size.get(), "Size");

        ReadVolumeParams(force);
        m_Emitter.ReadFromXmlNode(emitter);
        ReadRenderParams(vis);
        ReadMeshParams(mesh);

        // Keyframes are stored as "k:<index>" attributes on the Color and Size nodes.
        for (int i = 0; i < TIME_DISCRETION; ++i)
        {
            CStr const key = CStr("k:") + CStr(i);

            CStr str = color->GetAttribute(key.c_str());
            sscanf(str.c_str(), "%i", &m_colors[i]);

            str = size->GetAttribute(key.c_str());
            sscanf(str.c_str(), "%f", &m_sizes[i]);
        }

        if (interpolateColorsOnLoad)
        {
            InterpolateColors();
        }

        for (auto*& attractor : m_Attractors)
        {
            delete attractor;
            attractor = nullptr;
        }

        psroot->GetFirstChild(attr.get(), "Attr");
        while (!attr->IsEmpty())
        {
            m_Attractors.push_back(Attr::Factory(m_file, attr));
            attr->GetNextSibling(attr.get(), "Attr");
        }

        return 1;
    }

    float ParticleSystem::GetBoundRadius() const
    {
        return 40.0f;
    }

    ParticleSystem::~ParticleSystem()
    {
        M3D_RENDERER->ReleaseTexture(m_texAdd);

        for (auto*& attractor : m_Attractors)
        {
            delete attractor;
            attractor = nullptr;
        }
        m_Attractors.clear();

        if (m_shader)
        {
            m_shader->Release();
            m_shader = nullptr;
        }
    }

    void ParticleSystem::SetTextureName(CStr const& name)
    {
        M3D_RENDERER->ReleaseTexture(this->m_texAdd);
        if (!name.empty())
        {
            this->m_texAdd = M3D_RENDERER->AddTexture(name, 0);
            M3D_RENDERER->SetTextureParameter(this->m_texAdd, m3d::rend::TexParam::TM_WRAP_S, 3u);
            M3D_RENDERER->SetTextureParameter(this->m_texAdd, m3d::rend::TexParam::TM_WRAP_T, 3u);
            M3D_RENDERER->SetTextureParameter(this->m_texAdd, m3d::rend::TexParam::TM_TEX_FILTER, 2u);
        }
        this->m_texName = name;
    }

    void ParticleSystem::Reset(ParticlesList* parts)
    {
        // RVA 0x8E80C0
        parts->m_vel = ZeroVector;
        parts->m_rotvel = ZeroVector;
        parts->m_mrotvel = ZeroVector;
        parts->m_accel = ZeroVector;
        parts->m_rotaccel = ZeroVector;
        parts->m_origin = ZeroVector;

        // The binary drains the list on m_particles rather than on the counter,
        // decrementing m_numParticles once per node (0x8E80F4..0x8E8149), so a
        // counter that disagrees with the list cannot walk off the end.
        while (parts->m_particles)
        {
            auto* temp = parts->m_particles;
            parts->m_particles = temp->m_next;
            ParticlesPool.Delete(temp);
            --parts->m_numParticles;
        }

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

    void ParticleSystem::AddParticles(ParticlesList* parts, retruxx::vector<CVector> const* newPoses)
    {
        for (auto const& pos : *newPoses)
        {
            AddParticle(parts, &pos);
        }
    }

    void ParticleSystem::AddParticle(ParticlesList* parts, CVector const* pos)
    {
        // RVA 0x8E9B00
        // Get a free particle from the pool
        m3d::Particle* particle = ParticlesPool.New();

        // Add particle to the list
        particle->m_next = parts->m_particles;
        parts->m_particles = particle;
        ++parts->m_numParticles;

        // Generate random TTL (time to live)
        float randomFactor = static_cast<float>(HIWORD(rndGet())) * 0.000015259022f;
        particle->m_fade =
            (this->m_Emitter.m_ttlMax - this->m_Emitter.m_ttlMin) * randomFactor + this->m_Emitter.m_ttlMin;
        particle->m_time0 = parts->m_time;

        // Calculate particle direction and origin
        if (!this->m_updateXForm)
        {
            // Simple world space calculation
            CVector diff = *pos -
                CVector(parts->m_curXFormToWorld._41, parts->m_curXFormToWorld._42, parts->m_curXFormToWorld._43);
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

            float invLength = 1.0f /
                sqrtf(localPos.x * localPos.x + localPos.y * localPos.y + localPos.z * localPos.z + 1.1920929e-7f);

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
        particle->m_origin =
            CVector(parts->m_curXFormToWorld._41, parts->m_curXFormToWorld._42, parts->m_curXFormToWorld._43);

        // Calculate initial size based on lifetime
        setParticleSize(particle);

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
        // RVA 0x8E89C0
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
                parts->m_RandShader = m3d::rnd(0.0f, 1.0f);
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
                this->m_Attractors[j]->AffectParticle(
                    current, parts->m_time, Local, this->m_orient, this->m_scaleparts);
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
                    ((this->m_Emitter.m_ttlMax - this->m_Emitter.m_ttlMin) * (float)(seed >> 16) * 0.000015259022f) +
                    this->m_Emitter.m_ttlMin;
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

                // Calculate initial position: starts out as the volume-force offset
                // position and is replaced by the mesh emitter vertex when one is set.
                float posX = org.x;
                float posY = org.y;
                float posZ = org.z;

                if (parts->m_meshEmitterVerts)
                {
                    float* vertexData = (float*)((char*)parts->m_meshEmitterVerts[parts->m_numSkinMesh] +
                                                 parts->m_VertexTypeSizes[parts->m_numSkinMesh] * this->m_numvert);
                    posX = parts->m_origin.x + vertexData[0];
                    posY = parts->m_origin.y + vertexData[1];
                    posZ = parts->m_origin.z + vertexData[2];
                }

                // Set scaled positions
                newParticle->m_locorigin.x = this->m_scaleparts * posX;
                newParticle->m_locorigin.y = this->m_scaleparts * posY;
                newParticle->m_locorigin.z = this->m_scaleparts * posZ;
                newParticle->m_forigin = newParticle->m_locorigin;

                // Only the velocities are cleared here; m_accel / m_rotaccel are
                // cleared further down, AFTER the attractor InitParticle pass.
                newParticle->m_vel.x = 0.0f;
                newParticle->m_vel.y = 0.0f;
                newParticle->m_vel.z = 0.0f;
                newParticle->m_rotvel.x = 0.0f;
                newParticle->m_rotvel.y = 0.0f;
                newParticle->m_rotvel.z = 0.0f;

                // Apply attractor initialization
                for (unsigned int m = 0; m < this->m_Attractors.size(); m++)
                {
                    this->m_Attractors[m]->InitParticle(
                        newParticle, parts->m_time, Local, this->m_orient, this->m_scaleparts);
                }

                // Apply parent velocity if not updating transform
                if (!this->m_updateXForm)
                {
                    newParticle->m_vel.x += parts->m_worldVel.x * this->m_parentDependency;
                    newParticle->m_vel.y += parts->m_worldVel.y * this->m_parentDependency;
                    newParticle->m_vel.z += parts->m_worldVel.z * this->m_parentDependency;
                }

                // NOTE - original defect, kept verbatim: the accelerations are
                // zeroed HERE (0x8E927C for m_accel, 0x8E92A5 for m_rotaccel),
                // i.e. after the Attr::InitParticle pass above. Any initial
                // acceleration an attractor writes during InitParticle is
                // therefore discarded before the particle ever steps.
                newParticle->m_accel.x = 0.0f;
                newParticle->m_accel.y = 0.0f;
                newParticle->m_accel.z = 0.0f;
                newParticle->m_rotaccel.x = 0.0f;
                newParticle->m_rotaccel.y = 0.0f;
                newParticle->m_rotaccel.z = 0.0f;

                // Update total lifetime
                newParticle->m_ttl = newParticle->m_fade;

                // Initialize trail if enabled
                if (this->m_HaveTrail)
                {
                    newParticle->m_trail = (m3d::ParticleBase*)TrailsPool.New();
                    newParticle->m_trailSize = 0;
                }

                // Set initial world position
                newParticle->m_origin.x = parts->m_curXFormToWorld._41;
                newParticle->m_origin.y = parts->m_curXFormToWorld._42;
                newParticle->m_origin.z = parts->m_curXFormToWorld._43;

                // Update vertex index
                if (this->m_backflag)
                {
                    --this->m_numvert;
                }
                else
                {
                    ++this->m_numvert;
                }
            } while ((!this->m_CreateOne || this->m_Emitter.m_wtime.m_length == 0.1f) &&
                     parts->m_numParticles < parts->m_maxParticles);
        }

        // Update particle colors and sizes
        for (m3d::Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            this->SetParticleColor(particle, fader);

            // Interpolate particle size based on lifetime
            this->setParticleSize(particle);
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
                    org.x = (rmat._11 * currentParticle->m_locorigin.x + rmat._21 * currentParticle->m_locorigin.y +
                             rmat._31 * currentParticle->m_locorigin.z) +
                        local_41;

                    org.y = (rmat._12 * currentParticle->m_locorigin.x + rmat._22 * currentParticle->m_locorigin.y +
                             rmat._32 * currentParticle->m_locorigin.z) +
                        local_42;

                    org.z = (rmat._13 * currentParticle->m_locorigin.x + rmat._23 * currentParticle->m_locorigin.y +
                             rmat._33 * currentParticle->m_locorigin.z) +
                        local_43;

                    // Transform next particle position
                    org1.x = (rmat._11 * nextParticle->m_locorigin.x + rmat._21 * nextParticle->m_locorigin.y +
                              rmat._31 * nextParticle->m_locorigin.z) +
                        local_41;

                    org1.y = (rmat._12 * nextParticle->m_locorigin.x + rmat._22 * nextParticle->m_locorigin.y +
                              rmat._32 * nextParticle->m_locorigin.z) +
                        local_42;

                    org1.z = (rmat._13 * nextParticle->m_locorigin.x + rmat._23 * nextParticle->m_locorigin.y +
                              rmat._33 * nextParticle->m_locorigin.z) +
                        local_43;
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
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    bool StripOnePS::IsLocal()
    {
        // Strips are built in world space, so they never follow the node transform.
        return false;
    }

    rend::IbPoolField StripOnePS::m_IbPoolField;

    void StripOnePS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        short* base = reinterpret_cast<short*>(ptr + 4);
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

    int PhysicModelPS::Update(ParticlesList* parts, float lastFrameSecs, float)
    {
        if (parts->m_start1 > parts->m_PhaseTime)
        {
            parts->m_PhaseTime += lastFrameSecs;
            return 1;
        }

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
                parts->m_RandShader = m3d::rnd(0.0f, 1.0f);
            }
        }

        for (size_t i = 0; i < m_Attractors.size(); ++i)
        {
            m_Attractors[i]->SetState(parts->m_time);
        }

        CMatrix Local(parts->m_curXFormToWorld);
        Local.m[3][0] = 0.0f;
        Local.m[3][1] = 0.0f;
        Local.m[3][2] = 0.0f;

        // Particles here only age out; the spawned game objects run their own
        // physics, so there is no Step, trail or attractor pass over live ones.
        Particle** ppParticle = &parts->m_particles;
        while (*ppParticle)
        {
            Particle* current = *ppParticle;
            current->m_fade -= lastFrameSecs;
            if (current->m_fade > 0.0f)
            {
                ppParticle = &current->m_next;
            }
            else
            {
                *ppParticle = current->m_next;
                ParticlesPool.Delete(current);
                --parts->m_numParticles;
            }
        }

        unsigned const particlesToEmit = m_Emitter.Emit(parts->m_time, lastFrameSecs);
        unsigned const dwParticlesEmit = parts->m_numParticles + particlesToEmit;

        if (parts->m_numParticles < parts->m_maxParticles)
        {
            do
            {
                if (parts->m_numParticles >= dwParticlesEmit)
                {
                    break;
                }

                Particle* particle = ParticlesPool.New();
                particle->m_next = parts->m_particles;
                parts->m_particles = particle;
                ++parts->m_numParticles;

                particle->m_fade =
                    (m_Emitter.m_ttlMax - m_Emitter.m_ttlMin) * (static_cast<float>(rndGet() >> 16) * 0.000015259022f) +
                    m_Emitter.m_ttlMin;
                particle->m_time0 = parts->m_time;

                // Each particle is backed by a real splinter object, so the editor
                // does not spawn them.
                if (ai::theObjects->m_SaveType != ai::ObjContainer::SAVE_EDITOR)
                {
                    int const protoId = ai::thePrototypeManager->GetPrototypeId(CStr("particleSplinter"));
                    int const objId = ai::theObjects->CreateNewObject(protoId, "", -1, -1);
                    auto* splinter = static_cast<ai::DummyObject*>(ai::theObjects->GetEntityByObjId(objId));

                    float const scale =
                        ((m_sizes[1] - m_sizes[0]) * (static_cast<float>(rndGet() >> 16) * 0.000015259022f) +
                         m_sizes[0]) *
                        m_scaleparts;

                    splinter->SetModelName(m_modelName.c_str());
                    splinter->SetScale(scale, true);
                    splinter->SetDeadTimer(static_cast<int>(1000 * static_cast<int>(particle->m_fade)), true);

                    // Mass follows the cube of how much larger this splinter is
                    // than the smallest one the system can make.
                    float const minSize = (m_sizes[0] > m_sizes[1]) ? m_sizes[1] : m_sizes[0];
                    float const sizeRatio = scale / (m_scaleparts * minSize);
                    splinter->SetMass(m_sizes[2] * sizeRatio * sizeRatio * sizeRatio);

                    CVector dest;
                    if (m_x0Cst == PS_CST_CARTHESIAN)
                    {
                        m3d::CalcForcesCarthesian(dest, m_x0, parts->m_time);
                    }
                    else
                    {
                        m3d::CalcForcesPolar(dest, m_x0, parts->m_time);
                    }

                    CVector const localPos = (parts->m_origin + dest) * m_scaleparts;
                    CMatrix const& xf = parts->m_curXFormToWorld;
                    CVector const worldPos(
                        xf._11 * localPos.x + xf._21 * localPos.y + xf._31 * localPos.z + xf._41,
                        xf._12 * localPos.x + xf._22 * localPos.y + xf._32 * localPos.z + xf._42,
                        xf._13 * localPos.x + xf._23 * localPos.y + xf._33 * localPos.z + xf._43);
                    splinter->SetPosition(worldPos);

                    Quaternion rot;
                    rot.FromMatrix(xf.getInverse());
                    splinter->SetRotation(rot);

                    particle->m_vel = ZeroVector;
                    particle->m_rotvel = ZeroVector;

                    for (size_t i = 0; i < m_Attractors.size(); ++i)
                    {
                        m_Attractors[i]->InitParticle(particle, parts->m_time, Local, m_orient, m_scaleparts);
                    }

                    splinter->SetLinearVelocity(particle->m_vel);
                    splinter->SetAngularVelocity(particle->m_rotvel);
                }

                particle->m_ttl = particle->m_fade;
            } while (parts->m_numParticles < parts->m_maxParticles);
        }

        parts->m_time += lastFrameSecs;
        return 1;
    }

    cmn::XmlNode* PhysicModelPS::Write(fs::FileStream& out, cmn::XmlFile* xmlFile, cmn::XmlNode* root, CStr name)
    {
        ref_ptr<cmn::XmlNode> psroot = ParticleSystem::Write(out, xmlFile, root, name);

        ref_ptr<cmn::XmlNode> model = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Model");
        model->SetAttribute("Name", m_modelName.c_str());
        psroot->AddChild(model.get());

        return psroot.get();
    }

    void PhysicModelPS::WriteToProtos(PSProps& psprops, retruxx::vector<AttrProps>& AttrProtos)
    {
        ParticleSystem::WriteToProtos(psprops, AttrProtos);
        CopyToFixedBuffer(psprops.m_PartsModelName, m_modelName);
    }

    int PhysicModelPS::Read(fs::FileStream& in, cmn::XmlFile* m_file, cmn::XmlNode* psroot)
    {
        ref_ptr<cmn::XmlNode> model = m_file->CreateNode();
        psroot->GetFirstChild(model.get(), "Model");
        m_modelName = CStr(model->GetAttribute("Name"));

        return ParticleSystem::Read(in, m_file, psroot);
    }

    int PhysicModelPS::Render(CMatrix const*, ParticlesList*)
    {
        // Physic-model particles are drawn by the physics/model server, not here.
        return 1;
    }

    void PhysicModelPS::ReadFromProtos(PSProps const& Prototype, retruxx::vector<AttrProps> const& AttrProtos)
    {
        ParticleSystem::ReadFromProtos(Prototype, AttrProtos);
        m_modelName = Prototype.m_PartsModelName;
    }

    void PolyPS::ReleaseIb()
    {
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    rend::IbPoolField PolyPS::m_IbPoolField;

    void PolyPS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        short* base = reinterpret_cast<short*>(ptr + 4);
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

    int PolyPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = true;
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0)
        {
            return 1;
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPush(*local);
        }

        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_NONE, 0);

        rend::VbHandle vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZCT1);

        // Two vertices per particle plus four slack, which the texture-coordinate
        // pass below writes into.
        int const vertexCount = 2 * parts->m_numParticles + 4;
        int vbOffset = 0;
        char* vtxData = static_cast<char*>(M3D_RENDERER->LockVbStreaming(vb, vertexCount, vbOffset, 0));

        // The ribbon runs one edge along the particle's current position and the
        // other along the position it was emitted from.
        float* posPtr = reinterpret_cast<float*>(vtxData);
        float* uvPtr = reinterpret_cast<float*>(vtxData + 16);
        unsigned int uvVertex = 0;
        unsigned int index = 0;

        for (Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            CVector org;
            if (m_updateXForm)
            {
                org = particle->m_locorigin;
            }
            else
            {
                org = particle->m_origin + particle->m_locorigin;
            }

            posPtr[0] = org.x;
            posPtr[1] = org.y;
            posPtr[2] = org.z;
            posPtr[3] = *(float*)&particle->m_curClr;

            posPtr[6] = particle->m_forigin.x;
            posPtr[7] = particle->m_forigin.y;
            posPtr[8] = particle->m_forigin.z;
            posPtr[9] = *(float*)&particle->m_curClr;

            float u0;
            float u1;
            if (m_TexTiling)
            {
                u0 = 0.0f;
                u1 = 1.0f;
            }
            else
            {
                float const step = 1.0f / static_cast<float>(parts->m_numParticles);
                u0 = static_cast<float>(index) * step;
                u1 = static_cast<float>(index + 1) * step;
            }

            // NOTE: faithful to the original - the coordinate writer runs four
            // vertices at a time while u0/u1 only step once per particle, so it
            // covers all 2*numParticles vertices in half as many iterations and
            // each U value spans two ribbon segments.
            if (uvVertex < 2 * parts->m_numParticles)
            {
                uvPtr[0] = u0;    // vertex 0 tu
                uvPtr[1] = 1.0f;  // vertex 0 tv
                uvPtr[6] = u0;
                uvPtr[7] = 0.0f;
                uvPtr[12] = u1;
                uvPtr[13] = 1.0f;
                uvPtr[18] = u1;
                uvPtr[19] = 0.0f;
            }

            ++index;
            uvVertex += 4;
            posPtr += 12;
            uvPtr += 24;
        }

        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->SetIndices(m_IbPoolField, vbOffset);

        M3D_RENDERER->DrawIndexedPrimitiveEffect(
            rend::M3DPT_TRIANGLESTRIP,
            m_shader,
            0,
            2 * parts->m_numParticles,
            m_IbPoolField.RealOffset,
            2 * parts->m_numParticles - 2);

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPop(true);
        }

        return 1;
    }

    SkinPS::~SkinPS()
    {
        delete[] m_buffIndices;
        m_buffIndices = nullptr;
    }

    void SkinPS::Reset(ParticlesList* parts)
    {
        ParticleSystem::Reset(parts);

        if (!parts->m_skinIb)
        {
            parts->m_skinIb = new rend::IbHandle[parts->m_numMeshes];
        }
        parts->m_numIb = parts->m_numMeshes;

        delete[] m_buffIndices;
        m_buffIndices = new unsigned short*[parts->m_numMeshes];

        // Only fill the buffers the first time round; on a later Reset the index
        // buffers are already built and still valid.
        if (parts->m_skinIb[0].IsValid())
        {
            return;
        }

        for (int i = 0; i < parts->m_numMeshes; ++i)
        {
            if (parts->m_skinIb[i].IsValid())
            {
                M3D_RENDERER->ReleaseIb(parts->m_skinIb[i]);
            }

            // m_numMeshEmitterInds counts triangles: three 16-bit indices each.
            int const numTris = parts->m_numMeshEmitterInds[i];
            parts->m_skinIb[i] = M3D_RENDERER->AddIb(3 * numTris, false);

            m_buffIndices[i] = static_cast<unsigned short*>(M3D_RENDERER->LockIb(parts->m_skinIb[i], 0, 0, 0));
            memcpy(m_buffIndices[i], parts->m_meshEmitterInds[i], 6 * numTris);
            M3D_RENDERER->UnlockIb(parts->m_skinIb[i]);
        }
    }

    int SkinPS::Update(ParticlesList* parts, float lastFrameSecs, float)
    {
        // Skin particles follow the emitting mesh, so there is nothing to simulate;
        // only the list clock advances.
        parts->m_time += lastFrameSecs;
        return 1;
    }

    int SkinPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = true;
        if (parts->m_start1 > parts->m_PhaseTime)
        {
            return 1;
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPush(*local);
        }

        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_NONE, 0);

        if (m_Emitter.m_resettime != 0.0f)
        {
            if (parts->m_time > m_Emitter.m_resettime || parts->m_time == 0.0f)
            {
                Reset(parts);
            }
        }

        // NOTE: the shipped build interpolates a keyframe colour here from
        // m_time / m_Emitter.m_resettime, then reuses that stack slot as the
        // vertex-buffer offset before the colour is ever read. The vertices below
        // are written with a zero colour either way, so it is left out.

        // Re-emit the emitting mesh's own vertices, one draw call per mesh.
        for (int mesh = 0; mesh < parts->m_numMeshes; ++mesh)
        {
            rend::VbHandle vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZCT1);

            int const numVerts = parts->m_numMeshEmitterVerts[mesh];
            char const* srcVerts = static_cast<char const*>(parts->m_meshEmitterVerts[mesh]);
            unsigned const srcStride = parts->m_VertexTypeSizes[mesh];
            rend::VertexType const srcType = parts->m_VertexTypes[mesh];

            int vbOffset = 0;
            float* dst = static_cast<float*>(M3D_RENDERER->LockVbStreaming(vb, numVerts, vbOffset, 0));

            for (int i = 0; i < numVerts; ++i)
            {
                float const* src = reinterpret_cast<float const*>(srcVerts + i * srcStride);

                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = 0.0f;

                // The texture coordinates sit at a different offset per source
                // vertex format; the two-set formats take the second set.
                switch (srcType)
                {
                case rend::VERTEX_XYZNT1:
                case rend::VERTEX_XYZNT2:
                    dst[4] = src[6];
                    dst[5] = src[7];
                    break;
                case rend::VERTEX_XYZNCT1:
                    dst[4] = src[7];
                    dst[5] = src[8];
                    break;
                case rend::VERTEX_XYZNCT2:
                    dst[4] = src[9];
                    dst[5] = src[10];
                    break;
                default:
                    dst[4] = 0.0f;
                    dst[5] = 0.0f;
                    break;
                }

                dst += 6;
            }

            CMatrix* meshLocal = parts->m_local ? parts->m_local[mesh] : nullptr;
            if (meshLocal)
            {
                M3D_RENDERER->MatPush(*meshLocal);
            }

            M3D_RENDERER->UnlockVb(vb);
            M3D_RENDERER->SetToStream0(vb);
            M3D_RENDERER->SetIndices(parts->m_skinIb[mesh], 0);

            M3D_RENDERER->DrawIndexedPrimitiveEffect(
                rend::M3DPT_TRIANGLELIST, m_shader, 0, numVerts, 0, parts->m_numMeshEmitterInds[mesh]);

            if (meshLocal)
            {
                M3D_RENDERER->MatPop(true);
            }
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPop(true);
        }

        return 1;
    }

    SkinPS::SkinPS()
    {
        m_buffIndices = nullptr;
    }

    rend::IbPoolField Poly1PS::m_IbPoolField;

    void Poly1PS::ReleaseIb()
    {
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    void Poly1PS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        short* base = reinterpret_cast<short*>(ptr + 4);
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

    int Poly1PS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = true;
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0)
        {
            return 1;
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPush(*local);
        }

        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_NONE, 0);

        rend::VbHandle vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZCT1);

        int const vertexCount = 2 * parts->m_numParticles + 4;
        int vbOffset = 0;
        char* vtxData = static_cast<char*>(M3D_RENDERER->LockVbStreaming(vb, vertexCount, vbOffset, 0));

        float* posPtr = reinterpret_cast<float*>(vtxData);
        float* uvPtr = reinterpret_cast<float*>(vtxData + 16);
        unsigned int uvVertex = 0;
        unsigned int index = 0;

        for (Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            // The ribbon hangs from the particle down to the emitter's height, so
            // the second vertex keeps the same X/Z but drops the local Y offset.
            CVector top;
            CVector bottom;
            if (m_updateXForm)
            {
                top = particle->m_locorigin;
                bottom = CVector(particle->m_locorigin.x, 0.0f, particle->m_locorigin.z);
            }
            else
            {
                top = particle->m_origin + particle->m_locorigin;
                bottom = CVector(
                    particle->m_origin.x + particle->m_locorigin.x,
                    particle->m_origin.y,
                    particle->m_origin.z + particle->m_locorigin.z);
            }

            posPtr[0] = top.x;
            posPtr[1] = top.y;
            posPtr[2] = top.z;
            posPtr[3] = *(float*)&particle->m_curClr;

            posPtr[6] = bottom.x;
            posPtr[7] = bottom.y;
            posPtr[8] = bottom.z;
            posPtr[9] = *(float*)&particle->m_curClr;

            float u0;
            float u1;
            if (m_TexTiling)
            {
                u0 = 0.0f;
                u1 = 1.0f;
            }
            else
            {
                float const step = 1.0f / static_cast<float>(parts->m_numParticles);
                u0 = static_cast<float>(index) * step;
                u1 = static_cast<float>(index + 1) * step;
            }

            // See PolyPS::Render - the coordinate writer advances four vertices per
            // particle, so it fills all of them in half the iterations.
            if (uvVertex < 2 * parts->m_numParticles)
            {
                uvPtr[0] = u0;
                uvPtr[1] = 1.0f;
                uvPtr[6] = u0;
                uvPtr[7] = 0.0f;
                uvPtr[12] = u1;
                uvPtr[13] = 1.0f;
                uvPtr[18] = u1;
                uvPtr[19] = 0.0f;
            }

            ++index;
            uvVertex += 4;
            posPtr += 12;
            uvPtr += 24;
        }

        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->SetIndices(m_IbPoolField, vbOffset);

        M3D_RENDERER->DrawIndexedPrimitiveEffect(
            rend::M3DPT_TRIANGLESTRIP,
            m_shader,
            0,
            2 * parts->m_numParticles,
            m_IbPoolField.RealOffset,
            2 * parts->m_numParticles - 2);

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPop(true);
        }

        return 1;
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
                    transformedPos.x = (renderMatrix._11 * particle->m_locorigin.x) +
                        (renderMatrix._21 * particle->m_locorigin.y) + (renderMatrix._31 * particle->m_locorigin.z);
                    transformedPos.y = (renderMatrix._12 * particle->m_locorigin.x) +
                        (renderMatrix._22 * particle->m_locorigin.y) + (renderMatrix._32 * particle->m_locorigin.z);
                    transformedPos.z = (renderMatrix._13 * particle->m_locorigin.x) +
                        (renderMatrix._23 * particle->m_locorigin.y) + (renderMatrix._33 * particle->m_locorigin.z);

                    // Transform forward origin
                    CVector transformedForward;
                    transformedForward.x = (renderMatrix._11 * particle->m_forigin.x) +
                        (renderMatrix._21 * particle->m_forigin.y) + (renderMatrix._31 * particle->m_forigin.z) +
                        localOrigin.x;
                    transformedForward.y = (renderMatrix._12 * particle->m_forigin.x) +
                        (renderMatrix._22 * particle->m_forigin.y) + (renderMatrix._32 * particle->m_forigin.z) +
                        localOrigin.y;
                    transformedForward.z = (renderMatrix._13 * particle->m_forigin.x) +
                        (renderMatrix._23 * particle->m_forigin.y) + (renderMatrix._33 * particle->m_forigin.z) +
                        localOrigin.z;

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
                        particle->m_size,    // startSize
                        particle->m_size,    // endSize
                        particle->m_curClr,  // startColor
                        particle->m_curClr,  // endColor
                        camOrg,              // camera position
                        0,                   // texture coordinate U
                        0,                   // texture coordinate V
                        1.0f,                // texture scale U
                        1.0f,                // texture scale V
                        0                    // flags
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
                        worldPos,            // startPos1
                        worldPos,            // startPos2
                        worldPos,            // endPos1
                        worldForward,        // endPos2
                        particle->m_size,    // startSize
                        particle->m_size,    // endSize
                        particle->m_curClr,  // startColor
                        particle->m_curClr,  // endColor
                        camOrg,              // camera position
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
                            transformedCurrent.x = (renderMatrix._11 * particle->m_locorigin.x) +
                                (renderMatrix._21 * particle->m_locorigin.y) +
                                (renderMatrix._31 * particle->m_locorigin.z) + localOrigin.x;
                            transformedCurrent.y = (renderMatrix._12 * particle->m_locorigin.x) +
                                (renderMatrix._22 * particle->m_locorigin.y) +
                                (renderMatrix._32 * particle->m_locorigin.z) + localOrigin.y;
                            transformedCurrent.z = (renderMatrix._13 * particle->m_locorigin.x) +
                                (renderMatrix._23 * particle->m_locorigin.y) +
                                (renderMatrix._33 * particle->m_locorigin.z) + localOrigin.z;

                            // Transform trail position
                            CVector transformedTrail;
                            transformedTrail.x = (renderMatrix._11 * trail->m_locorigin.x) +
                                (renderMatrix._21 * trail->m_locorigin.y) + (renderMatrix._31 * trail->m_locorigin.z) +
                                localOrigin.x;
                            transformedTrail.y = (renderMatrix._12 * trail->m_locorigin.x) +
                                (renderMatrix._22 * trail->m_locorigin.y) + (renderMatrix._32 * trail->m_locorigin.z) +
                                localOrigin.y;
                            transformedTrail.z = (renderMatrix._13 * trail->m_locorigin.x) +
                                (renderMatrix._23 * trail->m_locorigin.y) + (renderMatrix._33 * trail->m_locorigin.z) +
                                localOrigin.z;

                            addStripePart(
                                transformedCurrent,                  // startPos1
                                transformedTrail,                    // startPos2
                                transformedCurrent,                  // endPos1
                                transformedTrail,                    // endPos2
                                particle->m_size,                    // startSize
                                trail->m_size,                       // endSize
                                particle->m_curClr,                  // startColor
                                trail->m_curClr,                     // endColor
                                camOrg,                              // camera position
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
                                worldCurrent,                        // startPos1
                                worldTrail,                          // startPos2
                                worldCurrent,                        // endPos1
                                worldTrail,                          // endPos2
                                particle->m_size,                    // startSize
                                trail->m_size,                       // endSize
                                particle->m_curClr,                  // startColor
                                trail->m_curClr,                     // endColor
                                camOrg,                              // camera position
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
        short* base = reinterpret_cast<short*>(ptr + 4);
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
        // Strips are built in world space, so they never follow the node transform.
        return false;
    }

    void StripAllPS::ReleaseIb()
    {
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    float LandSpritePS::GetBoundRadius() const
    {
        return 10.0f;
    }

    bool LandSpritePS::IsLocal()
    {
        return false;
    }

    namespace
    {
        // RVA 0x8E05A0 - the road-sprite variant of the shadow transform helper in
        // scenegraph.cpp: it takes independent x/z scales and adds a rotation about
        // y so a sprite can be spun on the ground.
        CMatrix CalcLinearTransformRot(float sx, float sz, float tx, float tz, float roty)
        {
            CMatrix shift;
            shift.zero();
            shift._11 = 1.0f;
            shift._22 = 1.0f;
            shift._33 = 1.0f;
            shift._44 = 1.0f;
            shift._41 = -tx;
            shift._43 = -tz;

            // The y row is deliberately zero, so a point's height never reaches the
            // texture coordinates.
            CMatrix scale;
            scale.zero();
            scale._11 = sx;
            scale._33 = sz;
            scale._44 = 1.0f;

            float const s = std::sin(roty);
            float const c = std::cos(roty);

            CMatrix rt;
            rt.zero();
            rt._11 = c;
            rt._13 = -s;
            rt._22 = 1.0f;
            rt._31 = s;
            rt._33 = c;
            rt._44 = 1.0f;

            CMatrix shiftHalf1;
            shiftHalf1.zero();
            shiftHalf1._11 = 1.0f;
            shiftHalf1._22 = 1.0f;
            shiftHalf1._33 = 1.0f;
            shiftHalf1._44 = 1.0f;
            shiftHalf1._41 = 0.5f;
            shiftHalf1._43 = 0.5f;

            // Swaps the y and z columns.
            CMatrix change;
            change.zero();
            change._11 = 1.0f;
            change._23 = 1.0f;
            change._32 = 1.0f;
            change._44 = 1.0f;

            return shift * scale * rt * shiftHalf1 * change;
        }
    }  // namespace

    int LandSpritePS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = true;
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0 || !pClient)
        {
            return 1;
        }

        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);
        overlayStart();

        auto& world = pClient->GetWorld();
        auto& graph = world.GetGraph();
        auto* shader = graph.GetRoadSpriteShader();

        rend::Colorf const ambient(world.GetWeatherAmbientColor());
        shader->SetVector3(rend::IEffect::LightAmbient, CVector(ambient.r, ambient.g, ambient.b));

        rend::Colorf const diffuse(world.GetWeatherDiffuseColor());
        shader->SetVector3(rend::IEffect::LightDiffuse, CVector(diffuse.r, diffuse.g, diffuse.b));

        float fogStart = 0.0f;
        float fogEnd = 0.0f;
        world.GetLandscape().GetFogStartAndEnd(fogStart, fogEnd);
        float const fogReduce = world.GetWeatherManager().GetFogReduceFactorFromWeather();

        CVector fogTerm;
        fogTerm.x = fogReduce * fogEnd;
        fogTerm.z = fogReduce * fogStart;
        fogTerm.y = 1.0f / (fogTerm.x - fogTerm.z);
        shader->SetVector3(rend::IEffect::FogTerm, fogTerm);

        float const VISCELL_EDGE_LENGTH = 128.0f;
        float const cellSizeInv = 1.0f / VISCELL_EDGE_LENGTH;
        int const landSize = world.m_level->land_size;

        for (Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            CVector const pos(
                local->_41 + particle->m_locorigin.x,
                local->_42 + particle->m_locorigin.y,
                local->_43 + particle->m_locorigin.z);

            float const size = particle->m_size * 0.8f;
            float const halfSize = size * 0.5f;

            CMatrix const tr = CalcLinearTransformRot(1.0f / size, 1.0f / size, pos.x, pos.z, parts->m_spriteAngle);
            shader->SetMatrix(rend::IEffect::User_float4x4_param, tr);

            rend::Colorf const partColor(particle->m_curClr);

            // NOTE: the shipped build passes &partColor.b here, so the shader
            // receives (b, a) followed by two floats of adjacent stack rather than
            // the particle colour. This passes the colour the call plainly intends.
            nFloat4 const color = {partColor.r, partColor.g, partColor.b, partColor.a};
            shader->SetFloat4(rend::IEffect::User_float4_param, color);

            int xLow = static_cast<int>((pos.x - halfSize) * cellSizeInv);
            int xHigh = static_cast<int>((pos.x + halfSize) * cellSizeInv);
            int zLow = static_cast<int>((pos.z - halfSize) * cellSizeInv);
            int zHigh = static_cast<int>((pos.z + halfSize) * cellSizeInv);

            xLow = std::clamp(xLow, 0, landSize - 1);
            xHigh = std::clamp(xHigh, 0, landSize - 1);
            zLow = std::clamp(zLow, 0, landSize - 1);
            zHigh = std::clamp(zHigh, 0, landSize - 1);

            retruxx::vector<unsigned int> roadCells;
            for (int x = xLow; x <= xHigh; ++x)
            {
                for (int z = zLow; z <= zHigh; ++z)
                {
                    if (graph.IsCellEnabled(x, z))
                    {
                        roadCells.push_back(x + (z << 16));
                    }
                }
            }

            M3D_RENDERER->PushCull(rend::M3DCULL_CCW);
            M3D_RENDERER->PushZbState(rend::ZB_ENABLE);

            RoadInRadius2dTest roadTest(pos, halfSize);
            world.GetRoadManager().RenderRoads(roadCells, RRT_FOR_SPRITE, &roadTest, false);

            M3D_RENDERER->PopZbState();
            M3D_RENDERER->PopCull();

            world.GetLandscape().overlayShader = shader;
            world.GetLandscape().drawSpriteOverlayed2(pos.x, pos.z, halfSize, halfSize, 0, false);
            world.GetLandscape().overlayShader = nullptr;
        }

        overlayStop();

        return 1;
    }

    int SpritePS::Render(CMatrix const* local, ParticlesList* parts)
    {
        // TODO generated code SpritePS::Render
        // Early return if particles shouldn't be rendered
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0)
        {
            parts->m_renderCalled = true;
            return true;
        }

        parts->m_renderCalled = true;

        // Set up rendering state
        if (m_updateXForm)
        {
            m3d::Application::g_pApp->m_renderer->MatPush(*local);
        }

        ApplyBlending();
        m3d::Application::g_pApp->m_renderer->SetTexture(0, m_texAdd, -1.0f);
        m3d::Application::g_pApp->m_renderer->SetCull(rend::M3DCULL_NONE, 0);

        // Get vertex buffer for streaming
        int vertexOffset = 0;

        m3d::rend::VbHandle vbHandle = m3d::Application::g_pApp->m_renderer->GetVbStreaming(rend::VERTEX_XYZCT1);

        // Lock vertex buffer for writing
        int vertexCount = 4 * parts->m_numParticles;  // 4 vertices per particle (quad)
        m3d::rend::VertexXYZCT1* vertices = static_cast<m3d::rend::VertexXYZCT1*>(
            m3d::Application::g_pApp->m_renderer->LockVbStreaming(vbHandle, vertexCount, vertexOffset, 0));

        if (!vertices)
        {
            if (m_updateXForm)
            {
                m3d::Application::g_pApp->m_renderer->MatPop(1);
            }
            return false;
        }

        // Calculate sprite rotation matrix
        float sinAngle = sin(parts->m_spriteAngle);
        float cosAngle = cos(parts->m_spriteAngle);

        // Define quad corners (before rotation)
        CVector quadCorners[4];
        quadCorners[0] = CVector(-1.0f, -1.0f, 0.0f);  // bottom-left
        quadCorners[1] = CVector(1.0f, -1.0f, 0.0f);   // bottom-right
        quadCorners[2] = CVector(1.0f, 1.0f, 0.0f);    // top-right
        quadCorners[3] = CVector(-1.0f, 1.0f, 0.0f);   // top-left

        // Apply rotation to quad corners
        for (int i = 0; i < 4; ++i)
        {
            float x = quadCorners[i].x;
            float y = quadCorners[i].y;
            quadCorners[i].x = x * cosAngle - y * sinAngle;
            quadCorners[i].y = x * sinAngle + y * cosAngle;
        }

        // Get inverse matrix if needed for water height calculation
        CMatrix inverseMatrix;
        if (m_updateXForm && m_Specific)
        {
            inverseMatrix = local->getInverseRotTranslate();
        }

        // Process each particle
        m3d::Particle* currentParticle = parts->m_particles;
        m3d::rend::VertexXYZCT1* currentVertex = vertices;

        while (currentParticle)
        {
            // Calculate particle position
            CVector particlePos;
            if (m_updateXForm)
            {
                particlePos = currentParticle->m_locorigin;
            }
            else
            {
                particlePos = currentParticle->m_origin + currentParticle->m_locorigin;
            }

            // Handle water height for specific particles
            if (m_Specific)
            {
                if (m_updateXForm)
                {
                    // Transform to world space for water height lookup
                    CVector worldPos = local->vecMul(particlePos);
                    float waterHeight = m3d::pClient->GetWorld().GetLandscape().getWaterHeight(
                        static_cast<int>(worldPos.x * 0.03125f), static_cast<int>(worldPos.z * 0.03125f));

                    // Apply water height and transform back
                    CVector waterAdjustedPos(worldPos.x, waterHeight, worldPos.z);
                    particlePos = inverseMatrix.vecMul(waterAdjustedPos);
                }
                else
                {
                    // Simple water height adjustment
                    particlePos.y = m3d::pClient->GetWorld().GetLandscape().getWaterHeight(
                        static_cast<int>(particlePos.x * 0.03125f), static_cast<int>(particlePos.z * 0.03125f));
                }
            }

            // Generate quad vertices for this particle
            for (int corner = 0; corner < 4; ++corner)
            {
                // Calculate vertex position (particle center + scaled quad corner)
                CVector vertexPos = particlePos + quadCorners[corner] * currentParticle->m_size;

                currentVertex->x = vertexPos.x;
                currentVertex->y = vertexPos.y;
                currentVertex->z = vertexPos.z;

                // Set vertex color (particle color)
                currentVertex->c = currentParticle->m_curClr;

                // Set texture coordinates for quad corners
                switch (corner)
                {
                case 0:  // bottom-left
                    currentVertex->tu = 0.0f;
                    currentVertex->tv = 0.0f;
                    break;
                case 1:  // top-left
                    currentVertex->tu = 0.0f;
                    currentVertex->tv = 1.0f;
                    break;
                case 2:  // top-right
                    currentVertex->tu = 1.0f;
                    currentVertex->tv = 1.0f;
                    break;
                case 3:  // bottom-right
                    currentVertex->tu = 1.0f;
                    currentVertex->tv = 0.0f;
                    break;
                }

                ++currentVertex;
            }

            currentParticle = currentParticle->m_next;
        }

        // Unlock and render
        m3d::Application::g_pApp->m_renderer->UnlockVb(vbHandle);
        m3d::Application::g_pApp->m_renderer->SetToStream0(vbHandle);
        m3d::Application::g_pApp->m_renderer->SetIndices(m_IbPoolField, vertexOffset);

        // Draw the particles
        m3d::Application::g_pApp->m_renderer->DrawIndexedPrimitiveEffect(
            rend::M3DPT_TRIANGLELIST,
            m_shader,
            0,
            vertexCount,
            m_IbPoolField.RealOffset,
            2 * parts->m_numParticles  // 2 triangles per particle
        );

        // Clean up
        if (m_updateXForm)
        {
            m3d::Application::g_pApp->m_renderer->MatPop(1);
        }

        return true;
    }

    rend::IbPoolField SpritePS::m_IbPoolField;

    void SpritePS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<short*>(ptr + 4);
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
        return 10.0f;
    }

    void SpritePS::ReleaseIb()
    {
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    ModelPS::~ModelPS()
    {
        delete m_PartsModel;
        m_PartsModel = nullptr;

        delete m_Anim;
        m_Anim = nullptr;
    }

    int ModelPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = true;
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0)
        {
            return 1;
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPush(*local);
        }

        ApplyBlending();
        M3D_RENDERER->SetCull(rend::M3DCULL_CCW, 0);

        CVector const up(0.0f, 0.0f, 1.0f);
        CVector const eye = ZeroVector;

        for (Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            CMatrix sc;
            sc.zero();
            sc._11 = particle->m_size;
            sc._22 = particle->m_size;
            sc._33 = particle->m_size;
            sc._44 = 1.0f;

            CVector org;
            if (m_updateXForm)
            {
                org = particle->m_locorigin;
            }
            else
            {
                org = particle->m_origin + particle->m_locorigin;
            }

            // Each instance is oriented along its own velocity and then placed at
            // the particle position.
            CMatrix orient;
            orient.lookAtLH(eye, particle->m_vel, up);
            orient._41 = org.x;
            orient._42 = org.y;
            orient._43 = org.z;

            M3D_RENDERER->MatPush(orient);
            m_PartsModel->Render(sc, m_Anim, 0u, false);
            M3D_RENDERER->MatPop(true);
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPop(true);
        }

        return 1;
    }

    cmn::XmlNode* ModelPS::Write(fs::FileStream& out, cmn::XmlFile* xmlFile, cmn::XmlNode* root, CStr name)
    {
        ref_ptr<cmn::XmlNode> psroot = ParticleSystem::Write(out, xmlFile, root, name);

        ref_ptr<cmn::XmlNode> model = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "Model");
        model->SetAttribute("Name", m_PartsModelName.c_str());
        psroot->AddChild(model.get());

        return psroot.get();
    }

    int ModelPS::Read(fs::FileStream& in, cmn::XmlFile* m_file, cmn::XmlNode* psroot)
    {
        ref_ptr<cmn::XmlNode> model = m_file->CreateNode();
        psroot->GetFirstChild(model.get(), "Model");

        CStr const str = model->GetAttribute("Name");
        if (str.c_str() && *str.c_str())
        {
            m_PartsModelName = str;
            LoadPartsModel();
        }

        return ParticleSystem::Read(in, m_file, psroot);
    }

    void ModelPS::WriteToProtos(PSProps& psprops, retruxx::vector<AttrProps>& AttrProtos)
    {
        ParticleSystem::WriteToProtos(psprops, AttrProtos);
        CopyToFixedBuffer(psprops.m_PartsModelName, m_PartsModelName);
    }

    void ModelPS::ReadFromProtos(PSProps const& Prototype, retruxx::vector<AttrProps> const& AttrProtos)
    {
        ParticleSystem::ReadFromProtos(Prototype, AttrProtos);
        m_PartsModelName = Prototype.m_PartsModelName;

        delete m_PartsModel;
        m_PartsModel = nullptr;

        delete m_Anim;
        m_Anim = nullptr;

        if (m_PartsModelName.c_str() && *m_PartsModelName.c_str())
        {
            LoadPartsModel();
        }
    }

    void ModelPS::LoadPartsModel()
    {
        delete m_PartsModel;
        m_PartsModel = new AnimatedModel;
        m_PartsModel->Load(m_PartsModelName, true);

        delete m_Anim;
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
        // RVA 0x8E3010
        parts->m_renderCalled = true;

        if (parts->m_start1 > parts->m_PhaseTime || !parts->m_numParticles)
            return 1;

        if (m_updateXForm)
            M3D_RENDERER->MatPush(*local);

        CVector r, u, f;
        M3D_RENDERER->MatGetBasis(r, u, f);

        CVector quad[4];
        quad[0] = CVector(-u.x - r.x, -u.y - r.y, -u.z - r.z);
        quad[1] = CVector(-u.x + r.x, -u.y + r.y, -u.z + r.z);
        quad[2] = CVector(r.x + u.x, r.y + u.y, r.z + u.z);
        quad[3] = CVector(u.x - r.x, u.y - r.y, u.z - r.z);

        float DirScale = 1.0f;
        if (m_forv)
        {
            CVector const n = M3D_RENDERER->MatGetOrgInv().getNormalized();
            // The binary really does compute a dot against (0,1,0) in full,
            // keeping the dead (z + x) * 0.0 term.
            DirScale = (n.z + n.x) * 0.0f + n.y;
            if (DirScale <= 0.0f)
            {
                // NOTE - original defect, kept verbatim: this pop is NOT guarded
                // by m_updateXForm (0x8E3239 jumps straight to the MatPop tail,
                // skipping the m_updateXForm test at 0x8E35AE). When a glow quad
                // has m_updateXForm clear and bails out here it pops a matrix it
                // never pushed, unbalancing the renderer's matrix stack for
                // everything drawn after it in the frame.
                M3D_RENDERER->MatPop(true);
                return 1;
            }
        }

        CVector const org(local->_41, local->_42, local->_43);
        if (parts->m_TLM.TraceLine(org, M3D_APP->m_curCamera.m_worldOrigin))
        {
            // Same unguarded pop as above (0x8E327E).
            M3D_RENDERER->MatPop(true);
            return 1;
        }

        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);
        // vtable +0x30 is the PushZbState(ZbState) overload and the binary
        // passes 0, i.e. ZB_DISABLE - additive glow ignores the depth test.
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);

        m3d::rend::VbHandle vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZCT1);

        int const vertexCount = 4 * parts->m_numParticles;
        int vofs = 0;
        char* const vertexData = (char*)M3D_RENDERER->LockVbStreaming(vb, vertexCount, vofs, 0);

        Particle* particle = parts->m_particles;
        if (particle)
        {
            float* vertexPtr = (float*)vertexData;
            do
            {
                float orgX, orgY, orgZ;
                if (m_updateXForm)
                {
                    orgX = particle->m_locorigin.x;
                    orgY = particle->m_locorigin.y;
                    orgZ = particle->m_locorigin.z;
                }
                else
                {
                    orgX = particle->m_origin.x + particle->m_locorigin.x;
                    orgY = particle->m_origin.y + particle->m_locorigin.y;
                    orgZ = particle->m_origin.z + particle->m_locorigin.z;
                }

                // The only difference from QuadPS: the corner offsets are
                // scaled by m_size * DirScale rather than m_size alone.
                float const size = particle->m_size * DirScale;
                unsigned const clr = particle->m_curClr;

                vertexPtr[0] = orgX + quad[0].x * size;
                vertexPtr[1] = orgY + quad[0].y * size;
                vertexPtr[2] = orgZ + quad[0].z * size;
                vertexPtr[3] = *(float*)&clr;
                vertexPtr[4] = 0.0f;
                vertexPtr[5] = 0.0f;

                vertexPtr[6] = orgX + quad[1].x * size;
                vertexPtr[7] = orgY + quad[1].y * size;
                vertexPtr[8] = orgZ + quad[1].z * size;
                vertexPtr[9] = *(float*)&clr;
                vertexPtr[10] = 0.0f;
                vertexPtr[11] = 1.0f;

                vertexPtr[12] = orgX + quad[2].x * size;
                vertexPtr[13] = orgY + quad[2].y * size;
                vertexPtr[14] = orgZ + quad[2].z * size;
                vertexPtr[15] = *(float*)&clr;
                vertexPtr[16] = 1.0f;
                vertexPtr[17] = 1.0f;

                vertexPtr[18] = orgX + quad[3].x * size;
                vertexPtr[19] = orgY + quad[3].y * size;
                vertexPtr[20] = orgZ + quad[3].z * size;
                vertexPtr[21] = *(float*)&clr;
                vertexPtr[22] = 1.0f;
                vertexPtr[23] = 0.0f;

                particle = particle->m_next;
                vertexPtr += 24;
            } while (particle);
        }

        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->SetIndices(m_IbPoolField, vofs);
        M3D_RENDERER->DrawIndexedPrimitiveEffect(
            rend::M3DPT_TRIANGLELIST, m_shader, 0, vertexCount, m_IbPoolField.RealOffset, 2 * parts->m_numParticles);
        M3D_RENDERER->PopZbState();

        if (m_updateXForm)
            M3D_RENDERER->MatPop(true);

        return 1;
    }

    rend::IbPoolField GlowQuadPS::m_IbPoolField;

    void GlowQuadPS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        auto base = reinterpret_cast<short*>(ptr + 4);
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
        return 1.0f;
    }

    void GlowQuadPS::ReleaseIb()
    {
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
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
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    int RainPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        parts->m_renderCalled = true;
        if (parts->m_start1 > parts->m_PhaseTime || parts->m_numParticles == 0)
        {
            return 1;
        }

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPush(*local);
        }

        ApplyBlending();
        M3D_RENDERER->SetWhiteTexture(0);
        M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);

        rend::VbHandle vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZCT1);

        int const vertexCount = 2 * parts->m_numParticles;
        int vbOffset = 0;
        char* vtxData = static_cast<char*>(M3D_RENDERER->LockVbStreaming(vb, vertexCount, vbOffset, 0));

        float* vtxPtr = reinterpret_cast<float*>(vtxData);
        for (Particle* particle = parts->m_particles; particle; particle = particle->m_next)
        {
            CVector org;
            if (m_updateXForm)
            {
                org = particle->m_locorigin;
            }
            else
            {
                org = particle->m_origin + particle->m_locorigin;
            }

            // Each drop is one vertical line segment, coloured at the bottom and
            // fading to transparent black at the top.
            vtxPtr[0] = org.x;
            vtxPtr[1] = org.y;
            vtxPtr[2] = org.z;
            vtxPtr[3] = *(float*)&particle->m_curClr;
            vtxPtr[4] = 0.0f;
            vtxPtr[5] = 0.0f;

            vtxPtr[6] = org.x;
            vtxPtr[7] = org.y + particle->m_size;
            vtxPtr[8] = org.z;
            vtxPtr[9] = 0.0f;
            vtxPtr[10] = 0.0f;
            vtxPtr[11] = 1.0f;

            vtxPtr += 12;
        }

        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->SetIndices(m_IbPoolField, vbOffset);

        M3D_RENDERER->DrawIndexedPrimitiveEffect(
            rend::M3DPT_LINELIST,
            m_shader,
            0,
            2 * parts->m_numParticles,
            m_IbPoolField.RealOffset,
            parts->m_numParticles);

        if (m_updateXForm)
        {
            M3D_RENDERER->MatPop(true);
        }

        return 1;
    }

    rend::IbPoolField QuadPS::m_IbPoolField;

    int QuadPS::Render(CMatrix const* local, ParticlesList* parts)
    {
        // RVA 0x8E2B30. GlowQuadPS::Render is the same routine plus the m_forv
        // back-face test, the TraceLine occlusion test, the ZB_DISABLE push and
        // the DirScale factor on the corner offsets; this one has none of those.
        parts->m_renderCalled = true;

        if (parts->m_start1 > parts->m_PhaseTime || !parts->m_numParticles)
            return 1;

        if (m_updateXForm)
            M3D_RENDERER->MatPush(*local);

        CVector r, u, f;
        M3D_RENDERER->MatGetBasis(r, u, f);

        CVector quad[4];
        quad[0] = CVector(-u.x - r.x, -u.y - r.y, -u.z - r.z);
        quad[1] = CVector(-u.x + r.x, -u.y + r.y, -u.z + r.z);
        quad[2] = CVector(r.x + u.x, r.y + u.y, r.z + u.z);
        quad[3] = CVector(u.x - r.x, u.y - r.y, u.z - r.z);

        ApplyBlending();
        M3D_RENDERER->SetTexture(0, m_texAdd, -1.0f);
        M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);
        // No PushZbState here - unlike GlowQuadPS these quads keep the ambient
        // depth state.

        m3d::rend::VbHandle vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZCT1);

        int const vertexCount = 4 * parts->m_numParticles;
        // Zeroed before the lock at 0x8E2D5D; the renderer overwrites it, but
        // keep the store so an implementation that leaves it alone still reads 0.
        int vofs = 0;
        char* const vertexData = (char*)M3D_RENDERER->LockVbStreaming(vb, vertexCount, vofs, 0);

        Particle* particle = parts->m_particles;
        if (particle)
        {
            float* vertexPtr = (float*)vertexData;
            do
            {
                float orgX, orgY, orgZ;
                if (m_updateXForm)
                {
                    orgX = particle->m_locorigin.x;
                    orgY = particle->m_locorigin.y;
                    orgZ = particle->m_locorigin.z;
                }
                else
                {
                    orgX = particle->m_origin.x + particle->m_locorigin.x;
                    orgY = particle->m_origin.y + particle->m_locorigin.y;
                    orgZ = particle->m_origin.z + particle->m_locorigin.z;
                }

                float const size = particle->m_size;
                unsigned const clr = particle->m_curClr;

                // UVs run (0,0) (0,1) (1,1) (1,0) across the four corners.
                vertexPtr[0] = orgX + quad[0].x * size;
                vertexPtr[1] = orgY + quad[0].y * size;
                vertexPtr[2] = orgZ + quad[0].z * size;
                vertexPtr[3] = *(float*)&clr;
                vertexPtr[4] = 0.0f;
                vertexPtr[5] = 0.0f;

                vertexPtr[6] = orgX + quad[1].x * size;
                vertexPtr[7] = orgY + quad[1].y * size;
                vertexPtr[8] = orgZ + quad[1].z * size;
                vertexPtr[9] = *(float*)&clr;
                vertexPtr[10] = 0.0f;
                vertexPtr[11] = 1.0f;

                vertexPtr[12] = orgX + quad[2].x * size;
                vertexPtr[13] = orgY + quad[2].y * size;
                vertexPtr[14] = orgZ + quad[2].z * size;
                vertexPtr[15] = *(float*)&clr;
                vertexPtr[16] = 1.0f;
                vertexPtr[17] = 1.0f;

                vertexPtr[18] = orgX + quad[3].x * size;
                vertexPtr[19] = orgY + quad[3].y * size;
                vertexPtr[20] = orgZ + quad[3].z * size;
                vertexPtr[21] = *(float*)&clr;
                vertexPtr[22] = 1.0f;
                vertexPtr[23] = 0.0f;

                particle = particle->m_next;
                vertexPtr += 24;
            } while (particle);
        }

        M3D_RENDERER->UnlockVb(vb);
        M3D_RENDERER->SetToStream0(vb);
        M3D_RENDERER->SetIndices(m_IbPoolField, vofs);
        M3D_RENDERER->DrawIndexedPrimitiveEffect(
            rend::M3DPT_TRIANGLELIST,
            m_shader,
            0,
            vertexCount,
            m_IbPoolField.RealOffset,
            2 * parts->m_numParticles);

        if (m_updateXForm)
            M3D_RENDERER->MatPop(true);

        return 1;
    }

    void QuadPS::CreateIb()
    {
        //TODO: recreate normal logic
        m_IbPoolField = Application::g_pApp->m_renderer->AddIbPoolField(1200);
        auto ptr = static_cast<char*>(Application::g_pApp->m_renderer->LockIbPoolField(m_IbPoolField));
        auto inc = 2;
        short* base = reinterpret_cast<short*>(ptr + 4);
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
        M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
    }

    float QuadPS::GetBoundRadius() const
    {
        // The longest possible particle lifetime bounds how far one can travel.
        float const longestTtl = (m_Emitter.m_ttlMin <= m_Emitter.m_ttlMax) ? m_Emitter.m_ttlMax : m_Emitter.m_ttlMin;
        return longestTtl * 10.0f;
    }
}  // namespace m3d
