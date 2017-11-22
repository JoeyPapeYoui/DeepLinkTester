// © You i Labs Inc. 2000-2017. All rights reserved.
#ifndef _LIST_VIEW_APP_H_
#define _LIST_VIEW_APP_H_

#include "DataModel.h"

#include <framework/YiApp.h>
#include <signal/YiSignalHandler.h>
#include <streamer/YiViewAdapter.h>

/*!
 \defgroup listview List View
 \brief Reference list view implementation.
 \details This sample demonstrates how to apply You.i Engine's application development framework to a create a list.

 - Uses tmdb to populate list with movies
 - Contains a search field, which searches tmdb database
 - Uses CYIAbstractDataModel, CYIPooledViewRecycler

 @{
 */

class CYIListView;
class CYIPushButtonView;
class CYISceneView;
class CYITextEditView;
class CYITextSceneNode;

/*!
 \brief The main application class for the sample.
 \details This class acts as the entry point for the application.
 */
class ListViewApp : public CYIApp, public CYISignalHandler
{
public:
    ListViewApp();
    virtual ~ListViewApp();

    /*!
     \details Application set up and initialization occurs here.

     \snippet ListViewApp.cpp UserInit
     */
    virtual bool UserInit() override;

    /*!
     \details Called when both engine and user initialization has completed. UserStart() will be the first thing called when the application is ready to begin. 
     */
    virtual bool UserStart() override;

    /*!
     \details Called on each frame before the update of the scene tree. This function may be used to drive other time-dependent components of an application which are not managed by You.i Engine. 
     */
    virtual void UserUpdate() override;
    
private:
    /*!
     \details The ListAdapter will bridge the DataModel's content with the list view.
     
     The ListAdapter provides information to its associated list indicating how many items the list should contain. The adapter also creates and populates the views as those items become visible within the list.
     */
    class ListAdapter : public CYIViewAdapter, public CYISignalHandler
    {
    public:
        /*!
         \details pModel is a pointer to the data model, which is used by the adapter to populate its associated list. pItemViewTemplate is a view template for items in the list.
         */
        ListAdapter(DataModel *pModel, const CYISharedPtr<CYIAssetViewTemplate> &pItemViewTemplate, const CYISharedPtr<IYIViewRecycler> &pViewRecycler);

        /*!
         \details Returns the total number of items in the list (which will correspond to the total number of items in the data model). The returned value indicates the amount of list items that will be created.
         */
        virtual YI_UINT32 GetItemsCount() const override;

        /*!
         \details Returns the view template for index uIndex. In this case, the same view template is applied to all list items.
         */
        virtual CYISharedPtr<CYIAssetViewTemplate> GetViewTemplate(YI_UINT32 uIndex) const override;

        /*!
         \details Populates a view. This function is used to set the initial state of a view after it has been created. Views are created automatically by lists when they become visible (either at initialization or as a result of a user scroll)
         */
        virtual void PopulateView(YI_UINT32 uIndex, CYISceneView *pView) override;

    private:
        DataModel *m_pModel;
        CYISharedPtr<CYIAssetViewTemplate> m_pItemViewTemplate;
    };

    /*!
     \details Called when fetch returns.
     */
    void PopulateListView();

    CYIListView       *m_pListView;
    CYITextEditView   *m_pTextEditView;
    CYITextSceneNode  *m_pErrorTextNode;

    DataModel m_DataModel;
    CYISharedPtr<CYIAssetViewTemplate> m_pListItemViewTemplate;
};

/* @} */

#endif // _LIST_VIEW_APP_H_
