#include <m3dapp.h>
#include <scene/servers/serverstaticmodels.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/ini.h>
#include <gsmodel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <config.h>
#include <core/kernel.h>
#include <cmath>
#include <cstring>
#include <scene/nodes/sgnodeloadpoint.h>

namespace m3d
{
    struct OverrideRenderSettings
    {
        /* 0x0000 */ int m_dontOverrideBlend;
        /* 0x0004 */ m3d::rend::TextureState m_color0;
        /* 0x0008 */ m3d::rend::TextureState m_alpha0;
        /* 0x000c */ m3d::rend::TextureState m_color1;
        /* 0x0010 */ m3d::rend::TextureState m_alpha1;
        /* 0x0014 */ bool m_tessellate;
    }; /* size: 0x0018 */

    // What SgStaticModelNode hands to RenderItem (SgStaticModelNode::Render::renderInfo in the PDB).
    struct StaticModelRenderInfo
    {
        /* 0x0000 */ CMatrix* m_localXForm;
        /* 0x0004 */ float m_alpha;
        /* 0x0008 */ m3d::rend::Cull m_culling;
    }; /* size: 0x000c */
}

int ModelsRenderHandleCommon(int id, void* params)
{
    // RVA 0x777C90 - the state shared by static and animated models: alpha test, the texture stages (from the
    // override settings, or modulate/modulate then texture-modulate/previous) and lighting.
    auto* const renderer = M3D_RENDERER;
    if (id == -2)
    {
        renderer->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
        m3d::OverrideRenderSettings ors0;
        auto* settings = static_cast<m3d::OverrideRenderSettings*>(params);
        if (!settings)
        {
            settings = &ors0;
            ors0.m_color0 = m3d::rend::TS_MODULATE;
            ors0.m_alpha0 = m3d::rend::TS_MODULATE;
            ors0.m_color1 = m3d::rend::TS_TEX_MODULATE_PREV;
            ors0.m_alpha1 = m3d::rend::TS_PREV;
        }
        renderer->SetStageState(0, m3d::rend::BM_COLOR, settings->m_color0);
        renderer->SetStageState(0, m3d::rend::BM_ALPHA, settings->m_alpha0);
        renderer->SetStageState(1, m3d::rend::BM_COLOR, settings->m_color1);
        renderer->SetStageState(1, m3d::rend::BM_ALPHA, settings->m_alpha1);
        renderer->PushLighting(true);
        renderer->SetCull(m3d::rend::M3DCULL_CCW, false);
        return 1;
    }
    if (id == -3)
    {
        renderer->SetAlphaTest(0);
        renderer->PopLighting();
        return 1;
    }
    return 0;
}

namespace m3d
{
    int StaticModelsServer::GetItemProperty(int id, int prop, void* dest)
    {
        // RVA 0x778EF0
        if (DataServer::GetItemProperty(id, prop, dest))
        {
            return 1;
        }

        auto* const model = static_cast<CGSModel*>(m_models[id].m_ptr);
        switch (prop)
        {
        case PROP_INTERNAL_GETGSMODEL:
            *static_cast<CGSModel**>(dest) = model;
            return 1;
        case PROP_SRV_BOUNDING_BOX:
            *static_cast<PropSrvBoundingBox*>(dest)->m_destBox = model->m_box;
            return 1;
        case PROP_SRV_COLLISION_DATA_SZ:
            *static_cast<CollisionDataHeader*>(dest) = model->m_col_header;
            return 1;
        case PROP_SRV_COLLISION_DATA:
        {
            void** const buffers = static_cast<void**>(dest);
            memcpy(buffers[0], model->m_col_verts, sizeof(CollidingVertex) * model->m_col_header.numVertices);
            memcpy(buffers[1], model->m_col_idx, 3 * sizeof(unsigned short) * model->m_col_header.numFaces);
            return 1;
        }
        case PROP_SRV_LOADPOINT:
        {
            // A static model's loadpoints only carry a position.
            auto* const lp = static_cast<PropSrvLoadpoint*>(dest);
            LPoint const& point = model->m_loadPoints[lp->m_fromLoadpoint];
            lp->m_newOrg.x = point.x;
            lp->m_newOrg.y = point.y;
            lp->m_newOrg.z = point.z;
            return 1;
        }
        default:
            break;
        }

        if (prop != PROP_SRV_SNAPSHOT)
        {
            return 0;
        }
        if (static_cast<unsigned>(id) < m_models.size() && id >= 0)
        {
            *static_cast<unsigned char**>(dest) = m_shots[id];
            return 1;
        }
        M3D_LOG_INFO("Error: could't read snapshot for empty model");
        return 1;
    }

