//Benoit CHAPEROT 2003-2004 www.jstarlab.com
//some code inspired by Magic Software
#include <ode/common.h>
#include <ode/collision.h>
#include <ode/matrix.h>
#include <ode/rotation.h>
#include <ode/odemath.h>
#include "collision_kernel.h"
#include "collision_std.h"
#include "collision_std_internal.h"
#include "collision_util.h"
//#include <drawstuff/drawstuff.h>
#include "windows.h"
#include "ode\ode.h"

#define CONTACT(p,skip) ((dContactGeom*) (((char*)p) + (skip)))
#define MAXCONTACT 10
#define TERRAINTOL 0.0f

static bool IsAPowerOfTwo(int f)
{
	dAASSERT(f!=0);
	while ((f&1) != 1)	
		f >>= 1;

	return (f == 1);
}

static int GetPowerOfTwo(int f)
{
	dAASSERT(f!=0);
	int n = 0;
	while ((f&1) != 1)
	{
		n++;
		f >>= 1;
	}
	
	return n;
}

dxTerrainY::dxTerrainY (dSpaceID space, dReal *pHeights,dReal vLength,int nNumNodesPerSide, int bFinite, int bPlaceable) :
dxGeom (space,bPlaceable)
{
	dIASSERT(IsAPowerOfTwo(nNumNodesPerSide));
	dIASSERT(pHeights);
	dIASSERT(vLength > 0.f);
	dIASSERT(nNumNodesPerSide > 0);
	type = dTerrainYClass;
	m_vLength = vLength;
	m_pHeights = new dReal[nNumNodesPerSide * nNumNodesPerSide];
	dIASSERT(m_pHeights);
	m_nNumNodesPerSide = nNumNodesPerSide;
	m_vNodeLength = m_vLength / m_nNumNodesPerSide;
	m_vNodeLengthInv = m_nNumNodesPerSide / m_vLength;
	m_nNumNodesPerSideShift = GetPowerOfTwo(m_nNumNodesPerSide);
	m_nNumNodesPerSideMask  = m_nNumNodesPerSide - 1;
	m_vMinHeight = dInfinity;
	m_vMaxHeight = -dInfinity;
	m_bFinite = bFinite;

	for (int i=0;i<nNumNodesPerSide * nNumNodesPerSide;i++)
	{
		m_pHeights[i] = pHeights[i];
		if (m_pHeights[i] < m_vMinHeight)	m_vMinHeight = m_pHeights[i];
		if (m_pHeights[i] > m_vMaxHeight)	m_vMaxHeight = m_pHeights[i];
	}
	m_subdivisionRay = (dxRay *)dCreateRay(0, 1.0);
}

dxTerrainY::~dxTerrainY()
{
	dIASSERT(m_pHeights);
	delete [] m_pHeights;
}

void dxTerrainY::computeAABB()
{
	if (m_bFinite)
	{
		if (gflags & GEOM_PLACEABLE)
		{
			dReal dx[6],dy[6],dz[6];
			dx[0] = 0;
			dx[1] = R[0] * m_vLength;
			dx[2] = R[1] * m_vMinHeight;
			dx[3] = R[1] * m_vMaxHeight;
			dx[4] = 0;
			dx[5] = R[2] * m_vLength;

			dy[0] = 0;
			dy[1] = R[4] * m_vLength;
			dy[2] = R[5] * m_vMinHeight;
			dy[3] = R[5] * m_vMaxHeight;
			dy[4] = 0;
			dy[5] = R[6] * m_vLength;

			dz[0]  = 0;
			dz[1]  = R[8] * m_vLength;
			dz[2]  = R[9] * m_vMinHeight;
			dz[3]  = R[9] * m_vMaxHeight;
			dz[4]  = 0;
			dz[5]  = R[10] * m_vLength;

			aabb[0] = pos[0] + MIN(dx[0],dx[1]) + MIN(dx[2],dx[3]) + MIN(dx[4],dx[5]);
			aabb[1] = pos[0] + MAX(dx[0],dx[1]) + MAX(dx[2],dx[3]) + MAX(dx[4],dx[5]);
			aabb[2] = pos[1] + MIN(dy[0],dy[1]) + MIN(dy[2],dy[3]) + MIN(dy[4],dy[5]);
			aabb[3] = pos[1] + MAX(dy[0],dy[1]) + MAX(dy[2],dy[3]) + MAX(dy[4],dy[5]);
			aabb[4] = pos[2] + MIN(dz[0],dz[1]) + MIN(dz[2],dz[3]) + MIN(dz[4],dz[5]);
			aabb[5] = pos[2] + MAX(dz[0],dz[1]) + MAX(dz[2],dz[3]) + MAX(dz[4],dz[5]);
		}
		else
		{
			aabb[0] = 0;
			aabb[1] = m_vLength;
			aabb[2] = m_vMinHeight;
			aabb[3] = m_vMaxHeight;
			aabb[4] = 0;
			aabb[5] = m_vLength;
		}
	}
	else
	{
		if (gflags & GEOM_PLACEABLE)
		{
			aabb[0] = -dInfinity;
			aabb[1] = dInfinity;
			aabb[2] = -dInfinity;
			aabb[3] = dInfinity;
			aabb[4] = -dInfinity;
			aabb[5] = dInfinity;
		}
		else
		{
			aabb[0] = -dInfinity;
			aabb[1] = dInfinity;
			aabb[2] = m_vMinHeight;
			aabb[3] = m_vMaxHeight;
			aabb[4] = -dInfinity;
			aabb[5] = dInfinity;
		}
	}
}

