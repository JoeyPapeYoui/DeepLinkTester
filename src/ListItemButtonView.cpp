// © You i Labs Inc. 2000-2017. All rights reserved.
#include "ListItemButtonView.h"

#include <deeplinking/YiDeepLinkBridgeLocator.h>
#include <scenetree/YiTextSceneNode.h>

YI_TYPE_DEF_INST(ListItemButtonView, CYIPushButtonView)

static const CYIString LOG_TAG("ListItemButtonView");

ListItemButtonView::ListItemButtonView()
: m_pDeepLinkText(YI_NULL)
{
}

ListItemButtonView::~ListItemButtonView()
{
}

bool ListItemButtonView::Init()
{
    if (!CYIPushButtonView::Init())
    {
        return false;
    }

    bool bOK = true;

    bOK = bOK && FindNode<CYITextSceneNode>(m_pDeepLinkText, "DeepLinkName", CYISceneView::FETCH_MANDATORY, LOG_TAG);

    if (!bOK)
    {
        YI_ASSERT(false, LOG_TAG, "A required node could not be found.");
        return false;
    }

    return true;
}

void ListItemButtonView::SetDeepLinkInformation(const CYIString &name, const CYIUrl &url)
{
    m_deepLinkURL = url;
    
    CYIString text = name.IsEmpty() ? url.ToString() : name;
    
    CYIDeepLinkBridge *pDeepLinkBridge = CYIDeepLinkBridgeLocator::GetDeepLinkBridge();
    if (pDeepLinkBridge)
    {
        if (!pDeepLinkBridge->CanOpenUrl(m_deepLinkURL))
        {
            text += "\n(Cannot Open URL)";
        }
    }
    else
    {
        text += "\n(Open URL Not Supported)";
    }
    
    m_pDeepLinkText->SetText(text);
}

void ListItemButtonView::Reset()
{
    // The view is being recycled by the list view. It will be reset to original state here.
    m_deepLinkURL.Clear();
    m_pDeepLinkText->SetText("");
    CYIPushButtonView::Reset();
}

void ListItemButtonView::OnRelease()
{
    CYIPushButtonView::OnRelease();
    CYIDeepLinkBridge *pDeepLinkBridge = CYIDeepLinkBridgeLocator::GetDeepLinkBridge();
    if (pDeepLinkBridge)
    {
        pDeepLinkBridge->OpenUrl(m_deepLinkURL);
    }
}
