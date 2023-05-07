//
// Created by user on 30.03.23.
//

#ifndef DIPLOMA_ACTION_HPP
#define DIPLOMA_ACTION_HPP

#include <simgrid/s4u.hpp>
#include <string>


struct Action {
    enum ActionType {
        GET, PUT
    };

    unsigned long long id;
    ActionType actionType;
    std::string hostName;
    unsigned long cost;
    double time = -1;
};


#endif //DIPLOMA_ACTION_HPP
