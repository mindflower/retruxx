#pragma once
#include "game/uiwindows/miscwindows/bindkeyswnd.h"

#include <map>
#include <core/clazz.h>

class WeaponGroupManager :  public m3d::Object
{
public:
    int DeleteWeaponGroup(int);
    void ClearSavedGroups();
    int ValidateWeaponGroups();
    void GetAllWeapons(retruxx::set<CStr> &) const ;
    void SaveWeaponGroups();
    virtual m3d::Object * Clone();
    class WeaponGroup * GetWeaponGroupById(int) const ;
    void KeepFire();
    int GameDataUpdate(void *,int);
    WeaponGroup * CreateWeaponGroup(int);
    int LoadFromXml(m3d::cmn::XmlFile *, m3d::cmn::XmlNode const *);
    static int __fastcall GetWeaponGroupsMaxCount();
    static int __fastcall GetDefaultWeaponGroupIdForWeapon(CStr const &);
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    int AddWeaponGroup(WeaponGroup *);
    void ReloadAllWeapon();
    void OnPlayerVehicleChanged();
    int Init();
    int AddWeaponToWeaponGroup(CStr const &,int);
    int AddWeaponToWeaponGroup(int,int);
    static CStr __fastcall GetGunPartNameByGunId(int);
    void RestoreWeaponGroups();
    virtual ~WeaponGroupManager();
    void OnVehiclePartChanged(void *);
    int SaveToXml(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *) const ;
    void ClearGroups();
    int RemoveWeaponFromWeaponGroup(int);
    int RemoveWeaponFromWeaponGroup(CStr const &);
    virtual m3d::Class * GetClass() const ;
    int GetWeaponGroupIdForWeapon(CStr const &) const ;
    void Clear();
protected:
    WeaponGroupManager(WeaponGroupManager const &);
    WeaponGroupManager();

public:
    RT_CLASS_DECLARE(WeaponGroupManager);

private:
    retruxx::map<int,WeaponGroup *> m_weaponGroups;
    retruxx::map<int,WeaponGroup *> m_savedWeaponGroups;
};

class WeaponGroup : public m3d::Object
{
public:
    using GunPartNamesSet = std::set<CStr, std::less<CStr>, std::allocator<CStr>>;

public:
    int GetGroupId() const;
    Impulse GetImpulseId() const;
    const std::set<CStr, std::less<CStr>, std::allocator<CStr>>& GetWeapons() const;
    bool IncludesWeapon(const CStr& gunPartName) const;
    int AddWeapon(const CStr& gunPartName);
    int RemoveWeapon(const CStr& gunPartName);
    void Clear();
    bool IsValid() const;
    bool IsEmpty() const;
    bool CanFire() const;
    void KeepFire();
    void Reload();
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;

protected:
    WeaponGroup();
    WeaponGroup(const WeaponGroup& rhs);

public:
    virtual ~WeaponGroup() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classWeaponGroup;
    void SetGroupId(int groupId);
    static Impulse __fastcall GetImpulseByGroupId(int groupId);
    void ClearWeapons();
    /* 0x0034 */ int m_groupId;
    /* 0x0038 */ Impulse m_impulseId;
    /* 0x003c */ std::set<CStr, std::less<CStr>, std::allocator<CStr>> m_gunPartNames;
}; /* size: 0x0048 */
