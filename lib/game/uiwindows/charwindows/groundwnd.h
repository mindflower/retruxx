#pragma once
#include "childpanel.h"
#include <core/ref_ptr.h>

namespace ai
{
    class GeomRepository;
}

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
        class ButtonWnd;
    }
}

class RepositoryWnd;

class GroundWnd : public ChildPanel
{
public:
    class AuxInfo
    {
        friend class GroundWnd;

    public:
        AuxInfo();

    private:
        CStr m_wndPictureName;
        CStr m_wndRepositoryName;
        CStr m_btnPickUpAllName;
        CStr m_wndGroundPictureName;
        CStr m_groundPictureTexName;
    };

public:
    virtual void SetVehicleId(int);
    static m3d::Class * GetBaseClass();
    void PickUpAll();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual ~GroundWnd();

protected:
    virtual int OnAfterAddToWndStation();
    void SetupRepository();
    void OnRepositoryChanged(void *);
    void OnStartLevel();
    ai::GeomRepository * GetGroundRepository() const ;
    virtual int GameDataClear(bool);
    GroundWnd();
    GroundWnd(GroundWnd const &);
    void UpdatePickupButtonState();
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void UpdateGroundPicture();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);

public:
    RT_CLASS_DECLARE(GroundWnd);

private:
    GroundWnd::AuxInfo m_aif;
    ref_ptr<RepositoryWnd> m_wndRepository;
    m3d::ui::ButtonWnd *m_btnPickUpAll;
    m3d::ui::ImageWnd *m_wndGroundPicture;
};
