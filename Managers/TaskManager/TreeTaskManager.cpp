#include "TreeTaskManager.hpp"

TreeTaskManager& TreeTaskManager::getInstance() {
    static TreeTaskManager instance;
    return instance;
}