#pragma once

namespace ai
{
    class ai::SmokeScreenLocationPrototypeInfo : public ai::TemporaryLocationPrototypeInfo
    {
    public:
        SmokeScreenLocationPrototypeInfo(void);
        virtual class ai::Obj* CreateTargetObject(void) const;
    protected:
    private:

    };
    class ai::SmokeScreenLocation : public ai::TemporaryLocation
    {
    public:
        virtual struct m3d::Class* GetClass(void) const;
        virtual class ai::SmokeScreenLocationPrototypeInfo const* GetPrototypeInfo(void) const;
        static struct m3d::Class* GetBaseClass(void);
        SmokeScreenLocation(class ai::SmokeScreenLocationPrototypeInfo const&);
    protected:
        virtual void OnObjectOut(class ai::Obj*);
        virtual ~SmokeScreenLocation(void);
        virtual void OnObjectIn(class ai::Obj*);
    private:
        static class m3d::Object* CreateObject(void);
        virtual class m3d::Object* Clone(void);
    };
}
