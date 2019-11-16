//------------------------------------------------------------------------------
// Description: Json data filter
//------------------------------------------------------------------------------
#pragma once 
#include <nlohmann/json.hpp> 
#include "resourses.hpp"

using fp = std::pair<std::string, std::string>;
using ft = std::tuple<std::string, std::string, fp>;
using fpv = std::pair<std::string, std::vector<std::string> >;
using json = nlohmann::json;

namespace reader
{
    class IFilter
    {
        public:
            virtual ~IFilter() {};
            virtual ft filter_tbl_personality(const json& j_data) = 0;
            virtual fpv filter_tx_personality(const json& j_data) = 0;
    };
    typedef std::unique_ptr<IFilter> PFilter;
}