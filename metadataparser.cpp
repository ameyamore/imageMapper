#include "metadataparser.h"

metaDataParser::metaDataParser(QObject *parent) : QObject(parent)
{

}

QStringList metaDataParser::getImageList(QString path)
{

}

const QString &metaDataParser::getImageFolderPath() const
{
    return imageFolderPath;
}

void metaDataParser::setImageFolderPath(const QString &newImageFolderPath)
{
    imageFolderPath = newImageFolderPath;
}
