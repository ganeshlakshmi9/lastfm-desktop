/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd                                       *
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

#include "App.h"
#include "PlaybackEvent.h"
#include "PlayerManager.h"
#include "ScrobbleShepherd.h"
#include "Settings.h"
#include "widgets/SettingsDialog/SettingsDialog.h"


App::App( int argc, char** argv ) 
   : QApplication( argc, argv ),
     m_playerManager( 0 )
{
    if (The::user().isEmpty())
    {
        LoginDialog d;
        if (d.exec() == QDialog::Accepted)
        {
            The::settings().setUsername( d.username() );
            The::settings().setPassword( d.password() );

            //TODO bootstrapping
        }
        else
        {
            quit();
            return;
        }
    }

    m_scrobbler = new ScrobblerManager( this );

    connect( this, SIGNAL(event( int, QVariant )), SLOT(onAppEvent( int, QVariant )) );
}


void
App::setPlayerManager( class PlayerManager* p )
{
    Q_ASSERT( m_playerManager == 0 );

    m_playerManager = p; 
    connect( p, SIGNAL(event( int, QVariant )), SIGNAL(event( int, QVariant )) );
}


PlaybackState::Enum
App::state() const
{
    return m_playerManager->state();
}


void
App::onAppEvent( int e, const QVariant& )
{
    switch (e)
    {
        case PlaybackEvent::ScrobblePointReached:
            m_scrobbler->scrobble( The::playerManager().track() );
            break;

        case PlaybackEvent::PlaybackStarted:
        case PlaybackEvent::TrackChanged:
            m_scrobbler->nowPlaying( The::playerManager().track() );
            break;
    }
}


void 
App::onBootstrapCompleted( const QString& playerId, const QString& username )
{}


void
App::love()
{}


void
App::ban()
{}


namespace The
{
    App& app() { return *(App*)qApp; }
    PlayerManager& playerManager() { return The::app().playerManager(); }
}
