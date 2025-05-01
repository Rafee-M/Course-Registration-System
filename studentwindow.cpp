#include "studentwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

StudentWindow::StudentWindow(StudentList* students, Course** courseHead, QWidget* parent)
    : QDialog(parent), students(students), courseHead(courseHead)
{
    setWindowTitle("Student Login");
    auto layout = new QFormLayout(this);
    setMinimumSize(500, 300);
    resize(600, 350);

    // login fields
    usernameEdit = new QLineEdit; layout->addRow("ID:", usernameEdit);
    emailEdit    = new QLineEdit; layout->addRow("Email:", emailEdit);

    auto loginButton = new QPushButton("Login");
    layout->addRow(loginButton);
    connect(loginButton, &QPushButton::clicked, this, &StudentWindow::login);

    // course add/drop controls (disabled until login)
    addCourseEdit = new QLineEdit; addCourseEdit->setPlaceholderText("Course ID");
    addCourseBtn  = new QPushButton("Add Course"); addCourseBtn->setEnabled(false);
    dropCourseEdit= new QLineEdit; dropCourseEdit->setPlaceholderText("Course ID");
    dropCourseBtn = new QPushButton("Drop Course"); dropCourseBtn->setEnabled(false);

    layout->addRow(addCourseEdit, addCourseBtn);
    layout->addRow(dropCourseEdit, dropCourseBtn);

    // add‐course handler
    connect(addCourseBtn, &QPushButton::clicked, this, [=](){
        QString cid = addCourseEdit->text();
        Course* c = nullptr;
        for (Course* p = *courseHead; p; p = p->next) {
            if (p->getId() == cid) { c = p; break; }
        }
        if (c && currentStudent) {
            currentStudent->addCourse(c);
            QMessageBox::information(this,"Add Course","Enrolled in " + cid);
        } else {
            QMessageBox::warning(this,"Add Course","Course not found or not logged in");
        }
    });

    // drop‐course handler
    connect(dropCourseBtn, &QPushButton::clicked, this, [=](){
        if (currentStudent) {
            QString cid = dropCourseEdit->text();
            currentStudent->removeCourse(cid);
            QMessageBox::information(this,"Drop Course","Dropped " + cid);
        }
    });
}

void StudentWindow::login() {
    QString sid   = usernameEdit->text();
    QString email = emailEdit->text();
    currentStudent = students->findByIdEmail(sid, email);
    if (!currentStudent) {
        QMessageBox::warning(this, "Login Failed", "Invalid ID or email");
        return;
    }
    QMessageBox::information(this, "Login", "Welcome, " + currentStudent->getName());
    // enable course controls
    addCourseBtn->setEnabled(true);
    dropCourseBtn->setEnabled(true);
}
