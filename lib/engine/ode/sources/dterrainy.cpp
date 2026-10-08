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
	// RVA 0x8883B0 - retruxx: the shipped build splits a cell along its (x,z)-(x+1,z+1) diagonal, the one the
	// terrain is drawn with, not along stock ODE's (x+1,z)-(x,z+1). rx is measured back from the cell's far
	// x edge.
	int nX		= int(floor(x * m_vNodeLengthInv));
	int nZ		= int(floor(z * m_vNodeLengthInv));
	dReal rx	= (dReal(nX+1) * m_vNodeLength - x) * m_vNodeLengthInv;
	dReal dz	= (z - (dReal(nZ) * m_vNodeLength)) * m_vNodeLengthInv;
	dIASSERT((rx >= 0.f) && (rx <= 1.f));
	dIASSERT((dz >= 0.f) && (dz <= 1.f));

	dReal y,y0;

	if (dz + rx < 1.f)
	{
		y0	= GetHeight(nX+1,nZ);
		y	= (GetHeight(nX+1,nZ+1) - y0) * dz
			+ (GetHeight(nX,nZ) - y0) * rx
			+ y0;
	}
	else
	{
		y0	= GetHeight(nX,nZ+1);
		y	= (GetHeight(nX+1,nZ+1) - y0) * (1.f - rx)
			+ (GetHeight(nX,nZ) - y0) * (1.f - dz)
			+ y0;
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

	// RVA 0x888250 - retruxx: the two triangles meet on the (x,z)-(x+1,z+1) diagonal, see GetHeight.
	dReal rx	= (Max[0] - pos[0]) * m_vNodeLengthInv;
	dReal dz	= (pos[2] - Min[2]) * m_vNodeLengthInv;

	if ((w == 0) && (dz + rx > 1.f+TERRAINTOL))
		return false;

	if ((w == 1) && (dz + rx < 1.f-TERRAINTOL))
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
	// RVA 0x888580 - retruxx: the shipped build turns the cell's corners a quarter round from stock ODE, so the
	// shared edge BC is the (x,z)-(x+1,z+1) diagonal the terrain is drawn with.
	A[0] = (x+1) * m_vNodeLength;
	A[2] = z* m_vNodeLength;
	A[1] = GetHeight(x+1,z);
	B[0] = (x+1) * m_vNodeLength;
	B[2] = (z+1) * m_vNodeLength;
	B[1] = GetHeight(x+1,z+1);
	C[0] = x * m_vNodeLength;
	C[2] = z * m_vNodeLength;
	C[1] = GetHeight(x,z);
	D[0] = x * m_vNodeLength;
	D[2] = (z+1) * m_vNodeLength;
	D[1] = GetHeight(x,z+1);

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
		E[0] = (x+1) * m_vNodeLength;
		E[2] = (z+2) * m_vNodeLength;
		E[1] = GetHeight(x+1,z+2);
		F[0] = (x-1) * m_vNodeLength;
		F[2] = z * m_vNodeLength;
		F[1] = GetHeight(x-1,z);
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
	// RVA 0x8895C0 - retruxx: collides o2 (in the terrain's frame) with the terrain cells under its box. A geom
	// other than a ray whose box top is below the ground under its centre gets a single straight-up contact,
	// at most half its height deep, instead.
	// NOTE: the cells are still visited after the contact budget is used up, and only a minimum x above the
	// maximum (not equal to it) counts as an empty range, as shipped.
	if ((flags & 0xFFFF) == 0) flags = (flags & 0xFFFF0000) | 1;
	int const numMaxContacts = flags & 0xFFFF;
	if (o2->gflags & GEOM_AABB_BAD) {
		o2->computeAABB();
		o2->gflags &= ~GEOM_AABB_BAD;
	}

	int nMinX = (int) floor (terrain->m_vNodeLengthInv * o2->aabb[0]);
	int nMaxX = (int) floor (terrain->m_vNodeLengthInv * o2->aabb[1]) + 1;
	int nMinZ = (int) floor (terrain->m_vNodeLengthInv * o2->aabb[4]);
	int nMaxZ = (int) floor (terrain->m_vNodeLengthInv * o2->aabb[5]) + 1;
	if (terrain->m_bFinite) {
		if (nMinX < 0) nMinX = 0;
		if (nMaxX >= terrain->m_nNumNodesPerSide) nMaxX = terrain->m_nNumNodesPerSide;
		if (nMinZ < 0) nMinZ = 0;
		if (nMaxZ >= terrain->m_nNumNodesPerSide) nMaxZ = terrain->m_nNumNodesPerSide;
	}

	int numContacts = 0;
	if (nMinX <= nMaxX && nMinZ < nMaxZ) {
		dReal depth = 0;
		dReal cx = 0, top = 0, cz = 0;
		if (o2->type != dRayClass) {
			top = o2->aabb[3];
			cx = (o2->aabb[1] + o2->aabb[0]) * REAL(0.5);
			cz = (o2->aabb[5] + o2->aabb[4]) * REAL(0.5);
			depth = terrain->GetHeight (cx, cz) - top;
		}
		if (o2->type != dRayClass && depth > 0) {
			contact->depth = depth;
			dReal const halfHeight = (o2->aabb[3] - o2->aabb[2]) * REAL(0.5);
			if (depth > halfHeight) contact->depth = halfHeight;
			contact->pos[0] = cx;
			contact->pos[1] = top;
			contact->pos[2] = cz;
			contact->normal[0] = 0;
			contact->normal[1] = REAL(-1.0);
			contact->normal[2] = 0;
			numContacts = 1;
		}
		else {
			for (int x = nMinX; x < nMaxX; ++x) {
				for (int z = nMinZ; z < nMaxZ; ++z) {
					numContacts += terrain->dCollideTerrainUnit (x, z, o2, numMaxContacts - numContacts, flags,
						CONTACT (contact, skip * numContacts), skip);
				}
			}
		}
	}

	for (int i = 0; i < numContacts; ++i) {
		dContactGeom *c = CONTACT (contact, skip * i);
		c->g1 = terrain;
		c->g2 = o2;
	}
	return numContacts;
}

