#include "cbcharacteristicswnd.h"

#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimisc/guihelper.h"
#include "ui/image.h"

#include "server/damageinfo.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/basket.h"
#include "server/objects/cabin.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(CBCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CBCharacteristicsWnd);

CBCharacteristicsWnd::AuxInfo::AuxInfo()
{
    m_wndPiercingValName = "wndPiercingVal";
    m_wndBlastValName = "wndBlastVal";
    m_wndEnergyValName = "wndEnergyVal";
    m_wndMaxDurabilityValName = "wndMaxDurabilityVal";
    m_wndWeightValName = "wndWeightVal";
    m_wndCBImageName = "wndCBIco";
    m_wndCBNameName = "wndCBName";
}

CBCharacteristicsWnd::CBCharacteristicsWnd()
{
    m_wndPiercingVal = 0;
    m_wndBlastVal = 0;
    m_wndEnergyVal = 0;
    m_wndMaxDurabilityVal = 0;
    m_wndWeightVal = 0;
    m_wndCBImage = 0;
    m_wndCBName = 0;
    m_cbId = -1;
}

CBCharacteristicsWnd::CBCharacteristicsWnd(CBCharacteristicsWnd const&) : CBCharacteristicsWnd()
{
    // NOTE: the shipped copy ctor default-constructs the base and AuxInfo but
    // leaves every Wnd* member and m_cbId uninitialised; delegating to the
    // default ctor avoids reading uninitialised pointers.
}

CBCharacteristicsWnd::~CBCharacteristicsWnd()
{
    // m_aif's CStr members and the CharacteristicsWnd base clean up
    // automatically.
}

m3d::Object* CBCharacteristicsWnd::Clone()
{
    return new CBCharacteristicsWnd(*this);
}

m3d::Object* CBCharacteristicsWnd::CreateObject()
{
    return new CBCharacteristicsWnd;
}

m3d::Class* CBCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

m3d::Class* CBCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CharacteristicsWnd);
}

int CBCharacteristicsWnd::GameDataSetup()
{
    using namespace m3d::ui;

    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto find = [this, &res](CStr const& name, Wnd*& out)
        {
            auto* child = GetChildByName(name);
            if (child && child->IsKindOf(&Wnd::m_classWnd))
            {
                out = static_cast<Wnd*>(child);
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                res = 0;
            }
        };

        find(m_aif.m_wndPiercingValName, m_wndPiercingVal);
        find(m_aif.m_wndBlastValName, m_wndBlastVal);
        find(m_aif.m_wndEnergyValName, m_wndEnergyVal);
        find(m_aif.m_wndMaxDurabilityValName, m_wndMaxDurabilityVal);
        find(m_aif.m_wndWeightValName, m_wndWeightVal);

        auto* imgChild = GetChildByName(m_aif.m_wndCBImageName);
        if (imgChild && imgChild->IsKindOf(&ImageWnd::m_classImageWnd))
        {
            m_wndCBImage = static_cast<ImageWnd*>(imgChild);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndCBImageName + " is not found or incorrect type");
            res = 0;
        }

        auto* nameChild = GetChildByName(m_aif.m_wndCBNameName);
        if (nameChild && nameChild->IsKindOf(&Wnd::m_classWnd))
        {
            m_wndCBName = static_cast<Wnd*>(nameChild);
            if (res)
            {
                m_gameDataFlags |= 1u;
                UpdateCBImage();
                UpdateCBName();
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndCBNameName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("CBCharacteristicsWnd: error - fail to init because of a bad resource");
    return 0;
}

int CBCharacteristicsWnd::GameDataClear(bool)
{
    m_cbId = -1;
    return 1;
}

void CBCharacteristicsWnd::SetupForVehicle(int vehicleId)
{
    // RVA matches CharacteristicsWnd::SetupForVehicle's shape.
    m_vehicleId = vehicleId;
    if (!GetVehicle())
    {
        ClearCharacteristics();
        m_vehicleId = -1;
    }
}

void CBCharacteristicsWnd::SetupForCB(int cbId)
{
    m_cbId = cbId;

    ai::VehiclePart const* cb = GetCB();
    m3d::Object* parent = cb ? cb->GetParent() : nullptr;
    bool ok = cb && parent && parent->IsKindOf(&ai::Vehicle::m_classVehicle);
    if (ok)
    {
        SetupForVehicle(static_cast<ai::Vehicle*>(parent)->GetId());
        ok = GetVehicle() != nullptr;
    }
    if (!ok)
    {
        ClearCharacteristics();
        m_cbId = -1;
        SetupForVehicle(-1);
    }

    UpdateCBImage();
    UpdateCBName();
}

ai::VehiclePart const* CBCharacteristicsWnd::GetCB() const
{
    if (m_cbId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_cbId);
    if (obj && (obj->IsKindOf(&ai::Cabin::m_classCabin) || obj->IsKindOf(&ai::Basket::m_classBasket)))
    {
        return static_cast<ai::VehiclePart*>(obj);
    }
    return nullptr;
}

void CBCharacteristicsWnd::UpdateOnCBChanged()
{
    UpdateCBImage();
    UpdateCBName();
}

void CBCharacteristicsWnd::UpdateCBImage()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::VehiclePart const* cb = GetCB();
    m3d::rend::TexHandle ico;
    if (cb && cb->GetPrototypeInfo())
    {
        ico = M3D_APP->m_pInterfaceManager->GetIcoByName(cb->GetPrototypeInfo()->m_prototypeName, 0);
    }
    m_wndCBImage->SetImage(ico);
    m_wndCBImage->ShowWindow(cb != nullptr);
}

