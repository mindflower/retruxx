#include "electronicdigitalwnd.h"

#include <cstdlib>

#include <core/log.h>
#include <game/m3dgame.h>
#include <ui/image.h>

RT_CLASS_EXPORTS_BEGIN(ElectronicDigitalWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ElectronicDigitalWnd);

void ElectronicDigitalWnd::Digital::SetSymbol(ElectronicDigitalWnd::Symbol symbol, bool bReserved)
{
    m_bReserved = bReserved;
    m_symbol = symbol;
    if (m_wnd)
    {
        m_wnd->ShowWindow(symbol != SYMBOL_NUM_SYMBOLS);
    }
    UpdateTexture();
}

ElectronicDigitalWnd::Digital::Digital(
    m3d::ui::ImageWnd* wnd,
    ElectronicDigitalWnd::DigitalSize digitalSize,
    ElectronicDigitalWnd::Symbol symbol)
{
    m_wnd = wnd;
    m_symbol = symbol;
    m_bReserved = false;
    m_digitalSize = digitalSize;
    if (wnd)
    {
        wnd->ShowWindow(symbol != SYMBOL_NUM_SYMBOLS);
    }
}

ElectronicDigitalWnd::Digital::Digital()
{
    m_wnd = nullptr;
    m_symbol = SYMBOL_NUM_SYMBOLS;
    m_bReserved = false;
    m_digitalSize = DIGITAL_SIZE_SMALL;
}

bool ElectronicDigitalWnd::Digital::IsFree() const
{
    return !m_bReserved;
}

m3d::ui::ImageWnd* ElectronicDigitalWnd::Digital::GetWnd() const
{
    return m_wnd;
}

ElectronicDigitalWnd::Symbol ElectronicDigitalWnd::Digital::GetSymbol() const
{
    return m_symbol;
}

void ElectronicDigitalWnd::Digital::SetDigitalSize(ElectronicDigitalWnd::DigitalSize digitalSize)
{
    if (digitalSize != DIGITAL_SIZE_NUM_DIGITAL_SIZES)
    {
        m_digitalSize = digitalSize;
        UpdateTexture();
    }
}

void ElectronicDigitalWnd::Digital::UpdateTexture()
{
    if (!m_wnd)
    {
        return;
    }

    if (m_symbol == SYMBOL_NUM_SYMBOLS || m_digitalSize == DIGITAL_SIZE_NUM_DIGITAL_SIZES)
    {
        m_wnd->SetImage(m3d::rend::TexHandle{});
    }
    else
    {
        m_wnd->SetImage(ElectronicDigitalWnd::m_digitalTextures.m_textures[m_digitalSize][m_symbol]);
    }
}

void ElectronicDigitalWnd::Digital::Show(bool bShow)
{
    if (m_wnd)
    {
        m_wnd->ShowWindow(bShow);
    }
}

m3d::Object* ElectronicDigitalWnd::Clone()
{
    return new ElectronicDigitalWnd;
}

m3d::Class* ElectronicDigitalWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

ElectronicDigitalWnd::~ElectronicDigitalWnd()
{
    m_digitalTextures.Clear();
}

void ElectronicDigitalWnd::SetDigitalColor(unsigned digitalColor)
{
    m_digitalColor = digitalColor;
    for (Digital& digital : m_digitals)
    {
        if (digital.m_wnd)
        {
            digital.m_wnd->SetColor(m_digitalColor);
        }
    }
}

int ElectronicDigitalWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    using namespace m3d::ui;

    if ((m_gameDataFlags & 1) != 0)
    {
        M3D_LOG_INFO("ElectronicDigitalWnd::CreateFromPattern error - window is already inited");
        return 0;
    }

    if (!patternWnd)
    {
        M3D_LOG_INFO("ElectronicDigitalWnd::CreateFromPattern error - invalid patternWnd");
        return 0;
    }

    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("ElectronicDigitalWnd::CreateFromPattern error - cannot find parent for pattern wnd");
        return 0;
    }

    if (Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId()) == 0)
    {
        M3D_LOG_INFO("ElectronicDigitalWnd::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());
    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetTextColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    // Collect the digit image windows: every ImageWnd child of the pattern's parent whose
    // name contains "<patternName>_". They are gathered into a name-sorted map so the digits
    // end up ordered left-to-right, then re-parented (with parent-relative bounds) onto this.
    CStr const baseName = CStr(patternWnd->GetName()) + CStr("_");

    retruxx::map<CStr, m3d::ui::ImageWnd*> digitalWnds;
    for (m3d::Object* child = parent->GetFirstChild(); child; child = child->GetNextSibling())
    {
        if (!IS_KIND_OF(child, ImageWnd))
        {
            continue;
        }

        CStr const childName(child->GetName());
        if (childName.findsubstr(baseName.c_str()) != -1)
        {
            digitalWnds[childName] = static_cast<m3d::ui::ImageWnd*>(child);
        }
    }

    BoundsBase<float> const myBounds = GetBounds();
    for (auto const& kv : digitalWnds)
    {
        m3d::ui::ImageWnd* digitWnd = kv.second;

        m_digitals.emplace_back(digitWnd, m_digitalSize, SYMBOL_NUM_SYMBOLS);

        parent->RemoveChild(digitWnd);

        BoundsBase<float> b = digitWnd->GetBounds();
        b.x0 -= myBounds.x0;
        b.y0 -= myBounds.y0;
        digitWnd->SetBounds(b, true);

        AddChild(digitWnd);
    }

    SetDigitalColor(GetTextColor());

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}

m3d::Object* ElectronicDigitalWnd::CreateObject()
{
    return new ElectronicDigitalWnd;
}

int ElectronicDigitalWnd::ShowNumber(int number, bool bAdd, unsigned int numReservedSlots, bool bPaddFreeSlots)
{
    int result = 1;
    int n = number;
    if (number < 0)
    {
        n = 0;
        result = 0;
    }

    if (!bAdd)
    {
        Clear();
    }

    CStr strNumber;
    int len = strNumber.format("%*d", static_cast<int>(numReservedSlots), n);
    if (len > static_cast<int>(numReservedSlots))
    {
        result = 0;
        strNumber = strNumber.substr(0, static_cast<int>(numReservedSlots));
        len = strNumber.length();
    }

    char const* s = strNumber.c_str();
    for (int i = 0; i < len; ++i)
    {
        Symbol sym = SYMBOL_NUM_SYMBOLS;
        char c = s[i];
        if (c == ' ')
        {
            if (bPaddFreeSlots)
            {
                sym = SYMBOL_0;
            }
        }
        else
        {
            char cs[2] = {c, '\0'};
            unsigned int digit = static_cast<unsigned int>(atoi(cs));
            if (digit > 9)
            {
                result = 0;
            }
            else
            {
                sym = static_cast<Symbol>(digit);
            }
        }

        ShowSymbol(sym, true, 1u, false);
        s = strNumber.c_str();
    }

    return result;
}

int ElectronicDigitalWnd::ShowSymbol(Symbol symbol, bool bAdd, unsigned int numReservedSlots, bool bPaddFreeSlots)
{
    unsigned int const reserved = numReservedSlots ? numReservedSlots : 1u;

    if (!bAdd)
    {
        Clear();
    }

    int const firstFree = GetFirstFreeSlot();
    if (firstFree == -1)
    {
        return 0;
    }

    Symbol const fillSymbol = bPaddFreeSlots ? SYMBOL_0 : SYMBOL_NUM_SYMBOLS;
    for (int idx = firstFree; idx < firstFree + static_cast<int>(reserved) - 1; ++idx)
    {
        m_digitals[idx].SetSymbol(fillSymbol, true);
    }

    int const slot = GetFirstFreeSlot();
    if (slot == -1)
    {
        return 0;
    }

    m_digitals[slot].SetSymbol(symbol, true);
    return 1;
}

void ElectronicDigitalWnd::SetDigitalSize(DigitalSize digitalSize)
{
    if (digitalSize == DIGITAL_SIZE_NUM_DIGITAL_SIZES)
    {
        return;
    }

    m_digitalSize = digitalSize;

    for (size_t i = 0; i < m_digitals.size(); ++i)
    {
        Digital& digital = m_digitals[i];
        digital.m_digitalSize = m_digitalSize;
        digital.UpdateTexture();
    }
}

m3d::Class* ElectronicDigitalWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ElectronicDigitalWnd);
}

