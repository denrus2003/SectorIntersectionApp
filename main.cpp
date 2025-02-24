#include <QApplication>
#include <QSurfaceFormat>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Настройка OpenGL
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    // Создание и запуск основного окна
    MainWindow mainWindow;
    mainWindow.setWindowTitle("Задача 1: Пересечение секторных лучей");
    mainWindow.show();

    return app.exec();
}
