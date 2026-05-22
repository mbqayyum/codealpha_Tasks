// task 1: cgpa calculator
// codealpha internship - c++ programming
// calculates semester gpa and overall cgpa

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// structure to hold course info
struct course {
    string name;
    float credit_hours;
    string grade;
    float grade_points;
};

// structure to hold semester info
struct semester {
    int semester_number;
    vector<course> courses;
    float gpa;
    float total_credits;
    float total_grade_points;
};

// converts letter grade to grade point value
float get_grade_point(string grade) {
    if (grade == "A" || grade == "a") return 4.0;
    if (grade == "A-" || grade == "a-") return 3.7;
    if (grade == "B+" || grade == "b+") return 3.3;
    if (grade == "B" || grade == "b") return 3.0;
    if (grade == "B-" || grade == "b-") return 2.7;
    if (grade == "C+" || grade == "c+") return 2.3;
    if (grade == "C" || grade == "c") return 2.0;
    if (grade == "C-" || grade == "c-") return 1.7;
    if (grade == "D+" || grade == "d+") return 1.3;
    if (grade == "D" || grade == "d") return 1.0;
    if (grade == "F" || grade == "f") return 0.0;
    return -1.0; // invalid grade
}

// calculates gpa for a single semester
float calculate_gpa(vector<course>& courses, float& total_credits, float& total_points) {
    total_credits = 0;
    total_points = 0;

    for (int i = 0; i < courses.size(); i++) {
        total_credits += courses[i].credit_hours;
        total_points += courses[i].grade_points * courses[i].credit_hours;
    }

    if (total_credits == 0) return 0.0;
    return total_points / total_credits;
}

// calculates overall cgpa from all semesters
float calculate_cgpa(vector<semester>& semesters) {
    float all_credits = 0;
    float all_points = 0;

    for (int i = 0; i < semesters.size(); i++) {
        all_credits += semesters[i].total_credits;
        all_points += semesters[i].total_grade_points;
    }

    if (all_credits == 0) return 0.0;
    return all_points / all_credits;
}

// displays the results for a semester
void display_semester(semester& sem) {
    cout << "\n--- semester " << sem.semester_number << " results ---\n";
    cout << left << setw(20) << "course" << setw(10) << "credits" << setw(10) << "grade" << setw(15) << "grade points" << endl;
    cout << string(55, '-') << endl;

    for (int i = 0; i < sem.courses.size(); i++) {
        cout << left << setw(20) << sem.courses[i].name
            << setw(10) << sem.courses[i].credit_hours
            << setw(10) << sem.courses[i].grade
            << setw(15) << fixed << setprecision(2) << sem.courses[i].grade_points << endl;
    }

    cout << string(55, '-') << endl;
    cout << "total credits: " << sem.total_credits << endl;
    cout << "semester gpa: " << fixed << setprecision(2) << sem.gpa << endl;
}

// inputs course data for a semester
void input_semester(semester& sem) {
    int num_courses;
    cout << "\nhow many courses in semester " << sem.semester_number << "? ";
    cin >> num_courses;
    cin.ignore();

    for (int i = 0; i < num_courses; i++) {
        course c;
        cout << "\ncourse " << i + 1 << " name: ";
        getline(cin, c.name);

        cout << "credit hours: ";
        cin >> c.credit_hours;
        cin.ignore();

        // keep asking until valid grade is entered
        do {
            cout << "grade (A, A-, B+, B, B-, C+, C, C-, D+, D, F): ";
            getline(cin, c.grade);
            c.grade_points = get_grade_point(c.grade);
            if (c.grade_points < 0) {
                cout << "invalid grade, please try again.\n";
            }
        } while (c.grade_points < 0);

        sem.courses.push_back(c);
    }

    sem.gpa = calculate_gpa(sem.courses, sem.total_credits, sem.total_grade_points);
}

int main() {
    cout << "========================================\n";
    cout << "        cgpa calculator\n";
    cout << "        codealpha internship\n";
    cout << "========================================\n";

    vector<semester> semesters;
    int num_semesters;

    cout << "\nhow many semesters do you want to enter? ";
    cin >> num_semesters;

    for (int i = 0; i < num_semesters; i++) {
        semester sem;
        sem.semester_number = i + 1;
        input_semester(sem);
        display_semester(sem);
        semesters.push_back(sem);
    }

    // calculate and display overall cgpa
    float cgpa = calculate_cgpa(semesters);
    cout << "\n========================================\n";
    cout << "        overall results\n";
    cout << "========================================\n";

    for (int i = 0; i < semesters.size(); i++) {
        cout << "semester " << semesters[i].semester_number
            << " gpa: " << fixed << setprecision(2) << semesters[i].gpa << endl;
    }

    cout << "\n>> your cgpa is: " << fixed << setprecision(2) << cgpa << " <<\n";
    cout << "========================================\n";

    return 0;
}