dReal dxTerrainY::GetHeight(int x,int z)
{
	return m_pHeights[	(((unsigned int)(z) & m_nNumNodesPerSideMask) << m_nNumNodesPerSideShift)
					+	 ((unsigned int)(x) & m_nNumNodesPerSideMask)];
}

dReal dxTerrainY::GetHeight(dReal x,dReal z)
{
	int nX		= int(floor(x / m_vNodeLength));
	int nZ		= int(floor(z / m_vNodeLength));
	dReal dx	= (x - (dReal(nX) * m_vNodeLength)) / m_vNodeLength;
	dReal dz	= (z - (dReal(nZ) * m_vNodeLength)) / m_vNodeLength;
	dIASSERT((dx >= 0.f) && (dx <= 1.f));
	dIASSERT((dz >= 0.f) && (dz <= 1.f));

	dReal y,y0;
	
	if (dx + dz < 1.f)
	{
		y0	= GetHeight(nX,nZ);
		y	= y0	
			+ (GetHeight(nX+1,nZ) - y0) * dx
			+ (GetHeight(nX,nZ+1) - y0) * dz;
	}
	else
	{
		y0	= GetHeight(nX+1,nZ+1);
		y	= y0	
			+ (GetHeight(nX+1,nZ) - y0) * (1.f - dz)
			+ (GetHeight(nX,nZ+1) - y0) * (1.f - dx);
	}

	return y;	
}

bool dxTerrainY::IsOnTerrain(int nx,int nz,int w,dReal *pos)
{
	dVector3 Min,Max;
	Min[0] = nx * m_vNodeLength;
	Min[2] = nz * m_vNodeLength;
	Max[0] = (nx+1) * m_vNodeLength;
	Max[2] = (nz+1) * m_vNodeLength;
	dReal Tol = m_vNodeLength * TERRAINTOL;
	
	if ((pos[0]<Min[0]-Tol) || (pos[0]>Max[0]+Tol))
		return false;

	if ((pos[2]<Min[2]-Tol) || (pos[2]>Max[2]+Tol))
		return false;

	dReal dx	= (pos[0] - (dReal(nx) * m_vNodeLength)) / m_vNodeLength;
	dReal dz	= (pos[2] - (dReal(nz) * m_vNodeLength)) / m_vNodeLength;

	if ((w == 0) && (dx + dz > 1.f+TERRAINTOL))
		return false;

	if ((w == 1) && (dx + dz < 1.f-TERRAINTOL))
		return false;

	return true;
}

dGeomID dCreateTerrainY(dSpaceID space, dReal *pHeights,dReal vLength,int nNumNodesPerSide, int bFinite, int bPlaceable)
{
	return new dxTerrainY(space, pHeights,vLength,nNumNodesPerSide,bFinite,bPlaceable);
}

dReal dGeomTerrainYPointDepth (dGeomID g, dReal x, dReal y, dReal z)
{
	dUASSERT (g && g->type == dTerrainYClass,"argument not a terrain");
	dxTerrainY *t = (dxTerrainY*) g;
	return t->GetHeight(x,z) - y;
}

