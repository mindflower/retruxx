#pragma once
#include "sgnode.h"

namespace m3d
{
    // Asks the parent's server where a loadpoint currently is (server item property 0x3004).
    struct PropSrvLoadpoint
    {
        m3d::SgNode* m_node;
        CVector m_newOrg;
        Quaternion m_newRot;
        CVector m_newScale;
        int m_fromLoadpoint;
    };

    class SgLoadpointNode : public SgNode
    {
    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual Object* Clone(void);
        virtual int Think(int, int);
        virtual Class* GetClass() const;

    protected:
        virtual ~SgLoadpointNode();
        SgLoadpointNode(SgLoadpointNode const&);
        SgLoadpointNode();

    public:
        RT_CLASS_DECLARE(SgLoadpointNode);
    };
}
