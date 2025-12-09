#pragma once
#include <ui/ui.h>

class InfoWnd :  public m3d::ui::Wnd
{
public:
    class InfoItem
    {
    public:
        ~InfoItem();
        void Draw(m3d::ui::DrawInfo const&, BoundsBase<float> const&) const;
        void SetFlags(int);
        int GetFlags() const;
        void SetString(CStr);
        int GetId() const;
        void SetIco(m3d::rend::TexHandle);
        void SetIcoSize(PointBase<float> const&);
        CStr const& GetString() const;
        InfoItem(InfoWnd::InfoItem const&);
        InfoItem(int, CStr const&, m3d::rend::TexHandle, PointBase<float>);
        InfoWnd::InfoItem& operator=( InfoWnd::InfoItem const&);
        m3d::rend::TexHandle GetIco() const;
        PointBase<float> const& GetIcoSize() const;
        PointBase<float> GetDrawSize() const;

    private:
        void CalculateStringSize(void);
        void SetId(int);

    private:
        CStr m_string;
        PointBase<float> m_icoSize;
        m3d::rend::TexHandle m_ico;
        int m_flags;
        PointBase<float> m_auxStringSize;
        int m_id;
    };

public:
    InfoItem * GetItemById(int);
    static m3d::Object * CreateObject();
    int AddItemById(int,class InfoItem const &);
    virtual ~InfoWnd();
    virtual m3d::Object * Clone();
    void SetNumberOfColumns(int);

protected:
    InfoWnd();
    InfoWnd(InfoWnd const &);
    virtual int GameDataClear(bool);
    virtual int OnPaint(m3d::ui::DrawInfo const &);

public:
    RT_CLASS_DECLARE(InfoWnd);

private:
    std::vector<InfoWnd::InfoItem> m_items;
    int m_numberOfColumns;
};
