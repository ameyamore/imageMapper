#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QGeoRoute>

class imageModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    imageModel();
    enum imageModelRoles
    {
        GeoCoordinate = Qt::UserRole + 1,
        timeStamp
    };
};

#endif // IMAGEMODEL_H
