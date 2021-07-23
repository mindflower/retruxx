#pragma once

class IBase
{
protected:
    virtual ~IBase();

private:
    //IBase_vtbl* __vftable /*VFT*/;
};

namespace m3d
{
    class IDeviceResetCallback
    {
    public:
        IDeviceResetCallback();

    private:
       // m3d::IDeviceResetCallback_vtbl* __vftable /*VFT*/;
    };

    namespace input
    {
        class IInput : public IBase
        {
        };
    }
}

namespace snd
{
    class ISound : public IBase
    {
    };
}
