#include "DataHandler.hpp"

using namespace reader;

DataHandler::DataHandler(PConfig& upconfig) : m_upconfig(upconfig), m_last_rcv(0), m_last_ack(0), m_last_block(0)
{
    m_upmongo = std::make_unique<MongoDbWrapper>(MongoDbWrapper(m_upconfig->get_db_adress(), m_upconfig->get_db_port(), m_upconfig->get_db_name(), m_upconfig->get_db_user_name(), m_upconfig->get_db_user_pwd()));
    m_upfilter = std::make_unique<EosTokenFilter>();
}

void DataHandler::process_data(bool& answer_flag, const std::string& data)
{
    if(m_upconfig->is_bin_mode())
    {
        process_bin_data(answer_flag, data);
    }
    else
    {
        process_json_data(answer_flag, data);
    }
}

uint64_t DataHandler::get_last_ack()
{
    return m_last_ack;
}

bool DataHandler::is_valid_json(const std::string& data)
{
    if (nlohmann::json::accept(data))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DataHandler::process_bin_data(bool& answer_flag, const std::string& data)
{
    std::string j_data = data.substr(2*sizeof(uint32_t),data.size());
    
    if(is_valid_json(j_data))
    {
        uint32_t msg_type;
        std::copy(data.c_str(), data.c_str() + 4, reinterpret_cast<char*>(&msg_type));
        json j_out = json::parse(j_data);

        if (msg_type == FORK)
        {
            std::cout << "Message type: " << msg_type << std::endl;
            std::cout << std::setw(4) << j_out << std::endl << std::endl;
            m_last_block = std::stoi(j_out["block_num"].get<std::string>());
        }
        else if (msg_type == TBL_ROW)
        {
            std::cout << "Message type: " << msg_type << std::endl;
            std::cout << std::setw(4) << j_out << std::endl << std::endl;
            ft result = m_upfilter->filter_tbl_personality(j_out);
            auto status = j_out["added"].get<std::string>();

            if (!std::get<0>(result).empty() && status == "true")
            {
                //@coll_name, @document, @filter
                m_upmongo->update_one(std::get<0>(result), std::get<1>(result), std::get<2>(result));
            }
            else if (!std::get<0>(result).empty() && status == "false")
            {
                //@coll_name, @filter
                m_upmongo->remove_one(std::get<0>(result), std::get<1>(result));
            }
        }
        else if (msg_type == TX_TRACE)
        {
            std::cout << "Message type: " << msg_type << std::endl;
            std::cout << std::setw(4) << j_out << std::endl << std::endl;
            fpv result = m_upfilter->filter_tx_personality(j_out);
            if (!result.first.empty())
            {
                m_upmongo->insert_many(result.first, result.second);
            }
        }
        else if (msg_type == RCVR_PAUSE)
        {
            m_last_ack = m_last_block;
            answer_flag = true;
        }
        else if (msg_type == BLOCK_COMPLETED)
        {
            m_last_block = std::stoi(j_out["block_num"].get<std::string>());

            if ((m_last_block - m_last_rcv) >= 100)
            {
                m_last_rcv = m_last_block;
                json j_doc;
                j_doc["_id"] = std::to_string(0);
                j_doc["last_block_num"] = std::to_string(m_last_block);
                m_upmongo->update_one("last_block", j_doc.dump(), fp("_id", "0"));
            }

            if ((m_last_block - m_last_ack) >= m_upconfig->get_ack_number())
            {
                m_last_ack = m_last_block;
                answer_flag = true;
            }
        }
    }
    else
    {
        answer_flag = false;
    }
}
            
void DataHandler::process_json_data(bool& answer_flag, const std::string& data)
{
    if(is_valid_json(data))
    {
        nlohmann::json j_out = nlohmann::json::parse(data);
        auto msgtype = j_out["msgtype"].get<std::string>();

        if (msgtype == "FORK")
        {
            std::cout << std::setw(4) << j_out << std::endl << std::endl;
            /*
            uint64_t block_num = std::stoi(j_out["data"]["block_num"].get<std::string>());
            m_last_ack = block_num - 1; 
            answer_flag = true;
            */
            m_last_block = std::stoi(j_out["data"]["block_num"].get<std::string>());
        }
        else if (msgtype == "TBL_ROW")
        {
            std::cout << std::setw(4) << j_out << std::endl << std::endl;
            auto status = j_out["data"]["added"].get<std::string>();
            
            json j_tbl_data  = json::parse(j_out["data"].dump());
            ft result = m_upfilter->filter_tbl_personality(j_tbl_data);

            if (!std::get<0>(result).empty() && status == "true")
            {
                //@coll_name, @document, @filter
                m_upmongo->update_one(std::get<0>(result), std::get<1>(result), std::get<2>(result));
            }
            else if (!std::get<0>(result).empty() && status == "false")
            {
                //@coll_name, @filter
                m_upmongo->remove_one(std::get<0>(result), std::get<1>(result));
            }
        }
        else if (msgtype == "TX_TRACE")
        {
            std::cout << std::setw(4) << j_out << std::endl << std::endl;
            json j_tx_traces  = json::parse(j_out["data"].dump());
            fpv result = m_upfilter->filter_tx_personality(j_tx_traces);
            if (!result.first.empty())
            {
                m_upmongo->insert_many(result.first, result.second);
            }
        }
        else if (msgtype == "RCVR_PAUSE")
        {
            /* 
            if(m_last_block > m_last_ack)
            {
                m_last_ack = m_last_block;
                answer_flag = true;
            }
            */
            m_last_ack = m_last_block;
            answer_flag = true;
        }
        else if (msgtype == "BLOCK_COMPLETED")
        {
            m_last_block = std::stoi(j_out["data"]["block_num"].get<std::string>());

            if((m_last_block - m_last_rcv) >= 100)
            {
                m_last_rcv = m_last_block;
                json j_doc;
                j_doc["_id"] = std::to_string(0);
                j_doc["last_block_num"] = std::to_string(m_last_block);
                m_upmongo->update_one("last_block", j_doc.dump(), fp("_id", "0"));
            }

            if((m_last_block - m_last_ack) >= m_upconfig->get_ack_number())
            {
                m_last_ack = m_last_block;
                answer_flag = true;
            }
        }
    }
    else
    {
        answer_flag = false;
    }
}