#pragma once
#include <map>
#include <core/clazz.h>
#include <renderer/i_renderer.h>
#include <ui/ui.h>

class MsgInfo
{
public:
    enum MsgType
    {
        MSGTYPE_NORMAL = 0x0,
        MSGTYPE_SCROLL = 0x1,
    };

public:
    int GetModelSlot(void) const;
    class m3d::rend::TexHandle GetImageDownOverlay(void) const;
    float GetScrollSpeed(void) const;
    float GetFontSize(void) const;
    int GetTime(void) const;
    int GetId(void) const;
    CStr const& GetMsg(void) const;
    CStr const& GetSoundFileName(void) const;
    MsgInfo(void);
    m3d::rend::TexHandle GetImage(void) const;
    bool IsModelAutosized(void) const;
    CStr const& GetImageUpOverlayFileName(void) const;
    unsigned int GetModelSkin(void) const;
    int LoadFromXml(struct m3d::cmn::XmlFile*, struct m3d::cmn::XmlNode const*);
    m3d::ui::MbFlags GetMbFlags(void) const;
    MsgType GetMsgType(void) const;
    unsigned int GetModelCfg(void) const;
    CStr const& GetImageDownOverlayFileName(void) const;
    CStr const& GetImageFileName(void) const;
    ~MsgInfo(void);
    CStr const& GetModelName(void) const;
    class m3d::rend::TexHandle GetImageUpOverlay(void) const;
protected:
private:
    enum MsgType Str2MsgType(CStr const&) const;
    void AddImage(CStr const&, class m3d::rend::TexHandle&);

private:
    int m_id;
    CStr m_msg;
    CStr m_imageFileName;
    CStr m_imageUpOverlayFileName;
    CStr m_imageDownOverlayFileName;
    m3d::rend::TexHandle m_image;
    m3d::rend::TexHandle m_imageUpOverlay;
    m3d::rend::TexHandle m_imageDownOverlay;
    CStr m_soundFileName;
    int m_time;
    m3d::ui::MbFlags m_mbFlags;
    CStr m_modelName;
    unsigned int m_modelSkin;
    unsigned int m_modelCfg;
    int m_modelSlot;
    bool m_bModelAutosized;
    MsgInfo::MsgType m_msgType;
    float m_fontSize;
    float m_scrollSpeed;
};

class MsgManager :  public m3d::Object
{
public:
    int ShowMsgBox(int,bool);
    void Clear(bool);
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    MsgInfo const * GetMsgInfo(int) const ;
    static m3d::Object * CreateObject();
    virtual ~MsgManager();
    virtual m3d::Object * Clone();

protected:
    int AddMsg(MsgInfo *,bool);
    MsgManager(MsgManager const &);
    MsgManager();
    int LoadFromXml(CStr const &,bool);

public:
    RT_CLASS_DECLARE(MsgManager);

private:
    std::map<int,MsgInfo *> m_globalMsgs;
    std::map<int,MsgInfo *> m_levelMsgs;
};
