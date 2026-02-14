#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>

QString processFileData(QString data) {
    // Your custom operation goes here
    // Example: convert all text to uppercase
    return data.toUpper();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Simple File Processor");

    QVBoxLayout *layout = new QVBoxLayout(&window);
    QLabel *status = new QLabel("Choose a file to process...");
    QPushButton *openBtn = new QPushButton("Open File");

    layout->addWidget(status);
    layout->addWidget(openBtn);

    QObject::connect(openBtn, &QPushButton::clicked, [&]() {

        QString inputFile = QFileDialog::getOpenFileName(
            &window, "Select File", "", "All Files (*.*)");

        if (inputFile.isEmpty()) return;

        QFile file(inputFile);
        if (!file.open(QIODevice::ReadOnly)) {
            status->setText("Error opening file.");
            return;
        }

        QString data = file.readAll();
        file.close();

        QString newData = processFileData(data);

        QString outputFile = QFileDialog::getSaveFileName(
            &window, "Save Processed File", "", "All Files (*.*)");

        if (outputFile.isEmpty()) return;

        QFile out(outputFile);
        out.open(QIODevice::WriteOnly);
        out.write(newData.toUtf8());
        out.close();

        status->setText("File processed and saved!");
    });

    window.show();
    return app.exec();
}
