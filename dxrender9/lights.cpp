// Fixed-function lights and material of CDevice.
// Ported from the original dxrender9/lights.cpp (lights.obj).
#include "device.h"
#include "log.h"

// orig data 0x7e6398 - m3d::rend::LightType (POINT, SPOT, DIRECTIONAL) to D3DLIGHTTYPE.
static D3DLIGHTTYPE m3dLightTypeToD3dLightType[3] = {
    D3DLIGHT_POINT,
    D3DLIGHT_SPOT,
    D3DLIGHT_DIRECTIONAL,
};

// orig 0x64f270 lights.cpp:37
void CDevice::LightEnable(int numLight, int state)
{
    if (numLight < 8)
    {
        m_lightsEnabled[numLight] = state;
        m_pd3dDevice->LightEnable(numLight, state);
    }
}

// orig 0x64f3e0 lights.cpp:49
void CDevice::LightSet(int numLight, LightSource const& l)
{
    if (numLight < 8)
    {
        D3DLIGHT9 light;
        light.Type = m3dLightTypeToD3dLightType[l.m_type];

        light.Diffuse.r = l.m_diffuse.r;
        light.Diffuse.g = l.m_diffuse.g;
        light.Diffuse.b = l.m_diffuse.b;
        light.Diffuse.a = l.m_diffuse.a;

        light.Specular.r = l.m_specular.r;
        light.Specular.g = l.m_specular.g;
        light.Specular.b = l.m_specular.b;
        light.Specular.a = l.m_specular.a;

        light.Ambient.r = l.m_ambient.r;
        light.Ambient.g = l.m_ambient.g;
        light.Ambient.b = l.m_ambient.b;
        light.Ambient.a = l.m_ambient.a;

        light.Range = l.m_range;
        light.Falloff = l.m_falloff;
        light.Attenuation0 = l.m_attenuation0;
        light.Attenuation1 = l.m_attenuation1;
        light.Attenuation2 = l.m_attenuation2;
        light.Theta = l.m_theta;
        light.Phi = l.m_phi;

        light.Position.x = l.m_origin.x;
        light.Position.y = l.m_origin.y;
        light.Position.z = l.m_origin.z;

        light.Direction.x = l.m_direction.x;
        light.Direction.y = l.m_direction.y;
        light.Direction.z = l.m_direction.z;

        LightSet(numLight, light);
    }
}

// orig 0x64f2a0 lights.cpp:96
HRESULT CDevice::LightSet(int numLight, D3DLIGHT9 const& l)
{
    m_lights[numLight] = l;
    return m_pd3dDevice->SetLight(numLight, &l);
}

// orig 0x64f2f0 lights.cpp:104
void CDevice::MaterialSet(Material const& mat)
{
    D3DMATERIAL9 mm;

    mm.Diffuse.r = mat.m_diffuse.r;
    mm.Diffuse.g = mat.m_diffuse.g;
    mm.Diffuse.b = mat.m_diffuse.b;
    mm.Diffuse.a = mat.m_diffuse.a;

    mm.Ambient.r = mat.m_ambient.r;
    mm.Ambient.g = mat.m_ambient.g;
    mm.Ambient.b = mat.m_ambient.b;
    mm.Ambient.a = mat.m_ambient.a;

    mm.Specular.r = mat.m_specular.r;
    mm.Specular.g = mat.m_specular.g;
    mm.Specular.b = mat.m_specular.b;
    mm.Specular.a = mat.m_specular.a;

    mm.Emissive.r = mat.m_emissive.r;
    mm.Emissive.g = mat.m_emissive.g;
    mm.Emissive.b = mat.m_emissive.b;
    mm.Emissive.a = mat.m_emissive.a;

    mm.Power = mat.m_specularPower;

    m_lastResult = m_pd3dDevice->SetMaterial(&mm);
}
