#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class imageModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    imageModel();
    enum imageModelRoles
    {
        TypeRole = Qt::UserRole + 1,
        SizeRole
    };
};

#endif // IMAGEMODEL_H
