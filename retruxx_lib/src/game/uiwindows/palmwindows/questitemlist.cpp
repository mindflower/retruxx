#include "questitemlist.h"

RT_CLASS_EXPORTS_BEGIN(QuestItemList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItemList);

int QuestItemList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestItemList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int QuestItemList::CreateQuestItemList(ListType, BoundsBase<float> const&, unsigned, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestItemList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestItemList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::SetupForParentQuest(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestItemList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItemList::~QuestItemList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItemList::QuestItemList(QuestItemList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItemList::QuestItemList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::SetCurSel(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::OnNavPointAdded(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::OnQuestStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::AddQuestItem(help::QuestType, int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::OnNavPointDeleted(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::CreateStaticQuestItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::OnDynamicQuestStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItem* QuestItemList::GetQuestItemByQuestId(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::GetDynamicQuestIdsToShow(std::vector<int, std::allocator<int>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::GetStaticQuestIdsToShow(std::vector<int, std::allocator<int>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::GetQuestItemIdxByQuestId(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::UpdateQuestItem(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::UpdateBounds()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::OnNavPoint(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::CreateDynamicQuestItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::OnQuestWasTaken(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItem* QuestItemList::GetQuestItemForAlphaNpTexture(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItem* QuestItemList::CreateQuestItem(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::SortStaticQuests(std::vector<int, std::allocator<int>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestItemList::CanQuestBeShown(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemList::SortDynamicQuests(std::vector<int, std::allocator<int>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