    int StaticModelsServer::SaveAllLoadedEntities(char const* fileName)
    {
        // RVA 0x778930 - NOTE: the stream is never closed, and tessellate and trackland are not saved.
        ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
        ref_ptr root = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "StaticModels");
        int i = 0;
        for (auto it = m_models.begin(); it != m_models.end(); ++it, ++i)
        {
            ref_ptr modelNode = xmlFile->CreateNode(cmn::XML_NODE_ELEMENT, "model");
            modelNode->SetAttribute("id", it->m_name.c_str());
            int value = 0;
            GetItemProperty(i, PROP_MODEL_CAST_SHADOW, &value);
            modelNode->SetAttribute("shadow", CStr(value).c_str());
            GetItemProperty(i, PROP_MODEL_WIND_WAVY, &value);
            modelNode->SetAttribute("windwavy", CStr(value).c_str());
            GetItemProperty(i, PROP_MODEL_TRANS, &value);
            modelNode->SetAttribute("trans", CStr(value).c_str());
            GetItemProperty(i, PROP_MODEL_2SIDED, &value);
            modelNode->SetAttribute("twosided", CStr(value).c_str());
            modelNode->SetAttribute("file", it->m_fileName.c_str());
            root->AddChild(modelNode.get());
        }
        xmlFile->AddChild(root.get());

        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (stream->Open(fileName, fs::IStream::OPEN_WRITE))
        {
            xmlFile->Write(*stream);
        }
        return 1;
    }

    void StaticModelsServer::ReleaseBuffers()
    {
        // RVA 0x777C20
        for (int i = 0; i < m_numvbBanks; ++i)
        {
            if (m_vbArrayForAllModels[i].IsValid())
            {
                M3D_RENDERER->ReleaseVb(m_vbArrayForAllModels[i]);
            }
        }
        for (int i = 0; i < m_numibBanks; ++i)
        {
            if (m_ibArrayForAllModels[i].IsValid())
            {
                M3D_RENDERER->ReleaseIb(m_ibArrayForAllModels[i]);
            }
        }
    }

    void StaticModelsServer::RenderItem(int id, void* params)
    {
        m_profiler->StartCountdown();

        // Ensure models are prepared for rendering
        if (!this->m_preparedToRender)
        {
            this->m_preparedToRender = true;
            PrepareToRender();
        }


        if (id == -2) {
            // Begin rendering mode
            ModelsRenderHandleCommon(-2, params);

            rend::TexHandle handle;
            M3D_RENDERER->SetTexture(1, handle, -1.0f);

            this->m_curvbBank = 0;
            this->m_curibBank = 0;

            M3D_RENDERER->SetToStream0(this->m_vbArrayForAllModels[0]);
            M3D_RENDERER->SetIndices(this->m_ibArrayForAllModels[this->m_curibBank], 0);

            // Set some render state (gap80 suggests a specific function offset)
            M3D_RENDERER->PushFog(false);

            m3d::OverrideRenderSettings* settings = (m3d::OverrideRenderSettings*)params;
            // Set blend mode
            if (!settings || !settings->m_dontOverrideBlend) {
                M3D_RENDERER->SetBlend(m3d::rend::BlendMode::BM_ALPHA, 0);
            }

            // Handle tessellation
            if (settings)
            {
                this->m_tessellate = settings->m_tessellate;
            }

            if (this->m_tessellate)
            {
                float tessLevel = M3D_KERNEL->GetEngineCfg().m_NPatchLevel.GetF();

                if (tessLevel < 1.0f) {
                    this->m_tessellate = false;
                }

                if (M3D_RENDERER->GetMaxNPatchTessellationLevel() < 1.0) {
                    this->m_tessellate = false;
                }
            }

            M3D_RENDERER->SetAlphaTest(80);

        }
        else if (id == -3)
        {
            // End rendering mode
            m3d::OverrideRenderSettings* settings = (m3d::OverrideRenderSettings*)params;
            ModelsRenderHandleCommon(-3, settings);

            // Reset blend mode
            if (!settings || !settings->m_dontOverrideBlend) {
                M3D_RENDERER->SetBlend(m3d::rend::BlendMode::BM_NONE, 0);
            }

            // Handle tessellation
            this->m_tessellate = true;
            float tessLevel = M3D_KERNEL->GetEngineCfg().m_NPatchLevel.GetF();

            if (tessLevel < 1.0f) {
                this->m_tessellate = false;
            }

            if (M3D_RENDERER->GetMaxNPatchTessellationLevel() < 1.0) {
                this->m_tessellate = false;
            }

            M3D_RENDERER->PopFog();
        }
        else if (id != -4) {
            // Render specific model
            m3d::CGSModel* model = (m3d::CGSModel*)this->m_models[id].m_ptr;

            // Set vertex buffer if changed
            if (this->m_curvbBank != model->m_numvbbank)
            {
                M3D_RENDERER->SetToStream0(this->m_vbArrayForAllModels[model->m_numvbbank]);
                this->m_curvbBank = model->m_numvbbank;
            }

            // Set index buffer if changed
            if (this->m_curibBank != model->m_numibbank)
            {
                M3D_RENDERER->SetIndices(this->m_ibArrayForAllModels[model->m_numibbank], 0);
                this->m_curibBank = model->m_numibbank;
            }


            // RVA 0x7784E0 - see-through models do not write depth.
            auto const* const ri = static_cast<StaticModelRenderInfo const*>(params);
            M3D_RENDERER->SetCull(ri->m_culling, false);
            if (ri->m_alpha < 0.6f)
            {
                M3D_RENDERER->PushZbState(rend::ZB_NOWRITE);
            }
            rend::Material mtrl;
            mtrl.init(rend::Colorf(1.0f, 1.0f, 1.0f, ri->m_alpha));
            M3D_RENDERER->MaterialSet(mtrl);

            int tessModel = 0;
            if (m_tessellate)
            {
                GetItemProperty(id, PROP_MODEL_TESSELLATE, &tessModel);
                if (tessModel)
                {
                    M3D_RENDERER->SetNPatchLevel(M3D_ENGINE_CFG.m_NPatchLevel.GetF(), false);
                }
            }
            model->Render(*ri->m_localXForm, ri->m_alpha);
            if (m_tessellate && tessModel)
            {
                M3D_RENDERER->SetNPatchLevel(0.0f, false);
            }
            if (ri->m_alpha < 0.6f)
            {
                M3D_RENDERER->PopZbState();
            }

            //int v14 = params;
            //
            //// Set culling mode
            //M3D_RENDERER->SetCull(*reinterpret_cast<m3d::rend::Cull*>(v14 + 8), 0);
            //
            //// Handle alpha transparency
            //float alpha = *reinterpret_cast<float*>(v14 + 4);
            //if (alpha < 0.6f) {
            //    M3D_RENDERER->SetZState(2); // gap30 suggests a specific function
            //}
            //
            //// Set material
            //m3d::rend::Colorf diff(1.0f, 1.0f, 1.0f, alpha);
            //m3d::rend::Material mtrl;
            //mtrl.init(diff);
            //M3D_RENDERER->MaterialSet(&mtrl);
            //
            //// Handle tessellation
            //int tessModel = 0;
            //if (this->m_tessellate) {
            //    this->GetItemProperty(id, 4, &tessModel);
            //
            //    if (tessModel) {
            //        m3d::CVar* nPatchLevel = &m3d::g_Kernel->GetEngineCfg()->m_NPatchLevel;
            //        float tessLevel = (nPatchLevel->m_type == CVAR_FLOAT) ? nPatchLevel->m_f : static_cast<float>(nPatchLevel->m_i);
            //
            //        renderer->SetNPatchLevel(tessLevel, 0);
            //        params = *reinterpret_cast<int*>(&tessLevel);
            //    }
            //}
            //
            //// Render the model
            //const CMatrix* transform = *reinterpret_cast<const CMatrix**>(v14);
            //model->Render(transform, alpha);
            //
            //// Reset tessellation if used
            //if (this->m_tessellate && tessModel) {
            //    renderer->SetNPatchLevel(0.0f, 0);
            //}
            //
            //// Reset z-state if modified
            //if (alpha < 0.6f) {
            //    renderer->PopZbState();
            //}
        }

        // Update profiler
        m_profiler->EndCountdown();
    }

    int StaticModelsServer::SetItemProperty(int id, int prop, void* src)
    {
        // RVA 0x777930
        return DataServer::SetItemProperty(id, prop, src);
    }

    int StaticModelsServer::RemoveItem(int)
    {
        // RVA 0x777920 - static models live for the whole level.
        return 1;
    }

    int StaticModelsServer::Release()
    {
        // RVA 0x7791F0
        m_valid = false;
        for (size_t i = 0; i < m_models.size(); ++i)
        {
            delete static_cast<CGSModel*>(m_models[i].m_ptr);
            delete[] m_shots[i];
        }
        m_models = retruxx::vector<Model>();
        m_shots = retruxx::vector<unsigned char*>();
        ReleaseBuffers();
        return 1;
    }

    StaticModelsServer::StaticModelsServer()
    {
        auto idx = Application::g_pApp->GetProfilerStack().AddProfiler("static", 0x1E);
        if (idx < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }
        else
        {
            m_profiler = 0;
        }
    }

    int StaticModelsServer::Init()
    {
        m_valid = true;
        return 1;
    }

    StaticModelsServer::~StaticModelsServer()
    {
        // RVA 0x77A490
        Release();
    }

    int StaticModelsServer::AddItem(char const* params, char const* id)
    {
        // RVA 0x779DE0 - loads the model described by the <model id=...> entry of the XML file named in params,
        // together with its .raw snapshot (a square greyscale image, stored after a two-byte width/height header).
        int const existing = GetItemByName(id, false);
        if (existing != -1)
        {
            return existing;
        }

        Proto proto = PROTO_NONE;
        int paramsPos = 0;
        ParseProto(params, &proto, &paramsPos);
        if (proto != PROTO_FILE)
        {
            // NOTE: the message is indexed by the protocol, so anything but PROTO_NONE prints it with the first
            // characters cut off.
            M3D_LOG_INFO(CStr("protocol is not supported " + static_cast<int>(proto)));
            return -1;
        }

        int shadow = 1;
        int trans = 1;
        char const* const pp = params + paramsPos;
        CStr modelFileName;
        int windwavy = 0;
        int twosided = 0;
        int tessellate = 0;
        int trackland = 0;
        CStr err;
        ref_ptr<cmn::XmlFile> xmlFile(ReadXmlFile(pp, &err));
        if (!xmlFile)
        {
            M3D_LOG_INFO(CStr("StaticModelsServer: ") + err);
            return -1;
        }

        ref_ptr node = xmlFile->CreateNode();
        xmlFile->GetFirstChild(node.get(), "StaticModels");
        if (node->IsEmpty())
        {
            return -1;
        }
        node->GetFirstChild(node.get(), "model");
        // NOTE: an entry without an id is compared as a null string.
        while (!node->IsEmpty() && strcmp(node->GetAttribute("id"), id))
        {
            node->GetNextSibling(node.get(), "model");
        }
        if (node->IsEmpty())
        {
            return -1;
        }

        modelFileName = CStr(node->GetAttribute("file"));
        SafeIntAttrib(shadow, node.get(), "shadow");
        SafeIntAttrib(windwavy, node.get(), "windwavy");
        SafeIntAttrib(trans, node.get(), "trans");
        SafeIntAttrib(twosided, node.get(), "twosided");
        SafeIntAttrib(tessellate, node.get(), "tessellate");
        SafeIntAttrib(trackland, node.get(), "trackland");

        auto* const model = new CGSModel();
        if (!model->Load(modelFileName))
        {
            M3D_LOG_INFO(CStr("StaticModelsServer::AddItem: cannot load model ") + modelFileName);
            delete model;
            return -1;
        }
        m_models.push_back(Model(model, modelFileName.c_str(), pp, id));

        CStr const passfilename = modelFileName.substr(0, static_cast<int>(strlen(modelFileName.c_str())) - 4) +
            CStr(".raw");
        unsigned char* shot = nullptr;
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (stream->Open(passfilename.c_str(), fs::IStream::OPEN_READ))
        {
            unsigned int const size = stream->GetSize();
            shot = new unsigned char[size + 2];
            stream->ReadBytes(shot + 2, size);
            stream->Close();
            unsigned char const side = static_cast<unsigned char>(
                static_cast<__int64>(std::sqrt(static_cast<double>(static_cast<int>(size)))));
            shot[1] = side;
            shot[0] = side;
        }
        m_shots.push_back(shot);

        int const sh = static_cast<int>(m_models.size()) - 1;
        SetItemProperty(sh, PROP_MODEL_CAST_SHADOW, &shadow);
        SetItemProperty(sh, PROP_MODEL_WIND_WAVY, &windwavy);
        SetItemProperty(sh, PROP_MODEL_TRANS, &trans);
        SetItemProperty(sh, PROP_MODEL_2SIDED, &twosided);
        SetItemProperty(sh, PROP_MODEL_TESSELLATE, &tessellate);
        SetItemProperty(sh, PROP_MODEL_TRACK_LAND, &trackland);
        m_preparedToRender = false;
        return sh;
    }

    void StaticModelsServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemslist)
    {
        // TODO: generated code
        if (itemslist.empty())
        {
            return;
        }

        // Clear existing models and shots
        m_models.clear();
        m_shots.clear();

        // Parse protocol from the first item
        m3d::DataServer::Proto proto;
        int paramsPos;
        m3d::DataServer::ParseProto(itemslist[0].m_filename.c_str(), &proto, &paramsPos);

        if (proto != PROTO_FILE)
        {
            M3D_LOG_INFO("Protocol is not supported: " + CStr(proto));
            return;
        }

        // Parse XML file
        const char* xmlPath = itemslist[0].m_filename.c_str() + paramsPos;
        CStr errorStr;

        ref_ptr<m3d::cmn::XmlFile> xmlFile = m3d::ReadXmlFile(xmlPath, &errorStr);
        if (!xmlFile)
        {
            M3D_LOG_INFO("StaticModelsServer: " + errorStr);
            return;
        }

        // Process XML nodes
        ref_ptr<m3d::cmn::XmlNode> rootNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(rootNode, "StaticModels");

        if (rootNode->IsEmpty())
        {
            return;
        }

        ref_ptr<m3d::cmn::XmlNode> modelNode = xmlFile->CreateNode();
        rootNode->GetFirstChild(modelNode, "model");

        int processedCount = 0;

        while (!modelNode->IsEmpty())
        {
            const char* modelId = modelNode->GetAttribute("id");

            // Check if item already exists
            int existingItem = m3d::DataServer::GetItemByName(modelId, 0);

            // Call load callback if specified
            if (m_fnLoadCallback)
            {
                float progress = static_cast<float>(processedCount) / itemslist.size();
                m_fnLoadCallback(progress, m_fnLoadCallbackData);
            }

            if (existingItem == -1)
            {
                // Process new model
                CStr modelFile = modelNode->GetAttribute("file");

                int shadow = 0;
                int windWavy = 0;
                int trans = 0;
                int twoSided = 0;
                int tessellate = 0;
                int trackLand = 0;

                m3d::SafeIntAttrib(shadow, modelNode, "shadow");
                m3d::SafeIntAttrib(windWavy, modelNode, "windwavy");
                m3d::SafeIntAttrib(trans, modelNode, "trans");
                m3d::SafeIntAttrib(twoSided, modelNode, "twosided");
                m3d::SafeIntAttrib(tessellate, modelNode, "tessellate");
                m3d::SafeIntAttrib(trackLand, modelNode, "trackland");

                // Load model
                m3d::CGSModel* model = new m3d::CGSModel();
                if (model->Load(modelFile))
                {
                    // Create model data and add to collection
                    m3d::DataServer::Model modelData(
                        (void*)model,
                        modelFile.c_str(),
                        itemslist[0].m_filename.c_str(),
                        modelId
                    );

                    m_models.push_back(modelData);

                    // Load shot data
                    CStr shotFile = modelFile.substr(0, modelFile.rfind('.')) + ".raw";

                    scoped_ptr fileStream = M3D_KERNEL->GetFileServer().CreateFileStream();
                    if (fileStream->Open(shotFile.c_str(), fs::IStream::OPEN_READ))
                    {
                        size_t fileSize = fileStream->GetSize();
                        unsigned char* shotData = new unsigned char[fileSize + 2];
                        fileStream->ReadBytes(shotData + 2, fileSize);
                        fileStream->Close();

                        // Calculate dimensions (simplified)
                        size_t dimension = static_cast<size_t>(std::sqrt(fileSize));
                        shotData[0] = static_cast<unsigned char>(dimension);
                        shotData[1] = static_cast<unsigned char>(dimension);

                        m_shots.push_back(shotData);

                        // Set model properties
                        size_t modelIndex = m_models.size() - 1;
                        SetItemProperty(modelIndex, 0, &shadow);
                        SetItemProperty(modelIndex, 1, &windWavy);
                        SetItemProperty(modelIndex, 2, &trans);
                        SetItemProperty(modelIndex, 3, &twoSided);
                        SetItemProperty(modelIndex, 4, &tessellate);
                        SetItemProperty(modelIndex, 5, &trackLand);

                        m_preparedToRender = false;
                    }
                }
                else
                {
                    // Log error
                    M3D_LOG_INFO("StaticModelsServer::AddItem: cannot load model " + modelFile);
                    delete model;
                }
            }

            processedCount++;
            modelNode->GetNextSibling(modelNode, "model");
        }

        // Check for unread files
        for (size_t i = 0; i < itemslist.size(); ++i)
        {
            if (!itemslist.at(i).m_fileWasRead)
            {
                M3D_LOG_INFO("DataServer: cannot read file: " +
                    itemslist.at(i).m_filename + " id = " +
                    itemslist.at(i).m_id);
            }
        }
    }

    int StaticModelsServer::PrepareToRender()
    {
        // RVA 0x778000 - packs every model's mesh into shared vertex and index buffers of up to 64K entries each.
        ReleaseBuffers();

        // First pass: prepare the models and count the banks.
        int numVbBanks = 1;
        int numIbBanks = 1;
        int nvertices = 0;
        int nindices = 0;
        for (auto& item : m_models)
        {
            auto* const model = static_cast<CGSModel*>(item.m_ptr);
            // NOTE: the banks are split on the file's vertex and face counts, but filled with the draw counts.
            if (static_cast<unsigned>(nvertices) + model->m_header.m_numVertices > 0x10000)
            {
                nvertices = 0;
                ++numVbBanks;
            }
            if (model->m_header.m_numFaces + nindices + 2 * model->m_header.m_numFaces > 0x10000)
            {
                ++numIbBanks;
                nindices = 0;
            }
            if (!model->Prepare2Draw(DirectoryFromFileName(item.m_fileName), nvertices, nindices))
            {
                M3D_LOG_ERR(CStr("StaticScene: error preparing model ") + item.m_name);
                return 0;
            }
            nindices += model->m_numDrawIndices;
            nvertices += model->m_numDrawVerts;
        }

        m_numvbBanks = numVbBanks;
        m_numibBanks = numIbBanks;
        for (int i = 0; i < numVbBanks; ++i)
        {
            int const count = i == numVbBanks - 1 ? nvertices : 0x10000;
            m_vbArrayForAllModels[i] = M3D_RENDERER->AddVb(rend::VERTEX_XYZNT1, count, CStr("ModelMesh"), 0);
        }
        for (int i = 0; i < numIbBanks; ++i)
        {
            int const count = i == numIbBanks - 1 ? nindices : 0x10000;
            m_ibArrayForAllModels[i] = M3D_RENDERER->AddIb(count, false);
        }

        // Second pass: copy the meshes in.
        int curVbBank = 0;
        int curIbBank = 0;
        auto* verts = static_cast<rend::VertexXYZNT1*>(M3D_RENDERER->LockVb(m_vbArrayForAllModels[0], 0, 0, 0));
        auto* indices = static_cast<unsigned short*>(M3D_RENDERER->LockIb(m_ibArrayForAllModels[0], 0, 0, 0));
        int vertOffset = 0;
        int idxOffset = 0;
        int progress = 0;
        for (auto& item : m_models)
        {
            auto* const model = static_cast<CGSModel*>(item.m_ptr);
            if (m_fnLoadCallback)
            {
                m_fnLoadCallback(progress / static_cast<int>(m_models.size()), m_fnLoadCallbackData);
            }
            if (vertOffset + model->m_numDrawVerts > 0x10000)
            {
                M3D_RENDERER->UnlockVb(m_vbArrayForAllModels[curVbBank]);
                ++curVbBank;
                vertOffset = 0;
                verts = static_cast<rend::VertexXYZNT1*>(M3D_RENDERER->LockVb(m_vbArrayForAllModels[curVbBank], 0, 0, 0));
            }
            if (idxOffset + model->m_numDrawIndices > 0x10000)
            {
                M3D_RENDERER->UnlockIb(m_ibArrayForAllModels[curIbBank]);
                ++curIbBank;
                idxOffset = 0;
                indices = static_cast<unsigned short*>(M3D_RENDERER->LockIb(m_ibArrayForAllModels[curIbBank], 0, 0, 0));
            }
            model->Prepare2Draw2(&verts[vertOffset], &indices[idxOffset]);
            vertOffset += model->m_numDrawVerts;
            idxOffset += model->m_numDrawIndices;
            progress += 100;
            model->m_numvbbank = curVbBank;
            model->m_numibbank = curIbBank;
        }
        M3D_RENDERER->UnlockVb(m_vbArrayForAllModels[curVbBank]);
        M3D_RENDERER->UnlockIb(m_ibArrayForAllModels[curIbBank]);
        return 1;
    }
}
