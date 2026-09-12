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

#include "core/timer.h"
#include "scene/nodes/sgnodeanimatedmodel.h"

namespace m3d
{
    void AnimatedModelsServer::PostLoad()
    {
        for (const auto& model : m_models)
        {
            auto* dynamicModel = (DynamicModel*)model.m_ptr;
            for (auto& actionEffect : dynamicModel->m_effects)
            {
                std::vector<DynamicModel::auxEffectDesc> newEffectList;
                for (const auto& effect : actionEffect.lpEffects)
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModelsServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AnimatedModelsServer::IsBonePresentsInModel(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    bool AnimatedModelsServer::ReportServerInfo(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModelsServer::Init()
    {
        auto const logoFileName = g_Kernel->GetEngineCfg().m_loadFromGAM.GetB() ? "data\\models\\Logos\\Logos.gam" : "data\\models\\Logos\\Logos.sam";
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

    int AnimatedModelsServer::GetBoneMatrixByNameFromModelName(char const* modelname, CStr const& boneName, CMatrix& res, bool theLastOneOnly)
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModelsServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
            auto* node = (SgAnimatedModelNode*)src;
            ModelEffectList* list = nullptr;
            node->GetProperty(2, &list);
            list->adjustModelEffects(node, node->m_action);

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

            auto skinNum = dynamicModel->m_effects[node->m_action].skinNum;
            auto cfgNum = dynamicModel->m_effects[node->m_action].cfgNum;
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
            auto* node = (SgAnimatedModelNode*)src;
            AnimInfo* anim = nullptr;
            node->GetProperty(1, &anim);
            anim->SetAnimation(node->m_action);
            return 1;
        }
        case 8710:
        {
            auto* node = (SgAnimatedModelNode*)src;
            ModelEffectList* list = nullptr;
            node->GetProperty(2, &list);
            list->adjustModelEffects(node, node->m_action);

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

            // TODO: check this
            for (int i = 0; i < node->m_action; ++i)
            {
                auto skinNum = dynamicModel->m_effects[i].skinNum;
                auto cfgNum = dynamicModel->m_effects[i].cfgNum;
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

            return 1;
        }
        }
        RETRUXX_NOT_IMPLEMENTED;
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
            MeshImposteredSortPred(const ImpostoredMeshInfo* meshes) : m_meshes(meshes)
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
            /* 0x0000 */ const ImpostoredMeshInfo* m_meshes;
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
            MeshSortPred(const MeshInfo* meshes) : m_meshes(meshes)
            {
            }

            bool operator()(unsigned int meshIdx1, unsigned int meshIdx2) const
            {
                // TODO: check this
                auto& material1 = m_meshes[meshIdx1].material;
                auto& material2 = m_meshes[meshIdx2].material;
                if (material1->Shader.Handle < material2->Shader.Handle)
                {
                    return true;
                }
                if (material1->Shader.Handle == material2->Shader.Handle)
                {
                    return &material1->Textures.front().Handle < &material2->Textures.front().Handle;
                }

                return false;
            }
            /* 0x0000 */ const MeshInfo* m_meshes;
        }; /* size: 0x0004 */

        // Orders nodes by squared distance from the camera, nearest first.
        struct SortByDist
        {
            explicit SortByDist(const CVector& org) : m_org(org)
            {
            }

            bool operator()(m3d::SgNode* a, m3d::SgNode* b) const
            {
                const CVector da = a->GetOriginWorldAbsForSphere() - m_org;
                const CVector db = b->GetOriginWorldAbsForSphere() - m_org;
                return (db.x * db.x + db.y * db.y + db.z * db.z) > (da.x * da.x + da.y * da.y + da.z * da.z);
            }

            /* 0x0000 */ CVector m_org;
        }; /* size: 0x000c */

        // A material contributes an alpha-tested diffuse texture when it has any
        // texture at all and either carries no shader or its first technique is
        // flagged as alpha-using.
        bool HasAlphaTestedTexture(const m3d::DSurfaceMaterial& material)
        {
            return !material.Textures.empty() &&
                   (!material.Shader.Handle || material.Shader.Handle->GetTechniqueDesc(0).useAlpha);
        }

        void BindAlphaTestedTexture(const m3d::DSurfaceMaterial& material, int stage, int alphaTest)
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
        // TODO: generated code
        // Profile timing start
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

            // Calculate distance squared to view position
            distSq =
                ((currentNode->GetOriginWorldAbs().x - viewPos.x) * (currentNode->GetOriginWorldAbs().x - viewPos.x) +
                 (currentNode->GetOriginWorldAbs().y - viewPos.y) * (currentNode->GetOriginWorldAbs().y - viewPos.y) +
                 (currentNode->GetOriginWorldAbs().z - viewPos.z) * (currentNode->GetOriginWorldAbs().z - viewPos.z));

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

                // Clamp LOD level
                unsigned int maxLod = modelData->m_numLods;
                if (lodLevel >= maxLod)
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

            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(
                ("impostors = " + CStr(numMeshesImpostered)).c_str());

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
                            (1 - static_cast<int>(angle * -3.9788735f)) % 25 -
                            100 * static_cast<int>(scale * -100.0f));
                    }

                    M3D_RENDERER->SetVsFloatConst(20, instanceConsts, batch);
                    M3D_RENDERER->DrawIndexedPrimitiveShader(rend::M3DPT_TRIANGLELIST, 0, 4 * batch, 0, 2 * batch);

                    impostorTris += 2 * batch;
                    ++impostorDips;
                    cursor += batch;
                    groupCount -= batch;
                }
            }

            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(
                ("impostors tris = " + CStr(impostorTris)).c_str());
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(
                ("impostors dips = " + CStr(impostorDips)).c_str());
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

