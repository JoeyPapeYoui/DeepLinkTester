// © You i Labs Inc. 2000-2017. All rights reserved.
#include "DataModel.h"
#include "Urls.h"

#include <asset/YiAssetLoader.h>
#include <asset/YiAssetManager.h>
#include <framework/YiFramework.h>
#include <network/YiHTTPRequest.h>
#include <network/YiHTTPResponse.h>
#include <smartptr/YiScopedPtr.h>
#include <utility/YiParsingError.h>
#include <utility/YiRapidJSONUtility.h>

// This sample uses a data model with 0 rows and 1 column (poster art URI).
DataModel::DataModel()
: CYIAbstractDataModel(0, COLUMNS_COUNT)
, m_uAssetCount(0)
{
}

DataModel::~DataModel()
{
}

void DataModel::Fetch(const CYIString &keyword)
{
    if (m_pActiveRequest)
    {
        // Only allow one active request. If there is an active request cancel it before starting another.
        CYIHTTPService::GetInstance()->CancelRequest(m_pActiveRequest);
        m_pActiveRequest.Reset();
    }
    
    const CYIUrl requestURL = keyword.IsEmpty() ? CreateDefaultURL() : CreateSearchURL(keyword);
    m_pActiveRequest = CYISharedPtr<CYIHTTPRequest>(new CYIHTTPRequest(requestURL, CYIHTTPRequest::GET));
    m_pActiveRequest->NotifyResponse.Connect(*this, &::DataModel::OnRequestComplete);
    m_pActiveRequest->NotifyError.Connect(*this, &DataModel::OnRequestError);

    CYIHTTPService::GetInstance()->EnqueueRequest(m_pActiveRequest);
}

YI_UINT32 DataModel::GetAssetCount() const
{
    return m_uAssetCount;
}

CYIString DataModel::GetPosterUrl(YI_UINT32 uIndex)
{
    // Get the associated item
    const CYIDataModelIndex rowIndex = GetIndex(uIndex, 0);
    CYIAny posterImageURI = GetItemData(rowIndex);
    CYIString stringPosterImageURI = posterImageURI.ToString();

    CYIString imageUrl = Urls::IMAGE_BASE_URL + stringPosterImageURI;
    if (imageUrl.Contains("\""))
    {
        imageUrl.Remove("\"");
    }
    return imageUrl;
}

void DataModel::ClearData()
{
    YI_UINT32 uRowCount = GetRowCount();
    m_uAssetCount = 0;
    for(YI_UINT32 uRow = 0; uRow < uRowCount; uRow++)
    {
        RemoveRow(0); // We remove the first element uRowCount times because the indices are invalidated as we remove rows from the data model.
    }
}

DataModel::FETCH_RESULT DataModel::ParseFromJSONString(const CYIString &JSONString)
{
    CYIParsingError parsingError;

    CYIScopedPtr<yi::rapidjson::Document> pDocument(CYIRapidJSONUtility::CreateDocumentFromString(JSONString, parsingError));

    if (!parsingError.HasError())
    {
        yi::rapidjson::Value &results = (*pDocument)["results"];

        ClearData();

        // As each asset is parsed from the server, if valid, store the data into a new row in the data model.
        for (yi::rapidjson::Value::ConstValueIterator itr = results.Begin(); itr != results.End(); itr++)
        {
            CYIString posterPathUri;
            if (CYIRapidJSONUtility::GetStringField(itr, "poster_path", posterPathUri, parsingError))
            {
                if (posterPathUri.IsNotEmpty())
                {
                    YI_INT32 nNewRow = GetRowCount();
                    InsertRow(nNewRow);
                    SetItemData(GetIndex(nNewRow, 0), posterPathUri);
                    m_uAssetCount++;
                }
            }
        }
    }

    if(GetAssetCount() > 0)
    {
        return SUCCESS;
    }
    else
    {
        return NO_ASSETS;
    }
}

void DataModel::OnRequestComplete(const CYISharedPtr<CYIHTTPRequest> &pRequest, const CYISharedPtr<CYIHTTPResponse> &pResponse)
{
    YI_UNUSED(pRequest);

    m_pActiveRequest.Reset();
    FETCH_RESULT eFetchStatus = ParseFromJSONString(pResponse->GetBody());
    FetchEnded(eFetchStatus);
}

void DataModel::OnRequestError(const CYISharedPtr<CYIHTTPRequest> &pRequest, const HTTP_STATUS_CODE eHTTPStatusCode, const CYIString &errorMessage)
{
    YI_UNUSED(pRequest);
    YI_UNUSED(eHTTPStatusCode);
    YI_UNUSED(errorMessage);
    
    m_pActiveRequest.Reset();
    FetchEnded(HTTP_CLIENT_ERROR);
}

const CYIUrl DataModel::CreateDefaultURL() const
{
    return CYIUrl(Urls::BASE_URL + Urls::MOVIES + Urls::API_AND_URL + Urls::API_KEY);
}

const CYIUrl DataModel::CreateSearchURL(CYIString keyword) const
{
    while (keyword.Contains(" "))
    {
        keyword.Replace("+", keyword.IndexOf(" "), 1);
    }
    return CYIUrl(Urls::BASE_URL + Urls::SEARCH + Urls::API_URL + Urls::API_KEY + Urls::SEARCH_PARAMETERS + Urls::QUERY + keyword);
}
