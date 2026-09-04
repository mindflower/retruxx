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
        /* 0x0000 */ CStr m_txtName;
        /* 0x000c */ unsigned int m_timeColor;
        AuxInfo(HistoryWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0010 */

    struct Record
    {
        /* 0x0000 */ CStr m_strTextId;
        /* 0x000c */ m3d::AIParam m_time;
    }; /* size: 0x0028 */

    using RecordVector = std::vector<HistoryWnd::Record, std::allocator<HistoryWnd::Record>>;

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x110 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x114 */;
    virtual int OnAfterAddToWndStation() override /* 0x6c */;
    /* 0x0220 */ m3d::ui::TextBoxWnd* m_txt;
    /* 0x0224 */ HistoryWnd::AuxInfo m_aif;
    /* 0x0234 */ std::vector<HistoryWnd::Record, std::allocator<HistoryWnd::Record>> m_records;
    HistoryWnd();
    HistoryWnd(HistoryWnd const& rhs);

public:
    virtual ~HistoryWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classHistoryWnd;
}; /* size: 0x0244 */