                const TransparencyParams& params = node->GetTransparencyParams();

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

    int AnimatedModelsServer::RenderShadowVolumesSet(SgNode**, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AnimatedModelsServer::~AnimatedModelsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModelsServer::GenerateImpostorsIfNeeded()
    {
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
                    float angles[] = {0.0f,   14.4f,  28.8f,  43.2f,  57.6f,  72.0f,  86.4f,  100.8f, 115.2f, 129.6f, 144.0f, 158.4f, 172.8f,
                                      187.2f, 201.6f, 216.0f, 230.4f, 244.8f, 259.2f, 273.6f, 288.0f, 302.4f, 316.8f, 331.2f, 345.6f};

                    int offsetsX[] = {0, 51, 102, 153, 204, 0, 51, 102, 153, 204, 0, 51, 102, 153, 204, 0, 51, 102, 153, 204, 0, 51, 102, 153, 204};

                    int offsetsY[] = {0, 0, 0, 0, 0, 51, 51, 51, 51, 51, 102, 102, 102, 102, 102, 153, 153, 153, 153, 153, 204, 204, 204, 204, 204};

                    for (int j = 0; j < 25; j++)
                    {
                        RenderModelForImpostor(animModel, angles[j], offsetsX[j], offsetsY[j]);
                    }
                }

                M3D_RENDERER->RenderToTexFinish();
                // TODO: check this
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
        RETRUXX_NOT_IMPLEMENTED;
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
        ri->m_node->GetProperty(7816, &manualAnimControl);
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
            effectNode->SetOriginAbs({currentLoadpointMatrix._41, currentLoadpointMatrix._42, currentLoadpointMatrix._43});

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

    }  // namespace

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
            M3D_LOG_ERR("Error protocol: " + CStr(proto));
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

        for (; !modelNode->IsEmpty(); modelNode->GetNextSibling(modelNode, "model"))
        {
            CStr modelId = modelNode->GetAttribute("id");

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

                if (M3D_KERNEL->GetFileServer().FileExists(lodFilename.c_str()))
                {
                    m3d::AnimatedModel* lodModel = new m3d::AnimatedModel();
                    if (lodModel->Load(lodFilename.c_str(), true))
                    {
                        dynamicModel->m_mdl[lod] = lodModel;
                        dynamicModel->m_numLods++;
                    }
                    else
                    {
                        delete lodModel;
                    }
                }
            }

