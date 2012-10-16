/*
   Copyright 2005-2012 Last.fm Ltd.
      - Primarily authored by Frantz Joseph

   This file is part of the Last.fm Desktop Application Suite.

   lastfm-desktop is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lastfm-desktop is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with lastfm-desktop.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QString>
#include <QWebView>
#include <QNetworkCookieJar>

#include <lastfm/ws.h>

#include "lib/unicorn/UnicornSettings.h"

#include "PersistentCookieJar.h"
#include "AnalyticsService.h"


AnalyticsService::AnalyticsService()
{
    m_webView = new QWebView();
    PersistentCookieJar* jar = new PersistentCookieJar( this );
    m_webView->page()->networkAccessManager()->setCookieJar( jar );
    connect( m_webView, SIGNAL(loadFinished(bool)), jar, SLOT(save()) );
}

AnalyticsService::~AnalyticsService()
{
    m_webView->close();
}

void
AnalyticsService::sendEvent( const QString& category, const QString& action, const QString& label, const QString& value )
{
    m_webView->load( QString( "http://users.last.fm/~michael/ga.html#event?category=%1&action=%2&label=%3&value=%4" ).arg( category, action, label, value ) );
}

void
AnalyticsService::sendPageView( const QString& url )
{
    m_webView->load( QString( "http://users.last.fm/~michael/ga.html#pageview?url=%1" ).arg( url ) );
}
