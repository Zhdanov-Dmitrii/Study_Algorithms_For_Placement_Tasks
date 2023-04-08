//
// Created by user on 30.03.23.
//

#ifndef DIPLOMA_JOB_HPP
#define DIPLOMA_JOB_HPP

#include <vector>
#include <string>

struct Job {
public:
    enum JobType {
        STAR,
        GRID,
        CUBE,
        TREE,
        FULL
    };

    enum PlacementMode{
        SIMPLE,
        OPTIMAL,
        ADVANCED,
        RANDOM
    };

    struct Messages {
        int process;
        int count;
        unsigned long cost;
    };


    int id;
    int processes;
    JobType jobType;
    PlacementMode placementMode;
    std::vector<std::vector<Messages>> messages;
    double time = -1;


};


#endif //DIPLOMA_JOB_HPP
