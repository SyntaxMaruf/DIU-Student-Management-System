<div align="center">

# 🎓 DIU Student Management System

### Simple records. Clear results. One small C project.

**C11 · Linked List · File Handling · Grade Calculation**

Created by **Maruf Billah** · **First Year, Third Semester**

</div>

---

## 💡 My idea and the role of AI

I planned this project during the third semester of my first year. The idea came from me. I used AI assistance to turn my idea into working C code and to help prepare the project notes.

I share this clearly because honest credit matters. This is an AI-assisted learning project. It helps me study linked lists, student records, input checks, and file handling.

This is an independent academic project inspired by student record needs at Daffodil International University. It is not an official DIU product or a complete university administration system.

## 📖 What does this project do?

This program keeps student details and course results in one place. It runs in a terminal. A short menu lets you add, find, edit, and delete students. You can enter course marks and see grades and GPA. Changes are saved to a local file.

The goal is simple: keep the main tasks useful and easy to follow.

## ✨ Main features

| Feature | What it does |
| --- | --- |
| 👤 Add a student | Saves ID, name, department, batch, and section. |
| 📋 View students | Shows all students and their available GPA. |
| 🔎 Find a student | Uses a student ID to show a full report. |
| ✏️ Edit details | Changes name, department, batch, and section. The ID stays fixed. |
| 📚 Manage results | Adds a course result or updates an existing course code. |
| 🧮 Calculate grades | Converts total marks out of 100 into a grade and point. |
| 📊 Calculate GPA | Uses course credits to find a weighted GPA. |
| 🗑️ Delete a student | Asks you to type YES before deleting the student and results. |
| 💾 Save records | Saves after each change and loads records on the next run. |

## 🛡️ Small details that improve use

- Duplicate student IDs are blocked.
- ID and course code checks ignore letter case.
- An existing course code updates its result instead of adding a duplicate.
- Empty input and text that is too long are rejected.
- Invalid numbers, NaN, and infinity are rejected.
- Marks must be between 0 and 100.
- Course credits must be between 0.25 and 10.
- Menu input must be a whole number from 0 to 6.
- A student with no results shows “Not available”, not a false zero GPA.
- Memory allocation is checked. Student nodes are freed before exit.
- Save errors are reported. A previous data file is kept as a backup.
- The program checks the file header and record values before loading.

## 🧭 The menu

| Number | Action |
| --- | --- |
| 1 | Add student |
| 2 | View student list |
| 3 | Find student and view report |
| 4 | Edit student information |
| 5 | Add or update course result |
| 6 | Delete student |
| 0 | Save and exit |

There is no separate edit mode or login step. Choose 1 to start adding a student.

## ⚙️ How the code works

1. The program opens its data file when it starts.
2. It rebuilds the student linked list from saved records.
3. The menu reads your choice and calls the right function.
4. Each student node holds student details and a course array.
5. Course marks are converted into grades and grade points.
6. Each change triggers a save. The old file is kept before replacement.
7. On exit, all student nodes are freed.

### Why use a linked list?

Each student is one node. A node points to the next student. The program creates a new node when a student is added. When a student is deleted, it joins the surrounding links and frees that node.

There is no fixed student array limit. Available memory limits the number of students. Courses use a fixed array with a limit of **12 courses per student**. This keeps this version easier to study.

Student search, insertion at the end, and deletion take O(n) time. Showing all students also takes O(n). Here, n is the number of students. Saving rewrites the student records, so it also takes O(n).

## 🧮 Grades and GPA

Enter the **final total marks out of 100**, not only final exam marks.

| Marks | Grade | Point |
| --- | --- | --- |
| 80 to 100 | A+ | 4.00 |
| 75 to below 80 | A | 3.75 |
| 70 to below 75 | A- | 3.50 |
| 65 to below 70 | B+ | 3.25 |
| 60 to below 65 | B | 3.00 |
| 55 to below 60 | B- | 2.75 |
| 50 to below 55 | C+ | 2.50 |
| 45 to below 50 | C | 2.25 |
| 40 to below 45 | D | 2.00 |
| Below 40 | F | 0.00 |