typedef dReal dGetDepthFn(dGeomID g, dReal x, dReal y, dReal z);
#define RECOMPUTE_RAYNORMAL
//#define DO_RAYDEPTH

#define DMESS(A)	\
			dMessage(0,"Contact Plane (%d %d %d) %.5e %.5e (%.5e %.5e %.5e)(%.5e %.5e %.5e)).",	\
					x,z,A,	\
					pContact->depth,	\
					dGeomSphereGetRadius(o2),		\
					pContact->pos[0],	\
					pContact->pos[1],	\
					pContact->pos[2],	\
					pContact->normal[0],	\
					pContact->normal[1],	\
					pContact->normal[2]);
/*
(y is up)

A-B-E.x
|/|
C-D
|
F
.
z
*/
int dxTerrainY::dCollideTerrainUnit(
	int x,int z,dxGeom *o2,int numMaxContacts,
	int flags,dContactGeom *contact, int skip)
{
	dColliderFn *CollideRayN;
	dColliderFn *CollideNPlane;
	dGetDepthFn *GetDepth;
	int numContacts = 0;
	int numPlaneContacts = 0;
	int i;
	
	if (numContacts == numMaxContacts)
		return numContacts;

	dContactGeom PlaneContact[MAXCONTACT];
	flags = (flags & 0xffff0000) | MAXCONTACT;
	
	switch (o2->type)
	{
	case dSphereClass:
		CollideRayN		= dCollideRaySphere;
		CollideNPlane	= dCollideSpherePlane;
		GetDepth		= dGeomSpherePointDepth;
		break;
	case dBoxClass:
		CollideRayN		= dCollideRayBox;
		CollideNPlane	= dCollideBoxPlane;
		GetDepth		= dGeomBoxPointDepth;
		break;
	case dCCylinderClass:
		CollideRayN		= dCollideRayCCylinder;
		CollideNPlane	= dCollideCCylinderPlane;
		GetDepth		= dGeomCCylinderPointDepth;
		break;
	case dRayClass:
		CollideRayN		= NULL;
		CollideNPlane	= dCollideRayPlane;
		GetDepth		= NULL;
		break;
	case dConeClass:
		CollideRayN		= dCollideRayCone;
		CollideNPlane	= dCollideConePlane;
		GetDepth		= dGeomConePointDepth;
		break;
	default:
		dIASSERT(0);
	}

	dReal Plane[4],lBD,lCD,lBC;
	dVector3 A,B,C,D,BD,CD,BC,AB,AC;
	A[0] = x * m_vNodeLength;
	A[2] = z* m_vNodeLength;
	A[1] = GetHeight(x,z);
	B[0] = (x+1) * m_vNodeLength;
	B[2] = z * m_vNodeLength;
	B[1] = GetHeight(x+1,z);
	C[0] = x * m_vNodeLength;
	C[2] = (z+1) * m_vNodeLength;
	C[1] = GetHeight(x,z+1);
	D[0] = (x+1) * m_vNodeLength;
	D[2] = (z+1) * m_vNodeLength;
	D[1] = GetHeight(x+1,z+1);

	dOP(BC,-,C,B);
	lBC = dLENGTH(BC);
	dOPEC(BC,/=,lBC);

	dOP(BD,-,D,B);
	lBD = dLENGTH(BD);
	dOPEC(BD,/=,lBD);

	dOP(CD,-,D,C);
	lCD = dLENGTH(CD);
	dOPEC(CD,/=,lCD);

	dOP(AB,-,B,A);
	dNormalize3(AB);

	dOP(AC,-,C,A);
	dNormalize3(AC);

	if (CollideRayN)
	{
#ifdef RECOMPUTE_RAYNORMAL
		dVector3 E,F;
		dVector3 CE,FB,AD;
		dVector3 Normal[3];
		E[0] = (x+2) * m_vNodeLength;
		E[2] = z * m_vNodeLength;
		E[1] = GetHeight(x+2,z);
		F[0] = x * m_vNodeLength;
		F[2] = (z+2) * m_vNodeLength;
		F[1] = GetHeight(x,z+2);
		dOP(AD,-,D,A);
		dNormalize3(AD);
		dOP(CE,-,E,C);
		dNormalize3(CE);
		dOP(FB,-,B,F);
		dNormalize3(FB);

		//BC
		dCROSS(Normal[0],=,BC,AD);
		dNormalize3(Normal[0]);

		//BD
		dCROSS(Normal[1],=,BD,CE);
		dNormalize3(Normal[1]);

		//CD
		dCROSS(Normal[2],=,CD,FB);
		dNormalize3(Normal[2]);
#endif		
		int nA[3],nB[3];
		dContactGeom ContactA[3],ContactB[3];
		dxRay rayBC(0,lBC);	
		dGeomRaySet(&rayBC, B[0], B[1], B[2], BC[0], BC[1], BC[2]);
		nA[0] = CollideRayN(&rayBC,o2,flags,&ContactA[0],sizeof(dContactGeom));
		dGeomRaySet(&rayBC, C[0], C[1], C[2], -BC[0], -BC[1], -BC[2]);
		nB[0] = CollideRayN(&rayBC,o2,flags,&ContactB[0],sizeof(dContactGeom));
		
		dxRay rayBD(0,lBD);	
		dGeomRaySet(&rayBD, B[0], B[1], B[2], BD[0], BD[1], BD[2]);
		nA[1] = CollideRayN(&rayBD,o2,flags,&ContactA[1],sizeof(dContactGeom));
		dGeomRaySet(&rayBD, D[0], D[1], D[2], -BD[0], -BD[1], -BD[2]);
		nB[1] = CollideRayN(&rayBD,o2,flags,&ContactB[1],sizeof(dContactGeom));
	
		dxRay rayCD(0,lCD);	
		dGeomRaySet(&rayCD, C[0], C[1], C[2], CD[0], CD[1], CD[2]);
		nA[2] = CollideRayN(&rayCD,o2,flags,&ContactA[2],sizeof(dContactGeom));
		dGeomRaySet(&rayCD, D[0], D[1], D[2], -CD[0], -CD[1], -CD[2]);
		nB[2] = CollideRayN(&rayCD,o2,flags,&ContactB[2],sizeof(dContactGeom));
	
		for (i=0;i<3;i++)
		{
			if (nA[i] & nB[i])
			{
				dContactGeom *pContact = CONTACT(contact,numContacts*skip);
				pContact->pos[0] = (ContactA[i].pos[0] + ContactB[i].pos[0])/2;
				pContact->pos[1] = (ContactA[i].pos[1] + ContactB[i].pos[1])/2;
				pContact->pos[2] = (ContactA[i].pos[2] + ContactB[i].pos[2])/2;
#ifdef RECOMPUTE_RAYNORMAL
				pContact->normal[0] = -Normal[i][0];
				pContact->normal[1] = -Normal[i][1];
				pContact->normal[2] = -Normal[i][2];
#else
				pContact->normal[0] = (ContactA[i].normal[0] + ContactB[i].normal[0])/2;	//0.f;
				pContact->normal[1] = (ContactA[i].normal[1] + ContactB[i].normal[1])/2;	//0.f;
				pContact->normal[2] = (ContactA[i].normal[2] + ContactB[i].normal[2])/2;	//-1.f;
				dNormalize3(pContact->normal);
#endif
#ifdef DO_RAYDEPTH
				dxRay rayV(0,1000.f);
				dGeomRaySet(&rayV,	pContact->pos[0],
									pContact->pos[1],
									pContact->pos[2],
									-pContact->normal[0],
									-pContact->normal[1],
									-pContact->normal[2]);
		
				dContactGeom ContactV;
				if (CollideRayN(&rayV,o2,flags,&ContactV,sizeof(dContactGeom)))
				{
					pContact->depth = ContactV.depth;
					numContacts++;	
				}
#else
				pContact->depth =  GetDepth(o2,
				pContact->pos[0],
				pContact->pos[1],
				pContact->pos[2]);
				numContacts++;
#endif
				if (numContacts == numMaxContacts)
					return numContacts;

			}
		}
	}

	dCROSS(Plane,=,AC,AB);
	dNormalize3(Plane);
	Plane[3] = Plane[0] * A[0] + Plane[1] * A[1] + Plane[2] * A[2];
	dxPlane planeABC(0,Plane[0],Plane[1],Plane[2],Plane[3]);
	numPlaneContacts = CollideNPlane(o2,&planeABC,flags,PlaneContact,sizeof(dContactGeom));

	for (i=0;i<numPlaneContacts;i++)
	{
		if (IsOnTerrain(x,z,0,PlaneContact[i].pos))
		{
			dContactGeom *pContact = CONTACT(contact,numContacts*skip);
			pContact->pos[0] = PlaneContact[i].pos[0];
			pContact->pos[1] = PlaneContact[i].pos[1];
			pContact->pos[2] = PlaneContact[i].pos[2];
			pContact->normal[0] = -PlaneContact[i].normal[0];
			pContact->normal[1] = -PlaneContact[i].normal[1];
			pContact->normal[2] = -PlaneContact[i].normal[2];
			pContact->depth = PlaneContact[i].depth;

			//DMESS(0);
			numContacts++;

			if (numContacts == numMaxContacts)
					return numContacts;
		}
	}

	dCROSS(Plane,=,BD,CD);
	dNormalize3(Plane);
	Plane[3] = Plane[0] * D[0] + Plane[1] * D[1] + Plane[2] * D[2];
	dxPlane planeDCB(0,Plane[0],Plane[1],Plane[2],Plane[3]);
	numPlaneContacts = CollideNPlane(o2,&planeDCB,flags,PlaneContact,sizeof(dContactGeom));

	for (i=0;i<numPlaneContacts;i++)
	{
		if (IsOnTerrain(x,z,1,PlaneContact[i].pos))
		{
			dContactGeom *pContact = CONTACT(contact,numContacts*skip);
			pContact->pos[0] = PlaneContact[i].pos[0];
			pContact->pos[1] = PlaneContact[i].pos[1];
			pContact->pos[2] = PlaneContact[i].pos[2];
			pContact->normal[0] = -PlaneContact[i].normal[0];
			pContact->normal[1] = -PlaneContact[i].normal[1];
			pContact->normal[2] = -PlaneContact[i].normal[2];
			pContact->depth = PlaneContact[i].depth;
			//DMESS(1);
			numContacts++;

			if (numContacts == numMaxContacts)
					return numContacts;
		}
	}

	return numContacts;
}

