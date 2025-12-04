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
        MSGTYPE_NORMAL = 0,
        MSGTYPE_SCROLL = 1,
    };

public:
    MsgInfo(const MsgInfo&);
    MsgInfo();
    ~MsgInfo();
    int GetId() const;
    int GetParentId() const;
    const CStr& GetMsg() const;
    const CStr& GetImageFileName() const;
    const CStr& GetImageUpOverlayFileName() const;
    const CStr& GetImageDownOverlayFileName() const;
    m3d::rend::TexHandle GetImage() const;
    m3d::rend::TexHandle GetImageUpOverlay() const;
    m3d::rend::TexHandle GetImageDownOverlay() const;
    const CStr& GetSoundFileName() const;
    int GetTime() const;
    m3d::ui::MbFlags GetMbFlags() const;
    const CStr& GetModelName() const;
    unsigned int GetModelSkin() const;
    unsigned int GetModelCfg() const;
    int GetModelSlot() const;
    bool IsModelAutosized() const;
    MsgType GetMsgType() const;
    float GetFontSize() const;
    float GetScrollSpeed() const;
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);

private:
    MsgInfo::MsgType Str2MsgType(const CStr& str) const;
    void AddImage(const CStr& fileName, m3d::rend::TexHandle& tex);
    /* 0x0000 */ int m_id;
    /* 0x0004 */ CStr m_msg;
    /* 0x0010 */ CStr m_imageFileName;
    /* 0x001c */ CStr m_imageUpOverlayFileName;
    /* 0x0028 */ CStr m_imageDownOverlayFileName;
    /* 0x0034 */ m3d::rend::TexHandle m_image;
    /* 0x0038 */ m3d::rend::TexHandle m_imageUpOverlay;
    /* 0x003c */ m3d::rend::TexHandle m_imageDownOverlay;
    /* 0x0040 */ CStr m_soundFileName;
    /* 0x004c */ int m_time;
    /* 0x0050 */ m3d::ui::MbFlags m_mbFlags;
    /* 0x0054 */ CStr m_modelName;
    /* 0x0060 */ unsigned int m_modelSkin;
    /* 0x0064 */ unsigned int m_modelCfg;
    /* 0x0068 */ int m_modelSlot;
    /* 0x006c */ bool m_bModelAutosized;
    /* 0x006d */ char Padding_235[3];
    /* 0x0070 */ MsgInfo::MsgType m_msgType;
    /* 0x0074 */ float m_fontSize;
    /* 0x0078 */ float m_scrollSpeed;
}; /* size: 0x007c */

class MsgManager :  public m3d::Object
{
public:
    int ShowMsgBox(int,bool);
    int Init(bool);
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
