#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QTextStream>
#include <QProcess>

#include "chat_interface.h"
#include "ui_chat_interface.h"
#include "chat.h"

chat_interface::chat_interface(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::chat_interface)
{
    ui->setupUi(this);
    ui->loadingBar->hide();
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
}

chat_interface::~chat_interface()
{
    delete ui;
}

void chat_interface::on_send_button_clicked()
{
    QString inputText = ui->prompt_input->text();
    ui->chat_list->addItem(inputText);
    ui->prompt_input->clear();
    ui->chat_list->addItem("***************************************************************************************************************************************");

    // Export the prompt to text file
    QFile file("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/messages/user_msg.txt");
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << inputText << "\n";
        file.close();
    }

    ch.merge_db_prompt();

    // Run the Python script
    process.start("python", QStringList() << "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/ai_api.py");

    ui->loadingBar->setRange(0, 0);
    ui->loadingBar->show();
}

void chat_interface::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->loadingBar->hide();

    QString responsePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/messages/response.txt";
    QFile responseFile(responsePath);
    QString response_txt;
    if (responseFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&responseFile);
        while (!in.atEnd())
        {
            response_txt = in.readLine();
            // Add each line to the chat_list
            ui->chat_list->addItem(response_txt);
        }
        responseFile.close();
    }
    ui->chat_list->addItem("***************************************************************************************************************************************");
}
