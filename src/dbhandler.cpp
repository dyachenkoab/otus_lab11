#include "dbhandler.h"

PostgresDB DbHandler::postgresDB;

void DbHandler::init()
{
    postgresDB.init();
}

void DbHandler::insert(const std::string &table, const std::string &id, std::string name)
{
    const int id_ { std::stoi(id) };
    postgresDB.insert(table, id_, std::move(name));
}

void DbHandler::truncate(const std::string &table)
{
    postgresDB.truncate(table);
}

std::string DbHandler::intersection()
{
    return postgresDB.intersection();
}

std::string DbHandler::symmetric_difference()
{
    return postgresDB.symmetric_difference();
}
