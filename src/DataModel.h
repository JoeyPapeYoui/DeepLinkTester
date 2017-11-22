// © You i Labs Inc. 2000-2017. All rights reserved.
#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

#include <datamodel/YiAbstractDataModel.h>
#include <network/YiHTTPService.h>
#include <network/YiUrl.h>
#include <signal/YiSignalHandler.h>
#include <utility/YiString.h>

/*!
 \addtogroup listview
 @{
 */

/*!
 \brief Data model that stores deep link information
 */
class DataModel : public CYIAbstractDataModel
{
public:

    DataModel();
    virtual ~DataModel();

    void AddListItem(CYIString name, CYIString url);
    
    /*!
     \details Returns the number of items.
     */
    YI_UINT32 GetAssetCount() const;

    /*!
     \details Returns the name of the Deep Link
     */
    CYIString GetDeepLinkName(YI_UINT32 uIndex);
    
    /*!
     \details Returns the URL for the Deep Link
     */
    CYIString GetDeepLinkURL(YI_UINT32 uIndex);

    /*!
     \details Remove all the rows in this data model.
     */
    void ClearData();

private:
    /*!
     \details Index into the underlying data model.
     */
    enum MODEL_COLUMN
    {
        DEEP_LINK_NAME,
        DEEP_LINK_URL,

        COLUMNS_COUNT // Must be last
    };

    /*!
     \details Parse the given JSON string into this data model class.
     */
    bool ParseFromJSONString(const CYIString &JSONString);

    std::vector<std::pair<CYIString, CYIString>> m_deepLinks;
    
    YI_DISALLOW_COPY_AND_ASSIGN(DataModel);
};

/*@}*/

#endif // _DATA_MODEL_H_
