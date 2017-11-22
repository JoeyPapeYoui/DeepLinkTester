// © You i Labs Inc. 2000-2017. All rights reserved.
#ifndef _LIST_ITEM_BUTTON_VIEW_
#define _LIST_ITEM_BUTTON_VIEW_

#include <view/YiPushButtonView.h>

class CYIImageView;
class CYISceneNode;
class CYIUrl;

/*!
 \addtogroup listview
 @{
 */

/*!
 \brief A button for use in a list view.

 \details The view contains an placeholder logo image and a downloaded image. The image will be downloaded when the view population 
 is requested by the list view. When the image download is complete the placeholder will be replaced with the downloaded image.

 \sa ListViewApp::ListAdapter::PopulateView
 */
class ListItemButtonView : public CYIPushButtonView
{
public:
    ListItemButtonView();
    virtual ~ListItemButtonView();

    /*!
        \details Initialize the view by caching the required sub-views.
     */
    virtual bool Init() override;
    
    /*!
        \details Called when the view is recycled by the list view's pooled view recycler. Here the view will be reset to its initial state for the next time it is used.
     */
    virtual void Reset() override;

    /*!
        \details Sets the name and URL of the deep link
     */
    void SetDeepLinkInformation(const CYIString &name, const CYIString &url);

protected:
    virtual void OnRelease() override;
    
private:
    CYITextSceneNode *m_pDeepLinkText;
    
    CYIString m_deepLinkURL;

    YI_TYPE_BASES(ListItemButtonView, CYIPushButtonView);
};

/*@}*/

#endif // _LIST_ITEM_BUTTON_VIEW_
