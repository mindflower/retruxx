#include "bindkeyswnd.h"

RT_CLASS_DEFINE(BindKeysWnd);

BindKeysWnd::KeySetButton::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

BindKeysWnd::BindKeysItem::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

BindKeysWnd::BindKeysList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

BindKeysWnd::AuxInfo::AuxInfo()
{
}

m3d::Object* BindKeysWnd::CreateObject()
{
    return new BindKeysWnd;
}

m3d::Class* BindKeysWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BindKeysWnd);
}

m3d::Class* BindKeysWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* BindKeysWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

BindKeysWnd::~BindKeysWnd()
{
    throw std::logic_error("Not implemented");
}

int BindKeysWnd::ApplyChanges(bool)
{
    throw std::logic_error("Not implemented");
}

int BindKeysWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int BindKeysWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void BindKeysWnd::LoadDefaultBindings()
{
    throw std::logic_error("Not implemented");
}

int BindKeysWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

BindKeysWnd::BindKeysWnd(BindKeysWnd const&)
{
    throw std::logic_error("Not implemented");
}

BindKeysWnd::BindKeysWnd()
{
}

int BindKeysWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void BindKeysWnd::ApplyBindings()
{
    throw std::logic_error("Not implemented");
}

void BindKeysWnd::CancelBindings()
{
    throw std::logic_error("Not implemented");
}
