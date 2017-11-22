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
        \details Sets the url that will be used to populate the internal image view. The image download will start after this method is called.
     */
    void SetImageURL(const CYIUrl &url);
    
private:
    /*!
        \details The image has been downloaded and assignment on the view succeeded. It is at this point that the placeholder should be hidden.
     */
    void OnImageAssigned();
    
    /*!
        \details The image download or assignment has failed. This view does nothing in this case other than log the error.
     */
    void OnImageAssignmentFailed(const CYIString &errorMessage);

    CYIImageView *m_pImageView;
    CYISceneNode *m_pPlaceholderLogo;

    YI_TYPE_BASES(ListItemButtonView, CYIPushButtonView);
};

/*@}*/

#endif // _LIST_ITEM_BUTTON_VIEW_
