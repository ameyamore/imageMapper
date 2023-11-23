#include "metadataparser.h"

metaDataParser::metaDataParser(QObject *parent) : QObject(parent)
{
    imageDirectory = new QDir (IMAGEPATH);
    qDebug()<<imageDirectory->absolutePath();
}

metaDataParser::~metaDataParser()
{
    delete imageDirectory;
}

void metaDataParser::generateImageList()
{
    imageList = imageDirectory->entryList();
}

void metaDataParser::createImageRecords()
{
    generateImageList();
    if(!imageList.empty())
    {
        foreach (QString image, imageList) {
            qDebug()<<image<<Qt::endl;
        }
    }
}
