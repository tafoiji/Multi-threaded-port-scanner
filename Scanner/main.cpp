#include <QApplication>
#include "Scanner.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle("Fusion");

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53)); // Фон окна
    palette.setColor(QPalette::WindowText, Qt::white); // Цвет текста
    palette.setColor(QPalette::Base, QColor(25, 25, 25)); // Цвет полей ввода
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white); // Цвет текста в полях
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);

    a.setPalette(palette);

    QApplication::setWindowIcon(QIcon(":/icons/Port.jpg"));

    MainWindow w;
    w.setWindowIcon(QIcon(":/icons/Port.jpg"));
    w.show();

    return a.exec();
}
