/*
    DIU StudentDesk - Simple Version
    Author: Maruf Billah
    Compile: gcc studentdesk.c -o studentdesk -std=c11

    Students use a Linked List. Each student has up to 12 courses.
    GPA covers the listed courses, not a full degree CGPA.
    Saved files are intended for the same program build.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define MAX_COURSES 12
#define DATA_FILE "studentdesk-simple.dat"

typedef struct {
    char code[24];
    double credit;
    double marks;
} Course;

typedef struct {
    char id[32];
    char name[80];
    char department[40];
    char batch[24];
    char section[16];
    int courseCount;
    Course courses[MAX_COURSES];
} Record;

typedef struct Student {
    Record data;
    struct Student *next;
} Student;

Student *head = NULL;
int unsaved = 0;

void freeStudents(void) {
    while (head != NULL) {
        Student *temp = head;
        head = head->next;
        free(temp);
    }
}

void readText(const char *message, char *text, int size) {
    for (;;) {
        int ch;
        size_t length;
        printf("%s", message);
        fflush(stdout);
        if (fgets(text, size, stdin) == NULL) {
            if (unsaved)
                puts("\nWarning: some changes were not saved.");
            freeStudents();
            exit(unsaved ? EXIT_FAILURE : EXIT_SUCCESS);
        }
        length = strlen(text);
        if (length > 0 && text[length - 1] == '\n') {
            text[--length] = '\0';
        } else {
            ch = getchar();
            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                puts("Text is too long. Please try again.");
                continue;
            }
        }
        while (length > 0 && isspace((unsigned char)text[length - 1]))
            text[--length] = '\0';
        char *start = text;
        while (isspace((unsigned char)*start))
            start++;
        if (start != text)
            memmove(text, start, strlen(start) + 1);
        if (text[0] != '\0')
            return;
        puts("This field cannot be empty.");
    }
}

double readNumber(const char *message, double min, double max) {
    char text[80], *end;
    double value;
    for (;;) {
        readText(message, text, sizeof(text));
        errno = 0;
        value = strtod(text, &end);
        if (errno == 0 && end != text && *end == '\0' &&
            isfinite(value) && value >= min && value <= max)
            return value;
        printf("Enter a number from %.2f to %.2f.\n", min, max);
    }
}

int readChoice(const char *message, int min, int max) {
    for (;;) {
        double value = readNumber(message, min, max);
        int choice = (int)value;
        if (value == choice)
            return choice;
        puts("Please enter a whole number.");
    }
}

int sameText(const char *first, const char *second) {
    while (*first && *second) {
        if (tolower((unsigned char)*first) != tolower((unsigned char)*second))
            return 0;
        first++;
        second++;
    }
    return *first == *second;
}

Student *findStudent(const char *id) {
    Student *current = head;
    while (current != NULL) {
        if (sameText(current->data.id, id))
            return current;
        current = current->next;
    }
    return NULL;
}

Student *selectStudent(void) {
    char id[32];
    readText("Enter student ID: ", id, sizeof(id));
    Student *student = findStudent(id);
    if (student == NULL)
        puts("Student not found. Please check the ID.");
    return student;
}

double gradePoint(double marks) {
    if (marks >= 80) return 4.00;
    if (marks >= 75) return 3.75;
    if (marks >= 70) return 3.50;
    if (marks >= 65) return 3.25;
    if (marks >= 60) return 3.00;
    if (marks >= 55) return 2.75;
    if (marks >= 50) return 2.50;
    if (marks >= 45) return 2.25;
    if (marks >= 40) return 2.00;
    return 0.00;
}

const char *letterGrade(double marks) {
    if (marks >= 80) return "A+";
    if (marks >= 75) return "A";
    if (marks >= 70) return "A-";
    if (marks >= 65) return "B+";
    if (marks >= 60) return "B";
    if (marks >= 55) return "B-";
    if (marks >= 50) return "C+";
    if (marks >= 45) return "C";
    if (marks >= 40) return "D";
    return "F";
}

double calculateGPA(const Record *record) {
    double totalPoints = 0, totalCredits = 0;
    for (int i = 0; i < record->courseCount; i++) {
        const Course *course = &record->courses[i];
        totalPoints += course->credit * gradePoint(course->marks);
        totalCredits += course->credit;
    }
    return totalCredits > 0 ? totalPoints / totalCredits : -1;
}

/* Save record values only, never Linked List pointers. */
int saveData(void) {
    FILE *file = fopen("studentdesk-simple.tmp", "wb");
    Student *current = head;
    int success = 1;
    if (file == NULL) {
        puts("Could not save. Changes are still in memory.");
        return 0;
    }
    if (fwrite("SDS1", 1, 4, file) != 4)
        success = 0;
    while (success && current != NULL) {
        if (fwrite(&current->data, sizeof(Record), 1, file) != 1)
            success = 0;
        current = current->next;
    }
    if (fclose(file) != 0)
        success = 0;
    if (!success) {
        puts("Save failed. Check available disk space.");
        return 0;
    }
    file = fopen(DATA_FILE, "rb");
    if (file != NULL) {
        fclose(file);
        if (remove("studentdesk-simple.bak") != 0 && errno != ENOENT) {
            puts("Save failed: could not replace the backup.");
            return 0;
        }
        if (rename(DATA_FILE, "studentdesk-simple.bak") != 0) {
            puts("Save failed: could not keep the previous data.");
            return 0;
        }
    }
    if (rename("studentdesk-simple.tmp", DATA_FILE) != 0) {
        rename("studentdesk-simple.bak", DATA_FILE);
        puts("Save failed. Check the data and backup files.");
        return 0;
    }
    unsaved = 0;
    return 1;
}

