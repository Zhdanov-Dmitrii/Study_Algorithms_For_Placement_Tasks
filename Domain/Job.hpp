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
    bool isValid;
    std::vector<std::vector<Messages>> messages;
    double time = -1;
    std::string path;


};


#endif //DIPLOMA_JOB_HPP
