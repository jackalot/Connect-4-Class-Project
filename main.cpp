#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <stdexcept>
#include <csignal>

// Signal handler for crash diagnostics
void signalHandler(int signum) {
    qDebug() << "Critical Error: Received signal " << signum;

    switch(signum) {
    case SIGSEGV:
        qDebug() << "Segmentation fault (invalid memory access)";
        break;
    case SIGABRT:
        qDebug() << "Abnormal termination";
        break;
    case SIGILL:
        qDebug() << "Illegal instruction";
        break;
    case SIGFPE:
        qDebug() << "Floating-point exception";
        break;
    default:
        qDebug() << "Unknown signal: " << signum;
    }

    // Log to file for post-mortem analysis
    FILE* crashLog = fopen("crash_log.txt", "a");
    if (crashLog) {
        fprintf(crashLog, "Crash detected. Signal: %d\n", signum);
        fclose(crashLog);
    }

    exit(signum);
}

int main(int argc, char *argv[])
{
    // Install signal handlers for crash diagnostics
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGFPE, signalHandler);

    try {
        // Extensive logging and error checking
        qDebug() << "Application starting";
        qDebug() << "Qt Version: " << QT_VERSION_STR;

        // Create application
        QApplication a(argc, argv);

        // Log application details
        qDebug() << "QApplication created";

        // Create main window with error checking
        MainWindow* w = nullptr;
        try {
            w = new MainWindow();

            if (!w) {
                throw std::runtime_error("Failed to create MainWindow");
            }

            qDebug() << "MainWindow created successfully";

            // Show window
            w->show();

            qDebug() << "MainWindow shown";

            // Run event loop
            int result = a.exec();

            qDebug() << "Application event loop completed with result: " << result;

            return result;
        }
        catch (const std::exception& e) {
            qDebug() << "Exception in MainWindow creation: " << e.what();

            // Show error message to user
            QMessageBox::critical(nullptr,
                                  "Initialization Error",
                                  QString("Failed to initialize main window: %1").arg(e.what())
                                  );

            // Clean up
            if (w) delete w;
            return -1;
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Unhandled exception in main: " << e.what();

        // Show error message to user
        QMessageBox::critical(nullptr,
                              "Critical Error",
                              QString("An unexpected error occurred: %1").arg(e.what())
                              );

        return -2;
    }
    catch (...) {
        qDebug() << "Unknown exception in main";

        QMessageBox::critical(nullptr,
                              "Critical Error",
                              "An unknown fatal error occurred"
                              );

        return -3;
    }
}
