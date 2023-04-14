#include "widget.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent), textEdit(new QTextEdit(this)), timer(new QTimer(this)), process(new QProcess(this))
{
    textEdit->setReadOnly(true);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);

    connect(timer, &QTimer::timeout, this, &Widget::updateBatteryInfo);
    connect(process, &QProcess::readyReadStandardOutput, [this]() {
        QString output = process->readAllStandardOutput();
        textEdit->setPlainText(output);
        recordBatteryData(output);
    });

    timer->start(5000);
    updateBatteryInfo();
}

void Widget::updateBatteryInfo()
{
    process->start("../P09C_GD", QStringList() << "-bat");
}
void Widget::recordBatteryData(const QString &batteryInfo)
{
    QFile file("battery_data.txt");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        QRegularExpression remainPercentRx("RemainPercent\\s*=\\s*(\\d+)%");
        QRegularExpression fullChargeCapacityRx("FullChargeCapacity\\s*=\\s*(\\d+)\\s+mAh");

        QRegularExpressionMatch remainPercentMatch = remainPercentRx.match(batteryInfo);
        QRegularExpressionMatch fullChargeCapacityMatch = fullChargeCapacityRx.match(batteryInfo);

        if (remainPercentMatch.hasMatch() && fullChargeCapacityMatch.hasMatch()) {
            QString remainPercent = remainPercentMatch.captured(1);
            QString fullChargeCapacity = fullChargeCapacityMatch.captured(1);
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            out << currentTime << " - RemainPercent: " << remainPercent << ", FullChargeCapacity: " << fullChargeCapacity << "\n";
        }
        file.close();
    }
}

