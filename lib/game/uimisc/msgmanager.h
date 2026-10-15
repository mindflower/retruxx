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

    std::map<int, MsgInfo*, std::less<int>, std::allocator<std::pair<int const, MsgInfo*>>> m_globalMsgs;
    std::map<int, MsgInfo*, std::less<int>, std::allocator<std::pair<int const, MsgInfo*>>> m_levelMsgs;

    MsgManager();
    MsgManager(MsgManager const& rhs);

public:
    virtual ~MsgManager() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMsgManager;
};
