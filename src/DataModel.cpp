// © You i Labs Inc. 2000-2017. All rights reserved.
#include "DataModel.h"
#include "Urls.h"

#include <asset/YiAssetLoader.h>
#include <asset/YiAssetManager.h>
#include <framework/YiApp.h>
#include <framework/YiAppContext.h>
#include <framework/YiFramework.h>
#include <network/YiHTTPRequest.h>
#include <network/YiHTTPResponse.h>
#include <smartptr/YiScopedPtr.h>
#include <utility/YiParsingError.h>
#include <utility/YiRapidJSONUtility.h>
#include <utility/YiDir.h>

// This sample uses a data model with 0 rows and 1 column (poster art URI).
DataModel::DataModel()
: CYIAbstractDataModel(0, COLUMNS_COUNT)
{
}

DataModel::~DataModel()
{
}

YI_UINT32 DataModel::GetAssetCount() const
{
    return GetRowCount();
}

void DataModel::ClearData()
{
    YI_UINT32 uRowCount = GetRowCount();
    for(YI_UINT32 uRow = 0; uRow < uRowCount; uRow++)
    {
        RemoveRow(0); // We remove the first element uRowCount times because the indices are invalidated as we remove rows from the data model.
    }
}

CYIString DataModel::GetDeepLinkName(YI_INT32 uIndex)
{
    return TrimQuotes(GetItemData(GetIndex(uIndex, DEEP_LINK_NAME)).ToString());
}

CYIString DataModel::GetDeepLinkURL(YI_INT32 uIndex)
{
    return TrimQuotes(GetItemData(GetIndex(uIndex, DEEP_LINK_URL)).ToString());
}

CYIString DataModel::TrimQuotes(const CYIString &inputString)
{
    CYIString strResult = inputString;
    if (strResult.GetLength() > 0 && strResult.At(0) == '"')
    {
        strResult = strResult.SubStr(1);
    }
    if (strResult.GetLength() > 0 && strResult.At(strResult.GetLength() - 1) == '"')
    {
        strResult = strResult.SubStr(0, strResult.GetLength() - 1);
    }
    return strResult;
}

void DataModel::PopulateDataModel()
{
    ParseFromJSONString(CYIFramework::GetInstance()->GetAssetLoader()->GetAssetLocator().GetBase() + "configuration" + CYIDir::GetSeparator() + "deeplinks" + CYIDir::GetSeparator() + "DeepLinks.json");
}

bool DataModel::ParseFromJSONString(const CYIString &JSONPath)
{
    CYIParsingError parsingError;

    CYIScopedPtr<yi::rapidjson::Document> pDocument(CYIRapidJSONUtility::CreateDocumentFromFile(JSONPath, parsingError));

    if (!parsingError.HasError())
    {
        yi::rapidjson::Value &deepLinks = (*pDocument)["DeepLinks"];

        ClearData();

        // As each asset is parsed from the server, if valid, store the data into a new row in the data model.
        for (yi::rapidjson::Value::ConstValueIterator itr = deepLinks.Begin(); itr != deepLinks.End(); itr++)
        {
            CYIString deepLinkURL;
            if (CYIRapidJSONUtility::GetStringField(itr, "url", deepLinkURL, parsingError))
            {
                CYIString deepLinkName;
                bool bNameSuccessul = CYIRapidJSONUtility::GetStringField(itr, "name", deepLinkName, parsingError);
                
                if (deepLinkURL.IsNotEmpty())
                {
                    YI_INT32 nNewRow = GetRowCount();
                    InsertRow(nNewRow);
                    SetItemData(GetIndex(nNewRow, DEEP_LINK_NAME), bNameSuccessul ? deepLinkName : "");
                    SetItemData(GetIndex(nNewRow, DEEP_LINK_URL), deepLinkURL);
                }
            }
        }
    }

    if(GetAssetCount() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
