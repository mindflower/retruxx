#pragma once
#include <map>
#include <ui/ui.h>
#include <vector>

enum
{
    INFOFLAG_FULL_ROW = 1,
    INFOFLAG_BIG_FONT = 2,
    INFOFLAG_CENTER = 4,
    INFOFLAG_SELECTED_COLOR = 8,
    INFOFLAG_WARN_COLOR = 16,
    INFOFLAG_BIG_SPACE = 32,
    INFOFLAG_TITLE = 43,
};

class InfoWnd : public m3d::ui::Wnd
{
public:
    class InfoItem
    {
    public:
        InfoItem(InfoWnd::InfoItem const& rhs);
        InfoItem(int flags, CStr const& string, m3d::rend::TexHandle ico, PointBase<float> icoSize);
        ~InfoItem();
        InfoWnd::InfoItem& operator=(InfoWnd::InfoItem const&);
        void Draw(m3d::ui::DrawInfo const& di, BoundsBase<float> const& rc) const;
        PointBase<float> GetDrawSize() const;
        void SetString(CStr const string);
        void SetIco(m3d::rend::TexHandle icoId);
        void SetIcoSize(PointBase<float> const& sz);
        void SetFlags(int flags);
        CStr const& GetString() const;
        m3d::rend::TexHandle GetIco() const;
        PointBase<float> const& GetIcoSize() const;
        int GetFlags() const;
        int GetId() const;

        struct AuxInfo
        {
            PointBase<float> m_defIcoSize;
            float m_space;
            CStr m_strDefaultColor;
            CStr m_strGreenColor;
            CStr m_strRedColor;
            float m_maxStringW;
            AuxInfo(InfoWnd::InfoItem::AuxInfo const&);
            AuxInfo();
        };

    private:
        // InfoWnd measures and positions the items it owns.
        friend class InfoWnd;

        void CalculateStringSize();
        void SetId(int id);
        CStr m_string;
        PointBase<float> m_icoSize;
        m3d::rend::TexHandle m_ico;
        int m_flags;
        PointBase<float> m_auxStringSize;
        int m_id;
        static InfoWnd::InfoItem::AuxInfo m_aif;
    };

    struct AuxInfo
    {
        float m_edgeSpace;
        float m_xSpace;
        float m_ySpace;
        float m_titleYSpace;
        AuxInfo();
    };

public:
    int AddItemById(int id, InfoWnd::InfoItem const& item);
    InfoWnd::InfoItem* GetItemById(int id);
    void SetNumberOfColumns(int number);
    int GetNumberOfColumns() const;

    struct RowInfo
    {
        float m_y0;
        float m_h;
    };

    struct ColumnInfo
    {
        float m_x0;
        float m_w;
    };

    using InfoItemVector = std::vector<InfoWnd::InfoItem, std::allocator<InfoWnd::InfoItem>>;
    using RowInfoVector = std::vector<InfoWnd::RowInfo, std::allocator<InfoWnd::RowInfo>>;
    using ColumnInfoVector = std::vector<InfoWnd::ColumnInfo, std::allocator<InfoWnd::ColumnInfo>>;
    using ItemPositionMap =
        std::map<int, PointBase<int>, std::less<int>, std::allocator<std::pair<int const, PointBase<int>>>>;
    using ItemPositionPair = std::pair<int, PointBase<int>>;

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    void RecalcLayot(
        m3d::ui::DrawInfo const& srcDi,
        m3d::ui::DrawInfo& newDi,
        std::vector<InfoWnd::RowInfo, std::allocator<InfoWnd::RowInfo>>& rowsInfo,
        std::vector<InfoWnd::ColumnInfo, std::allocator<InfoWnd::ColumnInfo>>& columnsInfo,
        std::map<int, PointBase<int>, std::less<int>, std::allocator<std::pair<int const, PointBase<int>>>>&
            itemPositions);
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    std::vector<InfoWnd::InfoItem, std::allocator<InfoWnd::InfoItem>> m_items;
    int m_numberOfColumns;
    static InfoWnd::AuxInfo m_aif0;
    InfoWnd();
    InfoWnd(InfoWnd const& rhs);

public:
    virtual ~InfoWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(InfoWnd);
};
