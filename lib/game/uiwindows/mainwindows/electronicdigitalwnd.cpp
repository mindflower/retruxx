#include "electronicdigitalwnd.h"
#include <core/log.h>

RT_CLASS_EXPORTS_BEGIN(ElectronicDigitalWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ElectronicDigitalWnd);

void ElectronicDigitalWnd::Digital::SetSymbol(ElectronicDigitalWnd::Symbol, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ElectronicDigitalWnd::Digital::Digital(
    m3d::ui::ImageWnd*,
    ElectronicDigitalWnd::DigitalSize,
    ElectronicDigitalWnd::Symbol)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ElectronicDigitalWnd::Digital::Digital()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool ElectronicDigitalWnd::Digital::IsFree() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::ui::ImageWnd* ElectronicDigitalWnd::Digital::GetWnd() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ElectronicDigitalWnd::Symbol ElectronicDigitalWnd::Digital::GetSymbol() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ElectronicDigitalWnd::Digital::SetDigitalSize(ElectronicDigitalWnd::DigitalSize)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ElectronicDigitalWnd::Digital::UpdateTexture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ElectronicDigitalWnd::Digital::Show(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ElectronicDigitalWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ElectronicDigitalWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

ElectronicDigitalWnd::~ElectronicDigitalWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ElectronicDigitalWnd::SetDigitalColor(unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ElectronicDigitalWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // TODO: implement ElectronicDigitalWnd::CreateFromPattern
    using namespace m3d::ui;

    if (!patternWnd)
    {
        M3D_LOG_INFO("ElectronicDigitalWnd::CreateFromPattern error - null patternWnd");
        return 0;
    }

    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("ElectronicDigitalWnd::CreateFromPattern error - null parent");
        return 0;
    }

    auto res = Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
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
    SetTextColor(patternWnd->GetColor());
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

    // TODO: !!!

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        // TODO: check this obj delete
        patternWnd->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}

m3d::Object* ElectronicDigitalWnd::CreateObject()
{
    return new ElectronicDigitalWnd;
}

int ElectronicDigitalWnd::ShowNumber(int, bool, unsigned, bool)
{
    // TODO: implement ElectronicDigitalWnd::ShowNumber
    return 1;
}

int ElectronicDigitalWnd::ShowSymbol(Symbol, bool, unsigned, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

int ElectronicDigitalWnd::GetFirstFreeSlot() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ElectronicDigitalWnd::ElectronicDigitalWnd()
{
    m_digitalSize = DIGITAL_SIZE_SMALL;
    m_digitalColor = -1;
    m_digitalTextures.Init();
}

int ElectronicDigitalWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ElectronicDigitalWnd::DigitalTextures::DigitalTextures()
{
    // ok
}

ElectronicDigitalWnd::DigitalTextures::~DigitalTextures()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ElectronicDigitalWnd::DigitalTextures::Init()
{
    // TODO: implement ElectronicDigitalWnd::DigitalTextures::Init
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void ElectronicDigitalWnd::DigitalTextures::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle ElectronicDigitalWnd::DigitalTextures::GetTexture(
    ElectronicDigitalWnd::Symbol symbol,
    ElectronicDigitalWnd::DigitalSize digitalSize) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr ElectronicDigitalWnd::DigitalTextures::GetTextureStrId(
    ElectronicDigitalWnd::Symbol symbol,
    ElectronicDigitalWnd::DigitalSize digitalSize) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
