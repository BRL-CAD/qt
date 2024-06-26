// Copyright (C) 2013 David Faure <faure+bluesystems@kde.org>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QDebug>
#include <QCoreApplication>
#include <QLockFile>
#include <QThread>

#ifdef Q_OS_UNIX
#  include <unistd.h>
#else
#  include <stdlib.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc <= 1)
        return -1;

    const QString lockName = QString::fromLocal8Bit(argv[1]);

    QString option;
    if (argc > 2)
        option = QString::fromLocal8Bit(argv[2]);

    if (option == "-uncleanexit") {
        QLockFile lockFile(lockName);
        lockFile.lock();
        // exit on purpose, so that the lock remains!
        _exit(0);
    } else if (option == "-busy") {
        QLockFile lockFile(lockName);
        lockFile.lock();
        QThread::msleep(500);
        return 0;
    } else {
        QLockFile lockFile(lockName);
        if (lockFile.isLocked()) // cannot happen, before calling lock or tryLock
            return QLockFile::UnknownError;

        lockFile.tryLock();
        return lockFile.error();
    }
}
