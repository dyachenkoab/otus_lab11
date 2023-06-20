#include "creature.h"

creature::creature(const std::string &name) : name_(name) { }

const std::string &creature::name() const
{
    return name_;
}
