#include "wanderersmanager.h"

namespace ai
{
    WanderersManagerPrototypeInfo::WanderersManagerPrototypeInfo()
    {
    }

    bool WanderersManagerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        return result;
    }

    ai::Obj* WanderersManagerPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WanderersManager::~WanderersManager()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WanderersManager::WanderersManager(const ai::WanderersManagerPrototypeInfo& prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* WanderersManager::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* WanderersManager::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* WanderersManager::GetBaseClass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* WanderersManager::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersManagerPrototypeInfo* WanderersManager::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersManager::OnEvent(const ai::Event& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool WanderersManager::CanChildBeAdded(m3d::Class* pClass) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::AddChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::Update(float elapsedTime, unsigned int workTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int WanderersManager::GetCaravansNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersManager::CaravanInfo& WanderersManager::GetCaravanInfo(unsigned int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int WanderersManager::GetWayPointsNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::WanderersManager::WayPoint& WanderersManager::GetWayPoint(unsigned int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadWayPointsFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadPrecisePathsFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadCaravansInfoFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadVagabondsInfoFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadWanderersStatesFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SaveWayPointsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SavePrecisePathsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SaveCaravansInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SaveVagabondInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SaveWanderersStatesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SpawnWanderer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SpawnCaravanTeam(int index)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SpawnVagabondTeam(int index)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersManager::_GetWayPointIndex(const CStr& wayPointName) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SetStatePrecisePath(ai::WanderersManager::WandererState& state, int beginIndex,
        int endIndex)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CVector2, retruxx::allocator<CVector2>> WanderersManager::_GetPrecisePath(int beginIndex,
        int endIndex) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int WanderersManager::_GetWanderersCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnTargetReached(const ai::Event& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnObjectDie(const ai::Event& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnObjectEntersLocation(const ai::Event& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnLostGuardsNeedDirection(const ai::Event& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
