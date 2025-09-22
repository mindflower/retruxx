#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <scene/servers/serveranimatedmodel.h>
#include <core/ini.h>
#include <client.h>
#include <world.h>
#include <core/log.h>
#include <file/fileserver.h>

namespace m3d
{
    extern CClient* pClient;
}

namespace m3d
{
    void AnimatedModelsServer::PostLoad()
    {
        for (auto& model : m_models)
        {
            throw retruxx::logic_error("Not implemented");
        }
    }

    int AnimatedModelsServer::AddItem(char const* params, char const* id)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::IsBonePresentsInModel(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderItem(int, void*)
    {
    }

    void AnimatedModelsServer::UnregisterNode(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::ReportServerInfo(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Init()
    {
        auto const logoFileName = g_Kernel->GetEngineCfg().m_loadFromGAM.GetB() ?
            "data\\models\\Logos\\Logos.gam" : "data\\models\\Logos\\Logos.sam";
        m_MeshMaterialManager.Init(logoFileName, g_Kernel->GetEngineCfg().m_pathToBelongsToLogos.GetS());

        m_impostorVs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/impostorTest_vs11.vs", "ImpostorVS", rend::IHlslShader::VS_1_1);
        if (!m_impostorVs)
        {
            return 0;
        }

        m_impostorPs = Application::g_pApp->m_renderer->NewHlslShader("data/shaders/impostorTest_ps11.ps", "ImpostorPS", rend::IHlslShader::PS_1_1);
        if (!m_impostorPs)
        {
            return 0;
        }

        m_valid = true;
        return 1;
    }

    CVector AnimatedModelsServer::GetBoundSizes(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetBoneMatrixByNameFromModelName(char const*, CStr const&, CMatrix&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModelsServer::AnimatedModelsServer()
    {
        auto idx = M3D_APP->GetProfilerStack().AddProfiler("animated", 0x1E);
        if (idx < M3D_APP->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = M3D_APP->GetProfilerStack().GetProfiler(idx);
        }
        idx = M3D_APP->GetDbgCounterStack().AddCounter("nodes");
        if (idx < M3D_APP->GetDbgCounterStack().GetNumCounters())
        {
            m_countNodes = M3D_APP->GetDbgCounterStack().GetCounter(idx);
            m_countNodes->SetI(0);
        }
        idx = M3D_APP->GetDbgCounterStack().AddCounter("meshes");
        if (idx < M3D_APP->GetDbgCounterStack().GetNumCounters())
        {
            m_countMeshes = M3D_APP->GetDbgCounterStack().GetCounter(idx);
            m_countMeshes->SetI(0);
        }
    }

    int AnimatedModelsServer::SetItemProperty(int id, int prop, void* src)
    {
        if (m3d::DataServer::SetItemProperty(id, prop, src))
            return 1;


        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderTransparents(SgNode**, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderShadowVolumesSet(SgNode**, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModelsServer::~AnimatedModelsServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GenerateImpostorsIfNeeded()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetItemProperty(int id, int prop, void* dest)
    {
        if (DataServer::GetItemProperty(id, prop, dest))
        {
            return 1;
        }
        if (id == -1)
        {
            return 0;
        }
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RegisterNode(SgNode* node)
    {
        AnimInfo* anim = nullptr;
        node->GetProperty(1u, &anim);
        anim = new AnimInfo;
        node->SetProperty(1u, &anim);

        int modelId = -1;
        node->GetProperty(4360u, &modelId);
        if (modelId >= 0)
        {
            if (modelId < m_models.size())
            {
                auto* model = reinterpret_cast<DynamicModel*>(&m_models[modelId]);
                auto& animModel = model->m_mdl[0];
                if (anim->m_forModel != animModel)
                {
                    anim->Release();
                    anim->CreateFor(animModel);
                }

                auto modelEffectList = new ModelEffectList(model);
                node->SetProperty(2u, &modelEffectList);

                m3d::Configuration* cfg = nullptr;
                node->GetProperty(8707u, &cfg);

                animModel->FromCfgNum(*cfg);
                animModel->CalculateMeshes(*cfg);

                void* temp = nullptr;
                node->GetProperty(8706u, &temp);
                node->SetProperty(8706u, &temp);

                bool useImpostors = true;
                node->GetProperty(8720u, &useImpostors);
                if (useImpostors)
                {
                    node->SetProperty(8720u, &model->m_useImpostors);
                }

                m3d::TransparencyType tt = TT_NONE;
                node->GetServerItemProperty(2u, &tt);
                node->SetTransparencyType(tt);
            }
        }

        if (!anim->IsEmpty())
        {
            // TODO: check this
            node->GetGraph()->LinkThinkNode(node);
        }
    }

    void AnimatedModelsServer::UpdateItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    namespace
    {
        void __fastcall DefineSkinsToLoad(m3d::LoadSkins& skinsToLoad, CStr const& paramsStr)
        {
            retruxx::string view(paramsStr.c_str(), paramsStr.length());
            const auto skinsPos = view.find("skins:");
            const auto postSkinsPos = skinsPos + 6;
            const auto semicolonPos = view.find(";", postSkinsPos);
            if (skinsPos == retruxx::string::npos || semicolonPos == retruxx::string::npos)
            {
                return;
            }

            const auto params = view.substr(postSkinsPos, semicolonPos - postSkinsPos);
            retruxx::vector<CStr> tokens;
            Tokenize(params.c_str(), tokens, "(), ;\t");

            skinsToLoad.loadAllSkins = tokens.empty();

            for (const auto& token : tokens)
            {
                skinsToLoad.loadSkins.insert(strToInt(token));
            }
        }

    }

    void AnimatedModelsServer::AddItemsList(retruxx::vector<ServerItem>& itemslist)
    {
        // TODO: generated code
        if (itemslist.empty())
            return;

        // Track loading time
        DWORD dwStartTime = GetTickCount();
        DWORD dwGamTime = 0;

        // Create list of skins to load
        retruxx::vector<LoadSkins> skinsToLoad(itemslist.size());

        // Pre-define skins to load from item parameters
        for (int i = 0; i < skinsToLoad.size(); ++i) {
            if (!itemslist[i].m_params.empty()) {
                DefineSkinsToLoad(skinsToLoad[i], itemslist[i].m_params);
            }
        }

        int numitems = itemslist.size();
        int iterNode = 0;

        // First pass: update existing models
        for (auto it = m_models.begin(); it != m_models.end(); ) {
            if (iterNode < numitems) {
                bool found = false;
                for (size_t i = 0; i < itemslist.size(); i++) {
                    if (itemslist[i].m_id == it->m_name) {
                        // Update existing model
                        itemslist[i].m_fileWasRead = true;
                        iterNode++;

                        DynamicModel* dynamicModel = reinterpret_cast<DynamicModel*>(it->m_ptr);
                        // Cache sound IDs
                        for (int j = 0; j < 32; j++) {
                            if (!dynamicModel->m_soundIds[j].empty()) {
                                m3d::Application::g_pApp->m_cachedSoundIDs.insert(dynamicModel->m_soundIds[j]);
                            }
                        }

                        // Reload skins and update cubemap
                        dynamicModel->m_mdl[0]->ReloadSkins(skinsToLoad[i]);
                        dynamicModel->m_mdl[0]->UpdateCubemap();

                        found = true;
                        break;
                    }
                }

                if (found) {
                    ++it;
                    continue;
                }
            }

            // Remove model that's no longer in the list
            if (it->m_ptr) {
                delete it->m_ptr;
                it->m_ptr = nullptr;
            }
            it = m_models.erase(it);
        }

        numitems -= iterNode;
        if (numitems <= 0) {
            return; // All items were existing models
        }

        // Parse protocol from first item
        m3d::DataServer::Proto proto;
        int protoPos;
        ParseProto(itemslist.front().m_filename.c_str(), &proto, &protoPos);

        if (proto != PROTO_FILE) {
            M3D_LOG_ERR("Error protocol: " + CStr(proto));
            return;
        }

        // Load XML file
        CStr xmlFilename = &itemslist.front().m_filename[protoPos];
        CStr xmlContent;

        ref_ptr xmlFile = m3d::ReadXmlFile(xmlFilename.c_str(), &xmlContent);
        if (!xmlFile) {
            M3D_LOG_ERR("ServerAnimatedModel: " + xmlFilename);
            return;
        }

        // Process XML models
        ref_ptr modelsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(modelsNode, "AnimatedModels");

        if (modelsNode->IsEmpty()) {
            return;
        }

        ref_ptr modelNode = xmlFile->CreateNode();
        modelsNode->GetFirstChild(modelNode, "model");

        while (!modelNode->IsEmpty()) {
            CStr modelId = modelNode->GetAttribute("id");

            // Check if this model is in our items list
            int itemIndex = -1;
            for (size_t i = 0; i < itemslist.size(); i++) {
                if (itemslist[i].m_id == modelId && !itemslist[i].m_fileWasRead) {
                    itemIndex = i;
                    break;
                }
            }

            if (itemIndex == -1) {
                 modelNode->GetNextSibling(modelNode, "model");
                continue;
            }

            // Parse model attributes
            CStr modelFile;
            SafeStrAttrib(modelFile, modelNode, "file");

            int shadow = 0;
            SafeIntAttrib(shadow, modelNode, "shadow");

            int windwavy = 0;
            SafeIntAttrib(shadow, modelNode, "windwavy");

            int tessellate = 0;
            SafeIntAttrib(shadow, modelNode, "tessellate");

            int trackland = 0;
            SafeIntAttrib(shadow, modelNode, "trackland");

            int shadowVolume = 0;
            SafeIntAttrib(shadow, modelNode, "shadowVolume");

            bool useImpostors = false;
            m3d::SafeBoolAttrib(useImpostors, modelNode, "useImpostors");

            bool composite = false;
            m3d::SafeBoolAttrib(composite, modelNode, "composite");

            bool passable = false;
            m3d::SafeBoolAttrib(passable, modelNode, "passable");

            int trans = 0;
            SafeIntAttrib(shadow, modelNode, "trans");

            CVector bBoxMin, bBoxMax;

            bool hasMin = m3d::SafeVectorAttrib(bBoxMin, modelNode, "bBoxMin");
            bool hasMax = m3d::SafeVectorAttrib(bBoxMax, modelNode, "bBoxMax");
            bool hasBBox = hasMin && hasMax;

            // Create dynamic model
            DynamicModel* dynamicModel = new DynamicModel();

            // Load main model
            DWORD loadStart = GetTickCount();
            m3d::AnimatedModel* mainModel = new m3d::AnimatedModel();
            mainModel->SetComposite(composite);
            mainModel->m_passable = passable;
            mainModel->SetSkinsToLoad(skinsToLoad[itemIndex]);

            bool loadFromGAM = M3D_KERNEL->GetEngineCfg().m_loadFromGAM.GetB();
            bool loadSuccess = false;

            if (loadFromGAM) {
                loadSuccess = mainModel->LoadGAM(modelFile.c_str(), true);
            }
            else {
                loadSuccess = mainModel->LoadSAM(modelFile.c_str(), true);
            }

            if (!loadSuccess) {
                delete dynamicModel;
                delete mainModel;
                continue;
            }

            dwGamTime += GetTickCount() - loadStart;

            if (hasBBox) {
                mainModel->m_box.Create(bBoxMin, bBoxMax);
            }

            dynamicModel->m_mdl[0] = mainModel;
            dynamicModel->m_numLods = 1;
            dynamicModel->m_curLod = 0;
            dynamicModel->m_useImpostors = useImpostors;

            // Load LOD models
            CStr baseFilename = modelFile;
            if (auto pos = baseFilename.rfind('.'); pos != -1)
            {
                baseFilename.del(pos, 4);
            }

            for (unsigned int lod = 1; lod < 5; lod++) {
                CStr lodFilename;
                if (loadFromGAM) {
                    lodFilename = baseFilename + "_lod" + CStr(lod) + ".gam";
                }
                else {
                    lodFilename = baseFilename + "_lod" + CStr(lod) + ".sam";
                }

                if (M3D_KERNEL->GetFileServer().FileExists(lodFilename.c_str())) {
                    m3d::AnimatedModel* lodModel = new m3d::AnimatedModel();
                    if (lodModel->Load(lodFilename.c_str(), true)) {
                        dynamicModel->m_mdl[lod] = lodModel;
                        dynamicModel->m_numLods++;
                    }
                    else {
                        delete lodModel;
                    }
                }
            }

            // Process sound effects
            ref_ptr soundNode = xmlFile->CreateNode();
            modelNode->GetFirstChild(soundNode, "sound");

            const auto actions = GetAnimActions();
            while (!soundNode->IsEmpty()) {
                CStr action;
                SafeStrAttrib(action, soundNode, "action");

                CStr soundId;
                SafeStrAttrib(soundId, soundNode, "id");

                bool looped = false;
                SafeBoolAttrib(looped, soundNode, "looped");

                // Find action index and assign sound
                for (size_t i = 0; i < AT_NUMTYPES; i++) {
                    if (actions[i].m_name == action) {
                        dynamicModel->m_soundIds[i] = soundId;
                        dynamicModel->m_soundsLooped[i] = looped;
                        m3d::Application::g_pApp->m_cachedSoundIDs.insert(soundId);
                        break;
                    }
                }

                soundNode->GetNextSibling(soundNode, "sound");
            }

            // Process action effects
            ref_ptr<m3d::cmn::XmlNode> actionNode = xmlFile->CreateNode();
            modelNode->GetFirstChild(actionNode, "action");
            while (!actionNode->IsEmpty()) {
                CStr actionName;
                SafeStrAttrib(actionName, actionNode, "name");

                int startAttackFrame = -1;
                SafeIntAttrib(startAttackFrame, actionNode, "startAttackFrame");

                int endAttackFrame = -1;
                SafeIntAttrib(endAttackFrame, actionNode, "endAttackFrame");

                int skinNum = -1;
                SafeIntAttrib(skinNum, actionNode, "skin");

                int cfgNum = -1;
                SafeIntAttrib(cfgNum, actionNode, "cfg");

                // Find action index
                size_t actionIndex = -1;
                for (size_t i = 0; i < AT_NUMTYPES; i++) {
                    if (actions[i].m_name == actionName) {
                        actionIndex = i;
                        break;
                    }
                }

                if (actionIndex != -1) {
                    // Process load points for this action
                    ref_ptr<m3d::cmn::XmlNode> lpNode = xmlFile->CreateNode();
                    actionNode->GetFirstChild(lpNode, "lp");
                    while (!lpNode->IsEmpty()) {
                        CStr lpId;
                        SafeStrAttrib(lpId, lpNode, "id");

                        CStr effectId;
                        SafeStrAttrib(effectId, lpNode, "effect_id");

                        bool restartOnAnimChange = 0;
                        SafeBoolAttrib(restartOnAnimChange, lpNode, "restartOnAnimationChange");

                        bool immediateRemove = 0;
                        SafeBoolAttrib(immediateRemove, lpNode, "ImmediateRemove");

                        DynamicModel::auxEffectDesc effectDesc;
                        effectDesc.m_lpId = mainModel->GetLoadPointIdByName(lpId.c_str());
                        effectDesc.m_effectName = effectId;
                        effectDesc.m_restartOnAnimChange = restartOnAnimChange;
                        effectDesc.m_immediateRemove = immediateRemove;

                        dynamicModel->m_effects[actionIndex].lpEffects.push_back(effectDesc);

                        lpNode->GetNextSibling(lpNode, "lp");
                    }

                    dynamicModel->m_effects[actionIndex].startAttackFrame = startAttackFrame;
                    dynamicModel->m_effects[actionIndex].endAttackFrame = endAttackFrame;
                    dynamicModel->m_effects[actionIndex].skinNum = skinNum;
                    dynamicModel->m_effects[actionIndex].cfgNum = cfgNum;
                }

                actionNode->GetNextSibling(actionNode, "action");
            }

            // Create impostors if needed
            if (useImpostors) {
                dynamicModel->_createImpostorShit();
            }

            // Add to server models
            m3d::DataServer::Model newModel((void*)dynamicModel, modelFile.c_str(), itemslist[itemIndex].m_filename.c_str(), modelId.c_str());
            m_models.push_back(newModel);

            // Update item properties
            size_t modelIndex = m_models.size() - 1;
            SetItemProperty(modelIndex, 0, &shadow);
            SetItemProperty(modelIndex, 1, &windwavy);
            SetItemProperty(modelIndex, 4, &tessellate);
            SetItemProperty(modelIndex, 5, &trackland);
            SetItemProperty(modelIndex, 6, &shadowVolume);
            SetItemProperty(modelIndex, 2, &trans);

            itemslist[itemIndex].m_fileWasRead = true;

            modelNode->GetNextSibling(modelNode, "model");
        }

        // Log unread files
        for (const auto& item : itemslist) {
            if (!item.m_fileWasRead) {
                M3D_LOG_ERR("ServerAnimatedModels: cannot read file: " + item.m_filename + " id = " + item.m_id);
            }
        }

        // Log loading statistics
        DWORD totalTime = GetTickCount() - dwStartTime;
        M3D_LOG_INFO("### Total time: " + CStr(totalTime) + ", GAM time: " + CStr(dwGamTime));
    }

    int AnimatedModelsServer::RenderMesh(SgAnimatedModelNode*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(AnimInfo*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderModelForImpostor(AnimatedModel*, float, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UpdateGlobalRenderingParams()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

ModelEffectList::ModelEffectList(DynamicModel* meta) :
    m_dynModel(meta)
{
}

DynamicModel::DynamicModel()
{
    memset(this->m_soundsLooped, 0, sizeof(this->m_soundsLooped));
    this->m_mdl[0] = 0;
    this->m_mdl[1] = 0;
    this->m_mdl[2] = 0;
    this->m_mdl[3] = 0;
    this->m_mdl[4] = 0;
    this->m_numLods = 0;
    this->m_curLod = 0;
    this->m_useImpostors = 0;
}

DynamicModel::~DynamicModel()
{
    // TODO: check this
    if (this->m_useImpostors)
        _releaseImpostorShit();
    for (auto& mdl : m_mdl)
    {
        delete mdl;
    }
}

void DynamicModel::_createImpostorShit()
{
    auto v2 = this->m_mdl[0];
    auto v4 = v2->m_box.m_box[3] - v2->m_box.m_box[0];
    auto v5 = v2->m_box.m_box[5] - v2->m_box.m_box[2];
    auto sy = v2->m_box.m_box[4] - v2->m_box.m_box[1];
    this->m_impostorDisplacement = v2->m_box.m_box[1];
    auto boxSizeMaxXZ2 = sqrt(v5 * v5 + v4 * v4) * 0.5;

    this->m_impostorVb = M3D_RENDERER->AddVb(m3d::rend::VertexType::VERTEX_IMPOSTORTEST, 240, "Impostors", 0);
    int* v6 = (int*)M3D_RENDERER->LockVb(m_impostorVb, 0, 0, 0);
    
    // TODO: check and refactor this
    auto v7 = 0.0 - boxSizeMaxXZ2;
    auto v8 = 0;
    auto v9 = 60;
    do
    {
        *(float*)v6 = v7;
        v6[1] = 0;
        v6[3] = 0;
        v6[4] = 1.0;
        auto v10 = (char*)(v6 + 5);
        *((float*)v10 - 3) = (float)v8;
        *((float*)v10 + 2) = (float)v8;
        *(float*)v10 = v7;
        *((float*)v10 + 1) = sy;
        *((int*)v10 + 3) = 0;
        *((int*)v10 + 4) = 0;
        v10 += 20;
        *((float*)v10 + 2) = (float)v8;
        *(float*)v10 = boxSizeMaxXZ2;
        *((int*)v10 + 1) = 0;
        *((float*)v10 + 3) = 1.0;
        *((float*)v10 + 4) = 1.0;
        v10 += 20;
        *((float*)v10 + 2) = (float)v8;
        *(float*)v10 = boxSizeMaxXZ2;
        *((float*)v10 + 1) = sy;
        *((float*)v10 + 3) = 1.0;
        *((int*)v10 + 4) = 0;
        v6 = (int*)(v10 + 20);
        ++v8;
        --v9;
    } while (v9);

    M3D_RENDERER->UnlockVb(m_impostorVb);

    this->m_impostorIb = M3D_RENDERER->AddIb(360, 0);
    char* v11 = (char*)M3D_RENDERER->LockIb(m_impostorIb, 0, 0, 0);

    auto v12 = 1;
    do
    {
        auto v13 = v11 + 2;
        *(v13 - 1) = v12 - 1;
        *v13++ = v12;
        *v13++ = v12 + 1;
        *v13++ = v12 + 1;
        *v13++ = v12;
        *v13 = v12 + 2;
        v12 += 4;
        v11 = (char*)(v13 + 1);
    } while ((unsigned __int16)v12 < 241u);

    M3D_RENDERER->UnlockIb(m_impostorIb);


    M3D_RENDERER->AddDynamicTexture((CStr("$ImpostorTex.") + m_mdl[0]->GetName()).c_str(), 256, 256, 1);
    M3D_RENDERER->SetTextureParameter(m_impostorTex, m3d::rend::TexParam::TM_WRAP_S, 3);
    M3D_RENDERER->SetTextureParameter(m_impostorTex, m3d::rend::TexParam::TM_WRAP_T, 3);
    M3D_RENDERER->SetTextureParameter(m_impostorTex, m3d::rend::TexParam::TM_TEX_FILTER, 1);

}

void DynamicModel::_releaseImpostorShit()
{
    throw retruxx::logic_error("Not implemented");
}

DynamicModel::auxActionEffectsDesc::auxActionEffectsDesc()
{
    this->endAttackFrame = -1;
    this->startAttackFrame = -1;
    this->cfgNum = -1;
    this->skinNum = -1;
}
