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

    class Weather : public m3d::Object
    {
    protected:
        Weather();
        Weather(const m3d::Weather& classe);

    public:
        virtual  ~Weather() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classWeather;
        CStr m_Name;
        /* 0x0040 */ CVector m_colorSets[4][7];
        /* 0x0190 */ CVector m_currentColors[7];
        /* 0x01e4 */ bool m_shadowVisibility[4];
        /* 0x01e8 */ float m_shadowTransparency[4];
        /* 0x01f8 */ float m_cloudsSpeed[4];
        /* 0x0208 */ float m_reduceDistFactor;
        /* 0x020c */ float m_weatherWeight;
        /* 0x0210 */ float m_waterSpeed;
        /* 0x0214 */ float m_waterHeightBig;
        /* 0x0218 */ float m_waterHeightSmall;
        /* 0x021c */ float m_waterSizeBig;
        /* 0x0220 */ float m_waterSizeSmall;
        /* 0x0224 */ float m_waterCourseAng;
        /* 0x0228 */ float m_waterSpecularM;
        /* 0x022c */ float m_waterSpecularS;
        /* 0x0230 */ float m_weatherSkyDomeFactor;
        m3d::WindInfo m_wind;
        /* 0x0270 */ CStr m_PostEffectName[4];
        /* 0x02a0 */ CStr m_lightmapTextureName[4];
        /* 0x02d0 */ CStr m_cloudsTextureName[4];
        virtual void DefaultInitialize() /* 0x3c */;
        virtual void Release() /* 0x40 */;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x08 */;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x10 */;
        virtual int ReadDetailFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x44 */;
        virtual int WriteDetailToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) /* 0x48 */;
        virtual int Update(float amount, int curServerTime) /* 0x4c */;
        virtual int Render() /* 0x50 */;
        virtual int UpdateColors(m3d::ColorItems colorItem, m3d::ColorTypes curTime) /* 0x54 */;
        virtual int TurnOffEffects() /* 0x58 */;
        virtual void SetUp() /* 0x5c */;
        unsigned int GetColorItemsNum() const;
        unsigned int GetColorTypesNum() const;
        const float GetSkyDomeFactor() const;
        void SetSkyDomeFactor(float);
        const char* ColorItemName(unsigned int i) const;
        const char* ColorTypeName(unsigned int i) const;
        CVector& Color(unsigned int, unsigned int);
        CVector& CurrentColor(unsigned int);
        const CVector& CurrentColor(unsigned int Item) const;
        const CStr& GetWeatherName() const;
        void SetWeatherName(const CStr& Name);
        void ChangeCloudTexture(CStr& Name);
        float CloudsSpeed(unsigned int) const;
        void SetCloudsSpeed(unsigned int, float);
        const m3d::WindInfo& GetWindInfo() const;
        const CStr& GetPostEffectName(unsigned int) const;
        void SetPostEffectName(unsigned int, const CStr&);
        float GetShadowTransparency(unsigned int curDayTime) const;
        void SetShadowTransparency(unsigned int, float);
        bool GetShadowVisibility(unsigned int curDayTime) const;
        void SetShadowVisibility(unsigned int, bool);
        float GetWaterSpeed() const;
        void SetWaterSpeed(float);
        float GetWaveHSmall() const;
        void SetWaveHSmall(float);
        float GetWaveHBig() const;
        void SetWaveHBig(float);
        float GetWaveSizeSmall() const;
        void SetWaveSizeSmall(float);
        float GetWaveSizeBig() const;
        void SetWaveSizeBig(float);
        float GetWaterCourseAngle() const;
        void SetWaterCourseAngle(float);
        void SetWaterSpecularS(float);
        void SetWaterSpecularM(float);
        float GetWaterSpecularM() const;
        float GetWaterSpecularS() const;
        const CStr& GetLightmapTexName(unsigned int curDayTime) const;
        void SetLightmapTexName(unsigned int, const CStr&);
        const CStr& GetCloudsTexName(unsigned int curDayTime) const;
        void SetCloudsTexName(unsigned int, const CStr&);
    }; /* size: 0x0300 */

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
