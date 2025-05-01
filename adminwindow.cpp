#include "adminwindow.h"
#include "teacher.h"
#include "teacherlist.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>

// helper functions for larger dialogs
static QString largeGetText(QWidget* parent,
                            const QString& title,
                            const QString& label,
                            bool password = false)
{
    QInputDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setLabelText(label);
    if (password) dlg.setTextEchoMode(QLineEdit::Password);
    dlg.setOption(QInputDialog::NoButtons, false);
    dlg.setMinimumSize(400, 200);
    dlg.resize(500, 250);
    return (dlg.exec() == QDialog::Accepted) ? dlg.textValue() : QString();
}

static int largeGetInt(QWidget* parent,
                       const QString& title,
                       const QString& label,
                       int value, int min, int max, int step, bool& ok)
{
    QInputDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setLabelText(label);
    dlg.setIntRange(min, max);
    dlg.setIntValue(value);
    dlg.setIntStep(step);
    dlg.setMinimumSize(400, 200);
    dlg.resize(500, 250);
    ok = (dlg.exec() == QDialog::Accepted);
    return dlg.intValue();
}

AdminWindow::AdminWindow(StudentList* students,
                         Course** courseHead,
                         TeacherList* teachers,
                         QWidget* parent)
    : QDialog(parent)
    , m_students(students)
    , m_courseHead(courseHead)
    , m_teachers(teachers)
{
    setWindowTitle("Admin Panel");
    setMinimumSize(600, 400);
    resize(700, 500);
    m_students->addStudent(new Student("sam",    "S01", "sam@x","123"));
    m_students->addStudent(new Student("john",   "J02", "john@x","123"));
    m_students->addStudent(new Student("mark",   "M03", "mark@x","123"));
    m_students->addStudent(new Student("henry",  "H04", "henry@x","123"));
    m_students->addStudent(new Student("selena", "S05", "selena@x","123"));
    // ── seed 5 courses ─────────────────────────────────────
    struct C { const char* id; const char* title; const char* ts; };
    C initCourses[] = {
        {"CSE101","Intro to CSE","Mon 9:00-11:00"},
        {"EEE102","Basic EEE",    "Tue 9:00-11:00"},
        {"BIO103","Fundamentals of Biology","Wed 9:00-11:00"},
        {"CHEM104","General Chemistry","Thu 9:00-11:00"},
        {"MATH105","Calculus I",  "Fri 9:00-11:00"}
    };
    for (auto &cc : initCourses) {
        Course* c = new Course(cc.id, cc.title, 3, 30, cc.ts);
        c->next = *m_courseHead;
        *m_courseHead = c;
    }

    // ── seed 5 teachers ────────────────────────────────────
    struct T { const char* id; const char* name; };
    T initTeachers[] = {
        {"T01","Alice"}, {"T02","Bob"}, {"T03","Carol"},
        {"T04","Dave"},  {"T05","Eve"}
    };
    for (auto &tt : initTeachers) {
        Teacher* t = new Teacher(tt.id, "123");
        t->next = m_teachers->head;
        m_teachers->head = t;
    }

    // ── build buttons ──────────────────────────────────────
    auto v = new QVBoxLayout(this);
    struct Btn { const char* txt; void (AdminWindow::*slot)(); };
    Btn buttons[] = {
        {"Add Course",                &AdminWindow::addCourse},
        {"Remove Course",             &AdminWindow::removeCourse},
        {"Add Student",               &AdminWindow::addStudent},
        {"Remove Student",            &AdminWindow::removeStudent},
        {"Drop Student from Course",  &AdminWindow::dropStudentFromCourse},
        {"View All Students",         &AdminWindow::viewAllStudents},
        {"View Enrolled Students",    &AdminWindow::viewEnrolledStudents},
        {"View All Courses",          &AdminWindow::viewAllCourses},
        {"Add Course to Student",     &AdminWindow::addCourseToStudent},
        {"Remove Student from Course",&AdminWindow::removeStudentFromCourse},
        {"Add Teacher",               &AdminWindow::addTeacher},
        {"Remove Teacher",            &AdminWindow::removeTeacher},
        {"Assign Course to Teacher",  &AdminWindow::assignCourseToTeacher},
        {"View Teacher Courses",      &AdminWindow::viewTeacherCourses}
    };

    QSize btnSize(140,30);
    for (auto &b : buttons) {
        QPushButton* btn = new QPushButton(b.txt);
        btn->setFixedSize(btnSize);
        v->addWidget(btn, 0, Qt::AlignHCenter);
        connect(btn, &QPushButton::clicked, this, b.slot);
    }
}

