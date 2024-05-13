#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "chat.h"

chat::chat()
{
}

void chat::csv_line()
{
    QString csvFilePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/database_1L/dbclean.csv";
    QString txtFilePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/database_1L/oneline.txt";

    QFile csvFile(csvFilePath);
    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open CSV file: " << csvFilePath;
        return;
    }

    QFile txtFile(txtFilePath);
    if (!txtFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to create or open text file: " << txtFilePath;
        csvFile.close();
        return;
    }

    QTextStream in(&csvFile);
    QTextStream out(&txtFile);

    // Write the header
    if (!in.atEnd())
    {
        QString header = in.readLine();
        out << "#HEADER#" << header << "#RECORD#";
    }

    // Write the records
    while (!in.atEnd())
    {
        QString record = in.readLine();
        out << record << "#RECORD#";
    }

    csvFile.close();
    txtFile.close();

    qDebug() << "CSV to one-line text conversion successful.";
}

void chat::clean_csv()
{
    QString inputFilePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/database_1L/dbraw.csv";
    QString outputFilePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/database_1L/dbclean.csv";

    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text) || !outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Error opening files.";
        return;
    }

    QTextStream inStream(&inputFile);
    QTextStream outStream(&outputFile);

    while (!inStream.atEnd())
    {
        // Read a line from the input file
        QString line = inStream.readLine();

        // Step 1: Remove <", ",>
        line.replace("\", \", ", "");

        // Step 2: Remove <","> at the beginning of the line
        if (line.startsWith("\", "))
        {
            line.remove(0, 3);
        }

        // Step 3: Remove <"> at the end of the line
        if (line.endsWith('\"'))
        {
            line.chop(1);
        }

        outStream << line << endl;
    }

    qDebug() << "CSV file cleaned successfully.";

    // Close the files
    inputFile.close();
    outputFile.close();
}

void chat::merge_db_prompt()
{
    QString dbPath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/database_1L/oneline.txt";
    QString basePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/base_prompt.txt";
    QString userMsgPath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/messages/user_msg.txt";
    QString outputPath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/messages/merged.txt";

    QFile dbFile(dbPath);
    QString dbContent;
    if (dbFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&dbFile);
        dbContent = in.readAll();
        dbFile.close();
    }

    // Read base prompt content
    QFile baseFile(basePath);
    QString baseContent;
    if (baseFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&baseFile);
        baseContent = in.readAll();
        baseFile.close();
    }

    // Replace {} in baseContent with dbContent
    baseContent.replace("{}", "{" + dbContent + "}");

    // Read user message
    QFile userMsgFile(userMsgPath);
    QString userMsg;
    if (userMsgFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&userMsgFile);
        userMsg = in.readAll();
        userMsgFile.close();
    }

    // Append user message to baseContent
    baseContent.append("\n" + userMsg);

    // Write to output file
    QFile outputFile(outputPath);
    if (outputFile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&outputFile);
        out << baseContent;
        outputFile.close();
    }
}
