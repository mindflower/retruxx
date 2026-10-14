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
        float m_space;
        CStr m_paneName;
        CStr m_selectPaneName;
        unsigned int m_textColor;
        AuxInfo(const SaveSellButton::AuxInfo&);
        AuxInfo();
    };

protected:
    ObjectInfo* m_objectInfo;
    m3d::ui::Wnd* m_lblObjectName;
    static SaveSellButton::AuxInfo m_aif;
    bool m_bSelected;
    SaveSellButton();
    SaveSellButton(SaveSellButton const& rhs);

public:
    virtual ~SaveSellButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SaveSellButton);

    // SaveSellList positions the buttons using the shared spacing and repaints
    // them as the selection moves.
    friend class SaveSellList;
};

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
        CStr m_btnNextName;
        CStr m_btnPrevName;
        CStr m_wndEmbossBtnNextName;
        CStr m_wndEmbossBtnPrevName;
        float m_space;
        AuxInfo(const SaveSellList::AuxInfo&);
        AuxInfo();
    };

    using SaveSellButtonVector = std::vector<SaveSellButton*, std::allocator<SaveSellButton*>>;

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    void RecalcLayot();
    void ScrollNext();
    void ScrollPrev();
    void UpdateNextPrevButtonState();
    bool CanScrollNext();
    bool CanScrollPrev();
    void EnsureShowButton(int idx);
    void SelectButton(int idx);
    m3d::ui::ButtonWnd* m_btnNext;
    m3d::ui::ButtonWnd* m_btnPrev;
    SaveSellList::AuxInfo m_aif;
    std::vector<SaveSellButton*, std::allocator<SaveSellButton*>> m_saveButtons;
    BoundsBase<float> m_listBounds;
    int m_firstSaveBtnId;
    int m_lastSaveBtnId;
    int m_selBtnId;
    SaveSellList();
    SaveSellList(SaveSellList const& rhs);

public:
    virtual ~SaveSellList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(SaveSellList);
};
