//------------------------------------------------------------------------------
// Description: Data handler
//------------------------------------------------------------------------------
#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <boost/log/trivial.hpp>
#include "Config.hpp"
#include "EosTokenFilter.hpp"
#include "MongoDbWrapper.hpp"
#include "ChronicleMsgTypes.hpp"

namespace reader
{
    class DataHandler
    {
        public:
            DataHandler(PConfig& upconfig);

            void process_data(bool& answer_flag, const std::string& data);
            uint64_t get_last_ack();

        private:
            bool is_valid_json(const std::string& data);
            void process_bin_data(bool& answer_flag, const std::string& data);
            void process_json_data(bool& answer_flag, const std::string& data);
            
            PConfig& m_upconfig;
            PFilter m_upfilter;
            PMongo m_upmongo;

            uint64_t m_last_rcv;
            uint64_t m_last_ack;
            uint64_t m_last_block;
    };
    typedef std::unique_ptr<DataHandler> PDataHandler;
}