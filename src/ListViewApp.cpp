// © You i Labs Inc. 2000-2017. All rights reserved.
#include "ListViewApp.h"

#include "ListItemButtonView.h"
#include "Urls.h"

#include <appium/YiWebDriverLocator.h>
#include <framework/YiFramework.h>
#include <import/YiViewTemplate.h>
#include <platform/YiApplicationUIBridgeLocator.h>
#include <platform/YiDeviceBridgeLocator.h>
#include <scenetree/YiSceneManager.h>
#include <scenetree/YiTextSceneNode.h>
#include <view/YiImageView.h>
#include <view/YiListView.h>
#include <view/YiPooledViewRecycler.h>
#include <view/YiPushButtonView.h>
#include <view/YiSceneView.h>
#include <view/YiTextEditView.h>
#include <view/YiViewRecycler.h>

static const CYIString LOG_TAG("ListViewApp");

ListViewApp::ListAdapter::ListAdapter(DataModel *pModel, const CYISharedPtr<CYIAssetViewTemplate> &pItemViewTemplate, const CYISharedPtr<IYIViewRecycler> &pViewRecycler)
: CYIViewAdapter(pViewRecycler)
, m_pModel(pModel)
, m_pItemViewTemplate(pItemViewTemplate)
{
}

YI_UINT32 ListViewApp::ListAdapter::GetItemsCount() const
{
    return m_pModel->GetAssetCount();
}

CYISharedPtr<CYIAssetViewTemplate> ListViewApp::ListAdapter::GetViewTemplate(YI_UINT32 uIndex) const
{
    YI_UNUSED(uIndex);
    return m_pItemViewTemplate;
}

void ListViewApp::ListAdapter::PopulateView(YI_UINT32 uIndex, CYISceneView *pView)
{
    if (m_pModel->GetAssetCount() > 0)
    {
        // Get the associated data model item
        CYIString deepLinkURL = m_pModel->GetDeepLinkURL(uIndex);
        CYIString deepLinkName = m_pModel->GetDeepLinkName(uIndex);

        ListItemButtonView *pItemView = YiDynamicCast<ListItemButtonView>(pView);
        if (pItemView)
        {
            pItemView->SetDeepLinkInformation(deepLinkName, deepLinkURL);
        }
    }
}

ListViewApp::ListViewApp()
: m_pListView(YI_NULL)
, m_pTextEditView(YI_NULL)
{
}

ListViewApp::~ListViewApp()
{
}

bool ListViewApp::UserInit()
{
//! [UserInit]

    // Start the web driver for allowing the use of Appium.
    CYIWebDriver *pWebDriver = CYIWebDriverLocator::GetWebDriver();
    if (pWebDriver)
    {
        pWebDriver->Start();
    }

    // Load a layout file which will be the root scene view.
    CYISceneView *pSceneViewMain = GetSceneManager()->LoadScene("ListView_Main.layout", CYISceneManager::SCALE_RESPONSIVE_LAYOUT, CYISceneManager::V_ALIGN_CENTER, CYISceneManager::H_ALIGN_CENTER);
    if (!pSceneViewMain)
    {
        YI_LOGE("ListViewApp::UserInit", "Loading scene has failed");
        return false;
    }
    // Add the scene view to the scene manager.
    GetSceneManager()->AddScene("MainComp", pSceneViewMain, 0, CYISceneManager::LAYER_OPAQUE);
    GetSceneManager()->StageScene("MainComp");
    
    // The application forces landscape for tablet and TV devices. Portrait for handset.
    // The default orientation is set here based on the dimensions of the lander.
    // Each screen will then make sure the orientation is enforced.
    glm::vec2 landerSize = CYIViewTemplate::GetViewTemplateSize("ListView_Main");
    YIViewProperty::Orientation eDefaultOrientation;
    if(landerSize.x > landerSize.y)
    {
        eDefaultOrientation = YIViewProperty::LANDSCAPE;
    }
    else
    {
        eDefaultOrientation = YIViewProperty::PORTRAIT_UPRIGHT;
    }
    
    // Force the platform orientation to the default orientation if not a handset device.
    CYIApplicationOrientationBridge *pOrientationBridge = CYIApplicationUIBridgeLocator::GetApplicationOrientationBridge();
    if (pOrientationBridge)
    {
        pOrientationBridge->SetOrientation(eDefaultOrientation);
    }
    
    SetDefaultOrientation(eDefaultOrientation);

    // Retrieve all mandatory and optional components, and ensure all mandatory components are present
    bool bOK = true;
    bOK = bOK && pSceneViewMain->FindNode<CYIListView>(m_pListView, "list", CYISceneView::FETCH_MANDATORY, LOG_TAG);
    bOK = bOK && pSceneViewMain->FindNode<CYITextEditView>(m_pTextEditView, "text-edit", CYISceneView::FETCH_MANDATORY, LOG_TAG);
    bOK = bOK && pSceneViewMain->FindNode<CYITextSceneNode>(m_pErrorTextNode, "ErrorPlaceholder", CYISceneView::FETCH_MANDATORY, LOG_TAG);
    
    if (!bOK)
    {
        YI_ASSERT(bOK, LOG_TAG, "A required node could not be found.");
        return false;
    }

    // Give the CYITextEditView initial focus.
    m_pTextEditView->RequestFocus();
    
    m_pErrorTextNode->Hide();

    // Load the view template for the list items. This template will be used by the list to build the list item's views.
    m_pListItemViewTemplate = CYIViewTemplate::GetViewTemplate("ListView_list-item");
    YI_ASSERT(m_pListItemViewTemplate, LOG_TAG, "Could not locate the list item view template.");
    if (!m_pListItemViewTemplate)
    {
        return false;
    }

    return true;
    
//! [UserInit]
}

void ListViewApp::PopulateListView()
{
    ListAdapter *pAdapter = new ListAdapter(&m_DataModel, m_pListItemViewTemplate, CYISharedPtr<IYIViewRecycler>(new CYIPooledViewRecycler()));
    m_pListView->SetAdapter(pAdapter);
}

bool ListViewApp::UserStart()
{
    // This is the first function called upon completion of engine and application initialization. Return false in case of failure.

    // Set the path where the service can find the SSL root certificate required as this sample makes HTTPS requests.
    CYIHTTPService::GetInstance()->SetSSLRootCertificate(GetAssetsPath() + "/cacert.pem");
    CYIHTTPService::GetInstance()->Start();
    
    PopulateListView();
    return true;
}

void ListViewApp::UserUpdate()
{
    // This per-frame hook can be used to drive components of an application which rely on a time-step and are not managed by You.i Engine.
}
