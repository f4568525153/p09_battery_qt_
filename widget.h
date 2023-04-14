#include <QWidget>
#include <QTextEdit>
#include <QTimer>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDateTime>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

private slots:
    void updateBatteryInfo();
    void recordBatteryData(const QString &batteryInfo);

private:
    QTextEdit *textEdit;
    QTimer *timer;
    QProcess *process;
};
