#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include "studentlogin.h"
#include "student.h"
#include "course.h"
#include "teacher.h"
#include "teacherlist.h"
#include "adminwindow.h"
#include "studentwindow.h"
#include "teacherwindow.h"

StudentList students;
Course*     courseHead = nullptr;
TeacherList teachers;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Seed initial data
    teachers.addTeacher(new Teacher("t01", "123"));


    // Main window
    QWidget window;
    window.setWindowTitle("Course Registration");
    window.setMinimumSize(500, 250);
    window.resize(600, 300);

    auto layout = new QVBoxLayout(&window);
    auto adminB   = new QPushButton("Admin");
    auto studentB = new QPushButton("Student");
    auto teacherB = new QPushButton("Teacher");
    QSize bs(120, 40);
    adminB->setFixedSize(bs);
    studentB->setFixedSize(bs);
    teacherB->setFixedSize(bs);

    layout->addStretch();
    layout->addWidget(adminB, 0, Qt::AlignHCenter);
    layout->addWidget(studentB, 0, Qt::AlignHCenter);
    layout->addWidget(teacherB, 0, Qt::AlignHCenter);
    layout->addStretch();

    // Create windows and dialogs
    AdminWindow   aw(&students, &courseHead, &teachers);
    StudentWindow sw(&students, &courseHead);
    TeacherWindow tw(&teachers, &window);
    StudentLoginDialog sdlg(&students, &courseHead);

    // Admin button → login
    QObject::connect(adminB, &QPushButton::clicked, [&]() {
        bool ok;
        QString pwd = QInputDialog::getText(&window, "Admin Login", "Password:",
                                            QLineEdit::Password, "", &ok);
        if (ok && pwd == "admin123")
            aw.show();
        else if (ok)
            QMessageBox::warning(&window, "Error", "Wrong password");
    });

    // Student button → login dialog
    QObject::connect(studentB, &QPushButton::clicked, [&]() {
        sdlg.exec();
    });

    // Teacher button → login dialog
    QObject::connect(teacherB, &QPushButton::clicked, [&]() {
        if (tw.login())
            tw.show();
    });

    window.show();
    return app.exec();
}
