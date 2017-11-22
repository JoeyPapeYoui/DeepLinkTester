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
{
    AddListItem("You.i", "https://www.youi.tv/");
    AddListItem("LinkedIn", "https://www.linkedin.com/");
    AddListItem("Facebook", "https://www.facebook.com/");
    AddListItem("IGN", "https://www.ign.com/");
    AddListItem("XKCD", "https://xkcd.com/");
}

DataModel::~DataModel()
{
}

void DataModel::AddListItem(CYIString name, CYIString url)
{
    m_deepLinks.push_back(std::pair<CYIString, CYIString>(name, url));
}

YI_UINT32 DataModel::GetAssetCount() const
{
    return (YI_UINT32)m_deepLinks.size();
}

void DataModel::ClearData()
{
    YI_UINT32 uRowCount = GetRowCount();
    m_deepLinks.clear();
    for(YI_UINT32 uRow = 0; uRow < uRowCount; uRow++)
    {
        RemoveRow(0); // We remove the first element uRowCount times because the indices are invalidated as we remove rows from the data model.
    }
}

CYIString DataModel::GetDeepLinkName(YI_UINT32 uIndex)
{
    return m_deepLinks[uIndex].first;
}

CYIString DataModel::GetDeepLinkURL(YI_UINT32 uIndex)
{
    return m_deepLinks[uIndex].second;
}

bool DataModel::ParseFromJSONString(const CYIString &JSONString)
{
    YI_UNUSED(JSONString);
    return true;
    /*
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
    }*/
}
