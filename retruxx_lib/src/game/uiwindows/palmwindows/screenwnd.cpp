#include "screenwnd.h"

#include "core/kernel.h"

RT_CLASS_EXPORTS_BEGIN(ScreenWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ScreenWnd);

m3d::Class* ScreenWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ScreenWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ScreenWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ScreenWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

ScreenWnd::~ScreenWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ScreenWnd::ScreenWnd() = default;

ScreenWnd::ScreenWnd(ScreenWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ScreenWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ScreenWnd::AddChild(m3d::Object* w)
{
    auto res = Wnd::AddChild(w);
    if (res)
    {
        M3D_ASSERT(IS_KIND_OF(w, Wnd));

        auto* wnd = RT_DYNCAST(w, Wnd);
        wnd->SetStyle(wnd->GetStyle() & 0xCFu);

        // Process children using iterative DFS
        std::vector<m3d::Object*> stack;
        stack.push_back(dynamic_cast<m3d::Object*>(wnd));

        while (!stack.empty())
        {
            m3d::Object* current = stack.back();
            stack.pop_back();

            // Process all siblings of the current node
            auto* sibling = dynamic_cast<Wnd*>(current->GetFirstChild());
            while (sibling)
            {
                sibling->SetStyle(sibling->GetStyle() & ~0x10u);

                // If this sibling has children, add to stack for processing
                if (sibling->GetFirstChild()) {
                    stack.push_back(sibling->GetFirstChild());
                }

                // Move to next sibling
                sibling = dynamic_cast<Wnd*>(sibling->GetNextSibling());
            }
        }
    }
    return res;
}
