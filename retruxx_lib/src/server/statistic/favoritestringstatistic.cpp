#include "favoritestringstatistic.h"

#include <stdexcept>

#include "core/aiparam.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(FavoriteStringStatistic)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(FavoriteStringStatistic);

    m3d::Class* FavoriteStringStatistic::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Statistic);
    }

    m3d::Object* FavoriteStringStatistic::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FavoriteStringStatistic::Zero()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr FavoriteStringStatistic::GetValue() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FavoriteStringStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FavoriteStringStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::AIParam FavoriteStringStatistic::GetValueAsAIParam() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* FavoriteStringStatistic::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    FavoriteStringStatistic::~FavoriteStringStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* FavoriteStringStatistic::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void FavoriteStringStatistic::Increase(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    FavoriteStringStatistic::FavoriteStringStatistic()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    FavoriteStringStatistic::FavoriteStringStatistic(FavoriteStringStatistic const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
