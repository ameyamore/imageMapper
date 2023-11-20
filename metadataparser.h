#ifndef METADATAPARSER_H
#define METADATAPARSER_H

#include <QObject>
#include "exiv2/exiv2.hpp"
#include <QLocation>
#include <QFileSelector>

class metaDataParser : public QObject
{
    Q_OBJECT
public:
    explicit metaDataParser(QObject *parent = nullptr);

    QStringList getImageList(QString path);

    const QString &getImageFolderPath() const;

    void setImageFolderPath(const QString &newImageFolderPath);

signals:

private:
    QString imageFolderPath;

};

#endif // METADATAPARSER_H
