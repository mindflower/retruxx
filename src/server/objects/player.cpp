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
}
