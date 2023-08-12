#ifndef TO_DO_LIST
#define TO_DO_LIST

#include <chrono>
#include <ctime>
#include <iomanip>

struct TaskNode {
    int index;
    std::string taskDescription;
    std::string inputData;
    std::string shcedTime;
    std::string status;
    TaskNode* next;

    TaskNode(const std::string& task, const std::string& data, const std::string& time, const std::string& state);
};

class TodoList {
public:
    TodoList(const std::string& filePath);
    ~TodoList();

    void addTask(const std::string& task, const std::string& data, const std::string& time, const std::string& status);
    bool parseDate(const std::string& dateString, struct tm& result);
    bool parseTime(const std::string& dateString, struct tm& result);
    void changeTaskDescription(const int taskIndex, std::string& newTask);
    void changeTaskStatus(const int taskIndex, const std::string& newStatus);
    void removeTask(int taskIndex);
    void taskStatusCounter();


    std::chrono::system_clock::time_point get_schedT() const;

    int get_completed_task_count() const;
    int get_inprocess_task_count() const;
    const TaskNode* get_task_by_node(int index) const;
    const TaskNode* get_head() const;

private:
    const int MonthsInYear = 12;
    const int DaysInMonth = 31;
    const int Min = 0;
    const int MaxMonth = MonthsInYear - 1;
    const int MinDay = 1;
    const int MaxMinAndSecond = 59;
    const int MaxHour = 23;

    int m_taskCount;
    int m_completedTasks;
    int m_inprocessTask;
    bool m_isLoadFr;
    bool m_invalidStatus = false;
    std::string m_filePath;

    std::chrono::system_clock::time_point m_scheduledTime;
    TaskNode* head;

    void changeTimeFormat(const TaskNode& node);

    void saveTasks();
    void loadTasks();
    void clearTasks();
};


#endif
