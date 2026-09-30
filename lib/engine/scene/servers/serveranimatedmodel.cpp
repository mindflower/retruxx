#include <algorithm>
#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <scene/servers/serveranimatedmodel.h>
#include <core/ini.h>
#include <client.h>
#include <world.h>
#include <core/log.h>
#include <file/fileserver.h>
#include <core/scoped_ptr.h>
#include <file/filestream.h>

#include "core/timer.h"
#include "scene/nodes/sgnodeanimatedmodel.h"
#include "engine/scene/shadows/shadowmanager.h"

namespace m3d
{
    bool AnimatedModelsServer::SortModelStatPred::operator()(ModelStat const& ms1, ModelStat const& ms2) const
    {
        // RVA 0x76D130 - most used models first.
        return ms1.instanceCount > ms2.instanceCount;
    }

    namespace
    {
        // The report pads each column out to a fixed width.
        CStr pad(int width)
        {
            return width > 0 ? CStr(' ', width) : CStr();
        }
    }  // namespace

    void AnimatedModelsServer::PostLoad()
    {
        for (auto const& model : m_models)
        {
            auto* dynamicModel = (DynamicModel*)model.m_ptr;
            for (auto& actionEffect : dynamicModel->m_effects)
            {
                std::vector<DynamicModel::auxEffectDesc> newEffectList;
                for (auto const& effect : actionEffect.lpEffects)
                {
                    DynamicModel::auxEffectDesc desc = effect;
                    auto id = M3D_KERNEL->GetEngineCfg().GetModelIdByName(effect.m_effectName);
                    if (id != -1)
                    {
                        desc.m_effectId = id;
                        newEffectList.push_back(std::move(desc));
                    }
                }
                actionEffect.lpEffects = newEffectList;
            }
        }
    }

    int AnimatedModelsServer::AddItem(char const* params, char const* id)
    {
        // RVA 0x772CD0 - loads the model named id out of the models XML the "file:" parameter points at: its model file,
        // its per-action sounds, and the effects hung on its load points.
        int const existing = GetItemByName(id, false);
        if (existing != -1)
        {
            return existing;
        }

        DataServer::Proto proto = PROTO_NONE;
        int paramsPos = 0;
        ParseProto(params, &proto, &paramsPos);
        if (proto != PROTO_FILE)
        {
            // NOTE: the message is indexed by the protocol, so anything but PROTO_NONE prints it with the first
            // characters cut off.
            M3D_LOG_INFO(CStr("protocol is not supported " + static_cast<int>(proto)));
            return -1;
        }

        char const* const fileName = params + paramsPos;
        int shadowed = 0;
        int winded = 0;
        int tessellate = 0;
        int trackland = 0;
        CVector bBoxMin = ZeroVector;
        CVector bBoxMax = ZeroVector;
        bool composite = false;
        bool passable = false;

        CStr err;
        ref_ptr xmlFile = ReadXmlFile(fileName, &err);
        if (!xmlFile)
        {
            M3D_LOG_ERR("ServerAnimatedModel: " + err);
            return -1;
        }

        ref_ptr modelNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild(modelNode, "AnimatedModels");
        if (modelNode->IsEmpty())
        {
            return -1;
        }
        for (modelNode->GetFirstChild(modelNode, "model"); !modelNode->IsEmpty();
             modelNode->GetNextSibling(modelNode, "model"))
        {
            if (!strcmp(modelNode->GetAttribute("id"), id))
            {
                break;
            }
        }
        if (modelNode->IsEmpty())
        {
            return -1;
        }

        CStr modelFileName = modelNode->GetAttribute("file");
        SafeIntAttrib(shadowed, modelNode, "shadow");
        SafeIntAttrib(winded, modelNode, "windwavy");
        SafeIntAttrib(tessellate, modelNode, "tessellate");
        SafeIntAttrib(trackland, modelNode, "trackland");
        SafeBoolAttrib(composite, modelNode, "composite");
        SafeBoolAttrib(passable, modelNode, "passable");
        bool const hasBBox =
            SafeVectorAttrib(bBoxMin, modelNode, "bBoxMin") && SafeVectorAttrib(bBoxMax, modelNode, "bBoxMax");

        auto* const model = new DynamicModel;

        // Per-action sounds.
        ref_ptr soundNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        for (modelNode->GetFirstChild(soundNode, "sound"); !soundNode->IsEmpty();
             soundNode->GetNextSibling(soundNode, "sound"))
        {
            char const* const action = soundNode->GetAttribute("action");
            char const* const soundId = soundNode->GetAttribute("id");
            char const* const loopedStr = soundNode->GetAttribute("looped");
            int const looped = loopedStr ? atoi(loopedStr) : 0;

            AnimAction const* animAction = GetAnimActions();
            for (int actionNum = 0; animAction->m_name; ++animAction, ++actionNum)
            {
                if (!strcmp(animAction->m_name, action))
                {
                    model->m_soundIds[actionNum] = soundId;
                    model->m_soundsLooped[actionNum] = looped;
                    Application::g_pApp->m_cachedSoundIDs.insert(model->m_soundIds[actionNum]);
                    break;
                }
            }
        }

        model->m_mdl[0] = new AnimatedModel;
        model->m_mdl[0]->m_composite = composite;
        model->m_mdl[0]->m_passable = passable;
        if (!model->m_mdl[0]->Load(modelFileName, true))
        {
            delete model;
            return -1;
        }
        if (hasBBox)
        {
            Aabb& box = model->m_mdl[0]->m_box;
            box.m_box[0] = bBoxMin.x;
            box.m_box[1] = bBoxMin.y;
            box.m_box[2] = bBoxMin.z;
            box.m_box[3] = bBoxMax.x;
            box.m_box[4] = bBoxMax.y;
            box.m_box[5] = bBoxMax.z;
        }

        // Per-action attack frames, skin and cfg, plus the effects hung on the model's load points.
        ref_ptr lpNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        for (modelNode->GetFirstChild(soundNode, "action"); !soundNode->IsEmpty();
             soundNode->GetNextSibling(soundNode, "action"))
        {
            char const* const actionName = soundNode->GetAttribute("name");
            int startAttackFrame = -1;
            SafeIntAttrib(startAttackFrame, soundNode, "startAttackFrame");
            int endAttackFrame = -1;
            SafeIntAttrib(endAttackFrame, soundNode, "endAttackFrame");
            int skinNum = -1;
            SafeIntAttrib(skinNum, soundNode, "skin");
            int cfgNum = -1;
            SafeIntAttrib(cfgNum, soundNode, "cfg");

            int actionNum = 0;
            AnimAction const* animAction = GetAnimActions();
            for (; animAction->m_name; ++animAction, ++actionNum)
            {
                if (!strcmp(animAction->m_name, actionName))
                {
                    break;
                }
            }
            if (!animAction->m_name)
            {
                continue;
            }

            for (soundNode->GetFirstChild(lpNode, "lp"); !lpNode->IsEmpty(); lpNode->GetNextSibling(lpNode, "lp"))
            {
                char const* const lpName = lpNode->GetAttribute("id");
                char const* const effectName = lpNode->GetAttribute("effect_id");
                int restartOnAnimChange = 0;
                SafeIntAttrib(restartOnAnimChange, lpNode, "restartOnAnimationChange");
                int immediateRemove = 1;
                SafeIntAttrib(immediateRemove, lpNode, "ImmediateRemove");

                // NOTE: m_effectId is left as it is; PostLoad fills it in from the effect's name.
                DynamicModel::auxEffectDesc desc;
                desc.m_lpId = model->m_mdl[0]->GetLoadPointIdByName(lpName);
                desc.m_lpName = lpName;
                desc.m_effectName = effectName;
                desc.m_restartOnAnimChange = restartOnAnimChange == 1;
                desc.m_immediateRemove = immediateRemove == 1;
                model->m_effects[actionNum].lpEffects.push_back(desc);
            }

            model->m_effects[actionNum].startAttackFrame = startAttackFrame;
            model->m_effects[actionNum].endAttackFrame = endAttackFrame;
            model->m_effects[actionNum].skinNum = skinNum;
            model->m_effects[actionNum].cfgNum = cfgNum;
        }

        m_models.push_back(Model(model, modelFileName.c_str(), fileName, id));
        int const handle = m_models.size() - 1;
        SetItemProperty(handle, PROP_MODEL_CAST_SHADOW, &shadowed);
        SetItemProperty(handle, PROP_MODEL_WIND_WAVY, &winded);
        SetItemProperty(handle, PROP_MODEL_TESSELLATE, &tessellate);
        SetItemProperty(handle, PROP_MODEL_TRACK_LAND, &trackland);
        return handle;
    }

    int AnimatedModelsServer::RemoveItem(int)
    {
        // RVA 0x76D320 - models are never removed one by one; Release frees them all.
        return 1;
    }

    bool AnimatedModelsServer::IsBonePresentsInModel(char const* modelname, char const* bonename)
    {
        // RVA 0x76FF30
        int const item = GetItemByName(modelname, true);
        if (item == -1)
        {
            return false;
        }
        auto* const dynamicModel = (DynamicModel*)m_models[item].m_ptr;
        return dynamicModel->m_mdl[0]->GetLoadPointIdByName(bonename) >= 0;
    }

    void AnimatedModelsServer::RenderItem(int, void*)
    {
    }

    void AnimatedModelsServer::UnregisterNode(SgNode* node)
    {
        m3d::AnimInfo* anim = nullptr;
        node->GetProperty(1, &anim);
        if (!anim->m_Empty)
        {
            node->GetGraph()->UnlinkThinkNode(node);
        }
        delete anim;
        anim = nullptr;

        ModelEffectList* list = nullptr;
        node->GetProperty(2, &list);
        delete list;
        list = nullptr;

        node->SetProperty(1, &anim);
        node->SetProperty(2, &list);
    }

