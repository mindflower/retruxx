#pragma once

namespace ai
{
    class ai::NailLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        NailLocationPrototypeInfo(void);
        virtual class ai::Obj* CreateTargetObject(void) const;
    protected:
    private:
    };

    class ai::NailLocation : public ai::TemporaryLocation
    {
    public:
        static struct m3d::Class* __fastcall GetBaseClass(void);
        NailLocation(class ai::NailLocationPrototypeInfo const&);
        virtual class ai::NailLocationPrototypeInfo const* GetPrototypeInfo(void) const;
        virtual struct m3d::Class* GetClass(void) const;
    protected:
        virtual ~NailLocation(void);
        virtual void OnObjectIn(class ai::Obj*);
        virtual void OnObjectOut(class ai::Obj*);
    private:
        static class m3d::Object* __fastcall CreateObject(void);
        virtual class m3d::Object* Clone(void);
    };
}
