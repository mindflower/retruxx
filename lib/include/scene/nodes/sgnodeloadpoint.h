#pragma once
#include "sgnode.h"

namespace m3d
{
    // Asks the parent's server where a loadpoint currently is (server item property 0x3004).
    struct PropSrvLoadpoint
    {
        /* 0x0000 */ m3d::SgNode* m_node;
        /* 0x0004 */ CVector m_newOrg;
        /* 0x0010 */ Quaternion m_newRot;
        /* 0x0020 */ CVector m_newScale;
        /* 0x002c */ int m_fromLoadpoint;
    }; /* size: 0x0030 */

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