void AdminWindow::showMessage(const QString& title, const QString& msg) {
    QMessageBox::information(this, title, msg);
}

// — Add Course —
void AdminWindow::addCourse() {
    QString id = largeGetText(this, "Add Course", "Course ID:");
    if (id.isEmpty()) return;
    for (Course* p = *m_courseHead; p; p = p->next)
        if (p->getId() == id) { showMessage("Error","Course exists"); return; }

    QString title = largeGetText(this, "Add Course", "Title:");
    if (title.isEmpty()) return;

    QString ts = largeGetText(this, "Add Course", "Time slot:");
    if (ts.isEmpty()) return;

    bool ok;
    int ch  = largeGetInt(this, "Add Course", "Credit Hours:", 3, 0, 100, 1, ok);
    if (!ok) return;
    int cap = largeGetInt(this, "Add Course", "Max Capacity:", 30, 0, 500, 1, ok);
    if (!ok) return;

    Course* c = new Course(id, title, ch, cap, ts);
    c->next = *m_courseHead;
    *m_courseHead = c;
    showMessage("Add Course", "Course added.");
}

// — Remove Course —
void AdminWindow::removeCourse() {
    QString id = largeGetText(this, "Remove Course", "Course ID:");
    if (id.isEmpty()) return;

    Course* p = *m_courseHead; Course* prev = nullptr;
    while (p) {
        if (p->getId() == id) {
            if (prev) prev->next = p->next; else *m_courseHead = p->next;
            delete p;
            showMessage("Remove Course", "Deleted.");
            return;
        }
        prev = p; p = p->next;
    }
    showMessage("Remove Course", "Not found.");
}

// — Add Student —
void AdminWindow::addStudent() {
    QString n = largeGetText(this, "Add Student", "Name:");
    if (n.isEmpty()) return;
    QString i = largeGetText(this, "Add Student", "ID:");
    if (i.isEmpty()) return;
    if (m_students->findById(i)) { showMessage("Error","Student exists"); return; }

    QString e = largeGetText(this, "Add Student", "Email:");
    if (e.isEmpty()) return;
    QString p = largeGetText(this, "Add Student", "Password:", true);
    if (p.isEmpty()) return;

    m_students->addStudent(new Student(n,i,e,p));
    showMessage("Add Student", "Student added.");
}

// — Remove Student —
void AdminWindow::removeStudent() {
    QString sid = largeGetText(this, "Remove Student", "Student ID:");
    if (sid.isEmpty()) return;
    if (!m_students->findById(sid)) { showMessage("Error","No such student"); return; }
    m_students->removeStudent(sid);
    showMessage("Remove Student", "Done.");
}

// — Drop Student from Course —
void AdminWindow::dropStudentFromCourse() {
    QString sid = largeGetText(this, "Drop From Course", "Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","No such student"); return; }

    QString cid = largeGetText(this, "Drop From Course", "Course ID:");
    if (cid.isEmpty()) return;
    bool found=false;
    for (Course* c=s->courseHead; c; c=c->next) if (c->getId()==cid) { found=true; break; }
    if (!found) { showMessage("Error","Not enrolled"); return; }

    s->removeCourse(cid);
    showMessage("Drop From Course", "Done.");
}

// — View All Students —
void AdminWindow::viewAllStudents() {
    QString txt;
    for (Student* s=m_students->head; s; s=s->next)
        txt += s->getId() + " | " + s->getName() + "\n";
    showMessage("All Students", txt.isEmpty() ? "(none)" : txt);
}

// — View Enrolled Students —
void AdminWindow::viewEnrolledStudents() {
    QString cid = largeGetText(this, "View Enrolled", "Course ID:");
    if (cid.isEmpty()) return;
    Course* cp=nullptr;
    for (Course* c=*m_courseHead; c; c=c->next) if (c->getId()==cid) { cp=c; break; }
    if (!cp) { showMessage("Error","No such course"); return; }

    QString txt="ID | Name\n-------\n"; bool any=false;
    for (Student* s=m_students->head; s; s=s->next) {
        for (Course* c=s->courseHead; c; c=c->next)
            if (c->getId()==cid) { txt += s->getId()+" | "+s->getName()+"\n"; any=true; break; }
    }
    showMessage("Enrolled in "+cid, any?txt:"(none)");
}

