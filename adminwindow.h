#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include "student.h"
#include "course.h"
#include "teacherlist.h"

class AdminWindow : public QDialog {
    Q_OBJECT

public:
    explicit AdminWindow(StudentList* students,
                         Course** courseHead,
                         TeacherList* teachers,
                         QWidget* parent = nullptr);

private slots:
    // Student/Course management
    void addCourse();
    void removeCourse();
    void addStudent();
    void removeStudent();
    void dropStudentFromCourse();
    void viewAllStudents();
    void viewEnrolledStudents();
    void viewAllCourses();
    void addCourseToStudent();
    void removeStudentFromCourse();
    // Teacher management
    void addTeacher();
    void removeTeacher();
    void assignCourseToTeacher();
    void viewTeacherCourses();

private:
    StudentList* m_students;
    Course**     m_courseHead;
    TeacherList* m_teachers;

    void showMessage(const QString& title, const QString& msg);
};

#endif // ADMINWINDOW_H
