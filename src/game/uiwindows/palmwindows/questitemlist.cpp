#include "questitemlist.h"

RT_CLASS_EXPORTS_BEGIN(QuestItemList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItemList);

int QuestItemList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestItemList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int QuestItemList::CreateQuestItemList(ListType, BoundsBase<float> const&, unsigned, unsigned)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestItemList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestItemList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::SetupForParentQuest(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestItemList::Clone()
{
    throw std::logic_error("Not implemented");
}

QuestItemList::~QuestItemList()
{
    throw std::logic_error("Not implemented");
}

QuestItemList::QuestItemList(QuestItemList const&)
{
    throw std::logic_error("Not implemented");
}

QuestItemList::QuestItemList()
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::SetCurSel(int)
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::OnNavPointAdded(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::OnQuestStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::AddQuestItem(help::QuestType, int, bool)
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::OnNavPointDeleted(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::CreateStaticQuestItems()
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::OnDynamicQuestStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

QuestItem* QuestItemList::GetQuestItemByQuestId(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::GetDynamicQuestIdsToShow(std::vector<int, std::allocator<int>>&) const
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::GetStaticQuestIdsToShow(std::vector<int, std::allocator<int>>&) const
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::GetQuestItemIdxByQuestId(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::UpdateQuestItem(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::UpdateBounds()
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::OnNavPoint(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::CreateDynamicQuestItems()
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::OnQuestWasTaken(void*)
{
    throw std::logic_error("Not implemented");
}

QuestItem* QuestItemList::GetQuestItemForAlphaNpTexture(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

QuestItem* QuestItemList::CreateQuestItem(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::SortStaticQuests(std::vector<int, std::allocator<int>>&) const
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

bool QuestItemList::CanQuestBeShown(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

int QuestItemList::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}

void QuestItemList::SortDynamicQuests(std::vector<int, std::allocator<int>>&) const
{
    throw std::logic_error("Not implemented");
}