// — View All Courses —
void AdminWindow::viewAllCourses() {
    QString txt;
    for (Course* c=*m_courseHead; c; c=c->next)
        txt += c->getId() + " | " + c->getTitle() + " | " + c->getTimeSlot() + "\n";
    showMessage("All Courses", txt.isEmpty() ? "(none)" : txt);
}

// — Add Course to Student —
void AdminWindow::addCourseToStudent() {
    QString sid = largeGetText(this, "Enroll Student", "Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","No such student"); return; }

    QString cid = largeGetText(this, "Enroll Student", "Course ID:");
    if (cid.isEmpty()) return;
    Course* c=nullptr;
    for (Course* p=*m_courseHead; p; p=p->next) if (p->getId()==cid) { c=p; break; }
    if (!c) { showMessage("Error","No such course"); return; }

    s->addCourse(c);
    showMessage("Enroll Student", "Done.");
}

// — Remove Student from Course —
void AdminWindow::removeStudentFromCourse() {
    QString sid = largeGetText(this, "Unenroll Student", "Student ID:");
    if (sid.isEmpty()) return;
    Student* s = m_students->findById(sid);
    if (!s) { showMessage("Error","No such student"); return; }

    QString cid = largeGetText(this, "Unenroll Student", "Course ID:");
    if (cid.isEmpty()) return;
    bool enrolled=false;
    for (Course* c=s->courseHead; c; c=c->next) if (c->getId()==cid) { enrolled=true; break; }
    if (!enrolled) { showMessage("Error","Not enrolled"); return; }

    s->removeCourse(cid);
    showMessage("Unenroll Student", "Done.");
}

// — Add Teacher —
void AdminWindow::addTeacher() {
    QString id = largeGetText(this,"Add Teacher","Teacher ID:");
    if (id.isEmpty()) return;
    if (m_teachers->findById(id)) { showMessage("Error","Teacher exists"); return; }
    QString pw = largeGetText(this,"Add Teacher","Password:", true);
    if (pw.isEmpty()) return;
    m_teachers->addTeacher(new Teacher(id,pw));
    showMessage("Add Teacher","Teacher added.");
}

// — Remove Teacher —
void AdminWindow::removeTeacher() {
    QString id = largeGetText(this,"Remove Teacher","Teacher ID:");
    if (id.isEmpty()) return;
    Teacher* p = m_teachers->head; Teacher* prev=nullptr;
    while(p) {
        if (p->getId()==id) {
            if(prev) prev->next=p->next; else m_teachers->head=p->next;
            delete p;
            showMessage("Remove Teacher","Teacher removed.");
            return;
        }
        prev=p; p=p->next;
    }
    showMessage("Remove Teacher","Not found.");
}

// — Assign Course to Teacher —
void AdminWindow::assignCourseToTeacher() {
    QString tid = largeGetText(this,"Assign to Teacher","Teacher ID:");
    if (tid.isEmpty()) return;
    Teacher* t = m_teachers->findById(tid);
    if (!t) { showMessage("Error","Teacher not found"); return; }

    QString cid = largeGetText(this,"Assign to Teacher","Course ID:");
    if (cid.isEmpty()) return;
    Course* c=nullptr;
    for (Course* p=*m_courseHead; p; p=p->next) if(p->getId()==cid){c=p;break;}
    if(!c){ showMessage("Error","Course not found"); return; }

    t->assignCourse(c);
    showMessage("Assign Course","Done.");
}

// — View Teacher Courses —
void AdminWindow::viewTeacherCourses() {
    QString tid = largeGetText(this,"View Teacher Courses","Teacher ID:");
    if (tid.isEmpty()) return;
    Teacher* t = m_teachers->findById(tid);
    if (!t) { showMessage("Error","Teacher not found"); return; }

    QString out; t->viewCourses(out);
    showMessage("Courses of "+tid, out.isEmpty()? "(none)" : out);
}
