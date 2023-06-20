#ifndef DBHANDLER_H
#define DBHANDLER_H
#include <string>
#include <mutex>
#include "postgresdb.h"

class DbHandler
{

public:
    DbHandler() = default;
    ~DbHandler() = default;
    static void init();
    static void insert(const std::string &table, const std::string &id, std::string name);
    static void truncate(const std::string &table);
    static std::string intersection();
    static std::string symmetric_difference();

private:
    static PostgresDB postgresDB;
};


#endif // DBHANDLER_H
