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
        float m_flyTime;
        float m_Damage;
        float m_AverageSegmentLength;
        CStr m_EffectName;
        ThunderboltPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    // A branching lightning bolt from a ThunderboltLauncher to a set of targets. It lives for
    // m_flyTime seconds, damaging every target a branch touches.
    class Thunderbolt : public ai::Obj
    {
        friend class ThunderboltPrototypeInfo;

    protected:
        virtual  ~Thunderbolt() override;

    private:
        Thunderbolt(const ai::ThunderboltPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classThunderbolt;
        virtual const ai::ThunderboltPrototypeInfo* GetPrototypeInfo() const override;

        using Points = retruxx::vector<CVector, retruxx::allocator<CVector> >;
        using Segments = retruxx::vector<Segment, retruxx::allocator<Segment> >;

        class ThunderboltPoint
        {
        private:
            CVector m_Point;
            int m_ObjId;
            bool m_launcher;
            bool m_dependant;
            char Padding_133[2];
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
        };

        using ThunderboltPoints = retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> >;
        
        class ThunderboltSegment
        {
        private:
            ai::Thunderbolt::ThunderboltPoint m_Start;
            ai::Thunderbolt::ThunderboltPoint m_Finish;
            unsigned int m_nSteps;
            bool m_EffectCreated;
            char Padding_134[3];
            m3d::SgNode* m_EffectNode;
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
        };

        using ThunderboltSegments = retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment> >;

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void Remove() override;
        virtual void RenderDebugInfo() const override;
        void setTargets(const retruxx::vector<CVector, retruxx::allocator<CVector> >& targets);
        void setTargets(const retruxx::vector<int, retruxx::allocator<int> >& targets);
        retruxx::vector<int, retruxx::allocator<int> > getTargets() const;
        void setOrigin(const CVector& Origin);
        const CVector& getOrigin() const;
        void setSourceId(int S, bool launcher);
        int getSourceId() const;
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const;
        virtual bool CanLookAtTarget() const;

    private:
        ai::NumericInRangeRegenerating<float> m_lifeTime;
        ai::Thunderbolt::ThunderboltPoint m_Origin;
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> > m_Targets;
        retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment> > m_Segments;
        retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment> > getThunderboltSegmentsInt(ai::Thunderbolt::ThunderboltPoint& org, retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint> >& targets);
        void calcThunderboltSegments();
        void Render();
    };
}
