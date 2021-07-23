#pragma once

class WindowResourceInfo :  public ResourceInfo
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~WindowResourceInfo();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual int LoadFromXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);
    virtual bool IsValid() const ;
    virtual class m3d::Object * Clone();
protected:
    WindowResourceInfo(class WindowResourceInfo const &);
    WindowResourceInfo();
private:
    IsValid();
    GetBaseClass();
    int m_wndGuiId;
    CStr m_className;
    bool m_bShowImmediate;
};

class IcoResourceInfo :  public ResourceInfo
{
public:
    virtual int LoadFromXml(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~IcoResourceInfo();
    virtual class m3d::Object * Clone();
    virtual bool IsValid() const ;
protected:
    IcoResourceInfo(class IcoResourceInfo const &);
    IcoResourceInfo();
private:
    LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    Clone();
    CStr m_levelName;
};
