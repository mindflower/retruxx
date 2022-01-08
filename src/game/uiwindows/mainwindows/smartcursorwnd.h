#pragma once

class SmartCursorWnd :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual ~SmartCursorWnd();
    virtual struct m3d::Class * GetClass() const ;
protected:
    SmartCursorWnd(class SmartCursorWnd const &);
    SmartCursorWnd();
    virtual void OnNewFrame();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    virtual void ClearDrawInfo();
    virtual void DrawTexture(struct m3d::ui::DrawInfo const &,struct AuxDrawInfo const *) const ;
private:
    std::fill<AuxDrawInfo * *,AuxDrawInfo *>(AuxDrawInfo * *,AuxDrawInfo * *,AuxDrawInfo * const &);
    std::allocator<AuxDrawInfo *>::allocator<AuxDrawInfo *>();
    std::allocator<AuxDrawInfo *>::allocator<AuxDrawInfo *>();
    std::copy_backward<AuxDrawInfo * *,AuxDrawInfo * *>(AuxDrawInfo * *,AuxDrawInfo * *,AuxDrawInfo * *);
    std::_Ptr_cat<AuxDrawInfo>(AuxDrawInfo * *,AuxDrawInfo * *);
    std::_Copy_backward_opt<AuxDrawInfo * *,AuxDrawInfo * *>(AuxDrawInfo * *,AuxDrawInfo * *,AuxDrawInfo * *,std::_Scalar_ptr_iterator_tag);
    std::_Uninit_copy<AuxDrawInfo *,AuxDrawInfo *>(AuxDrawInfo * *,AuxDrawInfo * *,AuxDrawInfo * *,std::allocator<AuxDrawInfo *> &,std::_Scalar_ptr_iterator_tag);
    std::_Allocate<AuxDrawInfo *>(uint,AuxDrawInfo * *);
    std::vector<SmartCursorWnd::AuxDrawInfo *> m_drawInfo;
};
