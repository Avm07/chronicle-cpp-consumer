#include "EosTokenFilter.hpp"

using namespace reader;

ft EosTokenFilter::filter_tbl_personality(const json& j_data)
{
    auto code = j_data["kvo"]["code"].get<std::string>();
    auto scope = j_data["kvo"]["scope"].get<std::string>();
    
    if (code == eosio_token && scope == scope)
    {
        auto table = j_data["kvo"]["table"].get<std::string>();
        auto value = j_data["kvo"]["value"];
        return std::make_tuple(table, value.dump(),std::make_pair("_id",scope));
    }
    else
    {
        return std::make_tuple("", "", std::make_pair("",""));
    }
}   

fpv EosTokenFilter::filter_tx_personality(const json& j_data)
{
    std::vector<std::string> transfers;
    json j_act_traces  = json::parse(j_data["trace"]["action_traces"].dump());

    for(auto& item : j_act_traces)
    {
        if(item["act"]["name"] == "transfer")
        {
            json j_not = json::parse(item["act"]["data"].dump());
            j_not["block_num"] = j_data["block_num"].get<std::string>();
            j_not["block_timestamp"] = j_data["block_timestamp"].get<std::string>();
            j_not["global_sequence"] = item["receipt"]["global_sequence"].get<std::string>();
            j_not["_id"] = item["receipt"]["global_sequence"].get<std::string>();
            transfers.push_back(j_not.dump());
        }
    }

    if (!transfers.empty())
    {
        return fpv("transfers",transfers);
    }
    else
    {
        return fpv("",transfers);
    }
}