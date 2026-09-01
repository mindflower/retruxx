#pragma once
#include <core/aiparam.h>
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class TextBoxWnd;
    }
}

class HistoryWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
        friend class HistoryWnd;

    public:
        AuxInfo();

    private:
        CStr m_txtName;
        unsigned int m_timeColor;
    };

    class Record
    {
        friend class HistoryWnd;

    private:
        CStr m_strTextId;
        m3d::AIParam m_time;
    };

public:
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    int AddRecord(CStr const &,m3d::AIParam const &);
    virtual m3d::Class * GetClass() const ;
    virtual ~HistoryWnd();

protected:
    virtual int GameDataSetup();
    HistoryWnd();
    HistoryWnd(HistoryWnd const &);
    virtual int GameDataSave(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    virtual int OnAfterAddToWndStation();
    virtual int GameDataLoad(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(HistoryWnd);

private:
    m3d::ui::TextBoxWnd *m_txt;
    HistoryWnd::AuxInfo m_aif;
    std::vector<HistoryWnd::Record> m_records;
};
