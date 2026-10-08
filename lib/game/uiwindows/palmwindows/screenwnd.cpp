#include "screenwnd.h"

#include "core/kernel.h"

RT_CLASS_EXPORTS_BEGIN(ScreenWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ScreenWnd);

m3d::Class* ScreenWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* ScreenWnd::CreateObject()
{
    return new ScreenWnd();
}

m3d::Object* ScreenWnd::Clone()
{
    return new ScreenWnd();
}

m3d::Class* ScreenWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

ScreenWnd::~ScreenWnd() = default;

ScreenWnd::ScreenWnd() = default;

ScreenWnd::ScreenWnd(ScreenWnd const&) : ScreenWnd()
{
}

int ScreenWnd::GameDataSetup()
{
    return 1;
}

int ScreenWnd::AddChild(m3d::Object* w)
{
    auto res = Wnd::AddChild(w);
    if (!res)
    {
        return res;
    }

    M3D_ASSERT(IS_KIND_OF(w, Wnd));
    auto* wnd = RT_DYNCAST(w, Wnd);

    // Strip WS_ACTIVATION_CAPTURES_FOCUS | WS_ACTIVATABLE (0x3000) from the added
    // window itself, then WS_ACTIVATION_CAPTURES_FOCUS (0x1000) from every
    // descendant (iterative DFS).
    wnd->SetStyle(wnd->GetStyle() & ~0x3000u);

    std::vector<m3d::Object*> stack;
    stack.push_back(wnd);

    while (!stack.empty())
    {
        m3d::Object* current = stack.back();
        stack.pop_back();

        for (auto* child = RT_DYNCAST(current->GetFirstChild(), Wnd); child;
             child = RT_DYNCAST(child->GetNextSibling(), Wnd))
        {
            child->SetStyle(child->GetStyle() & ~0x1000u);
            if (child->GetFirstChild())
            {
                stack.push_back(child);
            }
        }
    }
    return res;
}
