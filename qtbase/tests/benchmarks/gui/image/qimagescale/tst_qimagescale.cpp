// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <qtest.h>
#include <QImage>

class tst_QImageScale : public QObject
{
    Q_OBJECT
private slots:
    void scaleRgb32_data();
    void scaleRgb32();

    void scaleArgb32pm_data();
    void scaleArgb32pm();

private:
    QImage generateImageRgb32(int width, int height);
    QImage generateImageArgb32(int width, int height);
};

void tst_QImageScale::scaleRgb32_data()
{
    QTest::addColumn<QImage>("inputImage");
    QTest::addColumn<QSize>("outputSize");

    QImage image = generateImageRgb32(1000, 1000);
    QTest::newRow("1000x1000 -> 2000x2000") << image << QSize(2000, 2000);
    QTest::newRow("1000x1000 -> 2000x1000") << image << QSize(2000, 1000);
    QTest::newRow("1000x1000 -> 1000x2000") << image << QSize(1000, 2000);
    QTest::newRow("1000x1000 -> 2000x500") << image << QSize(2000, 500);
    QTest::newRow("1000x1000 -> 500x2000") << image << QSize(500, 2000);
    QTest::newRow("1000x1000 -> 500x500") << image << QSize(500, 500);
    QTest::newRow("1000x1000 -> 200x200") << image << QSize(200, 200);
}

void tst_QImageScale::scaleRgb32()
{
    QFETCH(QImage, inputImage);
    QFETCH(QSize, outputSize);

    QBENCHMARK {
        volatile QImage output = inputImage.scaled(outputSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        // we need the volatile and the following to make sure the compiler does not do
        // anything stupid :)
        (void)output;
    }
}

void tst_QImageScale::scaleArgb32pm_data()
{
    QTest::addColumn<QImage>("inputImage");
    QTest::addColumn<QSize>("outputSize");

    QImage image = generateImageArgb32(1000, 1000).convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QTest::newRow("1000x1000 -> 2000x2000") << image << QSize(2000, 2000);
    QTest::newRow("1000x1000 -> 2000x1000") << image << QSize(2000, 1000);
    QTest::newRow("1000x1000 -> 1000x2000") << image << QSize(1000, 2000);
    QTest::newRow("1000x1000 -> 2000x500") << image << QSize(2000, 500);
    QTest::newRow("1000x1000 -> 500x2000") << image << QSize(500, 2000);
    QTest::newRow("1000x1000 -> 500x500") << image << QSize(500, 500);
    QTest::newRow("1000x1000 -> 200x200") << image << QSize(200, 200);
}

void tst_QImageScale::scaleArgb32pm()
{
    QFETCH(QImage, inputImage);
    QFETCH(QSize, outputSize);

    QBENCHMARK {
        volatile QImage output = inputImage.scaled(outputSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        // we need the volatile and the following to make sure the compiler does not do
        // anything stupid :)
        (void)output;
    }
}

/*
 Fill a RGB32 image with "random" pixel values.
 */
QImage tst_QImageScale::generateImageRgb32(int width, int height)
{
    QImage image(width, height, QImage::Format_RGB32);

    for (int y = 0; y < image.height(); ++y) {
        QRgb *scanline = (QRgb*)image.scanLine(y);
        for (int x = 0; x < width; ++x)
            scanline[x] = qRgb(x, y, x ^ y);
    }
    return image;
}

/*
 Fill a ARGB32 image with "random" pixel values.
 */
QImage tst_QImageScale::generateImageArgb32(int width, int height)
{
    QImage image(width, height, QImage::Format_ARGB32);
    const int byteWidth = width * 4;

    for (int y = 0; y < image.height(); ++y) {
        uchar *scanline = image.scanLine(y);
        for (int x = 0; x < byteWidth; ++x)
            scanline[x] = x ^ y;
    }
    return image;
}

QTEST_MAIN(tst_QImageScale)
#include "tst_qimagescale.moc"
