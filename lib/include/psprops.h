#pragma once
#include <math/vector.h>

#include <cstdint>

// Particle-system and attractor records are written to and read from particle files byte
// for byte (ParticlesServer), so every field has a fixed width and packing is explicit.
#pragma pack(push, 4)

struct WorkTime
{
    WorkTime();
    float m_start;
    float m_length;
    float m_repeat;
};

enum ForceType : int32_t
{
    PS_FORCE_RANDOM = 0x0,
    PS_FORCE_SINE = 0x1,
};

struct Force
{
    float m_min;
    float m_max;
    float m_freq;
    ForceType m_type;
    Force();
    void SetAxis(float forceMin, float forceMax, ForceType ft, float period);
    void GetAxis(float&, float&, ForceType&, float&) const;
};

enum CoordinatesSystemType : int32_t
{
    PS_CST_CARTHESIAN = 0x0,
    PS_CST_POLAR = 0x1,
    PS_CST_POLAR_ORG = 0x2,
};

enum TimeMode : int32_t
{
    PS_TIME_LOCAL = 0x1,
    PS_TIME_GLOBAL = 0x0,
};

enum ForceMode : int32_t
{
    PS_FORCE_VEL = 0x0,
    PS_FORCE_ACCEL = 0x1,
    PS_FORCE_POS = 0x2,
};

enum WorkMode : int32_t
{
    OFF = 0x0,
    ACCELERATION = 0x1,
    SPEED = 0x2,
    WMPOSITION = 0x3,
};

enum GameInteraction : int32_t
{
    GI_WIND = 0x0,
    GI_SIZE = 0x1,
};

enum PBlendMode : int32_t
{
    PS_ADD = 0x0,
    PS_ALPHA = 0x1,
    PS_ADDSMOOTH = 0x2,
    PS_ADDSIGNED = 0x3,
};

enum PsShaderType : int32_t
{
    PSST_DUST = 0x0,
    PSST_LIGHT = 0x1,
    PSST_NOFOG_DUST = 0x2,
    PSST_NOFOG_LIGHT = 0x3,
};

struct PSProps
{
    char m_Name[50];
    char m_ClassName[50];
    bool m_Specific;
    bool m_HaveTrail;
    char Padding_193[2];
    int32_t m_trailLen;
    bool m_CreateOne;
    bool m_autoMeshEmitter;
    char Padding_194[2];
    float m_meshradius;
    int32_t m_points;
    CVector m_point1;
    CVector m_point2;
    CVector m_point2Max;
    int32_t m_colors[20];
    float m_sizes[20];
    PBlendMode m_blendMode;
    bool m_forv;
    bool m_back;
    bool m_orient;
    char Padding_195;
    float m_scaleparts;
    bool m_updateXForm;
    char Padding_196[3];
    float m_SpriteAngle;
    int32_t m_TexTiling;
    char m_texName[50];
    char Padding_197[2];
    PsShaderType m_shaderType;
    float m_emitAtPeriod;
    WorkTime m_wtime;
    uint32_t m_maxParticles;
    float m_ttlMin;
    float m_ttlMax;
    float m_resettime;
    bool m_localStop;
    char Padding_198[3];
    float m_stopTime;
    float m_start;
    Force m_x0[3];
    CoordinatesSystemType m_x0Cst;
    Force m_pos[3];
    float m_parentDependency;
    CVector m_bBoxMin;
    CVector m_bBoxMax;
    char m_PartsModelName[50];
};

#pragma pack(pop)

static_assert(sizeof(WorkTime) == 0x000c);
static_assert(sizeof(Force) == 0x0010);
static_assert(sizeof(PSProps) == 0x026c);

#pragma pack(push, 4)
struct AttrProps
{
    char m_Name[50];
    char m_ClassName[50];
    bool m_On;
    char Padding_173[3];
    WorkTime m_wtime;
    TimeMode m_timemode;
    ForceMode m_mode;
    WorkMode m_State;
    bool m_emitterOn;
    char Padding_174[3];
    Force m_force[3];
    CVector m_org;
    CoordinatesSystemType m_csType;
    GameInteraction m_interactionType;
    float m_min;
    float m_max;
    float m_freq;
    ForceType m_type;
};
#pragma pack(pop)

static_assert(sizeof(AttrProps) == 0x00d8);
