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
    MsgInfo(MsgInfo const&);
    MsgInfo();
    ~MsgInfo();
    int GetId() const;
    int GetParentId() const;
    CStr const& GetMsg() const;
    CStr const& GetImageFileName() const;
    CStr const& GetImageUpOverlayFileName() const;
    CStr const& GetImageDownOverlayFileName() const;
    m3d::rend::TexHandle GetImage() const;
    m3d::rend::TexHandle GetImageUpOverlay() const;
    m3d::rend::TexHandle GetImageDownOverlay() const;
    CStr const& GetSoundFileName() const;
    int GetTime() const;
    m3d::ui::MbFlags GetMbFlags() const;
    CStr const& GetModelName() const;
    unsigned int GetModelSkin() const;
    unsigned int GetModelCfg() const;
    int GetModelSlot() const;
    bool IsModelAutosized() const;
    MsgType GetMsgType() const;
    float GetFontSize() const;
    float GetScrollSpeed() const;
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);

private:
    MsgInfo::MsgType Str2MsgType(CStr const& str) const;
    void AddImage(CStr const& fileName, m3d::rend::TexHandle& tex);
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

class MsgManager : public m3d::Object
{
public:
    MsgInfo const* GetMsgInfo(int msgId) const;
    int Init(bool bGlobal);
    void Clear(bool bGlobal);
    int ShowMsgBox(int msgId, bool pause);

    using MsgMap = std::map<int, MsgInfo*, std::less<int>, std::allocator<std::pair<int const, MsgInfo*>>>;
    using MsgPair = std::pair<int, MsgInfo*>;

protected:
    int LoadFromXml(CStr const& fileName, bool bGlobal);
    int AddMsg(MsgInfo* msgInfo, bool bGlobal);

    /* 0x0034 */ std::map<int, MsgInfo*, std::less<int>, std::allocator<std::pair<int const, MsgInfo*>>> m_globalMsgs;
    /* 0x0040 */ std::map<int, MsgInfo*, std::less<int>, std::allocator<std::pair<int const, MsgInfo*>>> m_levelMsgs;

    MsgManager();
    MsgManager(MsgManager const& rhs);

public:
    virtual ~MsgManager() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classMsgManager;
}; /* size: 0x004c */
