/***************************************************************************
**
** Copyright (c) 2013 Jolla Ltd.
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef LIPSTICKCOMPOSITORWINDOW_H
#define LIPSTICKCOMPOSITORWINDOW_H

#include <QWaylandSurfaceItem>
#include <QWaylandBufferRef>
#include <QPointer>
#include "lipstickglobal.h"

class LipstickCompositorWindowHwcNode;

class LIPSTICK_EXPORT LipstickCompositorWindow : public QWaylandSurfaceItem
{
    Q_OBJECT

    Q_PROPERTY(int windowId READ windowId CONSTANT)
    Q_PROPERTY(bool isInProcess READ isInProcess CONSTANT)
    Q_PROPERTY(bool isAlien READ isAlien CONSTANT)

    Q_PROPERTY(bool delayRemove READ delayRemove WRITE setDelayRemove NOTIFY delayRemoveChanged)
    Q_PROPERTY(QVariant userData READ userData WRITE setUserData NOTIFY userDataChanged)

    Q_PROPERTY(QString category READ category CONSTANT)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(qint64 processId READ processId CONSTANT)

    Q_PROPERTY(QRect mouseRegionBounds READ mouseRegionBounds NOTIFY mouseRegionBoundsChanged)
    Q_PROPERTY(bool focusOnTouch READ focusOnTouch WRITE setFocusOnTouch NOTIFY focusOnTouchChanged)

public:
    LipstickCompositorWindow(int windowId, const QString &, QWaylandQuickSurface *surface, QQuickItem *parent = 0);
    ~LipstickCompositorWindow();

    QVariant userData() const;
    void setUserData(QVariant);

    int windowId() const;
    qint64 processId() const;

    bool delayRemove() const;
    void setDelayRemove(bool);

    QString category() const;
    virtual QString title() const;
    virtual bool isInProcess() const;

    bool isAlien() const;

    QRect mouseRegionBounds() const;

    bool eventFilter(QObject *object, QEvent *event);

    Q_INVOKABLE void terminateProcess(int killTimeout);

    bool focusOnTouch() const;
    void setFocusOnTouch(bool focusOnTouch);

    Q_INVOKABLE void resize(const QSize &size);

protected:
    void itemChange(ItemChange change, const ItemChangeData &data);

    virtual bool event(QEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void touchEvent(QTouchEvent *event);

signals:
    void userDataChanged();
    void titleChanged();
    void delayRemoveChanged();
    void mouseRegionBoundsChanged();
    void committed();
    void focusOnTouchChanged();
    void resized();

private slots:
    void handleTouchCancel();
    void killProcess();

private:
    friend class LipstickCompositor;
    friend class WindowPixmapItem;
    void imageAddref(QQuickItem *item);
    void imageRelease(QQuickItem *item);

    bool canRemove() const;
    void tryRemove();
    void refreshMouseRegion();
    void refreshGrabbedKeys();
    void handleTouchEvent(QTouchEvent *e);

    qint64 m_processId;
    int m_windowId;
    bool m_isAlien;
    QString m_category;
    bool m_delayRemove:1;
    bool m_windowClosed:1;
    bool m_removePosted:1;
    bool m_mouseRegionValid:1;
    bool m_interceptingTouch:1;
    bool m_mapped : 1;
    bool m_focusOnTouch : 1;
    QVariant m_data;
    QRegion m_mouseRegion;
    QList<int> m_grabbedKeys;
    struct {
        QPointer<QWaylandSurface> oldFocus;
        QList<int> keys;
    } m_pressedGrabbedKeys;
    QVector<QQuickItem *> m_refs;
};

#endif // LIPSTICKCOMPOSITORWINDOW_H
