// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QRHIBACKINGSTORE_H
#define QRHIBACKINGSTORE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/private/qrasterbackingstore_p.h>

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QRhiBackingStore : public QRasterBackingStore
{
public:
    QRhiBackingStore(QWindow *window);
    ~QRhiBackingStore();

    void flush(QWindow *window, const QRegion &region, const QPoint &offset) override;
};

QT_END_NAMESPACE

#endif // QRHIBACKINGSTORE_H
