#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include "ui_mainwindow.h"


class MainWindow: public QMainWindow
{
Q_OBJECT

public:
	MainWindow( QWidget *parent=0 );

public slots:
	void on_aboutButton_clicked();
	void on_exitButton_clicked();
	void on_loadButton_clicked();
	void on_saveButton_clicked();

private:
	Ui::MainWindow ui;
	void saveHTML( QString fileName, QString text );
	void saveText( QString fileName, QString text, bool unicode );
};

#endif //__MAIN_WINDOW_H__
