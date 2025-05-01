#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
class StudentList;
class Course;
class QPushButton;

class AdminWindow : public QDialog {
    Q_OBJECT
public:
    AdminWindow(StudentList* students, Course** courseHead, QWidget* parent = nullptr);

private slots:
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

private:
    StudentList* m_students;
    Course**     m_courseHead;
    // no persistent line-edits here — we’ll use QInputDialog in each slot
    void showMessage(const QString& title, const QString& msg);
};

#endif // ADMINWINDOW_H