            // Process sound effects
            ref_ptr soundNode = xmlFile->CreateNode();
            modelNode->GetFirstChild(soundNode, "sound");

            const auto actions = GetAnimActions();
            while (!soundNode->IsEmpty())
            {
                CStr action;
                SafeStrAttrib(action, soundNode, "action");

                CStr soundId;
                SafeStrAttrib(soundId, soundNode, "id");

                bool looped = false;
                SafeBoolAttrib(looped, soundNode, "looped");

                // Find action index and assign sound
                for (size_t i = 0; i < AT_NUMTYPES; i++)
                {
                    if (actions[i].m_name == action)
                    {
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
            if (useImpostors)
            {
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
        }

        // Log unread files
        for (const auto& item : itemslist)
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
        const bool debugRender = M3D_ENGINE_CFG.m_g_renderMeshesDebug.GetB();
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
            memcpy(M3D_RENDERER->LockVbStreaming(vbHandle, mh.m_numVertices, vOfs, nullptr), verts, mh.m_numVertices * mh.m_VertexTypeSize);
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
        default: RETRUXX_NOT_IMPLEMENTED;
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
                rend::M3DPT_TRIANGLELIST, shader, 0, mh.m_numVertices, mh.m_IbPoolField.RealOffset, mh.m_numDrawIndices / 3);
        }
        else
        {
            M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST, 0, mh.m_numVertices, mh.m_IbPoolField.RealOffset, mh.m_numDrawIndices / 3);
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
        const bool debugRender = M3D_KERNEL->GetEngineCfg().m_g_renderMeshesDebug.GetB();
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
                rend::M3DPT_TRIANGLELIST, shader, 0, mh.m_numVertices, mh.m_IbPoolField.RealOffset, mh.m_numDrawIndices / 3);
        }
        else
        {
            M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST, 0, mh.m_numVertices, mh.m_IbPoolField.RealOffset, mh.m_numDrawIndices / 3);
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
                    "Critical render time for mesh of impostor '" + CStr(ai->m_forModel->GetName()) + "': " + CStr(curTimeUnscaled - beginRenderTime) + " ms");
            }
            else
            {
                M3D_LOG_INFO("Critical render time for mesh of unknown impostor: " + CStr(curTimeUnscaled - beginRenderTime) + " ms");
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
        const auto& v9 = m3d::g_Kernel->GetTimer();
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

ModelEffectList::ModelEffectList(DynamicModel* meta) :
    m_dynModel(meta)
{
}

void DeleteEffectNode(m3d::SgNode* parent, m3d::SgNode* node, bool immediateRemove)
{
    // TODO: generated code
    if (immediateRemove)
    {
        // Immediate removal from scene graph
        m3d::SceneGraph* graph = node->GetGraph();
        graph->RemoveNode(node);
    }
    else
    {
        // Deferred removal - mark for later cleanup
        node->RemoveImmediateAfterParent(false);

        // Use stack to traverse and mark all children as free-able
        std::vector<m3d::SgNode*> nodeStack;
        nodeStack.push_back(node);

        while (!nodeStack.empty())
        {
            m3d::SgNode* currentNode = nodeStack.back();
            nodeStack.pop_back();

            // Mark current node as free-able
            currentNode->CanBeFree();

            // Process all children
            m3d::SgNode* child = static_cast<m3d::SgNode*>(currentNode->GetFirstChild());
            while (child)
            {
                // Recursively mark children as free-able
                child->CanBeFree();

                // If child has children, add to stack for processing
                if (child->GetFirstChild())
                {
                    nodeStack.push_back(child);
                }

                child = static_cast<m3d::SgNode*>(child->GetNextSibling());
            }
        }

        // Schedule node for deferred removal
        m3d::SceneGraph* graph = node->GetGraph();
        graph->InsertInRemoveIfFree(node);
    }
}

void ModelEffectList::adjustModelEffects(m3d::SgNode* realModel,
    retruxx::vector<ModelEffectList::tEffect, retruxx::allocator<ModelEffectList::tEffect>>& newEffectList)
{
    // TODO: generated code ModelEffectList::adjustModelEffects
    std::vector<ModelEffectList::tEffect>& currentEffects = m_curEffectList;
    size_t currentSize = currentEffects.size();
    size_t newSize = newEffectList.size();

    size_t currentIdx = 0;
    size_t newIdx = 0;

    // Process both lists in order (sorted by lpId and effectId)
    while (currentIdx < currentSize && newIdx < newSize)
    {
        ModelEffectList::tEffect& currentEffect = currentEffects[currentIdx];
        ModelEffectList::tEffect& newEffect = (newEffectList)[newIdx];

        int currentLpId = currentEffect.m_desc->m_lpId;
        int newLpId = newEffect.m_desc->m_lpId;

        if (newLpId < currentLpId)
        {
            // Add new effect with lower lpId
            if (newEffect.m_desc->m_effectId == -1)
            {
                newEffect.m_effectNode = nullptr;
            }
            else
            {
                m3d::SgNode* effectNode = m3d::pClient->CreateServerControlledNode(newEffect.m_desc->m_effectId);
                newEffect.m_effectNode = effectNode;
                realModel->AddChild(effectNode);
                effectNode->RemoveImmediateAfterParent(newEffect.m_desc->m_immediateRemove);
                effectNode->SetPersistance(false);
            }
            newIdx++;
        }
        else if (newLpId > currentLpId)
        {
            // Remove current effect with lower lpId
            DeleteEffectNode(realModel, currentEffect.m_effectNode, currentEffect.m_desc->m_immediateRemove);
            currentIdx++;
        }
        else
        {
            // Same lpId - process by effectId
            int currentLpId = newLpId;  // Store for later use

            while (currentIdx < currentSize && newIdx < newSize)
            {
                ModelEffectList::tEffect& currentEffect = currentEffects[currentIdx];
                ModelEffectList::tEffect& newEffect = (newEffectList)[newIdx];

                // Check if we're still processing the same lpId
                if (currentEffect.m_desc->m_lpId != currentLpId || newEffect.m_desc->m_lpId != currentLpId)
                {
                    break;
                }

                int currentEffectId = currentEffect.m_desc->m_effectId;
                int newEffectId = newEffect.m_desc->m_effectId;

                if (newEffectId < currentEffectId)
                {
                    // Add new effect with lower effectId
                    if (newEffect.m_desc->m_effectId == -1)
                    {
                        newEffect.m_effectNode = nullptr;
                    }
                    else
                    {
                        m3d::SgNode* effectNode = m3d::pClient->CreateServerControlledNode(newEffect.m_desc->m_effectId);
                        newEffect.m_effectNode = effectNode;
                        realModel->AddChild(effectNode);
                        effectNode->RemoveImmediateAfterParent(newEffect.m_desc->m_immediateRemove);
                        effectNode->SetPersistance(false);
                    }
                    newIdx++;
                }
                else if (newEffectId > currentEffectId)
                {
                    // Remove current effect with lower effectId
                    DeleteEffectNode(realModel, currentEffect.m_effectNode, currentEffect.m_desc->m_immediateRemove);
                    currentIdx++;
                }
                else
                {
                    // Same effectId - reuse existing node
                    newEffect.m_effectNode = currentEffect.m_effectNode;

                    // Restart animation if needed
                    if (currentEffect.m_desc->m_restartOnAnimChange && currentEffect.m_effectNode)
                    {
                        std::vector<m3d::SgNode*> nodeStack;
                        nodeStack.push_back(currentEffect.m_effectNode);

                        while (!nodeStack.empty())
                        {
                            m3d::SgNode* currentNode = nodeStack.back();
                            nodeStack.pop_back();

                            // Restart current node animation
                            currentNode->Restart();

                            // Process children
                            m3d::SgNode* child = static_cast<m3d::SgNode*>(currentNode->GetFirstChild());
                            while (child)
                            {
                                nodeStack.push_back(child);
                                child = static_cast<m3d::SgNode*>(child->GetNextSibling());
                            }
                        }
                    }

                    currentIdx++;
                    newIdx++;
                }
            }

            // Remove any remaining current effects with this lpId
            while (currentIdx < currentSize && currentEffects[currentIdx].m_desc->m_lpId == currentLpId)
            {
                DeleteEffectNode(realModel, currentEffects[currentIdx].m_effectNode, currentEffects[currentIdx].m_desc->m_immediateRemove);
                currentIdx++;
            }

            // Add any remaining new effects with this lpId
            while (newIdx < newSize && (newEffectList)[newIdx].m_desc->m_lpId == currentLpId)
            {
                ModelEffectList::tEffect& newEffect = (newEffectList)[newIdx];
                if (newEffect.m_desc->m_effectId == -1)
                {
                    newEffect.m_effectNode = nullptr;
                }
                else
                {
                    m3d::SgNode* effectNode = m3d::pClient->CreateServerControlledNode(newEffect.m_desc->m_effectId);
                    newEffect.m_effectNode = effectNode;
                    realModel->AddChild(effectNode);
                    effectNode->RemoveImmediateAfterParent(newEffect.m_desc->m_immediateRemove);
                    effectNode->SetPersistance(false);
                }
                newIdx++;
            }
        }
    }

    // Remove any remaining current effects
    while (currentIdx < currentSize)
    {
        DeleteEffectNode(realModel, currentEffects[currentIdx].m_effectNode, currentEffects[currentIdx].m_desc->m_immediateRemove);
        currentIdx++;
    }

    // Add any remaining new effects
    while (newIdx < newSize)
    {
        ModelEffectList::tEffect& newEffect = (newEffectList)[newIdx];
        if (newEffect.m_desc->m_effectId == -1)
        {
            newEffect.m_effectNode = nullptr;
        }
        else
        {
            m3d::SgNode* effectNode = m3d::pClient->CreateServerControlledNode(newEffect.m_desc->m_effectId);
            newEffect.m_effectNode = effectNode;
            realModel->AddChild(effectNode);
            effectNode->RemoveImmediateAfterParent(newEffect.m_desc->m_immediateRemove);
            effectNode->SetPersistance(false);
        }
        newIdx++;
    }

    // Filter out invalid effects from the new list
    auto newEnd = std::remove_if(newEffectList.begin(), newEffectList.end(), [](const ModelEffectList::tEffect& effect) { return !effect.IsValid(); });
    newEffectList.erase(newEnd, newEffectList.end());

    // Update current effect list
    m_curEffectList = newEffectList;
}

void ModelEffectList::adjustModelEffects(m3d::SgNode* realModel,
    const retruxx::vector<ActionType, retruxx::allocator<ActionType>>& newActions)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        std::stable_sort(newEffectList.begin(), newEffectList.end(), SortPred());
    }
    adjustModelEffects(realModel, newEffectList);
}

bool ModelEffectList::SortPred::operator()(const ModelEffectList::tEffect& a, const ModelEffectList::tEffect& b)
{
    // TODO: check this
    if (a.m_desc->m_lpId == b.m_desc->m_lpId)
    {
        return a.m_desc->m_effectId < b.m_desc->m_effectId;
    }
    return a.m_desc->m_lpId < b.m_desc->m_lpId;
    //return a.m_desc->m_lpId < b.m_desc->m_lpId || a.m_desc->m_effectId < b.m_desc->m_effectId;
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
    RETRUXX_NOT_IMPLEMENTED;
}

DynamicModel::auxActionEffectsDesc::auxActionEffectsDesc()
{
    this->endAttackFrame = -1;
    this->startAttackFrame = -1;
    this->cfgNum = -1;
    this->skinNum = -1;
}
