#include "metadataparser.h"

metaDataParser::metaDataParser(QObject *parent) : QObject(parent)
{
    imageDirectory = new QDir (IMAGEPATH);
    GPSExifKeyVector = {Exiv2::ExifKey("Exif.Image.GPSTag"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSVersionID"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSLatitudeRef"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSLongitudeRef"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSAltitudeRef"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSAltitude"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSTimeStamp"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSDOP"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSProcessingMethod"),
                        Exiv2::ExifKey("Exif.GPSInfo.GPSDateStamp")};
}

metaDataParser::~metaDataParser()
{
    delete imageDirectory;
}

void metaDataParser::generateImageList()
{
    QStringList filter;
    filter<<"*.jpg"<<"*.JPG"<<"*.jpeg"<<"*.JPEG";
    imageList = imageDirectory->entryList(filter);
}

QVector<QGeoCoordinate> metaDataParser::getImageRecords()
{
    QVector<QGeoCoordinate> gpslist;
    for(auto record : qAsConst(imageRecords))
    {
        gpslist.push_back(record.imageGeoCord);
    }
    return gpslist;
}

void metaDataParser::createImageRecords()
{
    generateImageList();
    if(!imageList.empty())
    {
        foreach (QString image, imageList) {

            imageHolderStruct imageData(QGeoCoordinate(0.0,0.0),0.0);
            if(exifDatafromImage(imageDirectory->absoluteFilePath(image),imageData))
            {
                imageRecords.push_back(imageData);
            }
        }
    }
    sortHelper helper;
    std::sort(imageRecords.begin(),imageRecords.end(),helper);
    emit imageGPSListChanged();
}

double metaDataParser::calculateExivGPSCoord(std::string expression,std::string coordinateReference)
{
    double coordinate = 0.00;
    std::vector<std::string> expressionVector;
    while(expression.find(" ") != std::string::npos)
    {
        expressionVector.push_back(expression.substr(0,expression.find(" ")));
        expression = expression.erase(0,expression.find(" ") + 1);
    }
    expressionVector.push_back(expression);

    double powerCounter = 0;

    for(const std::string &expr : expressionVector)
    {
        std::istringstream exprStream(expr);
        double result;
        char expOperator;
        double operand;
        exprStream >> result >> expOperator >> operand;

        operand = operand * pow(60,powerCounter);
        result = result / operand;
        powerCounter++;
        coordinate += result;
    }
    if(coordinateReference == "S" || coordinateReference == "W" || coordinateReference == "-1")
    {
        coordinate *= -1;
    }
    return coordinate;
}

bool metaDataParser::exifDatafromImage(QString imagePath,imageHolderStruct& imageInfo)
{
    bool metadataParsed = false;
    Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(imagePath.toStdString());
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();

    double latitude = qQNaN();
    double longitude = qQNaN();
    double altitude = qQNaN();


    if(exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude")) != exifData.end() && exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitudeRef")) != exifData.end())
    {
        std::string latitudeString = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude"))->value().toString();
        std::string latitudeRefString = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitudeRef"))->value().toString().c_str();
        latitude = calculateExivGPSCoord(latitudeString,latitudeRefString);
    }

    if(exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude")) != exifData.end() && exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitudeRef")) != exifData.end())
    {
        std::string longitudeString = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude"))->value().toString();
        std::string longitudeRefString = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitudeRef"))->value().toString().c_str();
        longitude = calculateExivGPSCoord(longitudeString,longitudeRefString);
    }

    if(exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitude")) != exifData.end() && exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitudeRef")) != exifData.end())
    {
        std::string altitudeString = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitude"))->value().toString();
        std::string altitudeRefString = exifData.findKey(Exiv2::ExifKey("Exif.GPSInfo.GPSAltitudeRef"))->value().toString().c_str();
        altitude = calculateExivGPSCoord(altitudeString,altitudeRefString);
    }

    QGeoCoordinate geoCoordinate(latitude,longitude,altitude);

    if(geoCoordinate.isValid())
    {
        QDateTime imageDateTime;
        if(exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal")) != exifData.end())
        {
            QString creationDateTimeOfImage = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"))->toString().c_str();
            QStringList creationDateTimeContainer = creationDateTimeOfImage.split(" ");

            QDate date = QDate::fromString(creationDateTimeContainer.at(0),Qt::DateFormat::ISODate);
            QTime time = QTime::fromString(creationDateTimeContainer.at(1),"hh:mm:ss");

            imageDateTime.setDate(date);
            imageDateTime.setTime(time);
        }
        imageInfo.imageGeoCord = geoCoordinate;
        imageInfo.timeStamp = imageDateTime.toSecsSinceEpoch();
        metadataParsed = true;
    }

    return metadataParsed;
}
