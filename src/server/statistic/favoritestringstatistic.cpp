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
        throw std::logic_error("Not implemented");
    }

    void FavoriteStringStatistic::Zero()
    {
        throw std::logic_error("Not implemented");
    }

    CStr FavoriteStringStatistic::GetValue() const
    {
        throw std::logic_error("Not implemented");
    }

    void FavoriteStringStatistic::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void FavoriteStringStatistic::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam FavoriteStringStatistic::GetValueAsAIParam() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* FavoriteStringStatistic::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    FavoriteStringStatistic::~FavoriteStringStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* FavoriteStringStatistic::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void FavoriteStringStatistic::Increase(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    FavoriteStringStatistic::FavoriteStringStatistic()
    {
        throw std::logic_error("Not implemented");
    }

    FavoriteStringStatistic::FavoriteStringStatistic(FavoriteStringStatistic const&)
    {
        throw std::logic_error("Not implemented");
    }
}
