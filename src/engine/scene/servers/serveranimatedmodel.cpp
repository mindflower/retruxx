#include <scene/servers/serveranimatedmodel.h>

namespace m3d
{
    void AnimatedModelsServer::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::IsBonePresentsInModel(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UnregisterNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModelsServer::ReportServerInfo(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    CVector AnimatedModelsServer::GetBoundSizes(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetBoneMatrixByNameFromModelName(char const*, CStr const&, CMatrix&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModelsServer::AnimatedModelsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderNodeSet(SgNode**, unsigned, RenderNodeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderTransparents(SgNode**, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderShadowVolumesSet(SgNode**, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModelsServer::~AnimatedModelsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GenerateImpostorsIfNeeded()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RegisterNode(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UpdateItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::AddItemsList(std::vector<ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(SgAnimatedModelNode*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModelsServer::RenderMesh(AnimInfo*, AnimatedModel::Mesh&, rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::RenderModelForImpostor(AnimatedModel*, float, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModelsServer::UpdateGlobalRenderingParams()
    {
        throw std::logic_error("Not implemented");
    }
}
