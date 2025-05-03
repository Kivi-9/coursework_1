#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include "fitnessmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_addButton_clicked();

    void on_editButton_clicked();

    void on_deleteButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    fitnessmodel *f_model;
    int f_currentId = 1;

    void updateForm(const Fitness& fitness);
    void clearForm();
    bool validateForm();
    Fitness getFitnessFromForm() const;
    int getSelectedRow();
};
#endif // MAINWINDOW_H
