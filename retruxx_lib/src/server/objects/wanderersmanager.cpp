#include "wanderersmanager.h"

namespace ai
{
    WanderersManagerPrototypeInfo::WanderersManagerPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool WanderersManagerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* WanderersManagerPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    WanderersManager::~WanderersManager()
    {
        throw std::logic_error("Not implemented");
    }

    WanderersManager::WanderersManager(const ai::WanderersManagerPrototypeInfo& prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* WanderersManager::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* WanderersManager::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* WanderersManager::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* WanderersManager::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::WanderersManagerPrototypeInfo* WanderersManager::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    int WanderersManager::OnEvent(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    bool WanderersManager::CanChildBeAdded(m3d::Class* pClass) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::AddChild(ai::Obj* pObj)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::Update(float elapsedTime, unsigned int workTime)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int WanderersManager::GetCaravansNum() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::WanderersManager::CaravanInfo& WanderersManager::GetCaravanInfo(unsigned int) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int WanderersManager::GetWayPointsNum() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::WanderersManager::WayPoint& WanderersManager::GetWayPoint(unsigned int) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_LoadWayPointsFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_LoadPrecisePathsFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_LoadCaravansInfoFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_LoadVagabondsInfoFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_LoadWanderersStatesFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SaveWayPointsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SavePrecisePathsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SaveCaravansInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SaveVagabondInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SaveWanderersStatesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SpawnWanderer()
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SpawnCaravanTeam(int index)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SpawnVagabondTeam(int index)
    {
        throw std::logic_error("Not implemented");
    }

    int WanderersManager::_GetWayPointIndex(const CStr& wayPointName) const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_SetStatePrecisePath(ai::WanderersManager::WandererState& state, int beginIndex,
        int endIndex)
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<CVector2, retruxx::allocator<CVector2>> WanderersManager::_GetPrecisePath(int beginIndex,
        int endIndex) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int WanderersManager::_GetWanderersCount() const
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_OnTargetReached(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_OnObjectDie(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_OnObjectEntersLocation(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }

    void WanderersManager::_OnLostGuardsNeedDirection(const ai::Event& evn)
    {
        throw std::logic_error("Not implemented");
    }
}