Grade scale reference: [DIU Student Handbook 2024](https://webbackend.daffodilvarsity.edu.bd/brochure/student-handbook2024/PDF.pdf).

**GPA = sum of (course credit × grade point) / total course credits**

Example: a 3-credit course with 80 marks gives 12 points. A 1-credit course with 75 marks gives 3.75 points. GPA = 15.75 / 4 = **3.9375**, shown as **3.94**.

Failed courses count with a zero grade point. This version has no pending-result state. Add a course when its result is ready.

The GPA covers only the courses currently listed. It is not a full degree CGPA. Semester history and official retake rules are not included. Entering an existing course code replaces its previous result.

## 🚀 Run the project

You need a C compiler with C11 support. VS Code is an editor; GCC must also be installed and available in your terminal.

### Windows — PowerShell

Open the project folder in VS Code. Open its terminal and run:

```powershell
gcc studentdesk.c -o studentdesk.exe -std=c11 -Wall -Wextra
.\studentdesk.exe
```

### Linux or macOS with a C compiler

```sh
gcc studentdesk.c -o studentdesk -std=c11 -Wall -Wextra
./studentdesk
```

### Code::Blocks

Create a C console project. Replace the contents of main.c with studentdesk.c. Use a C11-capable compiler, then Build and Run.

## 🧪 Try it in one minute

1. Choose **1**. Add a sample student with ID `DEMO-001`.
2. Choose **5**. Enter `DEMO-001`, course code `CSE133`, credit `3`, and marks `80`.
3. Choose **3**. Enter `DEMO-001` to view the report.
4. You should see **A+**, **4.00**, and **GPA 4.00**.
5. Choose **0**. Run again and find the same student to check saving.

Use sample details when trying the project.

## 🗂️ Project files

| File | Purpose |
| --- | --- |
| studentdesk.c | Complete C source code. |
| README.md | Project story, features, code guide, and run steps. |
| .gitignore | Keeps local data and compiled files out of Git. |

The program creates these files in the terminal's current folder:

| Local file | Purpose |
| --- | --- |
| studentdesk-simple.dat | Current student records. |
| studentdesk-simple.bak | Previous saved data, when one exists. |
| studentdesk-simple.tmp | Temporary file used while saving. |

Run from the same folder each time to use the same records. Local data files contain student details. They are not encrypted. Do not commit real student records to GitHub.

If the main file is damaged, close the program and keep a copy of it. If a known-good `.bak` file exists, copy it to `studentdesk-simple.dat` to restore the previous data. Keep extra backups elsewhere for important work.

## 🧩 Function guide

| Functions | Job |
| --- | --- |
| readText, readNumber, readChoice | Read and check user input. |
| sameText, findStudent, selectStudent | Compare text and locate a student. |
| gradePoint, letterGrade, calculateGPA | Calculate grades and GPA. |
| saveData, loadData, validRecord, saveChanges | Save, check, and load data. |
| enterDetails, showStudent | Read details and show a full report. |
| addStudent, showAllStudents, editStudent | Handle student records. |
| manageCourses | Add or update a course result. |
| deleteStudent, freeStudents | Remove nodes and free memory. |
| main | Show the menu and run the selected task. |

## ✅ Checks performed

The source compiled on Linux with GCC using C11 and `-Wall -Wextra -Wpedantic -Werror`. Basic checks passed for student creation, the 80-mark A+ result, GPA calculation, and save/reload. The Windows commands are provided, but Windows execution has not been tested here.

## 📌 Current scope

This is a small, single-user academic console project. It has no login system, database server, attendance, payments, semester history, or web interface. Use only one program instance per data folder. Binary files are meant for the same program build and are not a portable exchange format. File checks and a previous-save backup do not guarantee recovery from every disk or power failure.

## 👨‍💻 Author

**Maruf Billah**  
BSc in Computer Science and Engineering  
Daffodil International University  
Project stage: First Year, Third Semester  
GitHub: [@SyntaxMaruf](https://github.com/SyntaxMaruf)

**Idea by me. Built with AI assistance. Shared for learning.**
