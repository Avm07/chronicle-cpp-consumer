#include "TCPSyncServer.hpp"

using namespace reader;

TCPSyncServer::TCPSyncServer(const std::string &cfg_dir, const std::string &cfg_name) : m_acceptor(m_io_context)
{
    m_upconfig = std::make_unique<Config>(Config(cfg_dir, cfg_name));
    m_updata = std::make_unique<DataHandler>(DataHandler(m_upconfig));
    m_acceptor = tcp::acceptor(m_io_context,tcp::endpoint(tcp::v4(),m_upconfig->get_server_port()));
}

void TCPSyncServer::do_accept()
{
    for (;;)
    {
        BOOST_LOG_TRIVIAL(info) << "Waiting for new connection...";
        tcp::socket socket(m_io_context);
        
        // Block until we get a connection
        m_acceptor.accept(socket);

        // Launch the session, transferring ownership of the socket
        //std::thread{std::bind(&TCPSyncServer::do_session, socket)}.detach();
        do_session(socket);    
    }
}

void TCPSyncServer::do_session(tcp::socket& sock)
{
    try
    {
        BOOST_LOG_TRIVIAL(info) << "Connection received";
        websocket::stream<tcp::socket> ws{std::move(sock)};

        // Accept the websocket handshake
        ws.accept();
        
        for (;;)
        {
            bool answer_flag = false;
            beast::flat_buffer buffer;

            // Read a message
            ws.read(buffer);

            m_updata->process_data(answer_flag,beast::buffers_to_string(buffer.data()));

            if(answer_flag)
            {
                ws.write(net::buffer(std::to_string(m_updata->get_last_ack())));
            }
        }
    }
    catch (beast::system_error const &se)
    {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            BOOST_LOG_TRIVIAL(error) << "Exception: " << se.code().message() << std::endl;
    }
    catch (std::exception const &e)
    {
        BOOST_LOG_TRIVIAL(error) << "Exception: " << e.what() << std::endl;
    }
}