int validRecord(const Record *record) {
    if (!memchr(record->id, '\0', sizeof(record->id)) ||
        !memchr(record->name, '\0', sizeof(record->name)) ||
        !memchr(record->department, '\0', sizeof(record->department)) ||
        !memchr(record->batch, '\0', sizeof(record->batch)) ||
        !memchr(record->section, '\0', sizeof(record->section)) ||
        record->id[0] == '\0' || record->courseCount < 0 ||
        record->courseCount > MAX_COURSES)
        return 0;
    for (int i = 0; i < record->courseCount; i++) {
        const Course *course = &record->courses[i];
        if (!memchr(course->code, '\0', sizeof(course->code)) ||
            course->code[0] == '\0' || !isfinite(course->credit) ||
            !isfinite(course->marks) || course->credit < 0.25 ||
            course->credit > 10 || course->marks < 0 || course->marks > 100)
            return 0;
        for (int j = 0; j < i; j++)
            if (sameText(course->code, record->courses[j].code))
                return 0;
    }
    return 1;
}

int loadData(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    char header[4];
    Record record;
    Student **tail = &head;
    size_t bytes;
    if (file == NULL) {
        if (errno == ENOENT) return 1;
        puts("Cannot open the data file.");
        return 0;
    }
    if (fread(header, 1, 4, file) != 4 || memcmp(header, "SDS1", 4) != 0) {
        fclose(file);
        return 0;
    }
    while ((bytes = fread(&record, 1, sizeof(record), file)) > 0) {
        if (bytes != sizeof(record) || !validRecord(&record) ||
            findStudent(record.id) != NULL) {
            fclose(file);
            return 0;
        }
        Student *student = calloc(1, sizeof(Student));
        if (student == NULL) {
            fclose(file);
            puts("Not enough memory to load students.");
            return 0;
        }
        student->data = record;
        *tail = student;
        tail = &student->next;
    }
    int success = !ferror(file);
    fclose(file);
    return success;
}

void saveChanges(void) {
    unsaved = 1;
    if (saveData()) puts("Changes saved successfully.");
}

void enterDetails(Record *record) {
    readText("Full name: ", record->name, sizeof(record->name));
    readText("Department: ", record->department, sizeof(record->department));
    readText("Batch: ", record->batch, sizeof(record->batch));
    readText("Section: ", record->section, sizeof(record->section));
}

void showStudent(const Student *student) {
    const Record *record = &student->data;
    puts("\n========================================");
    printf("Name       : %s\n", record->name);
    printf("Student ID : %s\n", record->id);
    printf("Department : %s\n", record->department);
    printf("Batch      : %s\n", record->batch);
    printf("Section    : %s\n", record->section);
    puts("----------------------------------------");
    if (record->courseCount == 0) {
        puts("No course results added yet.");
    } else {
        printf("%-16s %7s %7s %6s %6s\n", "Course", "Credit", "Marks", "Grade", "Point");
        for (int i = 0; i < record->courseCount; i++) {
            const Course *course = &record->courses[i];
            printf("%-16s %7.2f %7.2f %6s %6.2f\n", course->code,
                   course->credit, course->marks, letterGrade(course->marks),
                   gradePoint(course->marks));
        }
        printf("\nGPA: %.2f / 4.00\n", calculateGPA(record));
        puts("GPA is based on the courses listed above.");
    }
    puts("========================================");
}