void CBCharacteristicsWnd::UpdateCBName()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::VehiclePart const* cb = GetCB();
    m_wndCBName->SetText(cb ? cb->GetFullDescriptionWithAffixes() : CStr());
}

void CBCharacteristicsWnd::UpdateCharacteristics()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    ai::VehiclePart const* cb = GetCB();
    if (!cb)
    {
        return;
    }
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }

    // In "hacked" (workshop preview) mode, compare against the player's own
    // vehicle instead of the previewed one.
    ai::Vehicle* baseVehicle = m_vehicleType != VehicleWnd::VEHICLE_NORMAL
                                   ? (ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr)
                                   : const_cast<ai::Vehicle*>(vehicle);
    if (!baseVehicle)
    {
        return;
    }

    ai::VehiclePart const* basePart = baseVehicle->GetPartByName(cb->GetPartName());

    int const piercing = static_cast<int>(cb->GetDurabilityCoeffForDamageType(ai::DAMAGE_PIERCING));
    int const blast = static_cast<int>(cb->GetDurabilityCoeffForDamageType(ai::DAMAGE_BLAST));
    int const energy = static_cast<int>(cb->GetDurabilityCoeffForDamageType(ai::DAMAGE_ENERGY));
    int const maxDurability = static_cast<int>(cb->Durability().maxValue().get());
    int const weight = static_cast<int>(cb->GetMass());

    int const basePiercing =
        basePart ? static_cast<int>(basePart->GetDurabilityCoeffForDamageType(ai::DAMAGE_PIERCING)) : 0;
    int const baseBlast = basePart ? static_cast<int>(basePart->GetDurabilityCoeffForDamageType(ai::DAMAGE_BLAST)) : 0;
    int const baseEnergy =
        basePart ? static_cast<int>(basePart->GetDurabilityCoeffForDamageType(ai::DAMAGE_ENERGY)) : 0;
    // NOTE: the shipped build always compares max durability against 0 (no
    // base-durability baseline is computed).
    int const baseMaxDurability = 0;
    // Weight's baseline goes through the property system (property id 20),
    // which reverses any installed gadget modification.
    int const baseWeight =
        basePart ? static_cast<int>(help::GetBasePropertyValFromObj(basePart, baseVehicle, 20)) : 0;

    UpdateValue(m_wndPiercingVal, piercing, basePiercing, false);
    UpdateValue(m_wndBlastVal, blast, baseBlast, false);
    UpdateValue(m_wndEnergyVal, energy, baseEnergy, false);
    UpdateValue(m_wndMaxDurabilityVal, maxDurability, baseMaxDurability, false);
    UpdateValue(m_wndWeightVal, weight, baseWeight, true);
}

void CBCharacteristicsWnd::ClearCharacteristics()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_wndPiercingVal->SetText(CStr());
        m_wndBlastVal->SetText(CStr());
        m_wndEnergyVal->SetText(CStr());
        m_wndMaxDurabilityVal->SetText(CStr());
        m_wndWeightVal->SetText(CStr());
    }
}
