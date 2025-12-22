//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Karon Stowe
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

// Course structure
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prereqList;
};

// Function prototypes
bool loadCourses(const string& filename,
                 vector<Course>& courseList,
                 unordered_map<string, int>& courseIndexMap);

void printCourse(const string& courseNumber,
                 const vector<Course>& courseList,
                 const unordered_map<string, int>& courseIndexMap);

void printAllCoursesSorted(const vector<Course>& courseList);

int main() {

    vector<Course> courseList;
    unordered_map<string, int> courseIndexMap;

    bool dataLoaded = false;
    bool exitProgram = false;

    while (!exitProgram) {

        cout << "\n          MAIN MENU" << endl;
        cout << "1. Load Course Data" << endl;
        cout << "2. Print Course Information" << endl;
        cout << "3. Print Sorted List of Courses" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";

        int userChoice;
        cin >> userChoice;

        switch (userChoice) {

        case 1: {   // Load course data
            string filename;
            cout << "Enter course data file name: ";
            cin >> filename;

            courseList.clear();
            courseIndexMap.clear();

            if (loadCourses(filename, courseList, courseIndexMap)) {
                dataLoaded = true;
                cout << "Course data loaded successfully." << endl;
            } else {
                dataLoaded = false;
            }
            break;
        }

        case 2: {   // Print course information
            if (!dataLoaded) {
                cout << "Error: No data loaded. Please load the file first." << endl;
            } else {
                string courseNumber;
                cout << "Enter course number: ";
                cin >> courseNumber;
                printCourse(courseNumber, courseList, courseIndexMap);
            }
            break;
        }

        case 3: {   // Print sorted course list
            if (!dataLoaded) {
                cout << "Error: No data loaded. Please load the file first." << endl;
            } else {
                printAllCoursesSorted(courseList);
            }
            break;
        }

        case 9:
            exitProgram = true;
            cout << "Exiting program..." << endl;
            break;

        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}

// Load course data into vector and hash table
bool loadCourses(const string& filename,
                 vector<Course>& courseList,
                 unordered_map<string, int>& courseIndexMap) {

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {

        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "Format Error: Missing course number or title." << endl;
            return false;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prereqList.push_back(tokens[i]);
        }

        courseList.push_back(course);
        courseIndexMap[course.courseNumber] = static_cast<int>(courseList.size()) - 1;
    }

    file.close();

    // Validate prerequisites
    for (const Course& course : courseList) {
        for (const string& prereq : course.prereqList) {
            if (courseIndexMap.find(prereq) == courseIndexMap.end()) {
                cout << "Format Error: Prerequisite does not exist." << endl;
                return false;
            }
        }
    }

    return true;
}

// Print a single course with prerequisites (UPDATED)
void printCourse(const string& courseNumber,
                 const vector<Course>& courseList,
                 const unordered_map<string, int>& courseIndexMap) {

    auto it = courseIndexMap.find(courseNumber);

    if (it == courseIndexMap.end()) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& course = courseList[it->second];

    cout << course.courseNumber << ": " << course.courseTitle << endl;

    if (course.prereqList.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites:" << endl;

        //Updated loop to print course information according to requirements
        for (const string& prereq : course.prereqList) {
            auto prereqIt = courseIndexMap.find(prereq);
            if (prereqIt != courseIndexMap.end()) {
                const Course& prereqCourse = courseList[prereqIt->second];
                cout << "- " << prereqCourse.courseNumber
                     << ": " << prereqCourse.courseTitle << endl;
            }
        }
    }
}

// Print all courses in alphanumeric order
void printAllCoursesSorted(const vector<Course>& courseList) {

    if (courseList.empty()) {
        cout << "No course data loaded." << endl;
        return;
    }

    vector<Course> sortedList = courseList;

    sort(sortedList.begin(), sortedList.end(),
         [](const Course& a, const Course& b) {
             return a.courseNumber < b.courseNumber;
         });

    cout << "Computer Science Courses (Sorted):" << endl;

    for (const Course& course : sortedList) {
        cout << course.courseNumber << ": " << course.courseTitle << endl;
    }
}
