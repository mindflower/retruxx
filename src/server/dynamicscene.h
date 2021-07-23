#pragma once
#include <core/clazz.h>
#include <deque>
#include <vector>

struct dContact;

namespace ai
{
    class Vehicle;
    class Bullet;
    class PhysicBody;
    class Shell;
    class Obj;

    class TraceLineCallback
    {
    public:
        virtual ~TraceLineCallback();

    private:
        //TraceLineCallback_vtbl* __vftable /*VFT*/;
    };

    class DynamicScene :  public m3d::Object
    {
    public:
        class SoilProps
        {
        public:
            void LoadFromXml(m3d::cmn::XmlNode const*);
            SoilProps();

        private:
            __int16 m_splashType;
            CStr m_splashTypeName;
            CStr m_wheelTraceTextureName;
            float m_friction;
            float m_resistance;
            int m_idx;
        };

    public:
        short GetBoEffectTypeByName(CStr const &);
        void DeleteAll();
        void PurgeBodies();
        CStr const & GetBoEffectTypeName(unsigned short);
        virtual m3d::Object * Clone();
        void LinkNodesFromBodyToSceneGraph(Obj *);
        static int __fastcall ProcessShellAndBody(Shell *,PhysicBody *,dContact *,unsigned int &,bool);
        void InitClashDecalId();
        void ReadSoilProps(char const *);
        CStr const & GetShellWaterEffectName(unsigned short) const ;
        bool LoadSceneFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *, std::vector<m3d::Class *> const &);
        void StepScene(float);
        bool SaveSceneToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
        SoilProps const & GetSoilProps(unsigned int,unsigned int) const ;
        CStr const & GetShellStaticsEffectName(unsigned short) const ;
        CStr const & GetSoilEffectName(unsigned int,unsigned short,bool) const ;
        void RenderDebugInfo();
        static m3d::Object * __fastcall CreateObject();
        CStr const & GetBoVehicleEffectName(unsigned short) const ;
        void CollideScene(float);
        CStr const & GetShellVehicleEffectName(unsigned short) const ;
        void CreateBoShellEffectNames();
        CStr const & GetVehicleSoilEffectName(unsigned short) const ;
        static m3d::Class * __fastcall GetBaseClass();
        CStr const & GetDecalName(int);
        static void __fastcall ClearOnce();
        int GetNumNearCallbacksLastFrame();
        int AddDecalName(CStr const &);
        int GetClashDecalId();
        static void __fastcall InitOnce();
        virtual ~DynamicScene();
        unsigned int GetWheelTypeByName(CStr const &);
        CStr const & GetBoShellEffectName(unsigned short,unsigned short);
        static void __fastcall CollideBullet(Bullet const &);
        bool LoadSceneFromFile(char const *,class std::vector<m3d::Class *> const &);
        int ReadNewObjectFromXml(m3d::cmn::XmlFile *, m3d::cmn::XmlNode const *, std::vector<m3d::Class *> const &);
        short GetExplosionType(CStr const &);
        bool SaveSceneToFile(char const *);
        Vehicle * GetVehicleControlledByPlayer() const ;
        CStr const & GetRoadEffectName(unsigned int,bool) const ;
        CStr const & GetShellRoadEffectName(unsigned short) const ;
        static void __fastcall Clear();
        virtual m3d::Class * GetClass() const ;
        CStr const & GetShellEffectName(unsigned short,unsigned short) const ;
        void UpdateSceneItems(float);

    protected:
        DynamicScene(DynamicScene const &);
        DynamicScene();

    private:
        void _InitWheelTraces();
        void _RecalcWheelEffectNames();
        void _AddSoilEffectNameForWheelTypeName(CStr const &);

    private:
        std::vector<SoilProps> m_soilProps;
        std::vector<std::vector<unsigned short>> m_soilPropsIdx;
        std::vector<CStr> m_wheelTypeNames;
        std::vector<std::vector<CStr>> m_soilEffectNames;
        std::vector<CStr> m_roadEffectNames;
        std::vector<CStr> m_soilSplashTypeNames;
        std::vector<CStr> m_shellTypesNames;
        std::vector<std::vector<CStr>> m_shellsEffectsNames;
        std::vector<CStr> m_shellWaterEffectNames;
        std::vector<CStr> m_shellsStaticsEffNames;
        std::vector<CStr> m_shellsVehiclesEffNames;
        std::vector<CStr> m_shellsRoadEffNames;
        std::vector<CStr> m_vehicleSoilEffectNames;
        std::vector<CStr> m_BoEffectTypeNames;
        std::vector<CStr> m_BoVehicleEffectNames;
        std::vector<std::vector<CStr>> m_BoShellEffectNames;
        std::vector<CStr> m_decalsNames;
        int m_clashDecalId;
        float m_physicTimeAccumulator;
        std::deque<int> m_timefilterValues;
    };
}