int dCollideTerrainYWithoutSubdivisions(
    dxTerrainY* terrain,
    dxGeom* o2, 
    unsigned int flags,
    dContactGeom* contact,
    int skip)
{
	// TODO: generated code dCollideTerrainYWithoutSubdivisions
    dxTerrainY* terrainPtr = terrain;
    int contactCount = 0;
    
    // Ensure we have at least 1 contact
    unsigned int numMaxTerrainContacts = (flags & 0xFFFF);
    if (numMaxTerrainContacts == 0) {
        flags = (flags & 0xFFFF0001) | 1;
        numMaxTerrainContacts = 1;
    }
    
    // Update object AABB if needed
    if ((o2->gflags & 2) != 0) {
        o2->computeAABB();
        o2->gflags &= ~2u;
    }
    
    // Calculate terrain grid bounds from object AABB
    int nMinX = (int)(floor(terrainPtr->m_vNodeLengthInv * o2->aabb[0]));
    int nMaxX = (int)(floor(terrainPtr->m_vNodeLengthInv * o2->aabb[1])) + 1;
    int nMinZ = (int)(floor(terrainPtr->m_vNodeLengthInv * o2->aabb[4]));
    int nMaxZ = (int)(floor(terrainPtr->m_vNodeLengthInv * o2->aabb[5])) + 1;
    
    // Clamp bounds for finite terrain
    if (terrainPtr->m_bFinite) {
        nMinX = (nMinX < 0) ? 0 : nMinX;
        if (nMaxX >= terrainPtr->m_nNumNodesPerSide) {
            nMaxX = terrainPtr->m_nNumNodesPerSide;
        }
        
        nMinZ = (nMinZ < 0) ? 0 : nMinZ;
        if (nMaxZ >= terrainPtr->m_nNumNodesPerSide) {
            nMaxZ = terrainPtr->m_nNumNodesPerSide;
        }
    }
    
    // Check if bounds are valid
    if (nMinX >= nMaxX || nMinZ >= nMaxZ) {
        // No collision possible
        goto SET_CONTACT_GEOMS;
    }
    
    // Special case for certain geometry types or simple height check
    if (o2->type == 5) {
        // Handle specific geometry type
        goto COLLIDE_WITH_TERRAIN_UNITS;
    }
    
    // Simple height-based collision test
    {
        float centerX = (o2->aabb[1] + o2->aabb[0]) * 0.5f;
        float centerY = o2->aabb[3];  // Top of AABB
        float centerZ = (o2->aabb[5] + o2->aabb[4]) * 0.5f;
        
        float terrainHeight = terrainPtr->GetHeight(centerX, centerZ);
        float penetrationDepth = terrainHeight - centerY;
        
        if (penetrationDepth <= 0.0f) {
            // No penetration, use detailed collision
            goto COLLIDE_WITH_TERRAIN_UNITS;
        }
        
        // Create contact for simple height collision
        contact->depth = penetrationDepth;
        
        // Clamp depth to half the object height
        float halfHeight = (o2->aabb[3] - o2->aabb[2]) * 0.5f;
        if (penetrationDepth > halfHeight) {
            contact->depth = halfHeight;
        }
        
        contact->pos[0] = centerX;
        contact->pos[1] = centerY;
        contact->pos[2] = centerZ;
        contact->normal[0] = 0.0f;
        contact->normal[1] = -1.0f;
        contact->normal[2] = 0.0f;
        
        contactCount = 1;
        goto SET_CONTACT_GEOMS;
    }

COLLIDE_WITH_TERRAIN_UNITS:
    // Detailed collision with terrain grid cells
    for (int x = nMinX; x < nMaxX; ++x) {
        for (int z = nMinZ; z < nMaxZ; ++z) {
            int remainingContacts = numMaxTerrainContacts - contactCount;
            if (remainingContacts <= 0) {
                break;
            }
            
            dContactGeom* currentContact = reinterpret_cast<dContactGeom*>(
                reinterpret_cast<char*>(contact) + skip * contactCount);
            
            int newContacts = terrainPtr->dCollideTerrainUnit(
                x, z,
                o2,
                remainingContacts,
                flags,
                currentContact,
                skip);
            
            contactCount += newContacts;
        }
    }

SET_CONTACT_GEOMS:
    // Set geometry pointers for all contacts
    if (contactCount > 0) {
        dContactGeom* currentContact = contact;
        for (int i = 0; i < contactCount; ++i) {
            currentContact->g1 = terrainPtr;
            currentContact->g2 = o2;
            currentContact = reinterpret_cast<dContactGeom*>(
                reinterpret_cast<char*>(currentContact) + skip);
        }
    }
    
    return contactCount;
}

