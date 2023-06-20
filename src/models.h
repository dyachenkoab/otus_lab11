#ifndef MODELS_H
#define MODELS_H
#include <odb/core.hxx>
#include <string>
#include <sstream>
#include <iostream>

#include "creature.h"
#include "cat.h"
#include "person.h"

struct Queries
{
    virtual ~Queries() { }
    virtual std::string result() = 0;
};

#pragma db view query("SELECT DISTINCT person.id, person.name, cat.name " \
              "FROM cat, person WHERE cat.id=person.id")
struct Intersection : public Queries
{
    size_t id;
    std::string person_name;
    std::string cat_name;
    std::string result() override
    {
        std::stringstream ss;
        ss << id << "," << cat_name << "," << person_name  << '\n';
        return ss.str();
    };
};
#pragma db view query("SELECT DISTINCT id, name "                         \
              "FROM person WHERE id NOT IN (SELECT id FROM cat) " \
              "UNION "                                            \
              "SELECT DISTINCT id, name "                         \
              "FROM cat WHERE id NOT IN (SELECT id FROM person) ORDER BY id")
struct SymmetricDifference : public Queries
{
    size_t id;
    std::string name;
    std::string result() override
    {
        std::stringstream ss;
        ss << id << "," << name << '\n';
        return ss.str();
    };
};

#endif // MODELS_H
