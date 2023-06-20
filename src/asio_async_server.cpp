#include "asio_async_server.h"

size_t Server::id = 0;

std::vector<std::string> Session::parseCommand(const std::string &command, std::string reg)
{
    std::regex word_regex(std::move(reg));
    std::vector<std::string> vec;

    std::smatch matches;

    if (std::regex_search(command, matches, word_regex)) {
        std::cout << "Match found with size - " << matches.size() << "\n";

        for (size_t i = 1; i < matches.size(); ++i) {
            vec.push_back(matches[i].str());
            std::cout << i << ": '" << matches[i].str() << "'\n";
        }
    } else {
        std::cout << "Match not found\n";
    }
    std::cout << "\n=======================================\n";
    return vec;
}

Session::Session(size_t id, tcp::socket socket, Server *server)
    : socket_(std::move(socket))
    , m_id(id)
    , m_server(server)
{
}

void Session::start()
{
    do_read();
}

size_t Session::id() const
{
    return m_id;
}

void Session::do_read()
{

    socket_.async_read_some(boost::asio::buffer(data_, max_length), [this](boost::system::error_code ec,
                                           std::size_t length) {
        if ((boost::asio::error::eof == ec) || (boost::asio::error::connection_reset == ec)) {
            std::cout << "DISCONNECT " << m_id << '\n';
            if (m_server) {
                m_server->disconnect(m_id);
            }
        }
        if (!ec) {
            std::string command { data_, length };
            std::cout << "command: " << command << " size:" << length << '\n';
            auto parsedCommand { parseCommand(command, "(^[A-Z_]*)") };

            if (parsedCommand.empty()) {
                do_read();
            }

            try {
                if (parsedCommand.at(0) == "INSERT") {
                    auto comPool =
                        parseCommand(command, "([\\w]*) ([\\d]) ([\\w]*)$");
                    DbHandler::insert(comPool.at(0), comPool.at(1),
                              std::move(comPool.at(2)));
                    do_write("OK\n");
                } else if (parsedCommand.at(0) == "TRUNCATE") {
                    auto comPool = parseCommand(command, "([\\w]*)$");
                    DbHandler::truncate(comPool.at(0));
                    do_write("OK\n");
                } else if (parsedCommand.at(0) == "INTERSECTION") {
                    do_write(DbHandler::intersection());
                    do_write("OK\n");

                } else if (parsedCommand.at(0) == "SYMMETRIC_DIFFERENCE") {
                    do_write(DbHandler::symmetric_difference());
                    do_write("OK\n");
                } else {
                    do_write("ERR Unknown command\n");
                }
            } catch (std::out_of_range &ex) {
                do_write("ERR Bad command\n");
            } catch (std::exception &ex) {
                std::stringstream err;
                err << "ERR"
                    << " " << ex.what() << '\n';
                do_write(err.str());
            }
            do_read();
        }
    });
}

void Session::do_write(std::string answer)
{
    boost::asio::async_write(socket_, boost::asio::buffer(std::move(answer), answer.size()),
                 [this](boost::system::error_code ec, std::size_t /*length*/) {
                     if (!ec) {
                         do_read();
                     }
                 });
}

Server::Server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    do_accept();
}

Server::~Server()
{
    pool.clear();
}

void Server::disconnect(size_t id)
{
    auto it = std::find_if(pool.begin(), pool.end(), [&id](const std::unique_ptr<Session> &session) {
        return session->id() == id;
    });
    if (it != std::end(pool)) {
        std::cout << "disconnect from server " << id << '\n';
        pool.erase(it);
    }
}

void Server::do_accept()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            pool.emplace_back(
                std::unique_ptr<Session>(new Session(id, std::move(socket), this)));
            ++id;
            std::cout << "connect with id - " << id << '\n';
            pool.back()->start();
        }

        do_accept();
    });
}