    bool AnimatedModelsServer::ReportServerInfo(char const* filename)
    {
        // RVA 0x773B40 - writes a table of every loaded model with how many nodes in the scene graph use it, sorted by
        // that count.
        retruxx::vector<ModelStat> modelStats(m_models.size());
        for (unsigned i = 0; i < modelStats.size(); ++i)
        {
            modelStats[i].handle = i;
            modelStats[i].instanceCount = 0;
        }

        retruxx::vector<Object*> stack;
        stack.push_back(pClient->GetWorld().GetGraph().GetRootNode());
        while (!stack.empty())
        {
            Object* const obj = stack.back();
            stack.pop_back();
            for (Object* child = obj->GetFirstChild(); child; child = child->GetNextSibling())
            {
                auto* const node = static_cast<SgNode*>(child);
                if (node->GetServer() == this)
                {
                    ++modelStats[node->GetServerHandle()].instanceCount;
                }
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }
        std::sort(modelStats.begin(), modelStats.end(), SortModelStatPred());

        scoped_ptr stream = g_Kernel->GetFileServer().CreateFileStream();
        if (!stream->Open(filename, fs::IStream::OPEN_WRITE))
        {
            return false;
        }

        *stream << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n";
        *stream << "-==\n";
        *stream << "-== Log category  : Loaded models info\n";
        *stream << "-== Build         : " << "retruxx - release version build v0.01" << "\n";
        *stream << "-==\n";
        *stream << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n\n";
        *stream << "** General stats **\n";

        CStr const levelName = NameFromFileName(g_Kernel->GetEngineCfg().m_levFileName.GetS());
        *stream << "Level name: " << levelName.substr(0, levelName.rfind('.')).c_str() << "\n";
        *stream << "Total models count: " << static_cast<int>(m_models.size()) << "\n\n";
        *stream << "** Models **\n";
        *stream << "   Id                          InstanceCount   MeshCount   PolyCount   FileName\n";
        *stream
            << "--------------------------------------------------------------------------------------------------\n";

        unsigned totalInstanceCount = 0;
        unsigned totalMeshCount = 0;
        unsigned totalMeshCountPerInstance = 0;
        for (unsigned i = 0; i < modelStats.size(); ++i)
        {
            AnimatedModel* mdl = nullptr;
            GetItemProperty(modelStats[i].handle, PROP_INTERNAL_GETMODEL, &mdl);
            CStr outStr = "   ";
            if (!mdl)
            {
                continue;
            }

            unsigned facesCount = 0;
            for (unsigned mesh = 0; mesh < mdl->m_numMeshes; ++mesh)
            {
                facesCount += mdl->m_meshes[mesh].m_numFaces;
            }

            outStr += m_models[modelStats[i].handle].m_name;
            outStr += pad(31 - outStr.length());
            outStr += CStr(modelStats[i].instanceCount);
            outStr += pad(47 - outStr.length());
            outStr += CStr(mdl->m_numMeshes);
            outStr += pad(59 - outStr.length());
            outStr += CStr(facesCount);
            outStr += pad(72 - outStr.length());
            outStr += m_models[modelStats[i].handle].m_fileName;
            outStr += CStr("\n");
            *stream << outStr.c_str();

            totalMeshCount += mdl->m_numMeshes;
            totalInstanceCount += modelStats[i].instanceCount;
            totalMeshCountPerInstance += modelStats[i].instanceCount * mdl->m_numMeshes;
        }

        *stream
            << "--------------------------------------------------------------------------------------------------\n\n";
        *stream << "Total instance count: " << totalInstanceCount << "\n";
        *stream << "Total mesh count (for 1 instance of every model): " << totalMeshCount << "\n";
        *stream << "Total mesh count (for each instance of every model): " << totalMeshCountPerInstance << "\n";
        stream->Close();
        return true;
    }

    int AnimatedModelsServer::Init()
    {
        auto const logoFileName = g_Kernel->GetEngineCfg().m_loadFromGAM.GetB() ? "data\\models\\Logos\\Logos.gam" :
                                                                                  "data\\models\\Logos\\Logos.sam";
        m_MeshMaterialManager.Init(logoFileName, g_Kernel->GetEngineCfg().m_pathToBelongsToLogos.GetS());

        m_impostorVs = Application::g_pApp->m_renderer->NewHlslShader(
            "data/shaders/impostorTest_vs11.vs", "ImpostorVS", rend::IHlslShader::VS_1_1);
        if (!m_impostorVs)
        {
            return 0;
        }

        m_impostorPs = Application::g_pApp->m_renderer->NewHlslShader(
            "data/shaders/impostorTest_ps11.ps", "ImpostorPS", rend::IHlslShader::PS_1_1);
        if (!m_impostorPs)
        {
            return 0;
        }

        m_valid = true;
        return 1;
    }

    CVector AnimatedModelsServer::GetBoundSizes(char const* modelName)
    {
        auto item = GetItemByName(modelName, true);
        if (item == -1)
        {
            return CVector(0.0, 0.0, 0.0);
        }
        auto* dynamicModel = (DynamicModel*)m_models[item].m_ptr;
        auto* animatedModel = dynamicModel->m_mdl[0];

        CVector result;
        result.y = animatedModel->m_box.m_box[4] - animatedModel->m_box.m_box[1];
        result.z = animatedModel->m_box.m_box[5] - animatedModel->m_box.m_box[2];
        result.x = animatedModel->m_box.m_box[3] - animatedModel->m_box.m_box[0];
        return result;
    }

    int AnimatedModelsServer::GetBoneMatrixByNameFromModelName(
        char const* modelname,
        CStr const& boneName,
        CMatrix& res,
        bool theLastOneOnly)
    {
        auto itemByName = m3d::DataServer::GetItemByName(modelname, 1);
        if (itemByName != -1)
        {
            auto* dynamicModel = (DynamicModel*)m_models[itemByName].m_ptr;
            auto* animatedModel = dynamicModel->m_mdl[0];
            return animatedModel->GetBoneMatrixByName(boneName, res, theLastOneOnly);
        }
        return 0;
    }

    int AnimatedModelsServer::SaveAllLoadedEntities(char const*)
    {
        // RVA 0x775250 - models are only ever loaded, never written back.
        return 1;
    }

    int AnimatedModelsServer::Release()
    {
        // RVA 0x774680
        m_valid = false;
        for (auto& model : m_models)
        {
            delete (DynamicModel*)model.m_ptr;
            model.m_ptr = nullptr;
        }
        m_MeshMaterialManager.Release();
        m_models.clear();
        if (m_impostorVs)
        {
            m_impostorVs->Release();
            m_impostorVs = nullptr;
        }
        if (m_impostorPs)
        {
            m_impostorPs->Release();
            m_impostorPs = nullptr;
        }
        return 1;
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

        switch (prop)
        {
        case 8704:
        {
            this->SetItemProperty(id, 8708, src);
            this->SetItemProperty(id, 8709, src);
            return 1;
        }
        case 8708:
        {
            // RVA 0x774BC0 - the effects of one action, then the skin and configuration it asks for.
            auto const* const ri = static_cast<PropSrvNodeAction const*>(src);
            SgNode* const node = ri->m_node;
            ModelEffectList* list = nullptr;
            node->GetProperty(2, &list);
            list->adjustModelEffects(node, ri->m_action);

            auto* dynamicModel = (DynamicModel*)m_models[id].m_ptr;
            auto* animatedModel = dynamicModel->m_mdl[0];

            for (int i = 0; i < list->m_curEffectList.size(); ++i)
            {
                auto mat = animatedModel->GetBoneMatrix(list->m_curEffectList[i].m_desc->m_lpId);
                CVector pos;
                pos.x = (float)((float)((float)(mat._11 + mat._21) + mat._31) * 0.0) + mat._41;
                pos.y = (float)((float)((float)(mat._12 + mat._22) + mat._32) * 0.0) + mat._42;
                pos.z = (float)((float)((float)(mat._13 + mat._23) + mat._33) * 0.0) + mat._43;
                list->m_curEffectList[i].m_effectNode->SetOriginAbs(pos);

                Quaternion quat;
                quat.FromMatrix(mat);
                list->m_curEffectList[i].m_effectNode->SetRotation(quat);
            }

            auto skinNum = dynamicModel->m_effects[ri->m_action].skinNum;
            auto cfgNum = dynamicModel->m_effects[ri->m_action].cfgNum;
            if (skinNum >= 0)
            {
                node->SetProperty(8706u, &skinNum);
            }
            if (cfgNum >= 0)
            {
                node->SetProperty(8707u, &cfgNum);
            }

            return 1;
        }
        case 8709:
        {
            // RVA 0x774BC0 - the model's own animation.
            auto const* const ri = static_cast<PropSrvNodeAction const*>(src);
            AnimInfo* anim = nullptr;
            ri->m_node->GetProperty(1, &anim);
            anim->SetAnimation(ri->m_action);
            return 1;
        }
        case 8710:
        {
            // RVA 0x774BC0 - the effects of every action in the list, then each action's skin and configuration in
            // turn, so the last one that sets them wins.
            auto const* const ri = static_cast<PropSrvNodeActions const*>(src);
            SgNode* const node = ri->m_node;
            ModelEffectList* list = nullptr;
            node->GetProperty(2, &list);
            list->adjustModelEffects(node, *ri->m_Actions);

            auto* dynamicModel = (DynamicModel*)m_models[id].m_ptr;
            auto* animatedModel = dynamicModel->m_mdl[0];

            for (int i = 0; i < list->m_curEffectList.size(); ++i)
            {
                auto mat = animatedModel->GetBoneMatrix(list->m_curEffectList[i].m_desc->m_lpId);
                // Here the origin is ZeroVector put through the full matrix rather than the folded form above.
                CVector pos;
                pos.x = mat._11 * ZeroVector.x + mat._31 * ZeroVector.z + mat._21 * ZeroVector.y + mat._41;
                pos.y = mat._32 * ZeroVector.z + mat._22 * ZeroVector.y + mat._12 * ZeroVector.x + mat._42;
                pos.z = mat._33 * ZeroVector.z + mat._23 * ZeroVector.y + mat._13 * ZeroVector.x + mat._43;
                list->m_curEffectList[i].m_effectNode->SetOriginAbs(pos);

                Quaternion quat;
                quat.FromMatrix(mat);
                list->m_curEffectList[i].m_effectNode->SetRotation(quat);
            }

            for (size_t i = 0; i < ri->m_Actions->size(); ++i)
            {
                ActionType const action = (*ri->m_Actions)[i];
                auto skinNum = dynamicModel->m_effects[action].skinNum;
                auto cfgNum = dynamicModel->m_effects[action].cfgNum;
                if (skinNum >= 0)
                {
                    node->SetProperty(8706u, &skinNum);
                }
                if (cfgNum >= 0)
                {
                    node->SetProperty(8707u, &cfgNum);
                }
            }
            return 1;
        }
        }
        // RVA 0x774BC0 - properties this server does not know about.
        return 0;
    }

    namespace
    {
        struct ImpostoredMeshInfo
        {
            /* 0x0000 */ m3d::SgAnimatedModelNode* nodeLookup;
            /* 0x0004 */ m3d::AnimatedModel* modelLookup;
            /* 0x0008 */ DynamicModel* dmLookup;
        }; /* size: 0x000c */

        struct MeshImposteredSortPred
        {
            MeshImposteredSortPred(ImpostoredMeshInfo const* meshes) : m_meshes(meshes)
            {
            }

            // Grouping the draw calls only needs entries that share a model to
            // end up adjacent, and every node with the same server handle
            // resolves to the same DynamicModel.
            bool operator()(unsigned int meshIdx1, unsigned int meshIdx2) const
            {
                return m_meshes[meshIdx1].nodeLookup->GetServerHandle() <
                    m_meshes[meshIdx2].nodeLookup->GetServerHandle();
            }
            /* 0x0000 */ ImpostoredMeshInfo const* m_meshes;
        }; /* size: 0x0004 */

        struct MeshInfo
        {
            /* 0x0000 */ m3d::AnimatedModel::Mesh* mesh;
            /* 0x0004 */ m3d::DSurfaceMaterial* material;
            /* 0x0008 */ m3d::SgAnimatedModelNode* nodeLookup;
            /* 0x000c */ m3d::AnimatedModel* modelLookup;
        }; /* size: 0x0010 */

        struct MeshSortPred
        {
            MeshSortPred(MeshInfo const* meshes) : m_meshes(meshes)
            {
            }

            bool operator()(unsigned int meshIdx1, unsigned int meshIdx2) const
            {
                // RVA 0x8F1F70 - by effect, then by where the material's texture list lives.
                // NOTE: the tie-break compares the addresses of the first texture handles, not the handles, as
                // shipped; comparing the list pointers orders the same and allows an empty list.
                auto const* material1 = m_meshes[meshIdx1].material;
                auto const* material2 = m_meshes[meshIdx2].material;
                if (material1->Shader.Handle != material2->Shader.Handle)
                {
                    return material1->Shader.Handle < material2->Shader.Handle;
                }
                return material1->Textures.data() < material2->Textures.data();
            }
            /* 0x0000 */ MeshInfo const* m_meshes;
        }; /* size: 0x0004 */

        // Orders nodes by squared distance from the camera, nearest first.
        struct SortByDist
        {
            explicit SortByDist(CVector const& org) : m_org(org)
            {
            }

            bool operator()(m3d::SgNode* a, m3d::SgNode* b) const
            {
                CVector const da = a->GetOriginWorldAbsForSphere() - m_org;
                CVector const db = b->GetOriginWorldAbsForSphere() - m_org;
                return (db.x * db.x + db.y * db.y + db.z * db.z) > (da.x * da.x + da.y * da.y + da.z * da.z);
            }

            /* 0x0000 */ CVector m_org;
        }; /* size: 0x000c */

        // A material contributes an alpha-tested diffuse texture when it has any
        // texture at all and either carries no shader or its first technique is
        // flagged as alpha-using.
        bool HasAlphaTestedTexture(m3d::DSurfaceMaterial const& material)
        {
            return !material.Textures.empty() &&
                (!material.Shader.Handle || material.Shader.Handle->GetTechniqueDesc(0).useAlpha);
        }

        void BindAlphaTestedTexture(m3d::DSurfaceMaterial const& material, int stage, int alphaTest)
        {
            if (HasAlphaTestedTexture(material))
            {
                M3D_RENDERER->SetAlphaTest(alphaTest);
                M3D_RENDERER->SetTexture(stage, material.Textures.front().Handle, -1.0);
            }
            else
            {
                M3D_RENDERER->SetAlphaTest(0);
                M3D_RENDERER->SetWhiteTexture(stage);
            }
        }
    }  // namespace

    int AnimatedModelsServer::RenderNodeSet(SgNode** nodes, unsigned numNodes, RenderNodeInfo rni)
    {
        // RVA 0x8F6210 - draws the nodes' meshes for one render pass, sorted by material. In the simple (colour)
        // pass, a node beyond the impostor distance (or past 150 when culling is inverted) that allows impostors
        // is instead drawn afterwards as a billboard, batched by model; the rest pick a LOD by distance.
        m_profiler->StartCountdown();

        int numMeshes = 0;
        int numMeshesImpostered = 0;

        // Generate impostors if needed
        if (!m3d::pClient->GetWorld().m_isWeatherActual && rni.isPrimaryRender)
        {
            GenerateImpostorsIfNeeded();
        }

        // Setup render state for simple rendering
        if (rni.rnt == RNT_SIMPLE)
        {
            M3D_RENDERER->SetAlphaTest(M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
            M3D_RENDERER->SetBlend(rend::BM_ALPHA, 0);

            // Set culling based on parameter
            if (rni.isCullInverted)
                M3D_RENDERER->SetCull(rend::M3DCULL_CW, 0);
            else
                M3D_RENDERER->SetCull(rend::M3DCULL_CCW, 0);

            M3D_RENDERER->SetZbState(rend::ZB_ENABLE, 0);
            M3D_RENDERER->SetFog(1, 0);
            M3D_RENDERER->SetBlend(rend::BM_NONE, 0);

            // Disable texture stages
            for (int i = 0; i < 8; i++)
            {
                M3D_RENDERER->TgDisable(i);
            }

            // Wireframe mode if enabled
            if (M3D_ENGINE_CFG.m_lsWireframe.GetB())
            {
                M3D_RENDERER->SetFillMode(rend::M3DFILL_WIREFRAME, 0);
            }

            UpdateGlobalRenderingParams();
        }

        // Get view position for distance calculations
        CVector viewPos = M3D_RENDERER->GetViewOrigin();
        float distSq = 0.0f;

        // Get impostor threshold
        float impostorThreshold = M3D_ENGINE_CFG.m_g_impostorThreshold.GetF();
        float impostorDistanceSquared = impostorThreshold * impostorThreshold;

        // Arrays for sorting meshes
        unsigned int meshesShifts[5000] = {0};
        unsigned int meshesShiftsImpostered[5000] = {0};
        MeshInfo meshes[5000];
        ImpostoredMeshInfo meshesImpostered[5000];

        // Process each node
        for (unsigned int nodeIndex = 0; nodeIndex < numNodes; nodeIndex++)
        {
            m3d::SgNode* currentNode = nodes[nodeIndex];
            DynamicModel* modelData = (DynamicModel*)this->m_models[currentNode->GetServerHandle()].m_ptr;

            CVector const& org = currentNode->GetOriginWorldAbs();
            distSq = (org.x - viewPos.x) * (org.x - viewPos.x) + (org.z - viewPos.z) * (org.z - viewPos.z) +
                (org.y - viewPos.y) * (org.y - viewPos.y);

            // Determine LOD level based on distance
            unsigned int lodLevel = false;
            if (rni.rnt == RNT_SIMPLE)
            {
                bool useImpostor = 0;
                currentNode->GetProperty(8720u, &useImpostor);

                // Check if should use impostor
                if (distSq > impostorDistanceSquared || (rni.isCullInverted && distSq > 22500.0f))
                {
                    if (useImpostor && rni.isUseImpostors)
                    {
                        // Add to impostor list
                        meshesShiftsImpostered[numMeshesImpostered] = numMeshesImpostered;
                        meshesImpostered[numMeshesImpostered].nodeLookup = (m3d::SgAnimatedModelNode*)currentNode;
                        meshesImpostered[numMeshesImpostered].dmLookup = modelData;
                        meshesImpostered[numMeshesImpostered].modelLookup = modelData->m_mdl[0];
                        numMeshesImpostered++;
                        continue;
                    }
                }

                // Determine LOD level
                if (distSq <= 90000.0f)
                {
                    if (distSq <= 40000.0f)
                    {
                        if (distSq > 10000.0f)
                            lodLevel = 1;
                    }
                    else
                    {
                        lodLevel = 2;
                    }
                }
                else
                {
                    lodLevel = 3;
                }
            }
            unsigned int const maxLod = modelData->m_numLods;
            if (lodLevel >= maxLod)
            {
                lodLevel = maxLod - 1;
            }

            // Get model and instances
            m3d::AnimatedModel* model = modelData->m_mdl[lodLevel];
            m3d::Configuration* configuration = 0;
            currentNode->GetProperty(8707u, &configuration);

            // Process each instance
            for (auto* mesh : configuration->m_meshes)
            {
                // Get material and mesh for this instance
                auto& material = this->m_MeshMaterialManager.GetMaterial(*currentNode, *mesh);

                // Add to mesh list for sorting
                meshesShifts[numMeshes] = numMeshes;
                meshes[numMeshes].nodeLookup = (m3d::SgAnimatedModelNode*)currentNode;
                meshes[numMeshes].modelLookup = model;
                meshes[numMeshes].material = &material;
                meshes[numMeshes].mesh = mesh;

                numMeshes++;
                assert(numMeshes <= 5000);
            }
        }

        // Sort meshes for optimal rendering
        if (numMeshes > 0)
        {
            std::stable_sort(meshesShifts, meshesShifts + (int)numMeshes, MeshSortPred(meshes));
        }

        if (rni.rnt == RNT_SIMPLE)
        {
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("meshes = " + CStr(numMeshes)).c_str());
        }

        SceneGraph* graph = nodes[0]->GetGraph();
        rend::IEffect* const contourShader = graph->GetContourShader();

        for (int i = 0; i < numMeshes; i++)
        {
            auto& mi = meshes[meshesShifts[i]];
            SgAnimatedModelNode* node = mi.nodeLookup;
            AnimatedModel::Mesh& mh = *mi.mesh;
            DSurfaceMaterial& material = *mi.material;
            CMatrix const& xform = node->GetCurrentMatrix();
            rend::IEffect* shader = nullptr;

            switch (rni.rnt)
            {
            case RNT_SIMPLE:
                graph->LightSetupLightsForNode(node);
                shader = mi.modelLookup->ApplyMaterial(material);
                break;

            case RNT_FOR_SHADOW:
                BindAlphaTestedTexture(material, 0, M3D_ENGINE_CFG.m_g_shadowAlphaTest.GetI());
                break;

            case RNT_FOR_PROJECTOR:
            {
                shader = graph->GetObjProjectorShader(material.Shader.Handle);
                CMatrix inv = xform.getInverse();
                CMatrix proj = xform * rni.projTansform;
                if (mh.m_meshType == 1)
                {
                    AnimInfo* anim = nullptr;
                    node->GetProperty(1, &anim);
                    CMatrix const& bone = anim->m_bonesAnim[mh.m_numNode].m_curMatrix;
                    inv = inv * bone.getInverse();
                    proj = bone * proj;
                }
                shader->SetMatrix(rend::IEffect::User_float4x4_param, proj);
                shader->SetVector3(rend::IEffect::User_float4_param, inv.vecMul(rni.projOrg));
                shader->SetVector3(rend::IEffect::User_float3_param, inv.vecRot(rni.projDir));
                BindAlphaTestedTexture(material, 1, M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
                break;
            }

            case RNT_FOR_POINTLIGHT:
            {
                shader = graph->GetObjectLightShader(material.Shader.Handle);
                CMatrix inv = xform.getInverse();
                if (mh.m_meshType == 1)
                {
                    AnimInfo* anim = nullptr;
                    node->GetProperty(1, &anim);
                    inv = inv * anim->m_bonesAnim[mh.m_numNode].m_curMatrix.getInverse();
                }
                shader->SetVector3(rend::IEffect::User_float4_param, inv.vecMul(rni.projOrg));
                BindAlphaTestedTexture(material, 0, M3D_ENGINE_CFG.m_alphaTestWorld.GetI());
                break;
            }

            case RNT_FOR_CONTOUR:
                if (HasAlphaTestedTexture(material))
                {
                    M3D_RENDERER->SetTexture(0, material.Textures.front().Handle, -1.0);
                }
                else
                {
                    M3D_RENDERER->SetWhiteTexture(0);
                }
                M3D_RENDERER->SetTFactor(node->GetContourColor(), false);
                shader = contourShader;
                shader->SetFloat(rend::IEffect::User_float_param, node->GetContourWidth());
                break;
            }

            M3D_RENDERER->MatPush(xform);
            RenderMesh(node, mh, shader);
            M3D_RENDERER->MatPop(1);
        }

        if (!rni.rnt && numMeshesImpostered != 0)
        {
            std::stable_sort(
                meshesShiftsImpostered,
                meshesShiftsImpostered + numMeshesImpostered,
                MeshImposteredSortPred(meshesImpostered));

            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("impostors = " + CStr(numMeshesImpostered)).c_str());

            M3D_RENDERER->SetCull(rend::M3DCULL_NONE, 0);

            CMatrix const viewProj = M3D_RENDERER->GetViewMatrix() * M3D_RENDERER->MatGetProj();
            m_impostorVs->SetMatrix(m_impostorVs->GetParamHandleByName("mViewProj"), viewProj);
            m_impostorVs->SetVector3(m_impostorVs->GetParamHandleByName("g_FogTerm"), m_fogTerm);

            // The shipped code also builds a pitch term here from the view
            // matrix, but rotYPR() calls identity() first and discards it, so
            // the billboard ends up as a pure yaw rotation.
            CMatrix const& view = M3D_RENDERER->GetViewMatrix();
            float yaw = 0.0f;
            float pitch = 0.0f;
            float roll = 0.0f;
            view.getYPR(yaw, pitch, roll);
            CMatrix billboard;
            billboard.rotYPR(-yaw, 0.0f, 0.0f);
            m_impostorVs->SetMatrix(m_impostorVs->GetParamHandleByName("mBillboard"), billboard);

            m_impostorVs->Apply();
            m_impostorPs->Apply();

            M3D_RENDERER->SetStageState(0, rend::BM_COLOR, rend::TS_TEXTURE);
            M3D_RENDERER->SetStageState(0, rend::BM_ALPHA, rend::TS_TEXTURE);
            M3D_RENDERER->SetBlend(rend::BM_NONE, 0);
            M3D_RENDERER->SetAlphaTest(5);

            int impostorTris = 0;
            int impostorDips = 0;

            // Impostors are drawn in runs that share one model, then split into
            // batches of at most MAX_INSTANCES_PER_BATCH so each instance's
            // placement fits in the vertex shader constant registers.
            int const MAX_INSTANCES_PER_BATCH = 60;
            float instanceConsts[4 * MAX_INSTANCES_PER_BATCH];

            int cursor = 0;
            int remaining = numMeshesImpostered;
            while (remaining > 0)
            {
                auto const& firstEntry = meshesImpostered[meshesShiftsImpostered[cursor]];
                AnimatedModel* const groupModel = firstEntry.modelLookup;
                DynamicModel* const dm = firstEntry.dmLookup;

                int groupCount = 0;
                while (groupCount < remaining &&
                       meshesImpostered[meshesShiftsImpostered[cursor + groupCount]].modelLookup == groupModel)
                {
                    ++groupCount;
                }
                remaining -= groupCount;

                M3D_RENDERER->SetTexture(0, dm->m_impostorTex, -1.0);
                M3D_RENDERER->SetToStream0(dm->m_impostorVb);
                M3D_RENDERER->SetIndices(dm->m_impostorIb, 0);

                while (groupCount > 0)
                {
                    int const batch = groupCount < MAX_INSTANCES_PER_BATCH ? groupCount : MAX_INSTANCES_PER_BATCH;
                    for (int b = 0; b < batch; ++b)
                    {
                        SgAnimatedModelNode* node = meshesImpostered[meshesShiftsImpostered[cursor + b]].nodeLookup;
                        CVector const& org = node->GetOriginWorldAbs();
                        float const scale = node->GetScale().x;

                        CVector const& camera = M3D_RENDERER->GetViewOrigin();
                        float const dx = camera.x - org.x;
                        float const dz = camera.z - org.z;

                        CMatrix const& xform = node->GetCurrentMatrix();
                        float angle = atan2f(dx, -dz) - atan2f(xform._13, xform._33);
                        if (angle < 0.0f)
                        {
                            angle += 6.2831855f;
                        }
                        if (angle >= 6.2831855f)
                        {
                            angle -= 6.2831855f;
                        }
                        angle = std::clamp(angle, 0.0f, 6.2831855f);

                        // x/z are the world position, y is nudged along the
                        // model's impostor displacement, and w packs the
                        // billboard frame index together with the node scale.
                        float* dst = instanceConsts + 4 * b;
                        dst[0] = org.x;
                        dst[1] = dm->m_impostorDisplacement * scale + org.y;
                        dst[2] = org.z;
                        dst[3] = static_cast<float>(
                            (1 - static_cast<int>(angle * -3.9788735f)) % 25 - 100 * static_cast<int>(scale * -100.0f));
                    }

                    M3D_RENDERER->SetVsFloatConst(20, instanceConsts, batch);
                    M3D_RENDERER->DrawIndexedPrimitiveShader(rend::M3DPT_TRIANGLELIST, 0, 4 * batch, 0, 2 * batch);

                    impostorTris += 2 * batch;
                    ++impostorDips;
                    cursor += batch;
                    groupCount -= batch;
                }
            }

            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("impostors tris = " + CStr(impostorTris)).c_str());
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("impostors dips = " + CStr(impostorDips)).c_str());
            M3D_RENDERER->SetBlend(rend::BM_NONE, 0);
        }

