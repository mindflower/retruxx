#pragma once
#include <math/vector.h>
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

class Segment;

namespace ai
{
    class ThunderboltPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        /* 0x0040 */ float m_flyTime;
        /* 0x0044 */ float m_Damage;
        /* 0x0048 */ float m_AverageSegmentLength;
        CStr m_EffectName;
        ThunderboltPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x08 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
    }; /* size: 0x0058 */

    class Thunderbolt : public ai::Obj
    {
    protected:
        virtual  ~Thunderbolt() override /* 0x00 */;

    private:
        Thunderbolt(const ai::ThunderboltPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classThunderbolt;
        virtual const ai::ThunderboltPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

        using Points = retruxx::vector<CVector, retruxx::allocator<CVector> >;
        using Segments = retruxx::vector<Segment, retruxx::allocator<Segment> >;

        class ThunderboltPoint
        {
        private:
            CVector m_Point;
            /* 0x000c */ int m_ObjId;
            /* 0x0010 */ bool m_launcher;
            /* 0x0011 */ bool m_dependant;
            /* 0x0012 */ char Padding_133[2];
            retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> > m_masterPoints;

        public:
            ThunderboltPoint(const ai::Thunderbolt::ThunderboltPoint& __that);
            ThunderboltPoint(const CVector& point);
            ThunderboltPoint(const int& objId, bool launcher);
            ThunderboltPoint();
            int getObjId() const;
            void setObjid(int objId, bool launcher);
            const CVector& getPoint() const;
            void setPoint(const CVector& point);
            void setDependencies(const retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> >& dependencies);
            const CVector& getThunderboltPoint();
        }; /* size: 0x0024 */

        using ThunderboltPoints = retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> >;
        
        class ThunderboltSegment
        {
        private:
            ai::Thunderbolt::ThunderboltPoint m_Start;
            ai::Thunderbolt::ThunderboltPoint m_Finish;
            /* 0x0048 */ unsigned int m_nSteps;
            /* 0x004c */ bool m_EffectCreated;
            /* 0x004d */ char Padding_134[3];
            /* 0x0050 */ m3d::SgNode* m_EffectNode;
            const retruxx::vector<CVector, retruxx::allocator<CVector> >& SegmentFinding(const Segment& seg) const;

        public:
            ThunderboltSegment(const ai::Thunderbolt::ThunderboltSegment& __that);
            ThunderboltSegment(const ai::ThunderboltPrototypeInfo* thunderboltPrototype, const ai::Thunderbolt::ThunderboltPoint& Start, const ai::Thunderbolt::ThunderboltPoint& Finish);
            ~ThunderboltSegment();
            const retruxx::vector<CVector, retruxx::allocator<CVector> >& getThunderboltPoints();
            void Render(const ai::ThunderboltPrototypeInfo* thunderboltPrototype);
            const CVector& getStartPoint();
            const CVector& getFinishPoint();
            int getStartObjId() const;
            int getFinishObjId() const;
        }; /* size: 0x0054 */

        using ThunderboltSegments = retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment> >;

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void Remove() override /* 0x54 */;
        virtual void RenderDebugInfo() const override /* 0xe4 */;
        void setTargets(const retruxx::vector<CVector, retruxx::allocator<CVector> >& targets);
        void setTargets(const retruxx::vector<int, retruxx::allocator<int> >& targets);
        retruxx::vector<int, retruxx::allocator<int> > getTargets() const;
        void setOrigin(const CVector& Origin);
        const CVector& getOrigin() const;
        void setSourceId(int S, bool launcher);
        int getSourceId() const;
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const /* 0x110 */;
        virtual bool CanLookAtTarget() const /* 0x114 */;

    private:
        ai::NumericInRangeRegenerating<float> m_lifeTime;
        ai::Thunderbolt::ThunderboltPoint m_Origin;
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> > m_Targets;
        retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment> > m_Segments;
        retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment> > getThunderboltSegmentsInt(ai::Thunderbolt::ThunderboltPoint& org, retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> >& targets);
        void calcThunderboltSegments();
        void Render();
    }; /* size: 0x01dc */
}
