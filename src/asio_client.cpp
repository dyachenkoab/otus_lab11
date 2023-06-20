#include <boost/asio.hpp>

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

namespace ba = boost::asio;

int main(int, char *[])
{

    std::locale::global(std::locale(""));

    try {
        ba::io_context io_context;

        ba::ip::tcp::endpoint ep(ba::ip::address::from_string("127.0.0.1"), 5000);
        ba::ip::tcp::socket sock(io_context);
        sock.connect(ep);

        //    id | name
        //    ---+----------
        //    0 | lean
        //    1 | sweater
        //    2 | frank
        //    3 | violation
        //    4 | quality
        //    5 | precision
        //    и содержимого таблицы B:
        //    id | name
        //    ---+----------
        //    3 | proposal
        //    4 | example
        //    5 | lake
        //    6 | flour
        //    7 | wonder
        //    8 | selection

        std::vector<std::string> dat { "lean",	    "sweater", "frank",
                           "violation", "quality", "precision" };
        std::vector<std::string> dat1 { "proposal", "example", "lake",
                        "flour",    "wonder",  "selection" };

        for (size_t i = 0; i < dat.size(); ++i) {
            std::ostringstream oss;
            oss << "INSERT "
                << "cat " << i << " " << dat.at(i);
            auto s { oss.str() };
            ba::write(sock, ba::buffer(s, s.size()));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            char data[1024];
            size_t len = sock.read_some(ba::buffer(data));
            std::cout << std::string { data, len } << std::endl;
        }

        for (size_t i = 0; i < dat1.size(); ++i) {
            std::ostringstream oss;
            oss << "INSERT "
                << "person " << i + 3 << " " << dat.at(i);
            auto s { oss.str() };
            ba::write(sock, ba::buffer(s, s.size()));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            char data[1024];
            size_t len = sock.read_some(ba::buffer(data));
            std::cout << std::string { data, len } << std::endl;
        }

        {
            std::ostringstream oss;
            oss << "INTERSECTION";
            auto s { oss.str() };
            ba::write(sock, ba::buffer(s, s.size()));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            char data[1024];
            size_t len = sock.read_some(ba::buffer(data));
            std::cout << std::string { data, len } << std::endl;
        }
        {
            std::ostringstream oss;
            oss << "SYMMETRIC_DIFFERENCE";
            auto s { oss.str() };
            ba::write(sock, ba::buffer(s, s.size()));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            char data[1024];
            size_t len = sock.read_some(ba::buffer(data));
            std::cout << std::string { data, len } << std::endl;
        }
        {
            std::ostringstream oss;
            oss << "TRUNCATE"
                << " "
                << "person";
            auto s { oss.str() };
            ba::write(sock, ba::buffer(s, s.size()));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            char data[1024];
            size_t len = sock.read_some(ba::buffer(data));
            std::cout << std::string { data, len } << std::endl;
        }

    } catch (const boost::system::system_error &ex) {
        std::cout << "boost exception! " << ex.what() << std::endl;
    } catch (const std::exception &ex) {
        std::cout << "std::exception! " << ex.what() << std::endl;
    }

    return 0;
}
