/**
 * @brief The main function of the application.
 * 
 * @param argc The number of command line arguments.
 * @param argv An array of command line arguments.
 * @return int The exit code of the application.
 */
#include <QApplication>
#include "vuepartie.h"
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    VuePartie partie;
    partie.show();
    return app.exec();
}

