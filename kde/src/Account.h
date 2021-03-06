/***************************************************************************
 *   Copyright (C) 2009-2010 by Savoir-Faire Linux                         *
 *   Author : Jérémy Quentin <jeremy.quentin@savoirfairelinux.com>         *
 *            Emmanuel Lepage Vallee <emmanuel.lepage@savoirfairelinux.com>*
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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QtCore/QString>
#include <QtGui/QListWidgetItem>
#include <QtGui/QColor>

#include "typedefs.h"
#include "AccountItemWidget.h"
#include "Item.h"

const QString account_state_name(QString & s);

class Account : public QObject, public Item<AccountItemWidget>{
Q_OBJECT
private:

   QString * accountId;
   MapStringString * accountDetails;
//    QListWidgetItem * item;
//    AccountItemWidget * itemWidget;

   Account();

public:
   
   ~Account();
   
   //Constructors
   static Account * buildExistingAccountFromId(QString _accountId);
   static Account * buildNewAccountFromAlias(QString alias);
   
   //Getters
   bool isNew() const;
   bool isChecked() const;
   const QString & getAccountId() const;
   MapStringString & getAccountDetails() const;
   QListWidgetItem * getItem();
   AccountItemWidget * getItemWidget();
   QString getStateName(QString & state);
   QColor getStateColor();
   QString getStateColorName();
   QString getAccountDetail(QString param) const;
   QString getAlias() const;
   bool isEnabled() const;
   bool isRegistered() const;
   
   //Setters
   void setAccountId(QString id);
   void setAccountDetails(MapStringString m);
   void setAccountDetail(QString param, QString val);
   
   //Updates
   void initItem();
   void initItemWidget();
   void updateState();
   
   //Operators
   bool operator==(const Account&)const;
   
public slots:
   void setEnabled(bool checked);
   
   
   
};



#endif