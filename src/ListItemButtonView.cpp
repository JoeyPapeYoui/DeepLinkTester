// © You i Labs Inc. 2000-2017. All rights reserved.
#include "ListItemButtonView.h"

#include "DeepLinkCaller-C-Interface.h"

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

void ListItemButtonView::SetDeepLinkInformation(const CYIString &name, const CYIString &url)
{
    m_deepLinkURL = url;
    m_pDeepLinkText->SetText(name.IsEmpty() ? url : name);
}

void ListItemButtonView::Reset()
{
    // The view is being recycled by the list view. It will be reset to original state here.
    m_deepLinkURL = "";
    m_pDeepLinkText->SetText("");
    CYIPushButtonView::Reset();
}

void ListItemButtonView::OnRelease()
{
    CYIPushButtonView::OnRelease();
    CallDeepLink(m_deepLinkURL);
}
