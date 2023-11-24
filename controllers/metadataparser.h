#ifndef METADATAPARSER_H
#define METADATAPARSER_H

#include <QObject>
#include <exiv2/exiv2.hpp>
#include <QLocation>
#include <QGeoCoordinate>
#include <QGeoRoute>
#include <QDir>
#include <QDebug>
#include "commonIncludes.h"
#include <vector>
#include <sstream>
#include <QQmlListProperty>

struct imageHolderStruct
{
    QGeoCoordinate imageGeoCord;
    double timeStamp;

    void operator()(const QGeoCoordinate& coordinate,double imageEpochTime) {
        this->imageGeoCord = coordinate;
        this->timeStamp = imageEpochTime;
    }
    imageHolderStruct(const QGeoCoordinate& coordinate,double imageEpochTime) : imageGeoCord(coordinate), timeStamp(imageEpochTime) {}
};

struct sortHelper
{
    bool operator()(imageHolderStruct a, imageHolderStruct b) const { return a.timeStamp < b.timeStamp; }
};

class metaDataParser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<QGeoCoordinate> coordList READ getImageRecords)
public:
    explicit metaDataParser(QObject *parent = nullptr);

    ~metaDataParser();
    void createImageRecords();

    double calculateExivGPSCoord(std::string expression, std::string coordinateReference);

    bool exifDatafromImage(QString imagePath, imageHolderStruct &imageInfo);

    Q_INVOKABLE QVector<QGeoCoordinate> getImageRecords();

signals:
    void imageGPSListChanged();
private:
    void generateImageList();

signals:

private:
    QList<imageHolderStruct> imageRecords;
    std::vector<Exiv2::ExifKey> GPSExifKeyVector;
    QStringList imageList;
    QDir* imageDirectory;
};

#endif // METADATAPARSER_H
