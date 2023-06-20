#ifndef POSTGRESDB_H
#define POSTGRESDB_H
#include <iostream>
#include <memory>
#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/transaction.hxx>

#include "cat.h"
#include "person.h"

#include "models.h"
#include "models.odb.hpp"

using namespace odb::core;

class PostgresDB
{
public:
    PostgresDB();

    void init();
    void insert(const std::string &table, size_t id, std::string name);
    void truncate(const std::string &table);
    std::string intersection();
    std::string symmetric_difference();

    template<typename T>
    std::string result()
    {
        typedef odb::result<T> result;
        transaction t(m_db->begin());
        result r(m_db->query<T>());
        std::string res;
        for (typename result::iterator i(r.begin()); i != r.end(); ++i) {
            res += i->result();
        }
        t.commit();
        return res;
    }

private:
    std::unique_ptr<database> m_db;

    std::map<std::string, std::function<void(size_t, std::string)>> m_creatures {
        { "cat", [&](size_t id, std::string name) { insert<cat>(id, std::move(name)); } },
        { "person", [&](size_t id, std::string name) { insert<person>(id, std::move(name)); } },
    };

    template<typename T>
    void insert(size_t id, std::string name);
};

#endif // POSTGRESDB_H
