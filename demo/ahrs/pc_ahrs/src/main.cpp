/* OpenGL Frame
   sudo apt-get install freeglut3 freeglut3-dev
*/

#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(0,800,600);


    if (!w.connect()) return 0;

    w.show();
    return a.exec();
}
