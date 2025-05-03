#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , f_model(new fitnessmodel(this))
{
    ui->setupUi(this);
    ui->tableView->setModel(f_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    clearForm();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete f_model;
}


//void MainWindow::on_pushButton_2_clicked()
//{
//    filename = QFileDialog::getOpenFileName(this, tr("Открыть..."), QDir::homePath(), tr("Text files (*.csv *.txt)"));
//    ui->tableView->setModel(csvModel);


//    QFile file(filename);
//    if (!file.open(QFile::ReadWrite | QFile::Text)) {
//        qCritical() << "Error while openning file";
//    }

//    QTextStream in(&file);
//    in.setCodec("UTF-8");

//    QStringList headers = in.readLine().split(';');
//    csvModel->setHorizontalHeaderLabels(headers);

//    while(!in.atEnd()){
//        QString line = in.readLine();
//        QList<QStandardItem*> standardItemsList;
//        for (QString& item : line.split(";")) {
//            standardItemsList.append(new QStandardItem(item));
//        }

//        csvModel->insertRow(csvModel->rowCount(), standardItemsList);
//    }

//    ui->tableView->resizeColumnsToContents();
//    file.close();
//}



void MainWindow::on_addButton_clicked()
{
    if(!validateForm()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля корректно");
        return;
    }

    Fitness fitness = getFitnessFromForm();
    fitness.id = f_currentId++;
    f_model->addFitness(fitness);
    clearForm();
}


void MainWindow::on_editButton_clicked()
{
    int row = getSelectedRow();
    if (row == -1) return;

    if (!validateForm()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля корректно");
        return;
    }

    Fitness fitness = getFitnessFromForm();
    fitness.id = f_model -> getFitness(row).id;
    f_model->updateFitness(row, fitness);
    clearForm();
}


void MainWindow::on_deleteButton_clicked()
{
    int row = getSelectedRow();
    if (row == -1) return;

    f_model->removeFitness(row);
    clearForm();
}


void MainWindow::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "CSV Files (*.csv)");

    if(filePath.isEmpty()) return;

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "id;name;target_muscle;weight_limit;is_electric;last_maintenance_date\n";

    const QList<Fitness>& fitnesses = f_model->getFitnesses();
    for(const Fitness& fitness : fitnesses) {
        QString escapedName = fitness.name;
        escapedName.replace(";", "\\;");

        QString escapedMuscle = fitness.target_muscle;
        escapedMuscle.replace(";", "\\;");

        QString escapedDate = fitness.last_maintenance_date;
        escapedMuscle.replace(";", "\\;");

        out << fitness.id << ";"
            << escapedName << ";"
            << fitness.target_muscle << ";"
            << fitness.weight_limit << ";"
            << fitness.is_electric << ";"
            << fitness.last_maintenance_date << "\n";
    }
    file.close();
    QMessageBox::information(this, "Успех", "Данные сохранены в файл");
}


void MainWindow::on_loadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для чтения");
        return;
    }

    QList<Fitness> fitnesses;
    QTextStream in(&file);
    in.setCodec("UTF-8");

    in.readLine(); // Пропускаем заголовок
//    f_model->setHeaderData(, Qt::Horizontal,in.readLine().split(";"));

    while (!in.atEnd()) {
        QString line = in.readLine();
        // qDebug() << line;
        line.replace("\\;", "\x01");
        QStringList fields = line.split(";");
        if (fields.size() == 6) {
            Fitness fitness;
            fitness.id = fields[0].toInt();
            fitness.name = fields[1].replace("\x01", ";");
            fitness.target_muscle = fields[2].replace("\x01", ";");
            fitness.weight_limit = fields[3].toInt();
            fitness.is_electric = fields[4] == "true" ? true : false;
            fitness.last_maintenance_date = fields[5].replace("\x01", ";");
            fitnesses.append(fitness);

            if (fitness.id >= f_currentId) {
                f_currentId = fitness.id + 1;
            }
        }
    }

    file.close();
    f_model->setFitnesses(fitnesses);
    QMessageBox::information(this, "Успех", "Данные загружены из файла");
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    int row = getSelectedRow();
    if(row != -1) {
        updateForm(f_model->getFitness(row));
    }
}

void MainWindow::updateForm(const Fitness &fitness) {
    ui->nameEdit->setText(fitness.name);
    ui->muscleEdit->setText(fitness.target_muscle);

    ui->weightEdit->setText(QString::number(fitness.weight_limit));
    ui->is_electricEdit->setText(fitness.is_electric ? "true" : "false");
    ui->dateEdit->setText(fitness.last_maintenance_date);
}

void MainWindow::clearForm(){
    ui->nameEdit->clear();
    ui->muscleEdit->clear();
    ui->weightEdit->clear();
    ui->is_electricEdit->clear();
    ui->dateEdit->clear();
    ui->tableView->clearSelection();
}

bool MainWindow::validateForm(){
    bool ok = true;
    ui->weightEdit->text().toInt(&ok);
    return !ui->nameEdit->text().isEmpty() &&
            !ui->muscleEdit->text().isEmpty() &&
            !ui->is_electricEdit->text().isEmpty() &&
            !ui->dateEdit->text().isEmpty() && ok;
}

Fitness MainWindow::getFitnessFromForm() const{
    Fitness fitness;
    fitness.name = ui->nameEdit->text();
    fitness.target_muscle = ui->muscleEdit->text();
    fitness.weight_limit = ui->weightEdit->text().toInt();
    fitness.is_electric = ui->is_electricEdit->text() == "true" ? true : false;
    fitness.last_maintenance_date = ui->dateEdit->text();

    return fitness;
}

int MainWindow::getSelectedRow(){
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для редактирования/удаления");
        return -1;
    }
    return selected.first().row();
}
