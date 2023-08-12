#include "todolist.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

TaskNode::TaskNode(const std::string& task, const std::string& data, 
                    const std::string& time, const std::string& state) :
                    taskDescription(task), inputData(data), shcedTime(time), 
                    status(state), next(nullptr) {}

TodoList::TodoList(const std::string& file) : m_filePath(file), m_completedTasks(0), m_inprocessTask(0) {
    head = nullptr;
    m_taskCount = 0;
    m_isLoadFr = false;
    loadTasks();
}

TodoList::~TodoList() {
    saveTasks();
    clearTasks();
}

// Public functions
void TodoList::addTask(const std::string& task, const std::string& data,
                         const std::string& time, const std::string& status) {

    TaskNode* newTask = new TaskNode(task, data, time, status);
    if(head == nullptr) {
        head = newTask;

    }else {
        TaskNode* current = head;
        while(current -> next != nullptr) {
            current  = current -> next;
        }
        current -> next = newTask; 
    }

    if(!m_isLoadFr) {

    }
    ++m_taskCount;
    saveTasks();
}

void TodoList::removeTask(const int taskIndex) {
    TaskNode* current  = head;
    TaskNode* prev  = nullptr;
    bool found{false};

    while (current != nullptr) {
        if(taskIndex == (current -> index - 1)) {
            found = true;
        }
        prev = current;
        current = current -> next;
    }
        if(found) {
            if(prev != nullptr) {
                prev -> next = current -> next; 
            } else {
                head = current -> next;
            }
            delete current;
            saveTasks();
        } else {
            throw std::invalid_argument("invlaid argument");
        }
}

void TodoList::changeTaskDescription(const int taskIndex, std::string& newTask) {
    TaskNode* current  = head;
    bool found {false};

    while (current != nullptr) {
        if(taskIndex == (current -> index - 1)) {
            found = true;
            break;
        }
    }

    if(found) {
        current -> taskDescription = newTask;
        saveTasks();
    } else {
        throw std::invalid_argument("Invlaid argument ");
    }
     
}

void TodoList::changeTaskStatus(const int index, const std::string& taskStatus) {
    TaskNode* current  = head;
    bool found {false};

    while (current != nullptr) {
        if(index == (current -> index - 1)) {
            found = true;
            break;
        }
    }

    if(found) {
        current -> status = taskStatus;
        saveTasks();
    } else {
        throw std::invalid_argument("Invlaid argument ");
    }
}

bool TodoList::parseDate(const std::string& dateString, struct tm& result) {
    std::istringstream ss(dateString);
    char delim;

    time_t currentTime = time(nullptr);
    struct tm* now = localtime(&currentTime);
    const int ThisYear = now -> tm_year + 1900;

    ss >> result.tm_mday >> delim >> result.tm_mon >> delim >> result.tm_year;
    if (ss.fail() || delim != '/' || result.tm_mday < MinDay || result.tm_mday > DaysInMonth 
    || result.tm_mon < Min || result.tm_mon > MaxMonth || result.tm_year < ThisYear) {

        return false;
    }

    result.tm_year -= 1900; // Years since 1900
    result.tm_mon -= 1;     // Months are 0-based

    return true;
}

bool TodoList::parseTime(const std::string& dateString, struct tm& result) {
    std::istringstream ss(dateString);
    char delim;

    ss >> result.tm_hour >> delim >> result.tm_min>> delim >> result.tm_sec;
    if (ss.fail() || delim != ':' || result.tm_hour < Min || result.tm_min < Min 
        || result.tm_sec < Min || result.tm_hour > MaxHour || result.tm_min > MaxMinAndSecond 
        || result.tm_sec > MaxMinAndSecond) {

        return false;
    }

    return true;
}

//privat functions
void TodoList::changeTimeFormat(const TaskNode& node) {

    std::tm scheduledTimeTm{};
    std::istringstream ss(node.shcedTime);
    ss >> std::get_time(&scheduledTimeTm, "%H:%M:%S");

    auto now = std::chrono::system_clock::now();
    std::time_t currentT = std::chrono::system_clock::to_time_t(now);
    std::tm* currenTm = std::localtime(&currentT);

    std::tm scheduledAdjusted = scheduledTimeTm;
    scheduledAdjusted.tm_year = currenTm -> tm_year;
    scheduledAdjusted.tm_mon = currenTm -> tm_mon;
    scheduledAdjusted.tm_mday = currenTm -> tm_mday;

    std::time_t scheduledTimeC = std::mktime(&scheduledAdjusted);
    m_scheduledTime = std::chrono::system_clock::from_time_t(scheduledTimeC);

}

void TodoList::taskStatusCounter() {
    TaskNode* current  = head;
    
    while(current != nullptr) {
        current -> status == "completed" ? ++m_completedTasks : ++m_inprocessTask;
        current = current -> next; 
    }
}

void TodoList::saveTasks() {
    std::ofstream outputFile(m_filePath, std::ofstream::out | std::ofstream::trunc);
    if (outputFile.is_open()) {
        TaskNode* current = head;

        while (current != nullptr) {
            outputFile << m_taskCount <<std::endl;
            outputFile << "Task: " << current -> taskDescription << std::endl;
            outputFile << "Due Date: " << current -> inputData << std::endl;
            outputFile << "Due Time: " << current -> shcedTime << std::endl;
            outputFile << "Status: " << current -> status << std::endl;
            ++m_taskCount;
            current -> index = m_taskCount;
            current = current->next;
        }

        m_taskCount = 0;
        outputFile.close();

    } else {
        m_invalidStatus = true;
    }
}

void TodoList::loadTasks() {
    m_isLoadFr = false;   
    std::ifstream inputFile(m_filePath);
    if (inputFile.is_open()) {
        std::string line {};

        while (std::getline(inputFile, line)) {
            std::string task, data, time, status;
            
            if (line.find("Task: ") != std::string::npos) {
                task = line.substr(6);

                if((std::getline(inputFile, line) && line.find("Due Date: ") 
                    != std::string::npos)) {
                    data = line.substr(10);

                    if(std::getline(inputFile, line) && line.find("Due Time: ") 
                        != std::string::npos) {
                        time = line.substr(10);

                        if(std::getline(inputFile, line) && line.find("Status: ") 
                        != std::string::npos) {
                            status = line.substr(8);
                            std::getline(inputFile, line);
                            addTask(task, data,time, status);
                        } else {
                            m_invalidStatus = true;
                        }

                    } else {
                        m_invalidStatus = true;
                    } 
                }
            }
        }
    }
    m_isLoadFr = true;
    inputFile.close();
}


void TodoList::clearTasks() {
    while (head != nullptr) {
        TaskNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

//getters
std::chrono::system_clock::time_point TodoList::get_schedT() const {
    return m_scheduledTime;
}

const TaskNode* TodoList::get_head() const {
    return head;    
}

int TodoList::get_completed_task_count() const {
    return m_completedTasks;
}

int TodoList::get_inprocess_task_count() const {
    return m_inprocessTask;
}

const TaskNode* TodoList::get_task_by_node(int index) const {
    TaskNode* current = head;
    int currIndex = 0;

    while (current != nullptr) {
        current = current->next;
        ++currIndex;
    }

    return current;
}