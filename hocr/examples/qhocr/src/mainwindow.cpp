#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>

#include "mainwindow.h"
#include <hocr.h>

MainWindow::MainWindow( QWidget *parent ):QMainWindow( parent )
{
	ui.setupUi( this );
}

void MainWindow::on_aboutButton_clicked()
{
	QMessageBox::information( 0, "About QHOCR", "QHOCR - a Qt4 GUI front end to the HOCR library" );
}

void MainWindow::on_exitButton_clicked()
{
	QApplication::exit(0);
}

void MainWindow::on_loadButton_clicked()
{
	QString s = QFileDialog::getOpenFileName(
                    this,
                    "Choose a scanned (300dpi) image",
                    "",
                    "Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif)");
	
	if (s.isEmpty())
		return;
		
	QPixmap pix(s);
	ui.scannedImage->setPixmap( pix );
	
	// hocr
	QImage img = pix.toImage();
	hocr_pixbuf hocr_pix;
	char text[3500];
	
	text[0] = 0;

	hocr_pix.pixels     = img.bits();
	hocr_pix.height     = img.size().height();
	hocr_pix.width      = img.size().width();	
	hocr_pix.rowstride  = img.bytesPerLine();
	hocr_pix.n_channels = img.depth() / 8;
	hocr_pix.brightness = 100;
	
	try{
		hocr_do_ocr(&hocr_pix, text, 3500);
		ui.translatedText->setPlainText( QString::fromUtf8(text) ); 
	}
	catch(...){
		QMessageBox::critical( this, "Error", "failed at hocr_do_ocr()" );
	}
	
}

void MainWindow::on_saveButton_clicked()
{
	QString s = QFileDialog::getSaveFileName(
                    this,
                    "Choose a file to save to",
                    "",
                    "Text files (*.txt *.utf8 *.html *.htm)");
					
					
	if (s.isEmpty())
		return;
		
	if (s.endsWith(".html") || s.endsWith(".htm"))
		saveHTML( s, ui.translatedText->toPlainText() );
	else if (s.endsWith(".utf8"))
		saveText( s, ui.translatedText->toPlainText(), true );
	else
		saveText( s, ui.translatedText->toPlainText(), false );
}

void MainWindow::saveHTML( QString fileName, QString text )
{
	QFile file( ":src/default.html" );
    file.open(QFile::ReadOnly | QFile::Text);
    QString html = file.readAll();
	
	html = QString(html).arg("File created by QHOCT - a Qt4 GUI for HOCR").arg( text );
	file.setFileName( fileName );
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical( this, "Error", "Cound not save file" );
		return;
	}
	QTextStream out(&file);
	out.setCodec(QTextCodec::codecForName("UTF-8")); 
	out << html;
}

void MainWindow::saveText( QString fileName, QString text, bool unicode )
{
	QFile file( fileName );
	
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical( this, "Error", "Cound not save file" );
		return;
	}

	QTextStream out(&file);
	out.setCodec( QTextCodec::codecForName(unicode?"UTF-8":"Windows 1255") );	
	out << text;
}
