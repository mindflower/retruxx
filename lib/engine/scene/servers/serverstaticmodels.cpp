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

int ModelsRenderHandleCommon(int, void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

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

    int StaticModelsServer::GetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StaticModelsServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void StaticModelsServer::ReleaseBuffers()
    {
        RETRUXX_NOT_IMPLEMENTED;
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


            RETRUXX_NOT_IMPLEMENTED;

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

    int StaticModelsServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StaticModelsServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StaticModelsServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int StaticModelsServer::AddItem(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }
}
