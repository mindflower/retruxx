#pragma once

class WeaponGroupManager :  public m3d::Object
{
public:
    int DeleteWeaponGroup(int);
    void ClearSavedGroups();
    int ValidateWeaponGroups();
    void GetAllWeapons(class std::set<CStr,struct std::less<CStr>,class std::allocator<CStr> > &) const ;
    void SaveWeaponGroups();
    virtual class m3d::Object * Clone();
    class WeaponGroup * GetWeaponGroupById(int) const ;
    void KeepFire();
    int GameDataUpdate(void *,int);
    class WeaponGroup * CreateWeaponGroup(int);
    int LoadFromXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);
    static int __fastcall GetWeaponGroupsMaxCount();
    static int __fastcall GetDefaultWeaponGroupIdForWeapon(CStr const &);
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    int AddWeaponGroup(class WeaponGroup *);
    void ReloadAllWeapon();
    void OnPlayerVehicleChanged();
    int Init();
    int AddWeaponToWeaponGroup(CStr const &,int);
    int AddWeaponToWeaponGroup(int,int);
    static CStr __fastcall GetGunPartNameByGunId(int);
    void RestoreWeaponGroups();
    virtual ~WeaponGroupManager();
    void OnVehiclePartChanged(void *);
    int SaveToXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *) const ;
    void ClearGroups();
    int RemoveWeaponFromWeaponGroup(int);
    int RemoveWeaponFromWeaponGroup(CStr const &);
    virtual struct m3d::Class * GetClass() const ;
    int GetWeaponGroupIdForWeapon(CStr const &) const ;
    void Clear();
protected:
    WeaponGroupManager(class WeaponGroupManager const &);
    WeaponGroupManager();
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
    int SaveToXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *) const ;
    bool CanFire() const ;
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    static enum Impulse __fastcall GetImpulseByGroupId(int);
    static class m3d::Object * CreateObject();
    int AddWeapon(CStr const &);
    void Clear();
    int GetGroupId() const ;
    virtual struct m3d::Class * GetClass() const ;
    enum Impulse GetImpulseId() const ;
    void KeepFire();
    int RemoveWeapon(CStr const &);
    int LoadFromXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);
    bool IncludesWeapon(CStr const &) const ;
    class WeaponGroup & operator=(class WeaponGroup const &);
    void SetGroupId(int);
    virtual ~WeaponGroup();
    class std::set<CStr,struct std::less<CStr>,class std::allocator<CStr> > const & GetWeapons() const ;
protected:
    WeaponGroup();
    WeaponGroup(class WeaponGroup const &);
private:
    int m_groupId;
    Impulse m_impulseId;
    std::set<CStr> m_gunPartNames;
};
