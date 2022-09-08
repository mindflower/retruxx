#include "player.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMoney)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddMoney)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetHealth)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMaxHealth)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetFuel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetMaxFuel)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, GetVehicle)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddItemsToRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, RemoveItemsFromRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, HasAmountOfItemsInRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, CanPlaceItemsToRepository)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, IsQuestItemPresent)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, AddQuestItem)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Player, RemoveQuestItem)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    Player* thePlayer = nullptr;

    RT_CLASS_EXPORTS_BEGIN(Player)
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetMoney, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, AddMoney, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetHealth, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetMaxHealth, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetFuel, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetMaxFuel, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, GetVehicle, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, AddItemsToRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, RemoveItemsFromRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, HasAmountOfItemsInRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, CanPlaceItemsToRepository, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, IsQuestItemPresent, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, AddQuestItem, "", "", "")
        RT_CLASS_EXPORT(Player, m3d::METHOD, RemoveQuestItem, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Player);

    CStr const& PlayerPrototypeInfo::GetModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    PlayerPrototypeInfo::PlayerPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* PlayerPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned PlayerPrototypeInfo::GetSkinNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned PlayerPrototypeInfo::GetCfgNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PlayerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Player::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    int Player::GetMoney() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::ChangeVehicleByExisting(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<CStr, std::allocator<CStr>> const& Player::GetQuestItemPrototypeNames() const
    {
        throw std::logic_error("Not implemented");
    }

    float Player::GetFuel() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::HuntQuestIsTaken() const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::RemoveQuestItem(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetCfgNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Player::GetModelName() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::CanPlaceItemsToRepository(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    Player::Player(PlayerPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* Player::GetVehicle() const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::AddMoney(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::IsQuestItemPresent(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::ChangeVehicleByNew(int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int Player::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    IzvratRepository* Player::GetRepository() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float Player::GetMaxHealth() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float Player::GetHealth() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    float Player::GetMaxFuel() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::RemoveItemsFromRepository(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    PlayerPrototypeInfo const* Player::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::AddItemsToRepository(char const*, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetSchwarz() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Player::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    InfoCone const& Player::GetInfoCone() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::AddQuestItem(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::HasAmountOfItemsInRepository(char const*, int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Player::GetLastSaveDir() const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::SetLastSaveDir(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Player::GetInfoObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Player::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    int Player::GetRadioManagerId() const
    {
        throw std::logic_error("Not implemented");
    }

    NumericBoundedBelow<int>& Player::Money()
    {
        throw std::logic_error("Not implemented");
    }

    NumericBoundedBelow<int> const& Player::Money() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Player::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Player::GetSkinNumber() const
    {
        throw std::logic_error("Not implemented");
    }

    Player::~Player()
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Player::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestForgotten(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Player::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestFailed(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnUnderAttack(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Player::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestTaken(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnMoneyValueAfterChange(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnNoticeEnemy(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Player::_OnDynamicQuestComplete(Event const&)
    {
        throw std::logic_error("Not implemented");
    }
}
