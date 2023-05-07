//
// Created by user on 30.03.23.
//

#ifndef DIPLOMA_TASK_HPP
#define DIPLOMA_TASK_HPP

#include <simgrid/s4u.hpp>
#include <unordered_map>

#include "Action.hpp"
#include "Job.hpp"

class Task {
    const unsigned long long id;
    std::string hostName;
    std::map<unsigned long long, Action> actions;
    std::shared_ptr<Job> job;
    double time = -1;

    void callback() const;
public:
    Task();
    Task(unsigned long long id);

    void operator()();

    const std::basic_string<char> getHostName() const;

    void setHostName(std::string_view hostName);
    void addAction(Action&& action);
    void setJob(const std::shared_ptr<Job>& jobPtr);
    void setTime(double time);

    int getId() const;
    double getTime() const;
    std::map<unsigned long long int, Action> & getActions();
    const std::map<unsigned long long, Action> & getActions() const;
    const std::shared_ptr<Job>& getJob() const;

};


#endif //DIPLOMA_TASK_HPP
