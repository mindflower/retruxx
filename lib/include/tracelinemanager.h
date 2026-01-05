#pragma once

struct CVector;

namespace ai
{
    class Ray;
    class ObjIdExceptionalTraceLineCallback;
}

namespace m3d
{
    class SgNode;

    class TraceLineManager
    {
    public:
        ~TraceLineManager();
        bool TraceLine(CVector const&, CVector const&);
        void SetTransparentBody(m3d::SgNode*);
        TraceLineManager(unsigned int);
        static void InitTraceLineRay(bool);

    public:
        static inline ai::Ray* m_traceLineRay = nullptr;

    private:
        unsigned int m_LastTimeUpdated;
        unsigned int m_Dt;
        bool m_LastVerdict;
        ai::ObjIdExceptionalTraceLineCallback* m_exceptionIds;
        m3d::SgNode* m_presentNode;
    };
}
