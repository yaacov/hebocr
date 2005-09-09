/***************************************************************************
 *            hocr_qt.h
 *
 *  Sun Aug 21 22:30:16 2005
 *  Copyright  2005  Yaacov Zamir
 *  <kzamir@walla.co.il>
 ****************************************************************************/

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


#ifndef __HOCR_QT_H__
#define __HOCR_QT_H__

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QTextEdit;
	
class hocr_qt:public QMainWindow
{
    Q_OBJECT

      public:
	hocr_qt ();

	  private slots:
	
	void open ();
	void apply ();
	void save ();
	bool saveFile (const QString & fileName);
	void zoomIn ();
	void zoomOut ();
	void normalSize ();
	void setFont ();
	void about ();

      private:
	
	void createActions ();
	void createToolBars();
	void createMenus();
	
	void scaleImage (double factor);
	
	void adjustScrollBar (QScrollBar * scrollBar, double factor);
	QLabel *imageLabel;
	QScrollArea *scrollArea;
	double scaleFactor;
	
	QDockWidget * dockedText;
	QTextEdit *textEdit;
	QString curFile;
	  
	QToolBar *fileToolBar;
	QToolBar *zoomToolBar;
	QToolBar *exitToolBar;
 	 
	QMenu *menuFile;
	QMenu *menuView;
	QMenu *menuHelp;
	
	QAction *openAct;
	QAction *applyAct;
	QAction *saveAct;
	
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	
	QAction *setFontAct;
	
	QAction *aboutAct;
	QAction *exitAct;
};

#endif
