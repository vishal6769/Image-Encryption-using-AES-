#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Input Window");
    window.resize(300, 100);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    QLabel *label = new QLabel("Write something:");
    QLineEdit *input = new QLineEdit();

    layout->addWidget(label);
    layout->addWidget(input);

    // When user presses Enter
    QObject::connect(input, &QLineEdit::returnPressed, [&]() {
        std::string text = input->text().toStdString();
        std::cout << "You wrote: " << text << std::endl;
        window.close();  // close window
        app.quit();      // exit app
    });

    window.show();
    return app.exec();
}
