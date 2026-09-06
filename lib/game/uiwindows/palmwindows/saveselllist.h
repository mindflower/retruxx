#pragma once
#include <ui/button.h>
#include <ui/ui.h>
#include <vector>

class ObjectInfo;

class SaveSellButton : public m3d::ui::ButtonWnd
{
public:
    int SetUp(ObjectInfo* objectInfo, float maxHeight, int id);
    ObjectInfo* GetObjectInfo() const;
    void Select(bool bSelect);
    bool IsSelected() const;

    struct AuxInfo
    {
        /* 0x0000 */ float m_space;
        /* 0x0004 */ CStr m_paneName;
        /* 0x0010 */ CStr m_selectPaneName;
        /* 0x001c */ unsigned int m_textColor;
        AuxInfo(const SaveSellButton::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0020 */

protected:
    /* 0x023c */ ObjectInfo* m_objectInfo;
    /* 0x0240 */ m3d::ui::Wnd* m_lblObjectName;
    static SaveSellButton::AuxInfo m_aif;
    /* 0x0244 */ bool m_bSelected;
    SaveSellButton();
    SaveSellButton(SaveSellButton const& rhs);

public:
    virtual ~SaveSellButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(SaveSellButton);

    // SaveSellList positions the buttons using the shared spacing and repaints
    // them as the selection moves.
    friend class SaveSellList;
}; /* size: 0x0248 */

class SaveSellList : public m3d::ui::Wnd
{
    using ObjectInfoVector = std::vector<ObjectInfo*, std::allocator<ObjectInfo*>>;

public:
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    int AddSaveButton(ObjectInfo* objectInfo);
    int RemoveSaveButton(ObjectInfo* objectInfo);
    int GetSaveButtonIdByObjectInfo(ObjectInfo* objectInfo) const;
    ObjectInfo* GetObjectInfoBySaveButtonId(int idx) const;
    void ShowAtBegin();
    void ShowAtEnd();
    int GetCurSel() const;
    void SetCurSel(int selIdx);
    void GetSavedObjectInfos(std::vector<ObjectInfo*, std::allocator<ObjectInfo*>>& objectInfos) const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_btnNextName;
        /* 0x000c */ CStr m_btnPrevName;
        /* 0x0018 */ CStr m_wndEmbossBtnNextName;
        /* 0x0024 */ CStr m_wndEmbossBtnPrevName;
        /* 0x0030 */ float m_space;
        AuxInfo(const SaveSellList::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0034 */

    using SaveSellButtonVector = std::vector<SaveSellButton*, std::allocator<SaveSellButton*>>;

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x110 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x114 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    void RecalcLayot();
    void ScrollNext();
    void ScrollPrev();
    void UpdateNextPrevButtonState();
    bool CanScrollNext();
    bool CanScrollPrev();
    void EnsureShowButton(int idx);
    void SelectButton(int idx);
    /* 0x0220 */ m3d::ui::ButtonWnd* m_btnNext;
    /* 0x0224 */ m3d::ui::ButtonWnd* m_btnPrev;
    /* 0x0228 */ SaveSellList::AuxInfo m_aif;
    /* 0x025c */ std::vector<SaveSellButton*, std::allocator<SaveSellButton*>> m_saveButtons;
    /* 0x026c */ BoundsBase<float> m_listBounds;
    /* 0x027c */ int m_firstSaveBtnId;
    /* 0x0280 */ int m_lastSaveBtnId;
    /* 0x0284 */ int m_selBtnId;
    SaveSellList();
    SaveSellList(SaveSellList const& rhs);

public:
    virtual ~SaveSellList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(SaveSellList);
}; /* size: 0x0288 */
