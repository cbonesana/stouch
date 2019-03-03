#include <QCoreApplication>

#include <QFile>
#include <QString>
#include <QByteArray>
#include <QImage>
#include <QLabel>

#include <stdio.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // load and encode image
    printf("%s \n\n", argv[1]);

    printf("Reading...");
    QFile *sourceFile = new QFile(argv[1]);
    sourceFile->open(QIODevice::ReadOnly);
    QByteArray sourceImage = sourceFile->readAll();
    int originalSize = sourceImage.length();
    sourceFile->close();

    printf("complete!\n");

    //printf("%s \n\n", sourceImage.data());

    printf("Encoding base64...");
    QString base64Encoded = QString(sourceImage.toBase64());
    int encodedSize = base64Encoded.size();
    printf("complete!\n");

    //printf("%s \n\n", qPrintable(base64Encoded));
    printf("Original size: %d \nEncoded size:  %d \n", originalSize, encodedSize);

    // decode and save image
    printf("Decoding...");
    QImage image;
    QByteArray base64Data = base64Encoded.toLocal8Bit();

    printf("complete!\n");
    printf("Saving...");
    bool resultLoad = image.loadFromData(QByteArray::fromBase64(base64Data));
    bool resultSave = image.save("copy.png","PNG");
    printf("complete!\n");

    if (resultLoad && resultSave)
        printf("Saved successfully!\n");
    else
        printf("Error while saving.\n");

    return 0;
}
