//------------------------------------------------------------------------------
// Description: Json data filter
//------------------------------------------------------------------------------
#pragma once 
#include "IFilter.hpp"

namespace reader
{
    class EosTokenFilter : public IFilter
    {
        public:
            EosTokenFilter(){}

            virtual ~EosTokenFilter() {};
            virtual ft filter_tbl_personality(const json& j_data);
            virtual fpv filter_tx_personality(const json& j_data);
    };
}