// #include "GuiQt.hpp"
#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget dialog;
    dialog.show();

    return QApplication::exec();  // this runs the main event loop and sees to
                                  // it that cleanup is done
}
