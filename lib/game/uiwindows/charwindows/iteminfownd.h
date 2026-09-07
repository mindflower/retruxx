#pragma once
#include "infownd.h"
#include <core/ref_ptr.h>
#include <server/geomrepositoryitem.h>

namespace ai
{
    class Gadget;
}

class ItemInfoWnd : public InfoWnd
{
public:
    void SetUpForRepositoryItem(ai::GeomRepositoryItem const& item, ref_ptr<m3d::ui::Wnd> srcWnd);
    void SetUpForObject(int objId, ref_ptr<m3d::ui::Wnd> srcWnd);
    // NOTE: declared in the PDB but no code was emitted for it in the shipped
    // binary and nothing references it, so it is left without a definition.
    void SetUpForGadget(ai::Gadget const*, ref_ptr<m3d::ui::Wnd>);
    ai::GeomRepositoryItem const& GetItem() const;
    ai::Obj const* GetObjectA() const;
    ref_ptr<m3d::ui::Wnd> GetSrcWnd() const;

    enum InfoType
    {
        INFOTYPE_REPOSITORY_ITEM = 0,
        INFOTYPE_OBJECT = 1,
    };

    enum Property
    {
        PROP_FULL_NAME = 0,
        PROP_AMOUNT = 1,
        PROP_ARMOR = 2,
        PROP_SOLIDITY = 3,
        PROP_SPEED = 4,
        PROP_TORQUE = 5,
        PROP_POWER = 6,
        PROP_DAMAGE = 7,
        PROP_FIRING_RATE = 8,
        PROP_FIRING_RANGE = 9,
        PROP_PRICE = 10,
        PROP_RESISTANCE = 11,
        PROP_SHELLPOOL = 12,
        PROP_CHARGE_SIZE = 13,
        PROP_RECHARGING_TIME = 14,
        PROP_REPOSITORY_CAPACITY = 15,
        PROP_DESCRIPTION = 16,
        PROP_GADGET_TYPE = 17,
        PROP_UNCOMPATIBLE = 18,
        PROP_TOO_RICH = 19,
        PROP_ACCURACY = 20,
        PROP_FIRING_TYPE = 21,
        PROP_BROKEN = 22,
        PROP_NO_AMMO = 23,
        PROP_NUM_PROPS = 24,
    };

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_propStrIds[24];
        AuxInfo(ItemInfoWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0120 */

protected:
    bool IsValid() const;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    int Update(bool firstUpdate);
    int ShowInfoBase(bool updateOnly);
    // NOTE: declared in the PDB but no code was emitted for it either; the
    // vehicle-part row is reached through ShowInfoForCabin / ShowInfoForBasket.
    int ShowInfoForVehiclePart(bool updateOnly);
    int ShowInfoForCabin(bool updateOnly);
    int ShowInfoForBasket(bool updateOnly);
    int ShowInfoForGun(bool updateOnly);
    int ShowInfoForGoods(bool updateOnly);
    int ShowInfoForGold(bool updateOnly);
    int ShowInfoForGadget(bool updateOnly);
    int ShowPropertyInfo(ItemInfoWnd::Property prop, bool updateOnly);
    int GetResourceId() const;
    bool NeedShowPrice() const;
    bool NeedShowChargeSize() const;
    /* 0x0234 */ ItemInfoWnd::InfoType m_infoType;
    /* 0x0238 */ ai::GeomRepositoryItem m_repositoryItem;
    /* 0x0254 */ int m_objId;
    /* 0x0258 */ CStr m_auxFullName;
    static ItemInfoWnd::AuxInfo m_aif;
    /* 0x0264 */ ref_ptr<m3d::ui::Wnd> m_srcWnd;
    ItemInfoWnd();
    ItemInfoWnd(ItemInfoWnd const& rhs);

public:
    virtual ~ItemInfoWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(ItemInfoWnd);
}; /* size: 0x0268 */
