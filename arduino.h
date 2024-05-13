#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Arduino
{
public:
    Arduino();
    int connect_arduino();            // connect to arduino
    int close_arduino();              // close arduino
    int write_to_arduino(QByteArray); // write to arduino
    QByteArray read_from_arduino();   // read from arduino
    QSerialPort *getserial();         // get arduino
    QString getarduino_port_name();

private:
    QSerialPort *serial;                               // declare serial port
    static const quint16 arduino_uno_vendor_id = 9025; // arduino uno vendor id
    static const quint16 arduino_uno_producy_id = 67;  // arduino uno product id
    QString arduino_port_name;                         // arduino uno port name
    bool arduino_is_available;                         // arduino uno availability

    QByteArray data; // variable to store arduino serial data
};

#endif // ARDUINO_H