void addStudent(void) {
    Record record = {0};
    puts("\n--- Add Student ---");
    readText("New student ID: ", record.id, sizeof(record.id));
    if (findStudent(record.id) != NULL) {
        puts("This ID already exists. Use a unique student ID.");
        return;
    }
    enterDetails(&record);
    Student *student = calloc(1, sizeof(Student));
    if (student == NULL) {
        puts("Not enough memory. Student was not added.");
        return;
    }
    student->data = record;
    Student **tail = &head;
    while (*tail != NULL) tail = &(*tail)->next;
    *tail = student;
    saveChanges();
}

void showAllStudents(void) {
    Student *current = head;
    int count = 0;
    if (head == NULL) {
        puts("No students yet. Choose 1 to add a student.");
        return;
    }
    printf("\n%-20s %-25s %-15s %s\n", "Student ID", "Name", "Department", "GPA");
    puts("--------------------------------------------------------------------");
    while (current != NULL) {
        double gpa = calculateGPA(&current->data);
        printf("%-20s %-25s %-15s ", current->data.id,
               current->data.name, current->data.department);
        if (gpa < 0) puts("Not available");
        else printf("%.2f\n", gpa);
        count++;
        current = current->next;
    }
    printf("\nTotal students: %d\n", count);
}

void editStudent(void) {
    Student *student = selectStudent();
    if (student == NULL) return;
    Record updated = student->data;
    printf("\nEditing: %s\n", updated.name);
    enterDetails(&updated);
    student->data = updated;
    saveChanges();
}

void manageCourses(void) {
    Student *student = selectStudent();
    if (student == NULL) return;
    showStudent(student);
    puts("\nNew course code: add a result.");
    puts("Existing course code: update its result.");
    char code[24];
    readText("Course code: ", code, sizeof(code));
    Record *record = &student->data;
    int index = -1;
    for (int i = 0; i < record->courseCount; i++) {
        if (sameText(record->courses[i].code, code)) {
            index = i;
            break;
        }
    }
    if (index == -1 && record->courseCount == MAX_COURSES) {
        printf("Maximum %d courses per student.\n", MAX_COURSES);
        return;
    }
    Course course = {0};
    strcpy(course.code, code);
    course.credit = readNumber("Course credit (0.25 to 10): ", 0.25, 10);
    course.marks = readNumber("Total marks out of 100: ", 0, 100);
    if (index == -1) index = record->courseCount++;
    record->courses[index] = course;
    printf("\nGrade: %s | Grade point: %.2f\n", letterGrade(course.marks), gradePoint(course.marks));
    saveChanges();
}

void deleteStudent(void) {
    Student *student = selectStudent();
    if (student == NULL) return;
    printf("\nStudent: %s | ID: %s\n", student->data.name, student->data.id);
    char answer[16];
    readText("Delete student and results? Type YES: ", answer, sizeof(answer));
    if (strcmp(answer, "YES") != 0) {
        puts("Cancelled. Student was kept.");
        return;
    }
    Student **link = &head;
    while (*link != student) link = &(*link)->next;
    *link = student->next;
    free(student);
    saveChanges();
}

int main(void) {
    if (!loadData()) {
        puts("Data could not be loaded safely.");
        puts("The existing file has not been overwritten.");
        puts("Keep a copy and check studentdesk-simple.bak.");
        freeStudents();
        return EXIT_FAILURE;
    }
    for (;;) {
        puts("\n========================================");
        puts("           DIU STUDENTDESK");
        puts("      Student Record Management");
        puts("========================================");
        puts("1. Add student");
        puts("2. View student list");
        puts("3. Find student and view report");
        puts("4. Edit student information");
        puts("5. Add or update course result");
        puts("6. Delete student");
        puts("0. Save and exit");
        puts("----------------------------------------");
        int choice = readChoice("Choose an option: ", 0, 6);
        switch (choice) {
            case 1: addStudent(); break;
            case 2: showAllStudents(); break;
            case 3: {
                Student *student = selectStudent();
                if (student != NULL) showStudent(student);
                break;
            }
            case 4: editStudent(); break;
            case 5: manageCourses(); break;
            case 6: deleteStudent(); break;
            case 0:
                if (unsaved && !saveData()) {
                    puts("Fix the save problem and try again.");
                    break;
                }
                freeStudents();
                puts("Thank you for using StudentDesk.");
                return EXIT_SUCCESS;
        }
    }
}
