#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include "student.h"
#include <QDialog>
class QLineEdit;
class QPushButton;
class StudentList;
class Course;

class StudentWindow : public QDialog {
    Q_OBJECT
public:
    StudentWindow(StudentList* students, Course** courseHead, QWidget* parent = nullptr);

private slots:
    void login();

private:
    StudentList* students;
    Course**     courseHead;
    QLineEdit*   usernameEdit;
    QLineEdit*   emailEdit;
    QLineEdit*   addCourseEdit;
    QPushButton* addCourseBtn;
    QLineEdit*   dropCourseEdit;
    QPushButton* dropCourseBtn;
    Student*     currentStudent = nullptr;
};

#endif // STUDENTWINDOW_H
