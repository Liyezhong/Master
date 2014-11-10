# include pri file of PasswordManager

!include("Scheduler.pri") {
    error("Scheduler.pri not found")
}
QT += xml
QT += xmlpatterns
QT += network
