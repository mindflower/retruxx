#pragma once
#include <core/aiparam.h>
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}  // namespace m3d

class HistoryWnd : public m3d::ui::Wnd
{
public:
    int AddRecord(CStr const& strTextId, m3d::AIParam const& time);

    struct AuxInfo
    {
        CStr m_txtName;
        unsigned int m_timeColor;
        AuxInfo(HistoryWnd::AuxInfo const&);
        AuxInfo();
    };

    struct Record
    {
        CStr m_strTextId;
        m3d::AIParam m_time;
    };

    using RecordVector = std::vector<HistoryWnd::Record, std::allocator<HistoryWnd::Record>>;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int OnAfterAddToWndStation() override;
    m3d::ui::TextBoxWnd* m_txt;
    HistoryWnd::AuxInfo m_aif;
    std::vector<HistoryWnd::Record, std::allocator<HistoryWnd::Record>> m_records;
    HistoryWnd();
    HistoryWnd(HistoryWnd const& rhs);

public:
    virtual ~HistoryWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classHistoryWnd;
};
