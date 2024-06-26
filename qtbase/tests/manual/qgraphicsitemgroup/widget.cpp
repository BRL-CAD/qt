// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#undef QT_NO_FOREACH // this file contains unported legacy Q_FOREACH uses

#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    previousSelectionCount(0)
{
    ui->setupUi(this);

    effect = new QGraphicsOpacityEffect;
    effect->setOpacity(0.3);
    ui->groupBox->setGraphicsEffect(effect);

    fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.3);
    fadeIn->setEndValue(1.);

    fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.);
    fadeOut->setEndValue(0.3);

    scene = new CustomScene;
    scene->setSceneRect(-250,-250,500,500);
    ui->view->setScene(scene);
    scene->setBackgroundBrush(Qt::white);
    ui->view->setInteractive(true);
    ui->view->setDragMode(QGraphicsView::RubberBandDrag);
    ui->view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->view->setRenderHints( QPainter::SmoothPixmapTransform |
                              QPainter::TextAntialiasing |
                              QPainter::Antialiasing );

    rectBlue = new CustomItem(-100, -100, 50, 50, QColor(80, 80, 200));
    scene->addItem(rectBlue);
    rectGreen = new CustomItem(100, -100, 50, 50, QColor(80, 200, 80));
    scene->addItem(rectGreen);
    rectRed = new CustomItem(-100, 100, 50, 50, QColor(200, 80, 80));
    scene->addItem(rectRed);
    rectYellow = new CustomItem(100, 100, 50, 50, QColor(200, 200, 20));
    scene->addItem(rectYellow);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSceneSelectionChanged()));
}

Widget::~Widget()
{
    delete ui;
    delete fadeIn;
    delete fadeOut;
}

void Widget::on_rotate_valueChanged(int value)
{
    if (scene->selectedItems().count() != 1)
        return;

    QGraphicsItem *item = scene->selectedItems().at(0);
    item->setRotation(value);
    ui->rotateItem->setValue(checkedItem()->rotation());
}

void Widget::on_scale_valueChanged(int value)
{
    if (scene->selectedItems().count() != 1)
        return;

    QGraphicsItem *item = scene->selectedItems().at(0);
    item->setScale(value / 10.);
    ui->scaleItem->setValue(checkedItem()->scale() * 10);
}

void Widget::on_rotateItem_valueChanged(int value)
{
    if (!scene->selectedItems().empty() && scene->selectedItems().at(0) == checkedItem())
        ui->rotate->setValue(value);
    else
        checkedItem()->setRotation(value);
}

void Widget::on_scaleItem_valueChanged(int value)
{
    if (!scene->selectedItems().empty() && scene->selectedItems().at(0) == checkedItem())
        ui->scale->setValue(value);
    else
        checkedItem()->setScale(value / 10.);
}

void Widget::on_group_clicked()
{
    const QList<QGraphicsItem*> all = scene->selectedItems();
    if (all.size() < 2)
        return;

    const QList<CustomItem*> items = scene->selectedCustomItems();
    QList<CustomGroup*> groups = scene->selectedCustomGroups();

    if (groups.size() == 1) {
        for (CustomItem *item : items) {
            item->setSelected(false);
            groups[0]->addToGroup(item);
        }

        return;
    }

    CustomGroup* group = new CustomGroup;
    scene->addItem(group);
    for (QGraphicsItem *item : all) {
        item->setSelected(false);
        group->addToGroup(item);
    }
    group->setSelected(true);

    updateUngroupButton();
}

void Widget::on_dismantle_clicked()
{
    const QList<CustomGroup*> groups = scene->selectedCustomGroups();

    for (CustomGroup *group : groups) {
        foreach (QGraphicsItem *item, group->childItems())
            group->removeFromGroup(item);

        delete group;
    }

    updateUngroupButton();
}

void Widget::on_merge_clicked()
{
    QList<CustomGroup*> groups = scene->selectedCustomGroups();
    if (groups.size() < 2)
        return;

    CustomGroup *newBigGroup = groups.takeFirst();
    foreach (CustomGroup *group, groups) {
        foreach (QGraphicsItem *item, group->childItems())
            item->setGroup(newBigGroup);

        delete group;
    }
}

void Widget::onSceneSelectionChanged()
{
    QList<QGraphicsItem*> all = scene->selectedItems();
    QList<CustomGroup*> groups = scene->selectedCustomGroups();

    if (all.empty() && all.count() != previousSelectionCount) {
        fadeOut->start();
    } else if (previousSelectionCount == 0) {
        fadeIn->start();
    }

    if (all.count() == 1) {
        QGraphicsItem *item = all.at(0);
        ui->rotate->setValue(item->rotation());
        ui->scale->setValue(item->scale() * 10);
    } else {
        ui->rotate->setValue(ui->rotate->minimum());
        ui->scale->setValue(ui->scale->minimum());
    }

    ui->rotate->setEnabled(all.size() == 1);
    ui->scale->setEnabled(all.size() == 1);
    ui->group->setEnabled(all.size() > 1);
    ui->dismantle->setEnabled(!groups.empty());
    ui->merge->setEnabled(groups.size() > 1);

    previousSelectionCount = all.size();

    updateUngroupButton();
}

void Widget::on_ungroup_clicked()
{
    QGraphicsItemGroup *oldGroup = checkedItem()->group();
    checkedItem()->setGroup(0);
    if (oldGroup && oldGroup->childItems().empty())
        delete oldGroup;

    updateUngroupButton();
}

void Widget::updateUngroupButton()
{
    ui->ungroup->setEnabled(checkedItem()->group() != 0);
}

CustomItem * Widget::checkedItem() const
{
    CustomItem *item = nullptr;

    if (ui->blue->isChecked())
        item = rectBlue;
    else if (ui->red->isChecked())
        item = rectRed;
    else if (ui->green->isChecked())
        item = rectGreen;
    else if (ui->yellow->isChecked())
        item = rectYellow;

    return item;
}

void Widget::on_buttonGroup_buttonClicked()
{
    ui->rotateItem->setValue(checkedItem()->rotation());
    ui->scaleItem->setValue(checkedItem()->scale() * 10);

    updateUngroupButton();
}