int dCollideTerrainY(dxGeom *o1, dxGeom *o2, int flags,dContactGeom *contact, int skip)
{
	// TODO: generated code dCollideTerrainY
	dIASSERT (skip >= (int)sizeof(dContactGeom));
	dIASSERT (o1->type == dTerrainYClass);
	int contactCount = 0;
    float* originalPos = nullptr;
    float* originalR = nullptr;
    int originalGFlags = 0;
    float originalAABB[6];
	dxTerrainY *terrain = (dxTerrainY*) o1;
    
    // Transform object into terrain's local space if terrain has transform flags
    if ((o1->gflags & 4) != 0) {
        // Save original object state
        originalPos = o2->pos;
        originalR = o2->R;
        originalGFlags = o2->gflags;
        memcpy(originalAABB, o2->aabb, sizeof(originalAABB));
        
        // Transform object position into terrain space
        float delta[3] = {
            o2->pos[0] - o1->pos[0],
            o2->pos[1] - o1->pos[1], 
            o2->pos[2] - o1->pos[2]
        };
        
        float localPos[3];
        localPos[0] = o1->R[0] * delta[0] + o1->R[4] * delta[1] + o1->R[8] * delta[2];
        localPos[1] = o1->R[1] * delta[0] + o1->R[5] * delta[1] + o1->R[9] * delta[2];
        localPos[2] = o1->R[2] * delta[0] + o1->R[6] * delta[1] + o1->R[10] * delta[2];
        
        // Transform object rotation into terrain space
        float localR[12];
        localR[0] = o1->R[0] * o2->R[0] + o1->R[4] * o2->R[4] + o1->R[8] * o2->R[8];
        localR[1] = o1->R[0] * o2->R[1] + o1->R[4] * o2->R[5] + o1->R[8] * o2->R[9];
        localR[2] = o1->R[0] * o2->R[2] + o1->R[4] * o2->R[6] + o1->R[8] * o2->R[10];
        
        localR[4] = o1->R[1] * o2->R[0] + o1->R[5] * o2->R[4] + o1->R[9] * o2->R[8];
        localR[5] = o1->R[1] * o2->R[1] + o1->R[5] * o2->R[5] + o1->R[9] * o2->R[9];
        localR[6] = o1->R[1] * o2->R[2] + o1->R[5] * o2->R[6] + o1->R[9] * o2->R[10];
        
        localR[8] = o1->R[2] * o2->R[0] + o1->R[6] * o2->R[4] + o1->R[10] * o2->R[8];
        localR[9] = o1->R[2] * o2->R[1] + o1->R[6] * o2->R[5] + o1->R[10] * o2->R[9];
        localR[10] = o1->R[2] * o2->R[2] + o1->R[6] * o2->R[6] + o1->R[10] * o2->R[10];
        
        // Update object to local space
        o2->pos[0] = localPos[0];
        o2->pos[1] = localPos[1];
        o2->pos[2] = localPos[2];
        
        memcpy(o2->R, localR, sizeof(localR));
        
        // Recompute AABB in local space
        o2->computeAABB();
    }
    
    // Handle different geometry types
    if (o2->type != 5) {
        // Non-ray geometry - use standard collision
        contactCount = dCollideTerrainYWithoutSubdivisions(terrain, o2, flags, contact, skip);
    } else {
        // Ray geometry - use subdivision for better accuracy
        int maxContacts = static_cast<unsigned short>(flags);
        int flagsHigh = flags & 0xFFFF0000;

		dxRay* ray = (dxRay*)o2;
        dxRay* subdivisionRay = terrain->m_subdivisionRay;
        
        // Get ray parameters
        float rayPos[3], rayDir[3];
        dGeomRayGet(o2, rayPos, rayDir);

        // Calculate flat length (projection onto XZ plane)
        float flatLength = sqrt(rayDir[0] * rayDir[0] + rayDir[2] * rayDir[2]) * ray->length;
        
        // If ray is too steep, fall back to standard collision
        if (flatLength < 0.1f) {
            contactCount = dCollideTerrainYWithoutSubdivisions(terrain, o2, maxContacts, contact, skip);
        } else {
            // Set up subdivision ray
            dGeomRaySet(subdivisionRay, rayPos[0], rayPos[1], rayPos[2], rayDir[0], rayDir[1], rayDir[2]);
            
            // Create step vector for subdivision
            float stepVector[3] = {rayDir[0], 0.0f, rayDir[2]};
            dNormalize3(stepVector);
            
            float stepSize = 32.0f;
            float stepScale = 1.0f / flatLength;
            float rayLength = ray->length;
            
            stepVector[0] *= stepSize;
            stepVector[1] = (rayLength * rayDir[1]) * stepScale * stepSize;
            stepVector[2] *= stepSize;
            
            float segmentLength = (stepScale * rayLength) * stepSize;
            dGeomRaySetLength(subdivisionRay, segmentLength);
            
            // Perform subdivision collision
            float currentT = 0.0f;
            float remainingLength = flatLength - stepSize;
            
            // Subdivide ray into segments
            while (remainingLength > 0.0f) {
                dGeomSetPosition(subdivisionRay, rayPos[0], rayPos[1], rayPos[2]);
                
                int segmentContacts = dCollideTerrainYWithoutSubdivisions(
                    terrain, subdivisionRay, maxContacts | flagsHigh,
                    reinterpret_cast<dContactGeom*>(reinterpret_cast<char*>(contact) + skip * contactCount),
                    skip);
                
                contactCount += segmentContacts;
                maxContacts -= contactCount;
                
                // Move to next segment
                rayPos[0] += stepVector[0];
                rayPos[1] += stepVector[1];
                rayPos[2] += stepVector[2];
                
                // Stop if we have contacts or ran out of contact slots
                if (contactCount > 0 || maxContacts <= 0) {
                    break;
                }
                
                // Check if we've processed enough segments
                if (remainingLength <= currentT + stepSize) {
                    break;
                }
                
                currentT += stepSize;
            }
            
            // Handle remaining ray segment if we have contact slots available
            if (maxContacts > 0 && contactCount == 0) {
                dGeomSetPosition(subdivisionRay, rayPos[0], rayPos[1], rayPos[2]);
                
                float remainingSegmentLength = (remainingLength - currentT) * stepScale * rayLength;
                dGeomRaySetLength(subdivisionRay, remainingSegmentLength);
                
                contactCount = dCollideTerrainYWithoutSubdivisions(
                    terrain, subdivisionRay, maxContacts | flagsHigh, contact, skip);
            }
            
            // Set geometry pointers for all contacts
            if (contactCount > 0) {
                dContactGeom* currentContact = contact;
                for (int i = 0; i < contactCount; ++i) {
                    currentContact->g1 = o1;
                    currentContact->g2 = o2;
                    currentContact = reinterpret_cast<dContactGeom*>(
                        reinterpret_cast<char*>(currentContact) + skip);
                }
            }
        }
    }
    
    // Restore original object state if we transformed it
    if ((o1->gflags & 4) != 0) {
        // Restore object state
        o2->pos = originalPos;
        o2->R = originalR;
        memcpy(o2->aabb, originalAABB, sizeof(originalAABB));
        o2->gflags = originalGFlags;
        
        // Transform contacts back to world space
        if (contactCount > 0) {
            dContactGeom* currentContact = contact;
            for (int i = 0; i < contactCount; ++i) {
                // Transform contact position back to world space
                float localPos[3] = {
                    currentContact->pos[0],
                    currentContact->pos[1], 
                    currentContact->pos[2]
                };
                
                currentContact->pos[0] = o1->pos[0] + 
                    o1->R[0] * localPos[0] + o1->R[4] * localPos[1] + o1->R[8] * localPos[2];
                currentContact->pos[1] = o1->pos[1] + 
                    o1->R[1] * localPos[0] + o1->R[5] * localPos[1] + o1->R[9] * localPos[2];
                currentContact->pos[2] = o1->pos[2] + 
                    o1->R[2] * localPos[0] + o1->R[6] * localPos[1] + o1->R[10] * localPos[2];
                
                // Transform contact normal back to world space
                float localNormal[3] = {
                    currentContact->normal[0],
                    currentContact->normal[1],
                    currentContact->normal[2]
                };
                
                currentContact->normal[0] = 
                    o1->R[0] * localNormal[0] + o1->R[4] * localNormal[1] + o1->R[8] * localNormal[2];
                currentContact->normal[1] = 
                    o1->R[1] * localNormal[0] + o1->R[5] * localNormal[1] + o1->R[9] * localNormal[2];
                currentContact->normal[2] = 
                    o1->R[2] * localNormal[0] + o1->R[6] * localNormal[1] + o1->R[10] * localNormal[2];
                
                currentContact = reinterpret_cast<dContactGeom*>(
                    reinterpret_cast<char*>(currentContact) + skip);
            }
        }
    }
    
    return contactCount;
}
/*
void dsDrawTerrainY(int x,int z,float vLength,float vNodeLength,int nNumNodesPerSide,float *pHeights,const float *pR,const float *ppos)
{
	float A[3],B[3],C[3],D[3];
	float R[12];
	float pos[3];
	if (pR)
		memcpy(R,pR,sizeof(R));
	else
	{
		memset(R,0,sizeof(R));
		R[0] = 1.f;
		R[5] = 1.f;
		R[10] = 1.f;
	}
	
	if (ppos)
		memcpy(pos,ppos,sizeof(pos));
	else
		memset(pos,0,sizeof(pos));
	
	float vx,vz;
	vx = vLength * x;
	vz = vLength * z;
	
	int i;
	for (i=0;i<nNumNodesPerSide;i++)
	{
		for (int j=0;j<nNumNodesPerSide;j++)
		{
			A[0] = i * vNodeLength + vx;
			A[2] = j * vNodeLength + vz;
			A[1] = GetHeight(i,j,nNumNodesPerSide,pHeights);
			B[0] = (i+1) * vNodeLength + vx;
			B[2] = j * vNodeLength + vz;
			B[1] = GetHeight(i+1,j,nNumNodesPerSide,pHeights);
			C[0] = i * vNodeLength + vx;
			C[2] = (j+1) * vNodeLength + vz;
			C[1] = GetHeight(i,j+1,nNumNodesPerSide,pHeights);
			D[0] = (i+1) * vNodeLength + vx;
			D[2] = (j+1) * vNodeLength + vz;
			D[1] = GetHeight(i+1,j+1,nNumNodesPerSide,pHeights);
			dsDrawTriangle(pos,R,C,B,A,1);
			dsDrawTriangle(pos,R,D,B,C,1);
		}
	}
}
*/