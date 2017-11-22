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
 \brief Data model that stores movie poster URIs.
 */
class DataModel : public CYIAbstractDataModel, public CYISignalHandler
{
public:
    /*!
     \details Enumeration of the result of the fetch operation triggered by Fetch(const CYIString &). This status is sent with the FetchEnded signal
     to inform the user of the result of the fetch operation.
     */
    enum FETCH_RESULT
    {
        SUCCESS,            /*!< Indicates that the fetch operation completed successfully and this DataModel was populated. */
        NO_ASSETS,          /*!< Indicates that the fetch operation completed but there were no assets retrieved and this DataModel was not populated. */
        PARSING_ERROR,      /*!< Indicates that the data was fetched but there was an error parsing the response and this DataModel was not populated. */
        HTTP_CLIENT_ERROR   /*!< Indicates that there was an error in the network request for the data and this DataModel was not populated. */
    };

    DataModel();
    virtual ~DataModel();

    /*!
     \details Make an HTTP request to fetch the screen's data based on the keyword. If the keyword is empty the full movie list will be used instead.
     */
    void Fetch(const CYIString &keyword = CYIString::EMPTY_STRING);

    /*!
     \details Returns the number of items.
     */
    YI_UINT32 GetAssetCount() const;

    /*!
     \details Return a string value for the poster url at uIndex.
     */
    CYIString GetPosterUrl(YI_UINT32 uIndex);

    /*!
     \details Remove all the rows in this data model.
     */
    void ClearData();

    /*!
     \details Signal fired when the Fetch(const CYIString &) request is completed or fails.
     */
    CYISignal<FETCH_RESULT> FetchEnded;

private:
    /*!
     \details Index into the underlying data model.
     */
    enum MODEL_COLUMN
    {
        POSTER_ART_URI,

        COLUMNS_COUNT // Must be last
    };

    /*!
     \details Parse the given JSON string into this data model class.
     */
    FETCH_RESULT ParseFromJSONString(const CYIString &JSONString);

    /*!
     \details Success callback for the Fetch(const CYIString &) request.
     */
    void OnRequestComplete(const CYISharedPtr<CYIHTTPRequest> &pRequest, const CYISharedPtr<CYIHTTPResponse> &pResponse);

    /*!
     \details Failure callback for the Fetch(const CYIString &) request.
     */
    void OnRequestError(const CYISharedPtr<CYIHTTPRequest> &pRequest, const HTTP_STATUS_CODE eHTTPStatusCode, const CYIString &errorMessage);

    /*!
     \details Create url for default movie display.
     */
    const CYIUrl CreateDefaultURL() const;

    /*!
     \details Create url based on keyword to display search results.
     */
    const CYIUrl CreateSearchURL(CYIString keyword) const;

    YI_UINT32 m_uAssetCount;
    CYISharedPtr<CYIHTTPRequest> m_pActiveRequest;
    
    YI_DISALLOW_COPY_AND_ASSIGN(DataModel);
};

/*@}*/

#endif // _DATA_MODEL_H_
