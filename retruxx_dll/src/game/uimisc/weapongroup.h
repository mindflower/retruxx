#pragma once
#include <map>
#include <core/clazz.h>

class WeaponGroupManager :  public m3d::Object
{
public:
    int DeleteWeaponGroup(int);
    void ClearSavedGroups();
    int ValidateWeaponGroups();
    void GetAllWeapons(std::set<CStr> &) const ;
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
    std::map<int,WeaponGroup *> m_weaponGroups;
    std::map<int,WeaponGroup *> m_savedWeaponGroups;
};

class WeaponGroup :  public m3d::Object
{
public:
    void Reload();
    bool IsEmpty() const ;
    void ClearWeapons();
    bool IsValid() const ;
    int SaveToXml(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *) const ;
    bool CanFire() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    static enum Impulse __fastcall GetImpulseByGroupId(int);
    static m3d::Object * CreateObject();
    int AddWeapon(CStr const &);
    void Clear();
    int GetGroupId() const ;
    virtual m3d::Class * GetClass() const ;
    Impulse GetImpulseId() const ;
    void KeepFire();
    int RemoveWeapon(CStr const &);
    int LoadFromXml(m3d::cmn::XmlFile *, m3d::cmn::XmlNode const *);
    bool IncludesWeapon(CStr const &) const ;
    WeaponGroup & operator=(WeaponGroup const &);
    void SetGroupId(int);
    virtual ~WeaponGroup();
    std::set<CStr, std::less<CStr>, std::allocator<CStr> > const & GetWeapons() const ;

protected:
    WeaponGroup();
    WeaponGroup(WeaponGroup const &);

public:
    RT_CLASS_DECLARE(WeaponGroup);

private:
    int m_groupId;
    Impulse m_impulseId;
    std::set<CStr> m_gunPartNames;
};
