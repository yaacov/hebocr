#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
	/*QMainWindow *window = new QMainWindow;
	Ui::MainWindow ui;
	ui.setupUi(window);
	*/
	
	QMainWindow *window = new MainWindow;
	window->show();
	return app.exec();
}
