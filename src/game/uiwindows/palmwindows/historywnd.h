#pragma once

class HistoryWnd :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    int AddRecord(CStr const &,class m3d::AIParam const &);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~HistoryWnd();
protected:
    virtual int GameDataSetup();
    HistoryWnd();
    HistoryWnd(class HistoryWnd const &);
    virtual int GameDataSave(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    virtual int OnAfterAddToWndStation();
    virtual int GameDataLoad(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *);
    virtual int GameDataClear(bool);
private:
    std::_Ptr_cat<Record *,Record *>(Record * &,Record * &);
    std::copy_backward<Record *,Record *>(Record *,Record *,Record *);
    std::_Destroy<Record>(Record *);
    std::allocator<Record>::allocator<Record>();
    std::allocator<Record>::allocator<Record>();
    std::fill<Record *,Record>(Record *,Record *,Record const &);
    std::_Construct<Record,Record>(Record *,Record const &);
    std::_Allocate<Record>(uint,Record *);
    std::_Copy_backward_opt<Record *,Record *>(Record *,Record *,Record *,std::_Nonscalar_ptr_iterator_tag);
    m3d::ui::TextBoxWnd *m_txt;
    HistoryWnd::AuxInfo m_aif;
    std::vector<HistoryWnd::Record> m_records;
};
