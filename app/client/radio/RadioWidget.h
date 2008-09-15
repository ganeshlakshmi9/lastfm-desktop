/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#ifndef RADIO_WIDGET_H
#define RADIO_WIDGET_H

#include <QMainWindow>


class RadioWidget : public QMainWindow
{
    Q_OBJECT
	
public:
    RadioWidget( QWidget* parent = 0 );
	
	struct 
	{
		class QTabWidget* tabWidget;
        class RadioMiniControls* controls;
	} ui;
	
protected:
	virtual void hideEvent( QHideEvent* )
	{
		emit hideEvent();
	}
	
signals:
	void hideEvent();
	
private slots:
    void onTune( const class RadioStation& );

public slots:
	void addTab( QWidget*, const QString );
};

#endif //RADIO_WIDGET_H