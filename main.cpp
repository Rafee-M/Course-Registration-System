#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include "student.h"
#include "adminwindow.h"
#include "studentwindow.h"
// THIS IS THE DEFINITION of the extern:
Course* courseHead = nullptr;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    StudentList students;
    Course* courseHead = nullptr; //Raf: remove copy

    QWidget window;
    window.setWindowTitle("Course Registration System");

    // set a reasonable overall size
    window.setMinimumSize(400, 200);
    window.resize(500, 250);

    // create layout
    auto layout = new QVBoxLayout(&window);

    // create buttons
    auto adminButton   = new QPushButton("Admin");
    auto studentButton = new QPushButton("Student");

    // clamp button size
    QSize btnSize(120, 40);
    adminButton  ->setFixedSize(btnSize);
    studentButton->setFixedSize(btnSize);

    // center them with stretches
    layout->addStretch();              // top spacer
    layout->addWidget(adminButton,   0, Qt::AlignHCenter);
    layout->addSpacing(10);
    layout->addWidget(studentButton, 0, Qt::AlignHCenter);
    layout->addStretch();              // bottom spacer

    // dialogs
    AdminWindow   adminWindow(&students, &courseHead);
    StudentWindow studentWindow(&students, &courseHead);

    QObject::connect(adminButton, &QPushButton::clicked, [&](){
        bool ok;
        QString pwd = QInputDialog::getText(&window,
                                            "Admin Login", "Password:",
                                            QLineEdit::Password, "", &ok);
        if (ok && pwd == "admin123") adminWindow.show();
        else if (ok) QMessageBox::warning(&window, "Error", "Incorrect password");
    });
    QObject::connect(studentButton, &QPushButton::clicked, [&](){
        studentWindow.show();
    });

    window.show();
    return app.exec();
}
