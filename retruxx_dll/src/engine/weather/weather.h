#pragma once
#include <vector>
#include <core/clazz.h>
#include <core/ref_ptr.h>
#include <core/stringm3d.h>
#include <math/vector.h>

namespace m3d
{
    class SgNode;

    enum ColorItems
    {
        CI_SKY = 0x0,
        CI_FOG = 0x1,
        CI_AMBIENT = 0x2,
        CI_DIFFUSE = 0x3,
        CI_SUN = 0x4,
        CI_PLANT = 0x5,
        CI_SPECULAR = 0x6,
        CI_NUM_COLORITEMS = 0x7,
    };

    enum ColorTypes
    {
        CT_SUNRISE = 0x0,
        CT_DAY = 0x1,
        CT_SUNSET = 0x2,
        CT_NIGHT = 0x3,
        CT_NUM_COLORTYPES = 0x4,
    };


    class WindInfo
    {
    public:
        void Write(ref_ptr<cmn::XmlNode>);
        void Read(ref_ptr<cmn::XmlNode>);
        WindInfo();
        CVector const& GetCurWind() const;
        CVector const& GetDeltaVel() const;
        void CalculateCurWind(float);

    private:
        float m_maxVel;
        float m_minVel;
        float m_changeVelTime;
        float m_changeDirTime;
        CVector m_prevValue;
        CVector m_curValue;
        CVector m_deltaValue;
        float m_DeltaVelChanged;
        float m_DeltaDirChanged;

    };

    class Weather : public Object
    {
    public:
        CStr const & GetWeatherName() const ;
        bool GetShadowVisibility(unsigned int) const ;
        static Class * GetBaseClass();
        CVector const & CurrentColor(unsigned int) const ;
        virtual ~Weather();
        float GetWaveHBig() const ;
        char const * ColorTypeName(unsigned int) const ;
        void ChangeCloudTexture(CStr &);
        static Object * CreateObject();
        virtual int UpdateColors(ColorItems,ColorTypes);
        void SetWeatherName(CStr const &);
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int TurnOffEffects();
        virtual int WriteDetailToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        float GetWaveSizeBig() const ;
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        float GetWaterSpecularS() const ;
        float GetWaterSpecularM() const ;
        CStr const & GetLightmapTexName(unsigned int) const ;
        virtual Object * Clone();
        virtual void SetUp();
        virtual int ReadDetailFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        WindInfo const & GetWindInfo() const ;
        float GetWaterSpeed() const ;
        float GetWaterCourseAngle() const ;
        virtual int Update(float,int);
        virtual void DefaultInitialize();
        char const * ColorItemName(unsigned int) const ;
        virtual Class * GetClass() const ;
        float GetWaveHSmall() const ;
        CStr const & GetCloudsTexName(unsigned int) const ;
        virtual void Release();
        virtual int Render();
        float GetShadowTransparency(unsigned int) const ;
        float GetWaveSizeSmall() const ;

    protected:
        Weather(Weather const &);
        Weather();

    public:
        RT_CLASS_DECLARE(Weather);

    private:
        CStr m_Name;
        CVector m_colorSets[7][4];
        CVector m_currentColors[7];
        bool m_shadowVisibility[4];
        float m_shadowTransparency[4];
        float m_cloudsSpeed[4];
        float m_reduceDistFactor;
        float m_weatherWeight;
        float m_waterSpeed;
        float m_waterHeightBig;
        float m_waterHeightSmall;
        float m_waterSizeBig;
        float m_waterSizeSmall;
        float m_waterCourseAng;
        float m_waterSpecularM;
        float m_waterSpecularS;
        float m_weatherSkyDomeFactor;
        WindInfo m_wind;
        CStr m_PostEffectName[4];
        CStr m_lightmapTextureName[4];
        CStr m_cloudsTextureName[4];
    };

    class WeatherClear : public Weather
    {
    public:
        static Class* GetBaseClass();
        virtual Object* Clone();
        virtual ~WeatherClear();
        virtual Class* GetClass() const;
        static Object* CreateObject();

    protected:
        WeatherClear(WeatherClear const&);
        WeatherClear();

    public:
        RT_CLASS_DECLARE(WeatherClear);
    };

    class WeatherInclement : public Weather
    {
    public:
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual void SetUp();
        virtual void RecreateEffect();
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        static Class* GetBaseClass();
        virtual ~WeatherInclement();
        virtual void DefaultInitialize();
        virtual Class* GetClass() const;
        static Object* CreateObject();
        virtual Object* Clone();
        virtual int Render();
        virtual int TurnOffEffects();

    protected:
        WeatherInclement(WeatherInclement const&);
        WeatherInclement();

    public:
        RT_CLASS_DECLARE(WeatherInclement);

    private:
        CStr m_inclementNodeName;
        CStr m_soundNodeName;
        int m_weatherDensity;
        float m_weatherDist;
        std::vector<SgNode*> m_weatherNodes;
    };

    class WeatherFoggy : public Weather
    {
    public:
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual ~WeatherFoggy();
        static Class* GetBaseClass();
        virtual Class* GetClass() const;
        static Object* CreateObject();
        virtual Object* Clone();

    protected:
        WeatherFoggy(WeatherFoggy const&);
        WeatherFoggy();

    public:
        RT_CLASS_DECLARE(WeatherFoggy);
    };

    class WeatherThunderstorm : public WeatherInclement
    {
    public:
        virtual void DefaultInitialize();
        virtual int Update(float, int);
        virtual Class* GetClass() const;
        virtual ~WeatherThunderstorm();
        virtual int Render();
        static Class* GetBaseClass();
        static Object* CreateObject();
        virtual int UpdateColors(ColorItems, ColorTypes);
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int TurnOffEffects();
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual void SetUp();
        virtual Object* Clone();

    protected:
        WeatherThunderstorm();
        WeatherThunderstorm(WeatherThunderstorm const&);

    public:
        RT_CLASS_DECLARE(WeatherThunderstorm);

    private:
        int m_lastthunderTime;
        int m_thunderTimeOut;
        int m_thunderWorktime;
        int m_curthunderTime;
        bool m_thunderActive;
        float m_thunderLerpValue;
        std::vector<int> m_effectId;
        int m_effectCount;
        std::vector<CStr> m_effectNames;
        int m_maxThunderDuty;
        int m_minThunderDuty;
        int m_minThunderPeriod;
        int m_maxThunderPeriod;
    };
}
