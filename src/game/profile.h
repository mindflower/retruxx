#pragma once
#include <core/clazz.h>

class Profile : public m3d::Object
{
public:
    int LoadFromXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);
    class CStr const & GetFolder() const ;
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    void SetFolder(class CStr const &);
    virtual class m3d::Object * Clone();
    int GetParam(enum ProfileParam,class m3d::AIParam &) const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    void SetName(class CStr const &);
    int SaveToXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *) const ;
    bool IsValid() const ;
    int SetParam(enum ProfileParam,class m3d::AIParam const &);
    virtual ~Profile();
    class CStr const & GetName() const ;
protected:
    int SetParamsDefault();
    void Clear();
    Profile(class Profile const &);
    Profile();
    enum ProfileParam ParamName2Id(class CStr const &) const ;
    class CStr ParamId2Name(enum ProfileParam) const ;
private:
    CStr m_name;
    CStr m_folder;
    std::map<enum ProfileParam,m3d::AIParam> m_params;
};

class ProfileManager :  public m3d::Object
{
public:
    class Profile * GetCurProfile() const ;
    int SetCurProfile(class CStr const &);
    class CStr GetDefaultProfileName() const ;
    virtual struct m3d::Class * GetClass() const ;
    int DeleteProfile(class CStr const &);
    class std::vector<class CStr,class std::allocator<class CStr> > GetProfilesNames() const ;
    int Done();
    int Init();
    class Profile const * CreateNewProfile(class CStr const &);
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~ProfileManager();
    static class m3d::Object * __fastcall CreateObject();
    int SaveProfile(class Profile const *) const ;
    int LoadProfiles();
    class Profile * GetProfileByName(class CStr const &) const ;
    virtual class m3d::Object * Clone();
protected:
    void Clear();
    class CStr GetProfileOwnFolderName(class CStr const &) const ;
    class Profile const * CreateDefaultProfile();
    int AddProfile(class Profile *);
    ProfileManager(class ProfileManager const &);
    ProfileManager();
    int GetProfileFiles(class std::vector<class CStr,class std::allocator<class CStr> > &) const ;
    class CStr GetProfileFolderName(class CStr const &) const ;
    class CStr GetProfileFilePath(class CStr const &) const ;
    class Profile * _GetProfileByName(class CStr const &) const ;
private:
    std::vector<Profile *> m_profiles;
    CStr m_curProfileName;
    m3d::CVar m_cvPathToProfiles;
    m3d::CVar m_cvProfileFileName;
};
