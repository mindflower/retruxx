#include "wanderersmanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(WanderersManager)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WanderersManager);

    WanderersManagerPrototypeInfo::WanderersManagerPrototypeInfo() = default;

    bool WanderersManagerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        return result;
    }

    ai::Obj* WanderersManagerPrototypeInfo::CreateTargetObject() const
    {
        return new WanderersManager(*this);
    }

    WanderersManager::~WanderersManager()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    WanderersManager::WanderersManager(ai::WanderersManagerPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_rebornTimeout = 10.0f;
        m_timeBeforeReborn = 0.0f;
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
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::Class* WanderersManager::GetClass() const
    {
        return RT_CLASS_LOCAL(WanderersManager);
    }

    ai::WanderersManagerPrototypeInfo const* WanderersManager::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int WanderersManager::OnEvent(ai::Event const& evn)
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

    void WanderersManager::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        // TODO: implement WanderersManager::LoadFromXML
    }

    void WanderersManager::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
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

    ai::WanderersManager::CaravanInfo const& WanderersManager::GetCaravanInfo(unsigned int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int WanderersManager::GetWayPointsNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::WanderersManager::WayPoint const& WanderersManager::GetWayPoint(unsigned int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadWayPointsFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadPrecisePathsFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadCaravansInfoFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadVagabondsInfoFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_LoadWanderersStatesFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
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

    int WanderersManager::_GetWayPointIndex(CStr const& wayPointName) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_SetStatePrecisePath(ai::WanderersManager::WandererState& state, int beginIndex, int endIndex)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CVector2, retruxx::allocator<CVector2>> WanderersManager::_GetPrecisePath(int beginIndex, int endIndex) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int WanderersManager::_GetWanderersCount() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnTargetReached(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnObjectDie(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnObjectEntersLocation(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void WanderersManager::_OnLostGuardsNeedDirection(ai::Event const& evn)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
