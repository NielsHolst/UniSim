/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef TREE_VIEW_WIDGET_H
#define TREE_VIEW_WIDGET_H
#include <QDialog>

class QLayout;
class QTreeWidget;

class TreeViewWidget : public QDialog
{
	Q_OBJECT
public:
    TreeViewWidget(QWidget *parent = 0);
private:
    QTreeWidget *tree;
    QLayout* createTree();
    QLayout* createButtons();
};


#endif
