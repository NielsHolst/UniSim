/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtGui>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <usbase/component.h>
#include <usengine/simulation.h>
#include "tree_view_widget.h"

using namespace UniSim;

TreeViewWidget::TreeViewWidget(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(createTree());
//    layout->addLayout(createButtons());
}

namespace {
    template<class T>
    QTreeWidgetItem* createItem(T *parentWidget, Component *component) {
        QString objectName{ component->id().label() };
        QVariant var = component->property("classLabel");
        QString className{ var.isValid() ? var.toString() : QString()};
        QStringList labels;
        labels << objectName << className;
        return new QTreeWidgetItem(parentWidget, labels);
    }

    void addComponentAndChildren(QTreeWidgetItem *parentWidget, Component *component) {
        QTreeWidgetItem *item;
        parentWidget->addChild( item = createItem(parentWidget, component) );
        auto children = component->seekChildren<Component*>("*");
        for (auto child : children)
            addComponentAndChildren(item, child);
    }
}

QLayout* TreeViewWidget::createTree() {
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tree = new QTreeWidget());

    tree->setColumnCount(2);
    auto children = simulation()->seekChildren<Component*>("*");
    for (auto child : children) {
        QTreeWidgetItem *topItem;
        tree->addTopLevelItem( topItem = createItem(tree, child) );
        auto grandchildren = child->seekChildren<Component*>("*");
        for (auto grandchild : grandchildren)
            addComponentAndChildren(topItem, grandchild);

    }
    return layout;
}

QLayout* TreeViewWidget::createButtons() {
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *closeButton;
    layout->addStretch();
    layout->addWidget(closeButton = new QPushButton("Close", this));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    closeButton->setDefault(true);
    return layout;
}

