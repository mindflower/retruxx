#include "historywnd.h"

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <ui/textbox.h>
#include <ui/scroll.h>
#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>

RT_CLASS_EXPORTS_BEGIN(HistoryWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(HistoryWnd);

HistoryWnd::AuxInfo::AuxInfo()
{
    m_txtName = "txt";
    m_timeColor = 0x88FF0000;
}

m3d::Class* HistoryWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* HistoryWnd::CreateObject()
{
    return new HistoryWnd;
}

m3d::Object* HistoryWnd::Clone()
{
    return new HistoryWnd;
}

int HistoryWnd::AddRecord(CStr const& strTextId, m3d::AIParam const& time)
{
    if (strTextId.empty())
    {
        M3D_LOG_INFO("HistoryWnd::AddRecord error - invalid text ID");
        return 0;
    }

    Record newRecord;
    newRecord.m_strTextId = strTextId;
    newRecord.m_time = time;
    m_records.push_back(newRecord);

    auto const timeElements = time.GetAsIdList();
    bool validTime = true;
    if (!timeElements.empty() && timeElements.size() != 5)
    {
        M3D_LOG_INFO("HistoryWnd::AddRecord warning - invalid time format, time param would be ignored");
        validTime = false;
    }

    if (m_txt)
    {
        CStr strTime;
        if (validTime && timeElements.size() == 5)
        {
            strTime = help::Color2Str(m_aif.m_timeColor) + CStr(timeElements[2]) + CStr(".") + CStr(timeElements[3]) +
                CStr(".") + CStr(timeElements[4]);
        }

        CStr const text = M3D_APP->GetStringByStringId0(strTextId);

        CStr allText = m_txt->GetText();
        if (!allText.empty())
        {
            allText += CStr("|@ffffffff|@ffffffff");
        }
        if (!strTime.empty())
        {
            allText += strTime;
            allText += CStr("|");
        }
        allText += help::Color2Str(m_txt->GetTextColor());
        allText += text;

        m_txt->SetText(allText);
        m_txt->SetVScrollToPos(100);
    }

    return validTime ? 1 : 0;
}

m3d::Class* HistoryWnd::GetClass() const
{
    return RT_CLASS_LOCAL(HistoryWnd);
}

HistoryWnd::~HistoryWnd()
{
    m_records.clear();
    if ((m_gameDataFlags & 1) != 0 && m_txt)
    {
        m_txt->SetText({});
    }
}

int HistoryWnd::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* child = GetChildByName(m_aif.m_txtName);
        if (child && child->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_gameDataFlags |= 1;
            m_txt = static_cast<m3d::ui::TextBoxWnd*>(child);
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_txtName + CStr(" is not found or incorrect type"));
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_INFO("HistoryWnd: error - fail to init because of a bad resource");
    return 0;
}

HistoryWnd::HistoryWnd()
{
    m_txt = 0;
}

HistoryWnd::HistoryWnd(HistoryWnd const&) : HistoryWnd()
{
}

int HistoryWnd::GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("HistoryWnd::GameDataSave error - journal has been not properly inited");
        return 0;
    }
    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("HistoryWnd::GameDataSave error - invalid params");
        return 0;
    }

    ref_ptr historyNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "History");
    guiNode->AddChild(historyNode);

    float vScrollPos = 0.0f;
    if (auto* scroll = m_txt->GetScrollVWnd())
    {
        vScrollPos = scroll->GetCurPos();
    }
    historyNode->SetAttribute("VScrollPos", CStr(vScrollPos).c_str());

    ref_ptr recordsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Records");
    historyNode->AddChild(recordsNode);

    for (auto const& record : m_records)
    {
        ref_ptr recordNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Record");
        recordsNode->AddChild(recordNode);
        recordNode->SetAttribute("TextId", record.m_strTextId.c_str());

        ref_ptr timeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Time");
        recordNode->AddChild(timeNode);
        record.m_time.SaveToXML(xmlFile, timeNode);
    }
    return 1;
}

int HistoryWnd::OnAfterAddToWndStation()
{
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

int HistoryWnd::GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("HistoryWnd::GameDataLoad error - HistoryWnd has been not properly inited");
        return 0;
    }

    GameDataClear(false);

    if (!xmlFile || !guiNode)
    {
        M3D_LOG_INFO("HistoryWnd::GameDataLoad error - invalid params");
        return 0;
    }

    ref_ptr historyNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    guiNode->GetFirstChild(historyNode, "History");
    if (historyNode->IsEmpty())
    {
        M3D_LOG_INFO("HistoryWnd::GameDataLoad error - cannot find history node");
        return 0;
    }

    float vScrollPos = 0.0f;
    m3d::SafeFloatAttrib(vScrollPos, historyNode, "VScrollPos");

    int res = 1;
    ref_ptr recordsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    historyNode->GetFirstChild(recordsNode, "Records");
    if (!recordsNode->IsEmpty())
    {
        ref_ptr recordNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (recordsNode->GetFirstChild(recordNode, "Record"); !recordNode->IsEmpty();
             recordNode->GetNextSibling(recordNode, "Record"))
        {
            CStr strTextId;
            m3d::SafeStrAttrib(strTextId, recordNode, "TextId");

            m3d::AIParam time;
            ref_ptr timeNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            recordNode->GetFirstChild(timeNode, "Time");
            if (!timeNode->IsEmpty())
            {
                time.LoadFromXML(xmlFile, timeNode);
            }

            res &= AddRecord(strTextId, time);
        }
    }

    if (auto* scroll = m_txt->GetScrollVWnd())
    {
        scroll->SetCurPos(vScrollPos);
    }

    if (!res)
    {
        M3D_LOG_INFO("HistoryWnd::GameDataLoad - loading with errors");
    }
    return res;
}

int HistoryWnd::GameDataClear(bool beforeContinuousLevel)
{
    if (!beforeContinuousLevel)
    {
        m_records.clear();
        if ((m_gameDataFlags & 1) != 0)
        {
            m_txt->SetText({});
        }
    }
    return 1;
}
