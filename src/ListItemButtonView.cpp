// © You i Labs Inc. 2000-2017. All rights reserved.
#include "ListItemButtonView.h"

#include <view/YiImageView.h>

YI_TYPE_DEF_INST(ListItemButtonView, CYIPushButtonView)

static const CYIString LOG_TAG("ListItemButtonView");

ListItemButtonView::ListItemButtonView()
: m_pImageView(YI_NULL)
, m_pPlaceholderLogo(YI_NULL)
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

    bOK = bOK && FindNode<CYIImageView>(m_pImageView, "list-item-bg", CYISceneView::FETCH_MANDATORY, LOG_TAG);
    bOK = bOK && FindNode<CYISceneNode>(m_pPlaceholderLogo, "logo-light.png", CYISceneView::FETCH_MANDATORY, LOG_TAG);

    if (!bOK)
    {
        YI_ASSERT(false, LOG_TAG, "A required node could not be found.");
        return false;
    }

    m_pImageView->ImageAssigned.Connect(*this, &ListItemButtonView::OnImageAssigned);
    m_pImageView->ImageAssignmentFailed.Connect(*this, &ListItemButtonView::OnImageAssignmentFailed);

    return true;
}

void ListItemButtonView::Reset()
{
    // The view is being recycled by the list view. It will be reset to original state here.
    m_pPlaceholderLogo->Show();
    m_pImageView->Reset();
    CYIPushButtonView::Reset();
}

void ListItemButtonView::SetImageURL(const CYIUrl &url)
{
    // The image will now be downloaded.
    m_pImageView->SetImage(url);
}

void ListItemButtonView::OnImageAssigned()
{
    // The image download has completed and been assigned on the image view. The logo can now be hidden.
    m_pPlaceholderLogo->Hide();
}

void ListItemButtonView::OnImageAssignmentFailed(const CYIString &errorMessage)
{
    YI_LOGE(LOG_TAG, "Image assignment failed: %s", errorMessage.GetData());
}
