#ifndef FITNESSMODEL_H
#define FITNESSMODEL_H

#include <QAbstractTableModel>
#include <QList>

struct Fitness {
    int id;
    QString name;
    QString target_muscle;
    int weight_limit;
    bool is_electric;
    QString last_maintenance_date;
};

class fitnessmodel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit fitnessmodel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    // Add data:
//    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

//    // Remove data:
//    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void addFitness(const Fitness& fitness);
    void removeFitness(int row);
    void updateFitness(int row, const Fitness &fitness);
    Fitness getFitness(int row) const;
    const QList<Fitness>& getFitnesses() const;
    void setFitnesses(const QList<Fitness> &fitnesses);
    void clear();

private:
    QList<Fitness> fitnesses;
};

#endif // FITNESSMODEL_H
