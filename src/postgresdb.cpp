#include "postgresdb.h"

PostgresDB::PostgresDB() { }

void PostgresDB::init()
{
    try {
        m_db.reset(new odb::pgsql::database("user4", // database login name
                          "", // password
                          "test_database" // database name
                          ));

        transaction t(m_db->begin());
        m_db->execute("DROP TABLE IF EXISTS \"person\" CASCADE");

        m_db->execute("DROP TABLE IF EXISTS \"cat\" CASCADE");

        m_db->execute("CREATE TABLE \"cat\" ("
                "id BIGINT NOT NULL PRIMARY KEY,"
                "name TEXT NOT NULL)");

        m_db->execute("CREATE TABLE \"person\" ("
                "id BIGINT NOT NULL PRIMARY KEY,"
                "name TEXT NOT NULL)");

        t.commit();

    } catch (const odb::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PostgresDB::insert(const std::string &table, size_t id, std::string name)
{
    m_creatures.at(table)(id, std::move(name));
}

template<typename T>
void PostgresDB::insert(size_t id, std::string name)
{
    std::unique_ptr<T> dbObject(new T(id, std::move(name)));
    transaction t(m_db->begin());
    m_db->persist(*dbObject);
    t.commit();
}

void PostgresDB::truncate(const std::string &table)
{
    transaction t(m_db->begin());
    if (table == "cat") {
        m_db->erase_query<cat>();
    } else if (table == "person") {
        m_db->erase_query<person>();
    } else {
        throw odb::unknown_schema(table);
    }
    t.commit();
}

std::string PostgresDB::intersection()
{
    return result<Intersection>();
}

std::string PostgresDB::symmetric_difference()
{
    return result<SymmetricDifference>();
}
