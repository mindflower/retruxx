#include "bulletcolliders.h"

#include <stdexcept>

#include "retruxx/common.h"

#include <core/kernel.h>
#include <math/quaternion.h>

#include "server/dynamicscene.h"
#include "server/objects/base/shell.h"
#include "server/objects/guns/gun.h"
#include "server/objects/physicbodies/physicbody.h"

namespace ai
{
	int CollideShellAndWater(m3d::Object* obj1, m3d::Object*, dContact* contact, unsigned&, bool)
	{
		// RVA 0x890E90 - a splash where the shell hits the water, then the shell is gone.
		Shell* shell = static_cast<Shell*>(obj1);
		if (!shell)
		{
			SYS_ERROR("shell");
		}
		if (!obj1->IsKindOf(RT_CLASS_LOCAL(Shell)))
		{
			SYS_ERROR("IS_KIND_OF( obj1, Shell )");
		}
		if (!shell->GetGunPrototypeInfo())
		{
			SYS_ERROR("shell->GetGunPrototypeInfo()");
		}

		CStr const& effectName = gDynamicScene->GetShellWaterEffectName(shell->GetGunPrototypeInfo()->m_explosionType);
		CVector const pos(contact->geom.pos[0], contact->geom.pos[1], contact->geom.pos[2]);
		PhysicBody::CreateEffectNode(effectName, pos, IdentityQuaternion, true, 1.0f);
		shell->Remove();
		return 0;
	}
}
