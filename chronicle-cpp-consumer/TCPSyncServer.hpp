#pragma once 
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include "Config.hpp"
#include "DataHandler.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace reader
{
    class TCPSyncServer
    {
        public:
            TCPSyncServer(const std::string &cfg_dir, const std::string &cfg_name);
            
            void do_accept();
            void do_session(tcp::socket& sock);

        private:
            net::io_context m_io_context;
            tcp::acceptor m_acceptor;

            PConfig m_upconfig;
            PDataHandler m_updata;  
    };
}