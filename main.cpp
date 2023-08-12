#include "todolist.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

void viewTasks(const TaskNode* node) {
    const TaskNode* current = node;

    while (current != nullptr) {
        std::cout << "Task: " << (current -> index - 1) << " " << current ->taskDescription << std::endl;
        std::cout << "Due Date: " << current -> inputData << std::endl;
        std::cout << "Due Time: " << current -> shcedTime << std::endl;
        std::cout << "Status: " << current -> status << std::endl;
        current = current -> next;
    }
    
}

void viewTasks(const TaskNode* node, const int index) {
    const TaskNode* current = node;
    bool found;
     while (current != nullptr) {
        if(index == (current -> index - 1)) {
            found = true;
            break;
        }
        current = current -> next;
    }   if(found) {
            std::cout << "Task you`re searching: " << std::endl;
            std::cout << "Task: " << (current -> index - 1) << " " << current ->taskDescription << std::endl;
            std::cout << "Due Date: " << current -> inputData << std::endl;
            std::cout << "Due Time: " << current -> shcedTime << std::endl;
            std::cout << "Status: " << current -> status << std::endl;
        } else {
            throw std::invalid_argument ("invalid argument");
        }
        
}

void userInterface(TodoList& list) {
    char choise {};

    do {
        std::cout << "Todolist menu:\n";
        std::cout << "For adding new tasks, enter _________________ 1:" << std::endl;
        std::cout << "For show all tasks, enter ___________________ 2:" << std::endl;
        std::cout << "For removing task, enter ____________________ 3:" << std::endl;
        std::cout << "Find and change task, enter _________________ 4:" << std::endl;
        std::cout << "Change tasks status, enter __________________ 5:" << std::endl;
        std::cout << "Show amount of completed/inprocess tasks ____ 6:" << std::endl;
        std::cout << "Exit, enter _________________________________ 7:" << std::endl;
        std::cout << "Enter your choise: " << std::endl;

        std::cin >> choise;
        switch(choise) {

            case '1': {
                std::string description;
                std::cout << "Enter task description: ";
                std::cin.ignore();
                std::getline(std::cin,description);

                std::string inputDate;
                std::cout << "Enter a date in DD/MM/YYYY format: " ;
                std::cin >> inputDate;

                struct tm targetDate;

                try {
                    if (!list.parseDate(inputDate, targetDate)) {
                        std::cout << "Invalid date format." << std::endl;

                        return;
                    }

                } catch (const std::exception& e) {
                    std::cout << "Error " << e.what() << std::endl;
                }

                std::string scheduledTimeStr;
                std:: cout << "Enter task schedule time: in format HH:MM:SS. ";
                std::cin >> scheduledTimeStr;

                struct  tm targetTime;

                 try {
                    if (!list.parseTime(scheduledTimeStr, targetTime)) {
                        std::cout << "Invalid time format." << std::endl;

                        return;
                    }

                } catch (const std::exception& e) {
                    std::cout << "Error " << e.what() << std::endl;
                }

                list.addTask(description, inputDate, scheduledTimeStr, "inprocess");
                std::cout << "Task added successfully:" << std::endl;

                break;
            }

            case '2': {
                const TaskNode* head = list.get_head();
                viewTasks(head);
                break;
            }

            case '3': {
                int taskIndex {};
                std::cout << "Enter task index you want to remove: ";
                list.removeTask(taskIndex);
                std::cout << "Task removed!!!" << std::endl;
            }

             case '4': {
                int taskIndex {};
                std::cout << "Enter task index you want to change: ";
                std::cin >> taskIndex;
                const TaskNode* head = list.get_head();
                viewTasks(head, taskIndex);
                std::cout << "If you want to change task enter 'y' "; 
                char ch {};
                std::cin >> ch;
                if('y' == ch) {
                    std::string newTask {};
                    std::cout << "Now enter new task: ";
                    std::getline (std::cin, newTask);
                    list.changeTaskDescription(taskIndex, newTask);
                    break;
                } 
                break;
            }

             case '5': {
                int taskIndex {};
                std::cout << "Enter task index you want to change status: ";
                std::cin >> taskIndex;

                std::string taskStatus {};
                std::cout << "Enter task status: ";
                std::getline(std::cin, taskStatus);
                list.changeTaskStatus(taskIndex, taskStatus);
                std::cout << "Task status was changed successfully!!";
                break;
            } 

            case '6': {
                list.taskStatusCounter();
                std::cout << "Amount of completed tasks: " << list.get_completed_task_count() << std::endl;
                std::cout << "Amount of in process tasks: " << list.get_inprocess_task_count() << std::endl;
                break;
            }
        }
    } while (choise != '7');
            
}

int main() {
    std::string filePath = "listbook.txt";
    TodoList list(filePath);
    userInterface(list);

    return 0;
}

