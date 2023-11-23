#ifndef METADATAPARSER_H
#define METADATAPARSER_H

#include <QObject>
#include "exiv2/exiv2.hpp"
#include <QLocation>
#include <QGeoCoordinate>
#include <QGeoRoute>
#include <QDir>
#include <QDebug>
#include "commonIncludes.h"

struct imageHolderStruct
{
    QGeoCoordinate imageGeoCord;
    double timeStamp;

    imageHolderStruct(QGeoCoordinate& coordinate,double imageEpochTime) : imageGeoCord(coordinate), timeStamp(imageEpochTime) {}
};

class metaDataParser : public QObject
{
    Q_OBJECT
public:
    explicit metaDataParser(QObject *parent = nullptr);

    ~metaDataParser();
    void createImageRecords();
private:
    void generateImageList();




signals:

private:
    QVector<imageHolderStruct> imageRecords;
    QStringList imageList;
    QDir* imageDirectory;
};

#endif // METADATAPARSER_H
