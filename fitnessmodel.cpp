#include "fitnessmodel.h"

fitnessmodel::fitnessmodel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant fitnessmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return "ID";
            case 1:
                return "Название";
            case 2:
                return "Группа мышц";
            case 3:
                return "Весовой лимит";
            case 4:
                return "Электрический";
            case 5:
                return "Дата последнего обслуживания";
        }
    }
}

int fitnessmodel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return fitnesses.size();
}

int fitnessmodel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 6; // id name target_muscle weight_limit is_electric last_maintenance_date
}

QVariant fitnessmodel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= fitnesses.size())
        return QVariant();

    const Fitness& fitness = fitnesses.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch(index.column()) {
            case 0:
                return fitness.id;
            case 1:
                return fitness.name;
            case 2:
                return fitness.target_muscle;
            case 3:
                return fitness.weight_limit;
            case 4:
                return fitness.is_electric;
            case 5:
                return fitness.last_maintenance_date;
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

bool fitnessmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    Fitness &fitness = fitnesses[index.row()];

    QString newValue = value.toString();

    // newValue.replace(";", "\\;");

    switch (index.column()) {
        case 1: fitness.name = newValue; break;
        case 2: fitness.target_muscle = newValue; break;
        case 3: fitness.weight_limit = newValue.toInt(); break;
        case 4: fitness.is_electric = newValue == "true" ? true : false; break;
        case 5: fitness.last_maintenance_date = newValue; break;
        default: return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags fitnessmodel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.column() != 0) // ID не редактируется
        flags |= Qt::ItemIsEditable;

    return flags;
}

void fitnessmodel::addFitness(const Fitness &fitness)
{
    beginInsertRows(QModelIndex(), fitnesses.size(), fitnesses.size());
    fitnesses.append(fitness);
    endInsertRows();
}

void fitnessmodel::removeFitness(int row)
{
    if (row < 0 || row >= fitnesses.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    fitnesses.removeAt(row);
    endRemoveRows();
}

void fitnessmodel::updateFitness(int row, const Fitness &fitness)
{
    if (row < 0 || row >= fitnesses.size())
        return;

    fitnesses[row] = fitness;
    emit dataChanged(index(row, 0), index(row, columnCount()-1));
}

Fitness fitnessmodel::getFitness(int row) const
{
    if (row >= 0 && row < fitnesses.size())
        return fitnesses.at(row);
    return Fitness();
}

const QList<Fitness>& fitnessmodel::getFitnesses() const
{
    return fitnesses;
}

void fitnessmodel::setFitnesses(const QList<Fitness> &fitnesses_n)
{
    beginResetModel();
    fitnesses = fitnesses_n;
    endResetModel();
}

void fitnessmodel::clear()
{
    beginResetModel();
    fitnesses.clear();
    endResetModel();
}

//bool fitnessmodel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endInsertRows();
//}

//bool fitnessmodel::insertColumns(int column, int count, const QModelIndex &parent)
//{
//    beginInsertColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endInsertColumns();
//}

//bool fitnessmodel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    // FIXME: Implement me!
//    endRemoveRows();
//}

//bool fitnessmodel::removeColumns(int column, int count, const QModelIndex &parent)
//{
//    beginRemoveColumns(parent, column, column + count - 1);
//    // FIXME: Implement me!
//    endRemoveColumns();
//}