int dCollideTerrainY(dxGeom *o1, dxGeom *o2, int flags,dContactGeom *contact, int skip)
{
	// RVA 0x889870 - retruxx: a placed terrain (o1 transformed) sees o2 through temporary local pos/R buffers. A
	// ray longer than 0.1 across the ground is cast in 32-unit steps (with a sub-ray), stopping at the first
	// step with contacts; the rest of the ray is cast if no step hit.
	dIASSERT (skip >= (int)sizeof(dContactGeom));
	dIASSERT (o1->type == dTerrainYClass);
	dxTerrainY *terrain = (dxTerrainY*) o1;
	bool const transformed = (o1->gflags & GEOM_PLACEABLE) != 0;

	dReal *posBak = 0;
	dReal *RBak = 0;
	int gflagsBak = 0;
	dReal aabbBak[6];
	dVector3 pos1;
	dMatrix3 R1;
	if (transformed) {
		dVector3 delta;
		dOP (delta, -, o2->pos, o1->pos);
		dMULTIPLY1_331 (pos1, o1->R, delta);
		dMULTIPLY1_333 (R1, o1->R, o2->R);
		posBak = o2->pos;
		RBak = o2->R;
		o2->pos = pos1;
		o2->R = R1;
		memcpy (aabbBak, o2->aabb, sizeof(aabbBak));
		gflagsBak = o2->gflags;
		o2->computeAABB();
	}

	int numContacts = 0;
	if (o2->type != dRayClass) {
		numContacts = dCollideTerrainYWithoutSubdivisions (terrain, o2, flags, contact, skip);
	}
	else {
		int maxContacts = flags & 0xFFFF;
		int const flagsHi = flags & 0xFFFF0000;
		dxGeom *subRay = terrain->m_subdivisionRay;
		dVector3 pos, dir;
		dGeomRayGet (o2, pos, dir);
		dReal const rayLength = ((dxRay*) o2)->length;
		dReal const flatLength = dSqrt (dir[2] * dir[2] + dir[0] * dir[0]) * rayLength;
		if (flatLength < REAL(0.1)) {
			numContacts = dCollideTerrainYWithoutSubdivisions (terrain, o2, maxContacts, contact, skip);
		}
		else {
			dGeomRaySet (subRay, pos[0], pos[1], pos[2], dir[0], dir[1], dir[2]);
			dVector3 step = { dir[0], 0, dir[2] };
			dNormalize3 (step);
			step[0] *= REAL(32.0);
			step[2] *= REAL(32.0);
			dReal const invFlat = REAL(1.0) / flatLength;
			step[1] = rayLength * dir[1] * invFlat * REAL(32.0);
			dGeomRaySetLength (subRay, invFlat * rayLength * REAL(32.0));

			// NOTE: the remaining budget is reduced by the running total each step, as shipped.
			dReal t = 0;
			dReal const lastStart = flatLength - REAL(32.0);
			bool skipRest = false;
			if (lastStart > 0) {
				for (;;) {
					dGeomSetPosition (subRay, pos[0], pos[1], pos[2]);
					numContacts += dCollideTerrainYWithoutSubdivisions (terrain, (dxGeom*) subRay, maxContacts | flagsHi,
						CONTACT (contact, skip * numContacts), skip);
					maxContacts -= numContacts;
					pos[0] += step[0];
					pos[1] += step[1];
					pos[2] += step[2];
					if (numContacts > 0) break;
					if (maxContacts < 0) { skipRest = true; break; }
					bool const last = lastStart <= t + REAL(32.0);
					t += REAL(32.0);
					if (last) break;
				}
			}
			if (!skipRest && maxContacts > 0 && numContacts == 0) {
				dGeomSetPosition (subRay, pos[0], pos[1], pos[2]);
				dGeomRaySetLength (subRay, (flatLength - t) * invFlat * rayLength);
				numContacts = dCollideTerrainYWithoutSubdivisions (terrain, (dxGeom*) subRay, maxContacts | flagsHi, contact, skip);
			}
			for (int i = 0; i < numContacts; ++i) {
				dContactGeom *c = CONTACT (contact, skip * i);
				c->g1 = o1;
				c->g2 = o2;
			}
		}
	}

	if (transformed) {
		o2->pos = posBak;
		o2->R = RBak;
		memcpy (o2->aabb, aabbBak, sizeof(aabbBak));
		o2->gflags = gflagsBak;
		for (int i = 0; i < numContacts; ++i) {
			dContactGeom *c = CONTACT (contact, skip * i);
			dVector3 v;
			dMULTIPLY0_331 (v, o1->R, c->pos);
			c->pos[0] = o1->pos[0] + v[0];
			c->pos[1] = o1->pos[1] + v[1];
			c->pos[2] = o1->pos[2] + v[2];
			dMULTIPLY0_331 (v, o1->R, c->normal);
			c->normal[0] = v[0];
			c->normal[1] = v[1];
			c->normal[2] = v[2];
		}
	}
	return numContacts;
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