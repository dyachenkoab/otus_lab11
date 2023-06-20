#include <iostream>
#include "dbhandler.h"

int main()
{
    PostgresDB db;
    db.init();

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

    db.insert("cat", 0, "Chi");
    db.insert("cat", 1, "Rosa");
    db.insert("cat", 2, "Merida");
    db.insert("cat", 3, "Romeo");
    db.insert("cat", 4, "Persival");
    db.insert("cat", 5, "Jackson");
    db.print<cat>();
    //    std::cout << "/=================================/" << std::endl;

    db.insert("person", 3, "Miranda");
    db.insert("person", 4, "Assol");
    db.insert("person", 5, "Hans");
    db.insert("person", 6, "Anthony");
    db.insert("person", 7, "Elizabeth");
    db.insert("person", 8, "Brian");
    //    std::cout << "/=================================/" << std::endl;

        db.print<person>();
    std::cout << "/=================================/" << std::endl;

    //    db.print<cat>();
    std::cout << "/=================================/" << std::endl;

    db.intersection();
    std::cout << "/=================================/" << std::endl;
    db.symmetric_difference();

    return 0;
}
