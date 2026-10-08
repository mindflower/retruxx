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
            /* 0x0000 */ PointBase<float> m_defIcoSize;
            /* 0x0008 */ float m_space;
            /* 0x000c */ CStr m_strDefaultColor;
            /* 0x0018 */ CStr m_strGreenColor;
            /* 0x0024 */ CStr m_strRedColor;
            /* 0x0030 */ float m_maxStringW;
            AuxInfo(InfoWnd::InfoItem::AuxInfo const&);
            AuxInfo();
        }; /* size: 0x0034 */

    private:
        // InfoWnd measures and positions the items it owns.
        friend class InfoWnd;

        void CalculateStringSize();
        void SetId(int id);
        /* 0x0000 */ CStr m_string;
        /* 0x000c */ PointBase<float> m_icoSize;
        /* 0x0014 */ m3d::rend::TexHandle m_ico;
        /* 0x0018 */ int m_flags;
        /* 0x001c */ PointBase<float> m_auxStringSize;
        /* 0x0024 */ int m_id;
        static InfoWnd::InfoItem::AuxInfo m_aif;
    }; /* size: 0x0028 */

    struct AuxInfo
    {
        /* 0x0000 */ float m_edgeSpace;
        /* 0x0004 */ float m_xSpace;
        /* 0x0008 */ float m_ySpace;
        /* 0x000c */ float m_titleYSpace;
        AuxInfo();
    }; /* size: 0x0010 */

public:
    int AddItemById(int id, InfoWnd::InfoItem const& item);
    InfoWnd::InfoItem* GetItemById(int id);
    void SetNumberOfColumns(int number);
    int GetNumberOfColumns() const;

    struct RowInfo
    {
        /* 0x0000 */ float m_y0;
        /* 0x0004 */ float m_h;
    }; /* size: 0x0008 */

    struct ColumnInfo
    {
        /* 0x0000 */ float m_x0;
        /* 0x0004 */ float m_w;
    }; /* size: 0x0008 */

    using InfoItemVector = std::vector<InfoWnd::InfoItem, std::allocator<InfoWnd::InfoItem>>;
    using RowInfoVector = std::vector<InfoWnd::RowInfo, std::allocator<InfoWnd::RowInfo>>;
    using ColumnInfoVector = std::vector<InfoWnd::ColumnInfo, std::allocator<InfoWnd::ColumnInfo>>;
    using ItemPositionMap =
        std::map<int, PointBase<int>, std::less<int>, std::allocator<std::pair<int const, PointBase<int>>>>;
    using ItemPositionPair = std::pair<int, PointBase<int>>;

protected:
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    void RecalcLayot(
        m3d::ui::DrawInfo const& srcDi,
        m3d::ui::DrawInfo& newDi,
        std::vector<InfoWnd::RowInfo, std::allocator<InfoWnd::RowInfo>>& rowsInfo,
        std::vector<InfoWnd::ColumnInfo, std::allocator<InfoWnd::ColumnInfo>>& columnsInfo,
        std::map<int, PointBase<int>, std::less<int>, std::allocator<std::pair<int const, PointBase<int>>>>&
            itemPositions);
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    /* 0x0220 */ std::vector<InfoWnd::InfoItem, std::allocator<InfoWnd::InfoItem>> m_items;
    /* 0x0230 */ int m_numberOfColumns;
    static InfoWnd::AuxInfo m_aif0;
    InfoWnd();
    InfoWnd(InfoWnd const& rhs);

public:
    virtual ~InfoWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(InfoWnd);
}; /* size: 0x0234 */
