/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "FriendsTuner.h"
#include "lib/types/User.h"
#include "Settings.h"
#include "StationDelegate.h"
#include "lib/radio/RadioStation.h"
#include <QNetworkAccessManager>

Q_DECLARE_METATYPE( QListWidgetItem* );

FriendsTuner::FriendsTuner()
{
	m_networkManager = new QNetworkAccessManager();
	setItemDelegate( new StationDelegate );
	setSortingEnabled( true );
	User u( The::settings().username() );
	WsReply* reply = u.getFriends();
	connect( reply, SIGNAL( finished( WsReply*)), SLOT(onFetchedFriends( WsReply*)) );
	connect( this, SIGNAL( itemClicked( QListWidgetItem* )), SLOT( onFriendClicked( QListWidgetItem*)) );
}


void
FriendsTuner::onFetchedFriends( WsReply* r )
{
	const UserList& friends = User::getFriends( r );
	
	foreach( User user, friends )
	{
		QListWidgetItem* item = new QListWidgetItem( user );
		item->setData( Qt::DecorationRole, QImage( ":/blank/user.png" ));
		addItem( item );
		
		if( user.smallImageUrl().isEmpty() )
			continue;
		
		QNetworkRequest request( user.smallImageUrl() );
		
		request.setAttribute( QNetworkRequest::User, QVariant::fromValue<QListWidgetItem*>( item ));
		QNetworkReply* get = m_networkManager->get( request );
		connect( get, SIGNAL(finished()), SLOT( onImageDownloaded()) );
		
	}
}


void
FriendsTuner::onFriendClicked( QListWidgetItem* i )
{
	const QString& username = i->data( Qt::DisplayRole ).toString();
	RadioStation r( username, RadioStation::Library );
	emit tune( r );
	i->setSelected( false );
}


void
FriendsTuner::onImageDownloaded()
{
	QNetworkReply* r = static_cast<QNetworkReply*>(sender());
	QListWidgetItem* i = r->request().attribute( QNetworkRequest::User ).value< QListWidgetItem*>();

	i->setData( Qt::DecorationRole, QImage::fromData( r->readAll() ));
}