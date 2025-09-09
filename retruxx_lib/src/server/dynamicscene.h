#pragma once
#include <core/clazz.h>
#include <deque>
#include <vector>
#include <engine/ode/sources/objects.h>

struct dContact;

namespace ai
{
    class Vehicle;
    class Bullet;
    class PhysicBody;
    class Shell;
    class Obj;
    class PhysicObj;

    class TraceLineCallback
    {
    public:
        virtual  ~TraceLineCallback() /* 0x00 */;
        virtual bool CollideId(int) const = 0 /* 0x04 */;
        virtual bool CollidePhysicObj(const ai::PhysicObj*) const = 0 /* 0x08 */;
    }; /* size: 0x0004 */

    class DynamicScene : public m3d::Object
    {
    protected:
        DynamicScene();
        DynamicScene(const ai::DynamicScene& __formal);

    public:
        virtual  ~DynamicScene() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classDynamicScene;

        struct SoilProps
        {
            /* 0x0000 */ short m_splashType;
            /* 0x0002 */ char Padding_30[2];
            CStr m_splashTypeName;
            CStr m_wheelTraceTextureName;
            /* 0x001c */ float m_friction;
            /* 0x0020 */ float m_resistance;
            /* 0x0024 */ int m_idx;
            SoilProps(const ai::DynamicScene::SoilProps& __that);
            SoilProps();
            void LoadFromXml(const m3d::cmn::XmlNode* xmlNode);
        }; /* size: 0x0028 */

    private:
        retruxx::vector<ai::DynamicScene::SoilProps, retruxx::allocator<ai::DynamicScene::SoilProps> > m_soilProps;
        retruxx::vector<retruxx::vector<unsigned short, retruxx::allocator<unsigned short> >, retruxx::allocator<retruxx::vector<unsigned short, retruxx::allocator<unsigned short> > > > m_soilPropsIdx;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_wheelTypeNames;
        retruxx::vector<retruxx::vector<CStr, retruxx::allocator<CStr> >, retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr> > > > m_soilEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_roadEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_soilSplashTypeNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_shellTypesNames;
        retruxx::vector<retruxx::vector<CStr, retruxx::allocator<CStr> >, retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr> > > > m_shellsEffectsNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_shellWaterEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_shellsStaticsEffNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_shellsVehiclesEffNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_shellsRoadEffNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_vehicleSoilEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_BoEffectTypeNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_BoVehicleEffectNames;
        retruxx::vector<retruxx::vector<CStr, retruxx::allocator<CStr> >, retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr> > > > m_BoShellEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr> > m_decalsNames;
        /* 0x0144 */ int m_clashDecalId;
        /* 0x0148 */ float m_physicTimeAccumulator;

    public:
        static void __fastcall Clear();
        static void __fastcall InitOnce();
        static void __fastcall ClearOnce();
        static int __fastcall ProcessShellAndBody(ai::Shell* shell, ai::PhysicBody* body, dContact* contact, unsigned int& numContacts, bool reverse);
        static void __fastcall CollideBullet(const ai::Bullet& bullet);
        void DeleteAll();
        void PurgeBodies();
        ai::Vehicle* GetVehicleControlledByPlayer() const;
        bool LoadSceneFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* rootNode, const retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*> >& allowedClasses);
        bool SaveSceneToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* sceneNode);
        bool LoadSceneFromFile(const char* fileName, const retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*> >& allowedClasses);
        bool SaveSceneToFile(const char* fileName);
        int ReadNewObjectFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode, const retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*> >& allowedClasses);
        void ReadSoilProps(const char* fileName);
        const ai::DynamicScene::SoilProps& GetSoilProps(unsigned int x, unsigned int z) const;
        unsigned int GetWheelTypeByName(const CStr& wheelTypeName);
        const CStr& GetSoilEffectName(unsigned int wheelType, unsigned short soilType, bool bVehicleIsBraking) const;
        const CStr& GetRoadEffectName(unsigned int wheelType, bool bVehicleIsBraking) const;
        short GetExplosionType(const CStr& shellTypeName);
        const CStr& GetShellEffectName(unsigned short shellType, unsigned short soilType) const;
        const CStr& GetShellWaterEffectName(unsigned short shellType) const;
        const CStr& GetShellRoadEffectName(unsigned short shellType) const;
        const CStr& GetShellStaticsEffectName(unsigned short shellType) const;
        const CStr& GetShellVehicleEffectName(unsigned short shellType) const;
        const CStr& GetVehicleSoilEffectName(unsigned short splashType) const;
        short GetBoEffectTypeByName(const CStr& BreakableObjectEffectName);
        const CStr& GetBoEffectTypeName(unsigned short BoEffectType);
        void CreateBoShellEffectNames();
        const CStr& GetBoShellEffectName(unsigned short BoEffectType, unsigned short ShellEffectType);
        const CStr& GetBoVehicleEffectName(unsigned short BoEffectType) const;
        int AddDecalName(const CStr& name);
        const CStr& GetDecalName(int id);
        void InitClashDecalId();
        int GetClashDecalId();
        void LinkNodesFromBodyToSceneGraph(ai::Obj* pObj);
        void CollideScene(float elapsedTime);
        void StepScene(float elapsedTime);
        void UpdateSceneItems(float elapsedTime);
        void RenderDebugInfo();
        int GetNumNearCallbacksLastFrame();

    private:
        retruxx::deque<int, retruxx::allocator<int> > m_timefilterValues;
        void _InitWheelTraces();
        void _RecalcWheelEffectNames();
        void _AddSoilEffectNameForWheelTypeName(const CStr& wheelTypeName);
    }; /* size: 0x0160 */

    inline dxSpace* gGlobalSpace = nullptr;
    inline dxSpace* gTempSpace = nullptr;
    inline dxSpace* gBulletSpace = nullptr;
    inline dxSpace* gIntersectionSpace = nullptr;
    inline dxSpace* gSpaceForAllPhysicParticles = nullptr;
    inline dxWorld* gGlobalWorld = nullptr;
    inline DynamicScene* gDynamicScene = nullptr;
}
