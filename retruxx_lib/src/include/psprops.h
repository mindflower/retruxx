#pragma once
#include <math/vector.h>

struct WorkTime
{
    WorkTime();
    /* 0x0000 */ float m_start;
    /* 0x0004 */ float m_length;
    /* 0x0008 */ float m_repeat;
}; /* size: 0x000c */

enum ForceType
{
    PS_FORCE_RANDOM = 0x0,
    PS_FORCE_SINE = 0x1,
};

struct Force
{
    /* 0x0000 */ float m_min;
    /* 0x0004 */ float m_max;
    /* 0x0008 */ float m_freq;
    /* 0x000c */ ForceType m_type;
    Force();
    void SetAxis(float forceMin, float forceMax, ForceType ft, float period);
    void GetAxis(float&, float&, ForceType&, float&) const;
}; /* size: 0x0010 */

enum CoordinatesSystemType
{
    PS_CST_CARTHESIAN = 0x0,
    PS_CST_POLAR = 0x1,
    PS_CST_POLAR_ORG = 0x2,
};

enum TimeMode
{
    PS_TIME_LOCAL = 0x1,
    PS_TIME_GLOBAL = 0x0,
};

enum ForceMode
{
    PS_FORCE_VEL = 0x0,
    PS_FORCE_ACCEL = 0x1,
    PS_FORCE_POS = 0x2,
};

enum WorkMode
{
    OFF = 0x0,
    ACCELERATION = 0x1,
    SPEED = 0x2,
    WMPOSITION = 0x3,
};

enum GameInteraction
{
    GI_WIND = 0x0,
    GI_SIZE = 0x1,
};

enum PBlendMode
{
    PS_ADD = 0x0,
    PS_ALPHA = 0x1,
    PS_ADDSMOOTH = 0x2,
    PS_ADDSIGNED = 0x3,
};

enum PsShaderType
{
    PSST_DUST = 0x0,
    PSST_LIGHT = 0x1,
    PSST_NOFOG_DUST = 0x2,
    PSST_NOFOG_LIGHT = 0x3,
};

struct PSProps
{
    /* 0x0000 */ char m_Name[50];
    /* 0x0032 */ char m_ClassName[50];
    /* 0x0064 */ bool m_Specific;
    /* 0x0065 */ bool m_HaveTrail;
    /* 0x0066 */ char Padding_193[2];
    /* 0x0068 */ int m_trailLen;
    /* 0x006c */ bool m_CreateOne;
    /* 0x006d */ bool m_autoMeshEmitter;
    /* 0x006e */ char Padding_194[2];
    /* 0x0070 */ float m_meshradius;
    /* 0x0074 */ int m_points;
    CVector m_point1;
    CVector m_point2;
    CVector m_point2Max;
    /* 0x009c */ int m_colors[20];
    /* 0x00ec */ float m_sizes[20];
    /* 0x013c */ PBlendMode m_blendMode;
    /* 0x0140 */ bool m_forv;
    /* 0x0141 */ bool m_back;
    /* 0x0142 */ bool m_orient;
    /* 0x0143 */ char Padding_195;
    /* 0x0144 */ float m_scaleparts;
    /* 0x0148 */ bool m_updateXForm;
    /* 0x0149 */ char Padding_196[3];
    /* 0x014c */ float m_SpriteAngle;
    /* 0x0150 */ int m_TexTiling;
    /* 0x0154 */ char m_texName[50];
    /* 0x0186 */ char Padding_197[2];
    /* 0x0188 */ PsShaderType m_shaderType;
    /* 0x018c */ float m_emitAtPeriod;
    WorkTime m_wtime;
    /* 0x019c */ unsigned int m_maxParticles;
    /* 0x01a0 */ float m_ttlMin;
    /* 0x01a4 */ float m_ttlMax;
    /* 0x01a8 */ float m_resettime;
    /* 0x01ac */ bool m_localStop;
    /* 0x01ad */ char Padding_198[3];
    /* 0x01b0 */ float m_stopTime;
    /* 0x01b4 */ float m_start;
    /* 0x01b8 */ Force m_x0[3];
    /* 0x01e8 */ CoordinatesSystemType m_x0Cst;
    /* 0x01ec */ Force m_pos[3];
    /* 0x021c */ float m_parentDependency;
    CVector m_bBoxMin;
    CVector m_bBoxMax;
    /* 0x0238 */ char m_PartsModelName[50];
}; /* size: 0x026c */

static_assert(sizeof(PSProps) == 0x026c);

struct AttrProps
{
    /* 0x0000 */ char m_Name[50];
    /* 0x0032 */ char m_ClassName[50];
    /* 0x0064 */ bool m_On;
    /* 0x0065 */ char Padding_173[3];
    WorkTime m_wtime;
    /* 0x0074 */ TimeMode m_timemode;
    /* 0x0078 */ ForceMode m_mode;
    /* 0x007c */ WorkMode m_State;
    /* 0x0080 */ bool m_emitterOn;
    /* 0x0081 */ char Padding_174[3];
    /* 0x0084 */ Force m_force[3];
    CVector m_org;
    /* 0x00c0 */ CoordinatesSystemType m_csType;
    /* 0x00c4 */ GameInteraction m_interactionType;
    /* 0x00c8 */ float m_min;
    /* 0x00cc */ float m_max;
    /* 0x00d0 */ float m_freq;
    /* 0x00d4 */ ForceType m_type;
}; /* size: 0x00d8 */

static_assert(sizeof(AttrProps) == 0x00d8);
