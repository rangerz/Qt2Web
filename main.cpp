#include "WidgetStreamer.h"
#include <QApplication>
#include <QTextEdit>

int main(int Argc, char *Argv[])
{
    QApplication App(Argc, Argv);

    QTextEdit Widget;
    Widget.show();

    WidgetStreamer Streamer((QWidget*)&Widget);

    return App.exec();
}