void ElectronicDigitalWnd::Clear()
{
    for (Digital& digital : m_digitals)
    {
        digital.m_symbol = SYMBOL_NUM_SYMBOLS;
        digital.m_bReserved = false;
        if (digital.GetWnd())
        {
            digital.GetWnd()->ShowWindow(false);
            digital.UpdateTexture();
        }
    }
}

int ElectronicDigitalWnd::GetFirstFreeSlot() const
{
    for (int i = 0; i < static_cast<int>(m_digitals.size()); ++i)
    {
        if (m_digitals[i].IsFree())
        {
            return i;
        }
    }
    return -1;
}

ElectronicDigitalWnd::ElectronicDigitalWnd()
{
    m_digitalSize = DIGITAL_SIZE_SMALL;
    m_digitalColor = -1;
    m_digitalTextures.Init();
}

int ElectronicDigitalWnd::GameDataClear(bool)
{
    Clear();
    return 1;
}

ElectronicDigitalWnd::DigitalTextures::DigitalTextures()
{
    for (auto& row : m_textures)
    {
        for (auto& tex : row)
        {
            tex.SetInvalid();
        }
    }
    m_refCount = 0;
}

ElectronicDigitalWnd::DigitalTextures::~DigitalTextures()
{
}

int ElectronicDigitalWnd::DigitalTextures::Init()
{
    if (++m_refCount <= 1)
    {
        for (int size = DIGITAL_SIZE_SMALL; size < DIGITAL_SIZE_NUM_DIGITAL_SIZES; ++size)
        {
            for (int sym = 0; sym < SYMBOL_NUM_SYMBOLS; ++sym)
            {
                m3d::rend::TexHandle& tex = m_textures[size][sym];
                tex = M3D_APP->m_pInterfaceManager->GetIcoByName(
                    GetTextureStrId(
                        static_cast<ElectronicDigitalWnd::Symbol>(sym),
                        static_cast<ElectronicDigitalWnd::DigitalSize>(size)),
                    0);
                M3D_RENDERER->ReferenceTexture(tex);
            }
        }
    }
    return 1;
}

void ElectronicDigitalWnd::DigitalTextures::Clear()
{
    if (m_refCount == 1)
    {
        for (auto& row : m_textures)
        {
            for (auto& tex : row)
            {
                M3D_RENDERER->ReleaseTexture(tex);
            }
        }
    }

    if (--m_refCount < 0)
    {
        m_refCount = 0;
    }
}

m3d::rend::TexHandle ElectronicDigitalWnd::DigitalTextures::GetTexture(
    ElectronicDigitalWnd::Symbol symbol,
    ElectronicDigitalWnd::DigitalSize digitalSize) const
{
    if (symbol == SYMBOL_NUM_SYMBOLS || digitalSize == DIGITAL_SIZE_NUM_DIGITAL_SIZES)
    {
        return m3d::rend::TexHandle{};
    }
    return m_textures[digitalSize][symbol];
}

CStr ElectronicDigitalWnd::DigitalTextures::GetTextureStrId(
    ElectronicDigitalWnd::Symbol symbol,
    ElectronicDigitalWnd::DigitalSize digitalSize) const
{
    if (symbol == SYMBOL_NUM_SYMBOLS || digitalSize == DIGITAL_SIZE_NUM_DIGITAL_SIZES)
    {
        return CStr();
    }

    CStr strSymbol;
    switch (symbol)
    {
    case SYMBOL_0:
    case SYMBOL_1:
    case SYMBOL_2:
    case SYMBOL_3:
    case SYMBOL_4:
    case SYMBOL_5:
    case SYMBOL_6:
    case SYMBOL_7:
    case SYMBOL_8:
    case SYMBOL_9:
        strSymbol = CStr(static_cast<int>(symbol));
        break;
    case SYMBOL_COLON:
        strSymbol = "Colon";
        break;
    case SYMBOL_METER:
        strSymbol = "Meter";
        break;
    case SYMBOL_KILOMETER:
        strSymbol = "Kilometer";
        break;
    default:
        break;
    }

    CStr strSize;
    if (digitalSize == DIGITAL_SIZE_SMALL)
    {
        strSize = "Small";
    }
    else if (digitalSize == DIGITAL_SIZE_LARGE)
    {
        strSize = "Large";
    }

    return CStr("Symbol_") + strSymbol + CStr("_") + strSize;
}
