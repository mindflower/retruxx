#pragma once
#include <math/vector.h>

class WorkTime
{
public:
    WorkTime();

private:
    float m_start;
    float m_length;
    float m_repeat;
};

enum ForceType
{
    PS_FORCE_RANDOM = 0x0,
    PS_FORCE_SINE = 0x1,
};

class Force
{
public:
    void SetAxis(float,float,ForceType,float);
    Force();

private:
    float m_min;
    float m_max;
    float m_freq;
    ForceType m_type;
};

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

class PSProps
{
private:
    char m_Name[50];
    char m_ClassName[50];
    bool m_Specific;
    bool m_HaveTrail;
    int m_trailLen;
    bool m_CreateOne;
    bool m_autoMeshEmitter;
    float m_meshradius;
    int m_points;
    CVector m_point1;
    CVector m_point2;
    CVector m_point2Max;
    int m_colors[20];
    float m_sizes[20];
    PBlendMode m_blendMode;
    bool m_forv;
    bool m_back;
    bool m_orient;
    float m_scaleparts;
    bool m_updateXForm;
    float m_SpriteAngle;
    int m_TexTiling;
    char m_texName[50];
    PsShaderType m_shaderType;
    float m_emitAtPeriod;
    WorkTime m_wtime;
    unsigned int m_maxParticles;
    float m_ttlMin;
    float m_ttlMax;
    float m_resettime;
    bool m_localStop;
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

class AttrProps
{
private:
    char m_Name[50];
    char m_ClassName[50];
    bool m_On;
    WorkTime m_wtime;
    TimeMode m_timemode;
    ForceMode m_mode;
    WorkMode m_State;
    bool m_emitterOn;
    Force m_force[3];
    CVector m_org;
    CoordinatesSystemType m_csType;
    GameInteraction m_interactionType;
    float m_min;
    float m_max;
    float m_freq;
    ForceType m_type;
};
