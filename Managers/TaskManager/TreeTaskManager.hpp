#ifndef DIPLOMA_TREETASKMANAGER_H
#define DIPLOMA_TREETASKMANAGER_H


class TreeTaskManager {
    ///Singleton
    TreeTaskManager() = default;
    TreeTaskManager(const TreeTaskManager&) = default;
    TreeTaskManager& operator=(TreeTaskManager&) = default;

public:
    TreeTaskManager& getInstance();
};


#endif //DIPLOMA_TREETASKMANAGER_H
