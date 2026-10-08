#include "defaultcollider.h"

#include <stdexcept>

#include "retruxx/common.h"

#include "server/objects/breakableobject.h"

namespace ai
{
    int DefaultCollider(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
    {
        return 1;
    }

    int EmptyCollider(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
    {
        return 0;
    }

    int CollideGeomObjAndLandscape(m3d::Object*, m3d::Object*, dContact* contact, unsigned& numContacts, bool)
    {
        // RVA 0x88D500 - a firm, non-bouncing contact with full friction.
        // NOTE: bounce and bounce_vel are filled in although dContactBounce is not set in the mode.
        for (unsigned i = 0; i < numContacts; ++i)
        {
            contact[i].surface.mode = dContactApprox1 | dContactSoftCFM | dContactSoftERP;
            contact[i].surface.mu = 1.0f;
            contact[i].surface.soft_cfm = 0.001f;
            contact[i].surface.soft_erp = 0.1f;
            contact[i].surface.bounce = 0.0f;
            contact[i].surface.bounce_vel = 10.0f;
        }
        return 1;
    }

    int CollideBreakableObjectAndGeomObject(m3d::Object* obj1, m3d::Object*, dContact* contact, unsigned& numContacts, bool)
    {
        // RVA 0x88D580 - wakes the breakable object and gives it a slightly springy, bouncy contact.
        static_cast<BreakableObject*>(obj1)->DoTouch();
        for (unsigned i = 0; i < numContacts; ++i)
        {
            contact[i].surface.mode = dContactApprox1 | dContactSoftCFM | dContactSoftERP | dContactBounce;
            contact[i].surface.mu = 0.80000001f;
            contact[i].surface.soft_erp = 0.89999998f;
            contact[i].surface.soft_cfm = 0.0f;
            contact[i].surface.bounce = 0.001f;
            contact[i].surface.bounce_vel = 0.0099999998f;
        }
        return 1;
    }
}  // namespace ai