        m_profiler->EndCountdown();
        return 1;
    }

    void AnimatedModelsServer::RenderTransparents(SgNode** nodes, unsigned numNodes)
    {
        SceneGraph* graph = &pClient->GetWorld().GetGraph();
        if (!numNodes)
        {
            return;
        }

        M3D_RENDERER->SetAlphaTest(1);
        M3D_RENDERER->SetBlend(rend::BM_ALPHA, 0);
        M3D_RENDERER->SetCull(rend::M3DCULL_CCW, 0);
        M3D_RENDERER->SetZbState(rend::ZB_ENABLE, 0);
        M3D_RENDERER->SetFog(false, false);
        for (int stage = 0; stage < 8; ++stage)
        {
            M3D_RENDERER->TgDisable(stage);
        }

        UpdateGlobalRenderingParams();

        std::sort(nodes, nodes + numNodes, SortByDist(M3D_RENDERER->MatGetOrgInv()));

        // The three transparency parameters are pushed into whichever shader the
        // meshes happen to use, so remember the last one that took each and reset
        // it once the whole set is drawn.
        rend::IEffect* transparencyShader = nullptr;
        rend::IEffect* transStartDistShader = nullptr;
        rend::IEffect* transObjWidthShader = nullptr;

        for (unsigned i = 0; i < numNodes; ++i)
        {
            auto* node = static_cast<SgAnimatedModelNode*>(nodes[i]);
            auto* model = static_cast<DynamicModel*>(m_models[node->GetServerHandle()].m_ptr)->m_mdl[0];

            Configuration* configuration = nullptr;
            node->GetProperty(PROP_DM_CFG, &configuration);

            graph->LightSetupLightsForNode(node);

            for (auto* mesh : configuration->m_meshes)
            {
                auto& material = m_MeshMaterialManager.GetMaterial(*node, *mesh);
                rend::IEffect* shader = model->ApplyMaterial(material);

                TransparencyParams const& params = node->GetTransparencyParams();

                if (shader->IsParameterUsed(rend::IEffect::Transparency))
                {
                    shader->SetFloat(rend::IEffect::Transparency, params.value);
                    transparencyShader = shader;
                }
                if (shader->IsParameterUsed(rend::IEffect::TransStartDist))
                {
                    shader->SetFloat(rend::IEffect::TransStartDist, params.startDist);
                    transStartDistShader = shader;
                }
                if (shader->IsParameterUsed(rend::IEffect::TransObjectWidth))
                {
                    shader->SetFloat(rend::IEffect::TransObjectWidth, params.objectWidth);
                    transObjWidthShader = shader;
                }

                M3D_RENDERER->MatPush(node->GetCurrentMatrix());
                RenderMesh(node, *mesh, shader);
                M3D_RENDERER->MatPop(true);
            }
        }

        // Put the shared shaders back to fully opaque so the next pass is not
        // drawn with this set's transparency still applied.
        if (transparencyShader)
        {
            transparencyShader->SetFloat(rend::IEffect::Transparency, 1.0f);
        }
        if (transStartDistShader)
        {
            transStartDistShader->SetFloat(rend::IEffect::TransStartDist, 10000.0f);
        }
        if (transObjWidthShader)
        {
            transObjWidthShader->SetFloat(rend::IEffect::TransObjectWidth, 0.0f);
        }
    }

    int AnimatedModelsServer::RenderShadowVolumesSet(SgNode** nodes, unsigned numNodes)
    {
        // RVA 0x8F45E0 - hands the sun direction and every mesh of at least 10 faces to the shadow manager, which draws
        // the stencil shadows. NOTE: nothing in the game ever creates m_ShadowMan, so with stencil shadows on and
        // casters present this dereferences a null manager, as the original does.
        if (!M3D_KERNEL->GetEngineCfg().m_g_stencilShadows.GetB())
        {
            return 1;
        }
        m_profiler->StartCountdown();
        if (numNodes)
        {
            CVector const& sunDir = pClient->GetWorld().m_sunDir;
            m_ShadowMan->AddDirectionalLight(CVector(0.0f - sunDir.x, 0.0f - sunDir.y, 0.0f - sunDir.z));
            if (!m_ShadowMan->ShadowsEnabled())
            {
                m_ShadowMan->EnableShadows(true);
            }
            unsigned char const alpha = static_cast<unsigned char>(
                static_cast<unsigned __int64>(pClient->GetWorld().GetSForShadowsFromWeather() * 76.5));
            m_ShadowMan->SetShadowColor(static_cast<unsigned int>(alpha) << 24);
            // NOTE: the full-screen quad is rebuilt every time shadows are drawn.
            m_ShadowMan->OnChangeScreenResolution();

            for (unsigned nodeNum = 0; nodeNum < numNodes; ++nodeNum)
            {
                SgNode* const node = nodes[nodeNum];
                AnimInfo* anim = nullptr;
                node->GetProperty(1, &anim);
                Configuration* cfg = nullptr;
                node->GetProperty(8707, &cfg);
                for (unsigned i = 0; i < cfg->m_meshes.size(); ++i)
                {
                    AnimatedModel::Mesh const* const mesh = cfg->m_meshes[i];
                    // NOTE: type 1 is let through the first test only to be dropped by the last one.
                    if ((mesh->m_meshType == 4 || mesh->m_meshType == 1) && mesh->m_numFaces >= 10 &&
                        mesh->m_meshType == 4)
                    {
                        m_ShadowMan->AddShadowCaster(node->m_currentXForm, *mesh);
                    }
                }
            }
            m_ShadowMan->BeginScene();
            m_ShadowMan->EndScene();
            m_numShadowingNodes = 0;
        }
        m_profiler->EndCountdown();
        return 1;
    }

    AnimatedModelsServer::~AnimatedModelsServer()
    {
        // RVA 0x775200
        delete m_ShadowMan;
        m_ShadowMan = nullptr;
        Release();
    }

    int AnimatedModelsServer::GenerateImpostorsIfNeeded()
    {
        // RVA 0x8F5700 - renders every impostor-using model from 25 angles into a 5x5 grid of 51-pixel cells of a
        // 256x256 texture, copied into the model's impostor texture. Runs once: it marks the weather as actual.
        pClient->GetWorld().m_isWeatherActual = true;

        bool wasInScene = false;
        if (M3D_RENDERER->InScene())
        {
            wasInScene = true;
        }
        else
        {
            M3D_RENDERER->BeginScene();
        }

        M3D_RENDERER->SetBlend(rend::BM_NONE, 0);
        M3D_RENDERER->SetCull(rend::M3DCULL_CCW, 0);
        M3D_RENDERER->SetAlphaTest(M3D_KERNEL->GetEngineCfg().m_alphaTestWorld.GetI());
        M3D_RENDERER->SetZbState(rend::ZB_ENABLE, 0);
        M3D_RENDERER->SetFog(0, 0);
        M3D_RENDERER->TgDisable(0);
        M3D_RENDERER->TgDisable(1);
        M3D_RENDERER->TgDisable(2);
        M3D_RENDERER->TgDisable(3);
        M3D_RENDERER->TgDisable(4);
        M3D_RENDERER->TgDisable(5);
        M3D_RENDERER->TgDisable(6);
        M3D_RENDERER->TgDisable(7);

        UpdateGlobalRenderingParams();

        auto tmpTexHack = M3D_RENDERER->AddDynamicTexture("$TmpTexHack", 256, 256, 0);
        auto impostorTmpTex = M3D_RENDERER->AddDynamicTexture("$ImpostorTmpTex", 256, 256, 0);

        auto saveView = M3D_RENDERER->GetViewMatrix();
        auto saveProj = M3D_RENDERER->MatGetProj();

        for (auto& model : m_models)
        {
            auto* dynamicModel = (DynamicModel*)model.m_ptr;
            if (dynamicModel->m_useImpostors)
            {
                auto* animModel = dynamicModel->m_mdl[0];
                auto v11 = animModel->m_box.m_box[4] - animModel->m_box.m_box[1];
                auto sy = v11;
                auto h2 = v11 * 0.5;
                auto mx = animModel->m_box.m_box[3] - animModel->m_box.m_box[0];
                auto mz = animModel->m_box.m_box[5] - animModel->m_box.m_box[2];

                CVector eye;
                CVector at;
                CVector up;

                up.y = 1.0;
                auto v12 = dynamicModel->m_impostorDisplacement + (float)(v11 * 0.5);
                up.x = 0.0;
                up.z = 0.0;

                at.x = 0.0;
                at.y = v12;
                at.z = 0.0;
                eye.x = 0.0;
                eye.y = v12;
                auto v22 = tan(0.1963495463132858);
                eye.z = -((float)(v11 * 0.5) / v22);

                CMatrix camera;
                camera.lookAtLH(eye, at, up);

                // Set up projection matrix
                CMatrix proj;
                memset(&proj, 0, sizeof(proj));
                proj.m[2][2] = 1.0f;
                proj.m[3][2] = 1.0002f;
                proj._43 = -1.0002f;

                float fov = sqrt(mz * mz + mx * mx) / sy * 0.19634955f;
                proj._11 = 1.0f / tan(fov);
                proj._22 = 1.0f / tan(0.19634955f);

                // Set matrices
                M3D_RENDERER->MatSet(camera);
                M3D_RENDERER->SetViewMatrix(camera);
                M3D_RENDERER->MatSetProj(proj);

                // First render pass
                M3D_RENDERER->RenderToTexStart(tmpTexHack, 1);
                M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, 0);
                M3D_RENDERER->RenderToTexFinish();

                // Second render pass - render impostor from multiple angles
                if (M3D_RENDERER->RenderToTexStart(impostorTmpTex, 1))
                {
                    M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, 0);
                    M3D_RENDERER->ClearViewport(rend::M3DCLEAR_CZ, 0);

                    // Render model from 25 different angles (5x5 grid)
                    float angles[] = {0.0f,   14.4f,  28.8f,  43.2f,  57.6f,  72.0f,  86.4f,  100.8f, 115.2f,
                                      129.6f, 144.0f, 158.4f, 172.8f, 187.2f, 201.6f, 216.0f, 230.4f, 244.8f,
                                      259.2f, 273.6f, 288.0f, 302.4f, 316.8f, 331.2f, 345.6f};

                    int offsetsX[] = {0,   51,  102, 153, 204, 0,   51,  102, 153, 204, 0,   51, 102,
                                      153, 204, 0,   51,  102, 153, 204, 0,   51,  102, 153, 204};

                    int offsetsY[] = {0,   0,   0,   0,   0,   51,  51,  51,  51,  51,  102, 102, 102,
                                      102, 102, 153, 153, 153, 153, 153, 204, 204, 204, 204, 204};

                    for (int j = 0; j < 25; j++)
                    {
                        RenderModelForImpostor(animModel, angles[j], offsetsX[j], offsetsY[j]);
                    }
                }

                M3D_RENDERER->RenderToTexFinish();
                M3D_RENDERER->TexCopy(dynamicModel->m_impostorTex, impostorTmpTex);
            }
        }

        if (!wasInScene)
        {
            M3D_RENDERER->EndScene();
        }

        M3D_RENDERER->MatSet(saveView);
        M3D_RENDERER->SetViewMatrix(saveView);
        M3D_RENDERER->MatSetProj(saveProj);
        M3D_RENDERER->ReleaseTexture(tmpTexHack);
        M3D_RENDERER->ReleaseTexture(impostorTmpTex);
        return 0;
    }

    int AnimatedModelsServer::GetItemProperty(int id, int prop, void* dest)
    {
        // TODO rebuild all cases and check everything
        if (DataServer::GetItemProperty(id, prop, dest))
        {
            return 1;
        }
        if (id == -1)
        {
            return 0;
        }
        if (prop == 16394)
        {
            auto* model = (DynamicModel*)m_models[id].m_ptr;
            *(AnimatedModel**)dest = model->m_mdl[0];
            return 1;
        }
        if (prop == 12288)
        {
            auto* model = (DynamicModel*)m_models[id].m_ptr;
            auto* animModel = model->m_mdl[0];
            if (animModel->m_composite)
            {
                auto* box = (m3d::PropSrvBoundingBox*)dest;

                AnimInfo* anim = nullptr;
                box->m_node->GetProperty(1, &anim);
                *box->m_destBox = anim->m_curBox;
            }
            else
            {
                auto* box = (m3d::PropSrvBoundingBox*)dest;
                auto p_m_box = &animModel->m_box;
                box->m_destBox->m_box[0] = p_m_box->m_box[0];
                box->m_destBox->m_box[1] = p_m_box->m_box[1];
                box->m_destBox->m_box[2] = p_m_box->m_box[2];
                box->m_destBox->m_box[3] = p_m_box->m_box[3];
                box->m_destBox->m_box[4] = p_m_box->m_box[4];
                box->m_destBox->m_box[5] = p_m_box->m_box[5];
            }
            return 1;
        }
        if (prop == 12296)
        {
            auto* model = (DynamicModel*)m_models[id].m_ptr;
            auto* animModel = model->m_mdl[0];

            auto* actionTime = (m3d::PropSrvActionTime*)dest;
            int const frames = animModel->GetFrames(actionTime->m_action, 1);
            actionTime->m_delta = (animModel->GetFps(actionTime->m_action) * frames) * 0.001;
            return 1;
        }
        return 0;
    }

    void AnimatedModelsServer::RegisterNode(SgNode* node)
    {
        // RVA 0x771F70 - gives the node a fresh AnimInfo and, for a valid model id, binds it to the model's first
        // LOD with an effect list, its configuration and impostor setting; an animated node joins the think list.
        // NOTE: the node's previous AnimInfo is read and then overwritten without being released, as shipped.
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
                auto* model = reinterpret_cast<DynamicModel*>(m_models[modelId].m_ptr);
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

                // Re-applies the skin to the new configuration.
                int skinNumber = 0;
                node->GetProperty(8706u, &skinNumber);
                node->SetProperty(8706u, &skinNumber);

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
            node->GetGraph()->LinkThinkNode(node);
        }
    }

    void AnimatedModelsServer::UpdateItem(int id, void* param)
    {
        m_profiler->StartCountdown();

        struct RenderInfo
        {
            /* 0x0000 */ m3d::SgNode* m_node;
            /* 0x0004 */ unsigned int m_dt;
            /* 0x0008 */ unsigned int m_fps;
        }; /* size: 0x000c */

        auto* ri = (RenderInfo*)param;
        AnimInfo* anim = nullptr;
        ri->m_node->GetProperty(1, &anim);

        int manualAnimControl = 0;
        ri->m_node->GetProperty(8716, &manualAnimControl);
        if (manualAnimControl == 0)
        {
            anim->MoveFrame(ri->m_dt);
        }

        Configuration* cfg = nullptr;
        ri->m_node->GetProperty(8707, &cfg);

        auto* dynamicModel = (DynamicModel*)m_models[id].m_ptr;
        auto* mdl = dynamicModel->m_mdl[0];

        mdl->Update(anim, true, cfg);

        ModelEffectList* effectList = nullptr;
        ri->m_node->GetProperty(2, &effectList);

        for (auto& effectDesc : effectList->m_curEffectList)
        {
            auto& effectNode = effectDesc.m_effectNode;
            auto currentLoadpointMatrix = anim->GetCurrentLoadpointMatrix(effectDesc.m_desc->m_lpId);
            effectNode->SetOriginAbs(
                {currentLoadpointMatrix._41, currentLoadpointMatrix._42, currentLoadpointMatrix._43});

            Quaternion q;
            q.FromMatrix(currentLoadpointMatrix);
            effectNode->SetRotation(q);
        }
        m_profiler->EndCountdown();
    }

    namespace
    {
        void __fastcall DefineSkinsToLoad(m3d::LoadSkins& skinsToLoad, CStr const& paramsStr)
        {
            retruxx::string view(paramsStr.c_str(), paramsStr.length());
            auto const skinsPos = view.find("skins:");
            auto const postSkinsPos = skinsPos + 6;
            auto const semicolonPos = view.find(";", postSkinsPos);
            if (skinsPos == retruxx::string::npos || semicolonPos == retruxx::string::npos)
            {
                return;
            }

            auto const params = view.substr(postSkinsPos, semicolonPos - postSkinsPos);
            retruxx::vector<CStr> tokens;
            Tokenize(params.c_str(), tokens, "(), ;\t");

            skinsToLoad.loadAllSkins = tokens.empty();

            for (auto const& token : tokens)
            {
                skinsToLoad.loadSkins.insert(strToInt(token));
            }
        }

    }  // namespace

    void AnimatedModelsServer::AddItemsList(retruxx::vector<ServerItem>& itemslist)
    {
        // RVA 0x7755F0 - keeps the loaded models still listed (reloading their skins) and drops the rest, then
        // loads the new ones from the first item's models XML: the model and its _lod1.._lod4 files, sounds and
        // per-action load point effects.
        if (itemslist.empty())
            return;

        // Track loading time
        DWORD dwStartTime = GetTickCount();
        DWORD dwGamTime = 0;

        // Create list of skins to load
        retruxx::vector<LoadSkins> skinsToLoad(itemslist.size());

        // Pre-define skins to load from item parameters
        for (int i = 0; i < skinsToLoad.size(); ++i)
        {
            if (!itemslist[i].m_params.empty())
            {
                DefineSkinsToLoad(skinsToLoad[i], itemslist[i].m_params);
            }
        }

        int numitems = itemslist.size();
        int iterNode = 0;

        // First pass: update existing models
        for (auto it = m_models.begin(); it != m_models.end();)
        {
            if (iterNode < numitems)
            {
                bool found = false;
                for (size_t i = 0; i < itemslist.size(); i++)
                {
                    if (itemslist[i].m_id == it->m_name)
                    {
                        // Update existing model
                        itemslist[i].m_fileWasRead = true;
                        iterNode++;

                        DynamicModel* dynamicModel = reinterpret_cast<DynamicModel*>(it->m_ptr);
                        // Cache sound IDs
                        for (int j = 0; j < 32; j++)
                        {
                            if (!dynamicModel->m_soundIds[j].empty())
                            {
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

                if (found)
                {
                    ++it;
                    continue;
                }
            }

            // Remove model that's no longer in the list
            if (it->m_ptr)
            {
                delete it->m_ptr;
                it->m_ptr = nullptr;
            }
            else
            {
                M3D_LOG_INFO("Warning: Something goes wrong!");
            }
            it = m_models.erase(it);
        }

        numitems -= iterNode;
        if (numitems <= 0)
        {
            return;  // All items were existing models
        }

        // Parse protocol from first item
        m3d::DataServer::Proto proto;
        int protoPos;
        ParseProto(itemslist.front().m_filename.c_str(), &proto, &protoPos);

        if (proto != PROTO_FILE)
        {
            // NOTE: the protocol is added to the literal as a pointer, dropping that many characters, as shipped.
            M3D_LOG_ERR(CStr("Error: protocol is not supported " + static_cast<int>(proto)));
            return;
        }

        // Load XML file
        CStr xmlFilename = &itemslist.front().m_filename[protoPos];
        CStr xmlContent;

        ref_ptr xmlFile = m3d::ReadXmlFile(xmlFilename.c_str(), &xmlContent);
        if (!xmlFile)
        {
            M3D_LOG_ERR("ServerAnimatedModel: " + xmlFilename);
            return;
        }

        // Process XML models
        ref_ptr modelsNode = xmlFile->CreateNode();
        xmlFile->GetFirstChild(modelsNode, "AnimatedModels");

        if (modelsNode->IsEmpty())
        {
            return;
        }

        ref_ptr modelNode = xmlFile->CreateNode();
        modelsNode->GetFirstChild(modelNode, "model");

        // Models already present or loaded so far, for the progress callback.
        int processed = 0;
        for (; !modelNode->IsEmpty(); modelNode->GetNextSibling(modelNode, "model"))
        {
            CStr modelId = modelNode->GetAttribute("id");
            int const existing = GetItemByName(modelId.c_str(), false);
            if (m_fnLoadCallback && processed < numitems)
            {
                m_fnLoadCallback(100 * processed / numitems, m_fnLoadCallbackData);
            }
            if (existing != -1)
            {
                ++processed;
                continue;
            }

            // Check if this model is in our items list
            int itemIndex = -1;
            for (size_t i = 0; i < itemslist.size(); i++)
            {
                if (itemslist[i].m_id == modelId && !itemslist[i].m_fileWasRead)
                {
                    itemIndex = i;
                    break;
                }
            }

            if (itemIndex == -1)
            {
                continue;
            }

            // Parse model attributes
            CStr modelFile;
            SafeStrAttrib(modelFile, modelNode, "file");

            int shadow = 0;
            SafeIntAttrib(shadow, modelNode, "shadow");

            int windwavy = 0;
            SafeIntAttrib(windwavy, modelNode, "windwavy");

            int tessellate = 0;
            SafeIntAttrib(tessellate, modelNode, "tessellate");

            int trackland = 0;
            SafeIntAttrib(trackland, modelNode, "trackland");

            int shadowVolume = 0;
            SafeIntAttrib(shadowVolume, modelNode, "shadowVolume");

            bool useImpostors = false;
            m3d::SafeBoolAttrib(useImpostors, modelNode, "useImpostors");

            bool composite = false;
            m3d::SafeBoolAttrib(composite, modelNode, "composite");

            bool passable = false;
            m3d::SafeBoolAttrib(passable, modelNode, "passable");

            int trans = 0;
            SafeIntAttrib(trans, modelNode, "trans");

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

            if (loadFromGAM)
            {
                loadSuccess = mainModel->LoadGAM(modelFile.c_str(), true);
            }
            else
            {
                loadSuccess = mainModel->LoadSAM(modelFile.c_str(), true);
            }

            if (!loadSuccess)
            {
                delete dynamicModel;
                delete mainModel;
                continue;
            }

            dwGamTime += GetTickCount() - loadStart;

            if (hasBBox)
            {
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

            for (unsigned int lod = 1; lod < 5; lod++)
            {
                CStr lodFilename;
                if (loadFromGAM)
                {
                    lodFilename = baseFilename + "_lod" + CStr(lod) + ".gam";
                }
                else
                {
                    lodFilename = baseFilename + "_lod" + CStr(lod) + ".sam";
                }

                // NOTE: a missing LOD file is skipped (leaving its slot empty while later ones still count), but a
                // LOD that fails to load ends the search, as shipped.
                if (M3D_KERNEL->GetFileServer().FileExists(lodFilename.c_str()))
                {
                    m3d::AnimatedModel* lodModel = new m3d::AnimatedModel();
                    if (!lodModel->Load(lodFilename.c_str(), true))
                    {
                        delete lodModel;
                        break;
                    }
                    dynamicModel->m_mdl[lod] = lodModel;
                    dynamicModel->m_numLods++;
                }
            }

            // Process sound effects
            ref_ptr soundNode = xmlFile->CreateNode();
            modelNode->GetFirstChild(soundNode, "sound");

            auto const actions = GetAnimActions();
            while (!soundNode->IsEmpty())
            {
                CStr action;
                SafeStrAttrib(action, soundNode, "action");

                CStr soundId;
                SafeStrAttrib(soundId, soundNode, "id");

                int looped = 0;
                SafeIntAttrib(looped, soundNode, "looped");

                // Find action index and assign sound
                for (size_t i = 0; i < AT_NUMTYPES; i++)
                {
                    if (actions[i].m_name == action)
                    {
                        dynamicModel->m_soundIds[i] = soundId;
                        dynamicModel->m_soundsLooped[i] = looped != 0;
                        m3d::Application::g_pApp->m_cachedSoundIDs.insert(soundId);
                        break;
                    }
                }

                soundNode->GetNextSibling(soundNode, "sound");
            }

            // Process action effects
            ref_ptr<m3d::cmn::XmlNode> actionNode = xmlFile->CreateNode();
            modelNode->GetFirstChild(actionNode, "action");
            while (!actionNode->IsEmpty())
            {
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
                for (size_t i = 0; i < AT_NUMTYPES; i++)
                {
                    if (actions[i].m_name == actionName)
                    {
                        actionIndex = i;
                        break;
                    }
                }

                if (actionIndex != -1)
                {
                    // Process load points for this action
                    ref_ptr<m3d::cmn::XmlNode> lpNode = xmlFile->CreateNode();
                    actionNode->GetFirstChild(lpNode, "lp");
                    while (!lpNode->IsEmpty())
                    {
                        CStr lpId;
                        SafeStrAttrib(lpId, lpNode, "id");

                        CStr effectId;
                        SafeStrAttrib(effectId, lpNode, "effect_id");

                        // RVA 0x7755F0 - both flags are integers compared with 1. An effect with no
                        // ImmediateRemove attribute is removed at once when its action ends (a glow that
                        // is left to fade out never does, which kept brake lights lit).
                        int restartOnAnimChange = 0;
                        SafeIntAttrib(restartOnAnimChange, lpNode, "restartOnAnimationChange");

                        int immediateRemove = 1;
                        SafeIntAttrib(immediateRemove, lpNode, "ImmediateRemove");

                        DynamicModel::auxEffectDesc effectDesc;
                        effectDesc.m_lpName = lpId;
                        effectDesc.m_lpId = mainModel->GetLoadPointIdByName(lpId.c_str());
                        effectDesc.m_effectName = effectId;
                        effectDesc.m_effectId = -1;
                        effectDesc.m_restartOnAnimChange = restartOnAnimChange == 1;
                        effectDesc.m_immediateRemove = immediateRemove == 1;

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
            if (useImpostors)
            {
                dynamicModel->_createImpostorShit();
            }

            // Add to server models
            m3d::DataServer::Model newModel(
                (void*)dynamicModel, modelFile.c_str(), itemslist[itemIndex].m_filename.c_str(), modelId.c_str());
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
            ++processed;
        }

        // Log unread files
        for (auto const& item : itemslist)
        {
            if (!item.m_fileWasRead)
            {
                M3D_LOG_ERR("ServerAnimatedModels: cannot read file: " + item.m_filename + " id = " + item.m_id);
            }
        }

        // Log loading statistics
        DWORD totalTime = GetTickCount() - dwStartTime;
        M3D_LOG_INFO("### Total time: " + CStr(totalTime) + ", GAM time: " + CStr(dwGamTime));
    }

    int AnimatedModelsServer::RenderMesh(SgAnimatedModelNode* node, AnimatedModel::Mesh& mh, rend::IEffect* shader)
    {
        unsigned beginRenderTime = 0;
        bool const debugRender = M3D_ENGINE_CFG.m_g_renderMeshesDebug.GetB();
        if (debugRender)
        {
            beginRenderTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        }

        switch (mh.m_meshType)
        {
        case 1:
        {
            AnimInfo* anim = nullptr;
            node->GetProperty(1, &anim);

            M3D_RENDERER->MatPushWorld();
            M3D_RENDERER->MatSetWorld(anim->m_bonesAnim[mh.m_numNode].m_curMatrix);
            M3D_RENDERER->SetToStream0(mh.m_VbPoolField);
            M3D_RENDERER->SetIndices(mh.m_IbPoolField, mh.m_VbPoolField.RealOffset);
            break;
        }
        case 2:
        {
            int vOfs = 0;
            AnimInfo* anim = nullptr;
            node->GetProperty(1, &anim);

            auto verts = anim->m_meshesVerts[mh.meshId];
            auto vbHandle = M3D_RENDERER->GetVbStreaming(mh.m_VertexType);
            memcpy(
                M3D_RENDERER->LockVbStreaming(vbHandle, mh.m_numVertices, vOfs, nullptr),
                verts,
                mh.m_numVertices * mh.m_VertexTypeSize);
            M3D_RENDERER->UnlockVb(vbHandle);
            M3D_RENDERER->SetToStream0(vbHandle);
            M3D_RENDERER->SetIndices(mh.m_IbPoolField, vOfs);
            break;
        }
        case 4:
        {
            M3D_RENDERER->SetToStream0(mh.m_VbPoolField);
            M3D_RENDERER->SetIndices(mh.m_IbPoolField, mh.m_VbPoolField.RealOffset);
            break;
        }
        // RVA 0x8F3D60 - any other mesh type keeps the buffers the caller set up.
        default:
            break;
        }

        if (shader)
        {
            if (m_globalFxParamAmbientNotActuated && shader->IsParameterUsed(rend::IEffect::LightAmbient))
            {
                shader->SetVector3(rend::IEffect::LightAmbient, m_colorAmbient);
                m_globalFxParamAmbientNotActuated = 0;
            }
            if (m_globalFxParamDiffuseNotActuated && shader->IsParameterUsed(rend::IEffect::LightDiffuse))
            {
                shader->SetVector3(rend::IEffect::LightDiffuse, m_colorDiffuse);
                m_globalFxParamDiffuseNotActuated = 0;
            }
            if (m_globalFxParamSpecularNotActuated && shader->IsParameterUsed(rend::IEffect::LightSpecular))
            {
                shader->SetVector3(rend::IEffect::LightSpecular, m_colorSpecular);
                m_globalFxParamSpecularNotActuated = 0;
            }
            if (m_globalFxParamPlantAmbientNotActuated && shader->IsParameterUsed(rend::IEffect::LightPlant))
            {
                shader->SetVector3(rend::IEffect::LightPlant, m_colorPlant);
                m_globalFxParamPlantAmbientNotActuated = 0;
            }
            if (m_globalFxParamFogNotActuated && shader->IsParameterUsed(rend::IEffect::FogTerm))
            {
                shader->SetVector3(rend::IEffect::FogTerm, m_fogTerm);
                m_globalFxParamFogNotActuated = 0;
            }
            if (m_globalFxParamFrameStartTimeNotActuated && shader->IsParameterUsed(rend::IEffect::Time_Linear))
            {
                auto frameStartTimeSec = M3D_KERNEL->GetTimer().GetFrameStartTimeSec();
                shader->SetFloat(rend::IEffect::Time_Linear, frameStartTimeSec);
                m_globalFxParamFrameStartTimeNotActuated = 0;
            }
            if (m_globalFxParamTreeBendTermNotActuated && shader->IsParameterUsed(rend::IEffect::Tree_Bend_Term))
            {
                shader->SetVector3(rend::IEffect::Tree_Bend_Term, m_treeBendTerm);
                this->m_globalFxParamTreeBendTermNotActuated = 0;
            }

            M3D_RENDERER->DrawIndexedPrimitiveEffect(
                rend::M3DPT_TRIANGLELIST,
                shader,
                0,
                mh.m_numVertices,
                mh.m_IbPoolField.RealOffset,
                mh.m_numDrawIndices / 3);
        }
        else
        {
            M3D_RENDERER->DrawIndexedPrimitive(
                rend::M3DPT_TRIANGLELIST, 0, mh.m_numVertices, mh.m_IbPoolField.RealOffset, mh.m_numDrawIndices / 3);
        }

        if (mh.m_meshType == 1)
        {
            M3D_RENDERER->MatPopWorld();
        }

        if (debugRender)
        {
            unsigned endRenderTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
            if (endRenderTime - beginRenderTime > 10)
            {
                M3D_LOG_ERR("Critical render time for mesh of model");
            }
        }
        return 1;
    }

    int AnimatedModelsServer::RenderMesh(AnimInfo* ai, AnimatedModel::Mesh& mh, rend::IEffect* shader)
    {
        unsigned beginRenderTime = 0;
        bool const debugRender = M3D_KERNEL->GetEngineCfg().m_g_renderMeshesDebug.GetB();
        if (debugRender)
        {
            beginRenderTime = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        }

        auto meshType = mh.m_meshType - 1;
        if (meshType)
        {
            if (meshType - 1 == 0)
            {
                int vofs = 0;
                auto& verts = ai->m_meshesVerts[mh.meshId];

                auto vb = M3D_RENDERER->GetVbStreaming(mh.m_VertexType);
                auto stream = M3D_RENDERER->LockVbStreaming(vb, mh.m_numVertices, vofs, nullptr);
                memcpy(stream, ai->m_meshesVerts[mh.meshId], mh.m_numVertices * mh.m_VertexTypeSize);
                M3D_RENDERER->UnlockVb(vb);
                M3D_RENDERER->SetToStream0(vb);
                M3D_RENDERER->SetIndices(mh.m_IbPoolField, vofs);
            }
            if (meshType - 1 == 2)
            {
                M3D_RENDERER->SetToStream0(mh.m_VbPoolField);
                M3D_RENDERER->SetIndices(mh.m_IbPoolField, mh.m_VbPoolField.RealOffset);
            }
        }
        else
        {
            M3D_RENDERER->MatPushWorld();
            M3D_RENDERER->MatSetWorld(ai->m_bonesAnim[mh.m_numNode].m_curMatrix);
            M3D_RENDERER->SetToStream0(mh.m_VbPoolField);
            M3D_RENDERER->SetIndices(mh.m_IbPoolField, mh.m_VbPoolField.RealOffset);
        }

        if (shader)
        {
            if (this->m_globalFxParamAmbientNotActuated && shader->IsParameterUsed(rend::IEffect::LightAmbient))
            {
                shader->SetVector3(rend::IEffect::LightAmbient, m_colorAmbient);
                this->m_globalFxParamAmbientNotActuated = 0;
            }
            if (this->m_globalFxParamDiffuseNotActuated && shader->IsParameterUsed(rend::IEffect::LightDiffuse))
            {
                shader->SetVector3(rend::IEffect::LightDiffuse, m_colorDiffuse);
                this->m_globalFxParamDiffuseNotActuated = 0;
            }
            if (this->m_globalFxParamSpecularNotActuated && shader->IsParameterUsed(rend::IEffect::LightSpecular))
            {
                shader->SetVector3(rend::IEffect::LightSpecular, m_colorSpecular);
                this->m_globalFxParamSpecularNotActuated = 0;
            }
            if (this->m_globalFxParamPlantAmbientNotActuated && shader->IsParameterUsed(rend::IEffect::LightPlant))
            {
                shader->SetVector3(rend::IEffect::LightPlant, m_colorPlant);
                this->m_globalFxParamPlantAmbientNotActuated = 0;
            }
            if (this->m_globalFxParamFogNotActuated && shader->IsParameterUsed(rend::IEffect::FogTerm))
            {
                shader->SetVector3(rend::IEffect::FogTerm, m_fogTerm);
                this->m_globalFxParamFogNotActuated = 0;
            }

            if (this->m_globalFxParamFrameStartTimeNotActuated && shader->IsParameterUsed(rend::IEffect::Time_Linear))
            {
                auto frameStartTimeSec = m3d::g_Kernel->GetTimer().GetFrameStartTimeSec();
                shader->SetFloat(rend::IEffect::Time_Linear, frameStartTimeSec);
                this->m_globalFxParamFrameStartTimeNotActuated = 0;
            }
            if (this->m_globalFxParamTreeBendTermNotActuated && shader->IsParameterUsed(rend::IEffect::Tree_Bend_Term))
            {
                shader->SetVector3(rend::IEffect::Tree_Bend_Term, m_treeBendTerm);
                this->m_globalFxParamTreeBendTermNotActuated = 0;
            }
            M3D_RENDERER->DrawIndexedPrimitiveEffect(
                rend::M3DPT_TRIANGLELIST,
                shader,
                0,
                mh.m_numVertices,
                mh.m_IbPoolField.RealOffset,
                mh.m_numDrawIndices / 3);
        }
        else
        {
            M3D_RENDERER->DrawIndexedPrimitive(
                rend::M3DPT_TRIANGLELIST, 0, mh.m_numVertices, mh.m_IbPoolField.RealOffset, mh.m_numDrawIndices / 3);
        }

        if (mh.m_meshType == 1)
        {
            M3D_RENDERER->MatPopWorld();
        }

        auto curTimeUnscaled = 0;
        if (debugRender)
        {
            curTimeUnscaled = M3D_KERNEL->GetTimer().GetCurTimeUnscaled();
        }

        if (curTimeUnscaled > beginRenderTime + 10)
        {
            if (ai->m_forModel)
            {
                M3D_LOG_INFO(
                    "Critical render time for mesh of impostor '" + CStr(ai->m_forModel->GetName()) +
                    "': " + CStr(curTimeUnscaled - beginRenderTime) + " ms");
            }
            else
            {
                M3D_LOG_INFO(
                    "Critical render time for mesh of unknown impostor: " + CStr(curTimeUnscaled - beginRenderTime) +
                    " ms");
            }
        }

        return 1;
    }

    void AnimatedModelsServer::RenderModelForImpostor(AnimatedModel* mdl, float rotY, int offX, int offY)
    {
        rend::Viewport port;
        port.m_height = 51;
        port.m_width = 51;
        port.m_zMin = 0.0;
        port.m_x0 = offX;
        port.m_y0 = offY;
        port.m_zMax = 1.0;
        M3D_RENDERER->SetViewport(port);

        CMatrix m;
        memset(&m, 0, sizeof(CMatrix));
        auto v7 = *(float*)&offY * 0.017453292;
        auto v19 = v7;
        auto v8 = sin(v7);
        auto v9 = 0;
        m._22 = 1.0;
        m._33 = cos(v19);
        m._11 = m._33;
        m._13 = -v8;
        m._31 = v8;

        auto animInfo = new AnimInfo;
        animInfo->CreateFor(mdl);
        animInfo->SetAnimation(AT_STAND1);
        M3D_RENDERER->MatPush(m);
        for (int i = 0; i < mdl->m_numMeshes; ++i)
        {
            auto& mesh = mdl->m_meshes[i];
            if (mesh.m_numNode >= 0)
            {
                auto& material = mesh.GetMaterial(0);
                auto* shader = mdl->ApplyMaterial(material);
                RenderMesh(animInfo, mesh, shader);
            }
        }
        M3D_RENDERER->MatPop(true);
        delete animInfo;
    }

    void AnimatedModelsServer::UpdateGlobalRenderingParams()
    {
        auto ambientColor = rend::Colorf(pClient->GetWorld().GetWeatherAmbientColor());
        this->m_colorAmbient.x = ambientColor.r;
        this->m_colorAmbient.y = ambientColor.g;
        this->m_colorAmbient.z = ambientColor.b;

        auto diffuseColor = rend::Colorf(pClient->GetWorld().GetWeatherDiffuseColor());
        this->m_colorDiffuse.x = diffuseColor.r;
        this->m_colorDiffuse.y = diffuseColor.g;
        this->m_colorDiffuse.z = diffuseColor.b;

        auto specularColor = rend::Colorf(pClient->GetWorld().GetWeatherSpecularColor());
        this->m_colorSpecular.x = specularColor.r;
        this->m_colorSpecular.y = specularColor.g;
        this->m_colorSpecular.z = specularColor.b;

        auto plantColor = rend::Colorf(pClient->GetWorld().GetWeatherPlantColor());
        this->m_colorPlant.x = plantColor.r;
        this->m_colorPlant.y = plantColor.g;
        this->m_colorPlant.z = plantColor.b;

        float s, e;
        pClient->GetWorld().GetLandscape().GetFogStartAndEnd(s, e);
        auto reduceFactor = pClient->GetWorld().GetWeatherManager().GetFogReduceFactorFromWeather();

        auto v7 = reduceFactor * s;
        auto v8 = reduceFactor * e;
        this->m_fogTerm.x = reduceFactor * e;
        this->m_fogTerm.z = v7;
        this->m_fogTerm.y = 1.0 / (float)(v8 - v7);
        auto const& v9 = m3d::g_Kernel->GetTimer();
        auto v10 = m3d::g_Kernel->GetTimer().GetFrameStartTimeSec() + m3d::g_Kernel->GetTimer().GetFrameStartTimeSec();
        this->m_treeBendTerm.z = 0.0;
        this->m_globalFxParamFogNotActuated = 1;
        this->m_globalFxParamDiffuseNotActuated = 1;
        this->m_globalFxParamAmbientNotActuated = 1;
        this->m_globalFxParamFrameStartTimeNotActuated = 1;
        this->m_globalFxParamPlantAmbientNotActuated = 1;
        this->m_globalFxParamTreeBendTermNotActuated = 1;
        this->m_globalFxParamSpecularNotActuated = 1;
        this->m_treeBendTerm.x = sin(v10) * 0.0099999998;
        this->m_treeBendTerm.y = cos(v10) * 0.0099999998;
    }
}  // namespace m3d

bool ModelEffectList::tEffect::IsValid() const
{
    return m_effectNode != 0;
}

ModelEffectList::ModelEffectList(DynamicModel* meta) : m_dynModel(meta)
{
}

void DeleteEffectNode(m3d::SgNode* parent, m3d::SgNode* node, bool immediateRemove)
{
    // RVA 0x771840 - either removed at once, or every node below it is told it may
    // die (particles finish their life) and the node goes once they all have.
    if (immediateRemove)
    {
        m3d::SceneGraph* graph = node->GetGraph();
        graph->RemoveNode(node);
        return;
    }

    node->RemoveImmediateAfterParent(false);
    retruxx::vector<m3d::Object*> stack;
    stack.push_back(node);
    while (!stack.empty())
    {
        m3d::Object* const current = stack.back();
        stack.pop_back();
        for (m3d::Object* child = current->GetFirstChild(); child; child = child->GetNextSibling())
        {
            static_cast<m3d::SgNode*>(child)->CanBeFree();
            if (child->GetFirstChild())
            {
                stack.push_back(child);
            }
        }
    }
    node->GetGraph()->InsertInRemoveIfFree(node);
}

void AddEffectNode(m3d::SgNode* parent, m3d::SgNode*& node, int effectId, bool immediateRemove)
{
    // RVA 0x76DEF0 - a new transient effect node under parent, or none for effect id -1.
    if (effectId == -1)
    {
        node = nullptr;
        return;
    }
    node = m3d::pClient->CreateServerControlledNode(effectId);
    parent->AddChild(node);
    node->RemoveImmediateAfterParent(immediateRemove);
    node->SetPersistance(false);
}

void ModelEffectList::adjustModelEffects(
    m3d::SgNode* realModel,
    retruxx::vector<ModelEffectList::tEffect, retruxx::allocator<ModelEffectList::tEffect>>& newEffectList)
{
    // RVA 0x772150 - moves from the current effects to newEffectList. Both are sorted by loadpoint, then effect id:
    // they are merged, keeping (and optionally restarting) the node of an effect in both, deleting the ones only
    // in the current list and creating the ones only in the new list. Entries left without a node are dropped.
    retruxx::vector<ModelEffectList::tEffect>& current = m_curEffectList;
    size_t const oldSize = current.size();
    size_t const newSize = newEffectList.size();
    size_t cur = 0;
    size_t nw = 0;
    auto const deleteCurrent = [&]()
    {
        DeleteEffectNode(realModel, current[cur].m_effectNode, current[cur].m_desc->m_immediateRemove);
        ++cur;
    };
    auto const addNew = [&]()
    {
        ModelEffectList::tEffect& effect = newEffectList[nw];
        AddEffectNode(realModel, effect.m_effectNode, effect.m_desc->m_effectId, effect.m_desc->m_immediateRemove);
        ++nw;
    };

    while (cur < oldSize && nw < newSize)
    {
        int const curLp = current[cur].m_desc->m_lpId;
        int const lp = newEffectList[nw].m_desc->m_lpId;
        if (lp < curLp)
        {
            addNew();
            continue;
        }
        if (lp > curLp)
        {
            while (cur < oldSize && current[cur].m_desc->m_lpId < lp)
            {
                deleteCurrent();
            }
            continue;
        }

        // The same loadpoint: merge by effect id.
        while (cur < oldSize && nw < newSize && current[cur].m_desc->m_lpId == lp &&
               newEffectList[nw].m_desc->m_lpId == lp)
        {
            int const curEffect = current[cur].m_desc->m_effectId;
            int const effect = newEffectList[nw].m_desc->m_effectId;
            if (effect < curEffect)
            {
                addNew();
            }
            else if (effect > curEffect)
            {
                while (cur < oldSize && current[cur].m_desc->m_lpId == lp && current[cur].m_desc->m_effectId < effect)
                {
                    deleteCurrent();
                }
            }
            else
            {
                // NOTE: a restart reaches only the nodes below the effect node, not the effect node itself.
                m3d::SgNode* const node = current[cur].m_effectNode;
                if (current[cur].m_desc->m_restartOnAnimChange && node)
                {
                    m3d::ForEachDescendant(
                        node,
                        [](m3d::SgNode* child)
                        {
                            child->Restart();
                        });
                }
                newEffectList[nw++].m_effectNode = node;
                ++cur;
            }
        }
        while (cur < oldSize && current[cur].m_desc->m_lpId == lp)
        {
            deleteCurrent();
        }
        while (nw < newSize && newEffectList[nw].m_desc->m_lpId == lp)
        {
            addNew();
        }
    }
    while (cur < oldSize)
    {
        deleteCurrent();
    }
    while (nw < newSize)
    {
        addNew();
    }

    newEffectList.erase(
        std::remove_if(
            newEffectList.begin(),
            newEffectList.end(),
            [](ModelEffectList::tEffect const& effect)
            {
                return !effect.IsValid();
            }),
        newEffectList.end());
    m_curEffectList = newEffectList;
}

namespace
{
    // RVA 0x7745C0 - std::stable_sort with SortPred as the shipped (VC7.1) library runs it. Lists of up to 32 go
    // through its insertion sort (0x770DD0): each entry is rotated to the front if it sorts before the first,
    // else back past the entries it sorts before. Reproduced exactly, since SortPred is inconsistent and the
    // order depends on the algorithm (and the modern library's debug checks would reject it).
    void StableSortEffects(retruxx::vector<ModelEffectList::tEffect>& effects)
    {
        if (effects.size() > 32)
        {
            // NOTE: longer lists take VC7.1's buffered merge sort, whose result under SortPred is not reproduced;
            // they are sorted by loadpoint, then effect id.
            std::stable_sort(
                effects.begin(),
                effects.end(),
                [](ModelEffectList::tEffect const& a, ModelEffectList::tEffect const& b)
                {
                    if (a.m_desc->m_lpId != b.m_desc->m_lpId)
                    {
                        return a.m_desc->m_lpId < b.m_desc->m_lpId;
                    }
                    return a.m_desc->m_effectId < b.m_desc->m_effectId;
                });
            return;
        }
        if (effects.size() < 2)
        {
            return;
        }
        ModelEffectList::SortPred pred;
        auto const first = effects.begin();
        for (auto next = first + 1; next != effects.end(); ++next)
        {
            auto pos = next;
            if (pred(*next, *first))
            {
                pos = first;
            }
            else
            {
                while (pred(*next, *(pos - 1)))
                {
                    --pos;
                }
            }
            std::rotate(pos, next, next + 1);
        }
    }
}  // namespace

void ModelEffectList::adjustModelEffects(
    m3d::SgNode* realModel,
    retruxx::vector<ActionType, retruxx::allocator<ActionType>> const& newActions)
{
    // RVA 0x7748E0 - the effects of all the given actions together, minus those on suppressed loadpoints.
    retruxx::vector<ModelEffectList::tEffect> newEffectList;
    newEffectList.reserve(10);
    retruxx::set<int>* suppressedLPs = nullptr;
    realModel->GetProperty(8714, &suppressedLPs);

    for (size_t j = 0; j < newActions.size(); ++j)
    {
        auto const& lpEffects = m_dynModel->m_effects[newActions[j]].lpEffects;
        for (size_t i = 0; i < lpEffects.size(); ++i)
        {
            tEffect effect;
            effect.m_effectNode = 0;
            effect.m_desc = &m_dynModel->m_effects[newActions[j]].lpEffects[i];
            if (!suppressedLPs || suppressedLPs->find(effect.m_desc->m_lpId) == suppressedLPs->end())
            {
                newEffectList.push_back(std::move(effect));
            }
        }
    }

    if (!newEffectList.empty())
    {
        StableSortEffects(newEffectList);
    }
    adjustModelEffects(realModel, newEffectList);
}

void ModelEffectList::adjustModelEffects(m3d::SgNode* realModel, ActionType newAction)
{
    retruxx::vector<ModelEffectList::tEffect> newEffectList;
    retruxx::set<int>* suppressedLPs = nullptr;
    realModel->GetProperty(8714, &suppressedLPs);

    if (suppressedLPs)
    {
        for (int i = 0; i < m_dynModel->m_effects[newAction].lpEffects.size(); ++i)
        {
            tEffect effect;
            effect.m_effectNode = 0;
            effect.m_desc = &m_dynModel->m_effects[newAction].lpEffects[i];
            auto it = suppressedLPs->find(effect.m_desc->m_lpId);
            if (it == suppressedLPs->end())
            {
                newEffectList.push_back(std::move(effect));
            }
        }
    }
    else
    {
        for (int i = 0; i < m_dynModel->m_effects[newAction].lpEffects.size(); ++i)
        {
            tEffect effect;
            effect.m_effectNode = 0;
            effect.m_desc = &m_dynModel->m_effects[newAction].lpEffects[i];
            newEffectList.push_back(std::move(effect));
        }
    }

    if (!newEffectList.empty())
    {
        StableSortEffects(newEffectList);
    }
    adjustModelEffects(realModel, newEffectList);
}

bool ModelEffectList::SortPred::operator()(ModelEffectList::tEffect const& a, ModelEffectList::tEffect const& b)
{
    // RVA 0x76D250
    // NOTE: not a strict weak ordering, as shipped: a lower loadpoint sorts first, but otherwise the effect ids
    // decide, even when a's loadpoint is the higher one. See StableSortEffects.
    if (a.m_desc->m_lpId < b.m_desc->m_lpId)
    {
        return true;
    }
    return a.m_desc->m_effectId < b.m_desc->m_effectId;
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
    // RVA 0x772A60
    if (m_useImpostors)
    {
        _releaseImpostorShit();
    }
    for (auto& mdl : m_mdl)
    {
        delete mdl;
        mdl = nullptr;
    }
}

void DynamicModel::_createImpostorShit()
{
    // RVA 0x76EBD0 - the impostor billboards: 60 instances of a quad as wide as the model's x/z diagonal and as
    // tall as its box, standing on the box bottom (x, y, instance index as z, then u, v), their index buffer, and
    // the 256x256 texture the views are rendered into.
    Aabb const& box = m_mdl[0]->m_box;
    float const mx = box.m_box[3] - box.m_box[0];
    float const mz = box.m_box[5] - box.m_box[2];
    float const sy = box.m_box[4] - box.m_box[1];
    m_impostorDisplacement = box.m_box[1];
    float const halfWidth = static_cast<float>(sqrt(mz * mz + mx * mx) * 0.5);

    m_impostorVb = M3D_RENDERER->AddVb(m3d::rend::VertexType::VERTEX_IMPOSTORTEST, 240, "Impostors", 0);
    auto* vertex = static_cast<float*>(M3D_RENDERER->LockVb(m_impostorVb, 0, 0, 0));
    for (int i = 0; i < 60; ++i)
    {
        float const quad[4][5] = {
            {-halfWidth, 0.0f, static_cast<float>(i), 0.0f, 1.0f},
            {-halfWidth, sy, static_cast<float>(i), 0.0f, 0.0f},
            {halfWidth, 0.0f, static_cast<float>(i), 1.0f, 1.0f},
            {halfWidth, sy, static_cast<float>(i), 1.0f, 0.0f}};
        memcpy(vertex, quad, sizeof(quad));
        vertex += 20;
    }
    M3D_RENDERER->UnlockVb(m_impostorVb);

    // Triangles (v, v+1, v+2) and (v+2, v+1, v+3) per quad.
    m_impostorIb = M3D_RENDERER->AddIb(360, 0);
    auto* index = static_cast<uint16_t*>(M3D_RENDERER->LockIb(m_impostorIb, 0, 0, 0));
    for (uint16_t v = 0; v < 240; v += 4, index += 6)
    {
        index[0] = v;
        index[1] = v + 1;
        index[2] = v + 2;
        index[3] = v + 2;
        index[4] = v + 1;
        index[5] = v + 3;
    }
    M3D_RENDERER->UnlockIb(m_impostorIb);

    m_impostorTex =
        M3D_RENDERER->AddDynamicTexture((CStr("$ImpostorTex.") + CStr(m_mdl[0]->GetName())).c_str(), 256, 256, 1);
    M3D_RENDERER->SetTextureParameter(m_impostorTex, m3d::rend::TexParam::TM_WRAP_S, 3);
    M3D_RENDERER->SetTextureParameter(m_impostorTex, m3d::rend::TexParam::TM_WRAP_T, 3);
    M3D_RENDERER->SetTextureParameter(m_impostorTex, m3d::rend::TexParam::TM_TEX_FILTER, 1);
}

void DynamicModel::_releaseImpostorShit()
{
    // RVA 0x76D1B0
    if (m_useImpostors)
    {
        M3D_RENDERER->ReleaseVb(m_impostorVb);
        M3D_RENDERER->ReleaseIb(m_impostorIb);
        M3D_RENDERER->ReleaseTexture(m_impostorTex);
    }
}

DynamicModel::auxActionEffectsDesc::auxActionEffectsDesc()
{
    this->endAttackFrame = -1;
    this->startAttackFrame = -1;
    this->cfgNum = -1;
    this->skinNum = -1;
}
