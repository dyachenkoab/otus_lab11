#ifndef ASIO_ASYNC_SERVER_H
#define ASIO_ASYNC_SERVER_H
//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <set>
#include <atomic>
#include <utility>
#include <sstream>
#include <regex>

#include <boost/asio.hpp>
#include "dbhandler.h"

using boost::asio::ip::tcp;
class Server;

class Session
{
public:
    Session(size_t id, tcp::socket socket, Server *server);
    void start();
    size_t id() const;

private:
    void do_read();
    void do_write(std::string answer);

    std::vector<std::string> parseCommand(const std::string &command, std::string reg);

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];

    size_t m_id;
    Server *m_server;
};

class Server
{
public:
    Server(boost::asio::io_context &io_context, short port);
    ~Server();

    void disconnect(size_t id);

private:
    void do_accept();

    tcp::acceptor acceptor_;
    std::vector<std::unique_ptr<Session>> pool;
    static size_t id;

};

int main(int argc, char *argv[])
{
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_server <port>\n";
            return 1;
        }

        DbHandler::init();

        boost::asio::io_context io_context;
        Server server(io_context, std::atoi(argv[1]));
        io_context.run();

    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}

#endif // ASIO_ASYNC_SERVER_H
