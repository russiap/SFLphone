/***************************************************************************
 *   Copyright (C) 2010 by Savoir-Faire Linux                              *
 *   Author : Mathieu Leduc-Hamel                                          *
 *   mathieu.leduc-hamel@savoirfairelinux.com                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
 * http://doc.trolltech.com/4.5/itemviews-editabletreemodel.html
 */

#ifndef CALLTREE_VIEW_H
#define CALLTREE_VIEW_H

#include <QTreeView>

class CallTreeModel;
class CallTreeItem;
class Call;
class QModelIndex;
class QTreeWidgetItem;
class QMimeData;

class CallTreeView : public QTreeView
{
	Q_OBJECT
public:
	CallTreeView(QWidget *parent);
	~CallTreeView();

	CallTreeItem* insert(CallTreeItem *item, Call* call);
	CallTreeItem* insert(Call* call);
	void remove(QModelIndex & index) const;
	void removeCurrent() const;
	CallTreeItem* currentItem();
	CallTreeItem* getItem(const QModelIndex &index);
	void setCurrentRow(int row);
	int count();

	bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);    
	QStringList mimeTypes() const;
	Qt::DropActions supportedDropActions () const;
private:
	CallTreeModel *treeModel;
signals:
	void currentItemChanged();
	void itemChanged();
};

#endif // CALLTREE_VIEW_H