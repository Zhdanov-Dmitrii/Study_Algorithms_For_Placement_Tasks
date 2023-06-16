//
// Created by user on 30.03.23.
//

#include "Task.hpp"
#include "../Managers/TaskManager/TaskManager.hpp" // сколько времени ушло на то, чтобы перетащить сюда(

XBT_LOG_NEW_DEFAULT_CATEGORY(sample_simulator, "Messages specific for this simulator");

Task::Task(unsigned long long id) : id(id) {

}

void Task::operator()() {
    std::unordered_map<std::string_view, pid_t> name_pid;
    for (auto &[_, action] : actions) {
        auto& host = action.hostName;

        if (name_pid.count(host) > 0)
            continue;

        auto f =[&host](simgrid::s4u::ActorPtr ptr) {
            return ptr->get_cname() == host;
        };
        name_pid[host] = engine->get_filtered_actors(f)[0]->get_pid();
    }


    simgrid::s4u::Mailbox *myMB = simgrid::s4u::Mailbox::by_name(hostName);
    double startTask = simgrid::s4u::Engine::get_clock();
    for (auto &[id, action] : actions) {
        auto mailbox = simgrid::s4u::Mailbox::by_name(action.hostName);

       if (action.actionType == Action::ActionType::GET) {
            auto *sender_time = myMB->get<double>();
            double comm_time = simgrid::s4u::Engine::get_clock() - *sender_time;
            delete sender_time;

            action.time = comm_time;
            //XBT_INFO("%s a message from %s in %f sec", myMB->get_cname(), mailbox->get_cname(), comm_time);
        } else if (action.actionType == Action::ActionType::PUT) {
            auto *payload = new double(simgrid::s4u::Engine::get_clock());
            mailbox->set_receiver(simgrid::s4u::Actor::by_pid(simgrid::s4u::this_actor::get_pid()));
            mailbox->put(payload, action.cost);
        }
    }
    time = simgrid::s4u::Engine::get_clock() - startTask;

    myMB->set_receiver(nullptr);
    callback();
}

void Task::callback() const {
    TaskManager::updateTimeTask(*this);
}

const std::basic_string<char> Task::getHostName() const {
    return hostName;
}


void Task::setHostName(std::string_view hostName) {
    this->hostName = hostName;
}

void Task::addAction(Action&& action) {
    this->actions.emplace(action.id, action);
}

void Task::setJob(const std::shared_ptr<Job>& jobPtr) {
    this->job = jobPtr;
}

int Task::getId() const {
    return id;
}

std::map<unsigned long long int, Action> & Task::getActions() {
    return actions;
}

void Task::setTime(double time) {
    this->time = time;
}

Task::Task() : id(-1){

}

double Task::getTime() const {
    return time;
}

const std::map<unsigned long long, Action> & Task::getActions() const {
    return actions;
}

const std::shared_ptr<Job> &Task::getJob() const {
    return job;
}

void Task::setEngine(simgrid::s4u::Engine *engine) {
    this->engine = engine;
}
