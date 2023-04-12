#include "WifiTestWidget.h"
#include "GlobleVar.h"
#include <QDebug>
#include <QRegExp>

WifiTestWidget::WifiTestWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.widget->hide();
	init();
}

WifiTestWidget::~WifiTestWidget()
{}

void WifiTestWidget::init()
{
	bindSinalSlot();
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(slot_getInfo_wifi()));
	ui.WifiConnect_Button->hide();
	ui.WIFIName_LineEdit->setEnabled(false);
	ui.PassWD_LineEdit->setEnabled(false);
	ui.ssid_lineEdit->setEnabled(false);
	ui.bssid_lineEdit->setEnabled(false);
	ui.flag_lineEdit->setEnabled(false);
	ui.frequency_lineEdit->setEnabled(false);
	ui.dBm_lineEdit->setEnabled(false);
	ui.key_mgmt_lineEdit->setEnabled(false);
	ui.linkspeed_lineEdit->setEnabled(false);
	ui.wpa_state_lineEdit->setEnabled(false);
	ui.ip_lineEdit->setEnabled(false);
}

void WifiTestWidget::setPCMD(ParsingCmd* cmd)
{
	_ParCMD = cmd;
}

void WifiTestWidget::bindSinalSlot()
{
	//bool bl = connect(ui.WifiConnect_Button, SIGNAL(clicked()), this, SLOT(slot_connect_wifi()));
	bool bl = connect(ui.true_Button, SIGNAL(clicked()), this, SLOT(ok_button()));
	bl = connect(ui.false_Button, SIGNAL(clicked()), this, SLOT(false_button()));
	bl = connect(ui.down_Button, SIGNAL(clicked()), this, SLOT(slot_iper3_down()));
	bl = connect(ui.up_Button, SIGNAL(clicked()), this, SLOT(slot_iper3_up()));
}

void WifiTestWidget::initPage(QMap<QString, QString> _map)
{
	ui.WIFIName_LineEdit->setText(QString::fromStdString(gVar->m_stCommonInfo.wifiName));
	ui.PassWD_LineEdit->setText(QString::fromStdString(gVar->m_stCommonInfo.wifiPassWD));
	QMap<QString, QString>::Iterator iter = _map.begin();
	bool bl = false;
	while (iter != _map.end())
	{
		if (iter.key().length() < 40) {
			if(ui.WIFIName_LineEdit->text() == iter.key())
			{
				bl = true;
				break;
			}
			//ui.WifiSite_ComboBox->addItem(iter.key());
		}		
		++iter;
	}
	ui.ssid_lineEdit->setText("");
	ui.bssid_lineEdit->setText("");
	ui.flag_lineEdit->setText("");
	ui.frequency_lineEdit->setText("");
	ui.dBm_lineEdit->setText("");
	ui.key_mgmt_lineEdit->setText("");
	ui.linkspeed_lineEdit->setText("");
	ui.wpa_state_lineEdit->setText("");
	ui.ip_lineEdit->setText("");
	ui.label_info->setText("");
	_timer->start(1000);
	if (bl) {
// 		process = new QProcess(this);
// 		QObject::connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_readyRead()));
// 		process->setProcessChannelMode(QProcess::MergedChannels);
		
	}
	else
	{
		ui.label_info->setText(QString::fromLocal8Bit("Wi-Fi名称不正确"));
	}
}

void WifiTestWidget::slot_connect_wifi()
{
	//_ParCMD->setWiFi(ui.WifiSite_ComboBox->currentText(), ui.PassWD_LineEdit->text());
	_ParCMD->setWiFi(ui.WIFIName_LineEdit->text(), ui.PassWD_LineEdit->text());
	//_timer->start(1000);
}

void WifiTestWidget::slot_getInfo_wifi()
{
	_ParCMD->getWiFi();
}

void WifiTestWidget::slot_upWiFiInfo(QVariant var)
{
	QMap<QString, QString> _map = var.value<QMap<QString, QString>>();
	QMap<QString, QString>::Iterator iter = _map.begin();
	while (iter != _map.end())
	{
		if(iter.key().compare("ssid") == 0){
			ui.ssid_lineEdit->setText(iter.value());
		}
		else if(iter.key().compare("bssid") == 0){
			ui.bssid_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("flag") == 0) {
			ui.flag_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("frequency") == 0) {
			ui.frequency_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("dBm") == 0) {
			if (iter.value().toInt() < gVar->m_stCommonInfo.SignalStrength) {
				ui.dBm_lineEdit->setStyleSheet("background-color:#A23400;");
			}else{
				ui.dBm_lineEdit->setStyleSheet("background-color:#64A600;");
			}
			ui.dBm_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("key_mgmt") == 0) {
			ui.key_mgmt_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("linkspeed") == 0) {
			ui.linkspeed_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("wpa_state") == 0) {
			ui.wpa_state_lineEdit->setText(iter.value());
		}
		else if (iter.key().compare("ip") == 0) {
			ui.ip_lineEdit->setText(iter.value());
		}
		++iter;
	}
}



void WifiTestWidget::slot_iper3_down()
{
	QString fileName = QCoreApplication::applicationDirPath();
	fileName += "/iperf-3.1.3-win64/";
	QString program = fileName + "iperf3.exe";
	QStringList argu;
	argu.append("-f");
	argu.append("-m");
	argu.append("-c");
	argu.append(ui.ip_lineEdit->text());
	//process->start(program, argu);
}

void WifiTestWidget::slot_iper3_up()
{
	QString fileName = QCoreApplication::applicationDirPath();
	fileName += "/iperf-3.1.3-win64/";
	QString program = fileName + "iperf3.exe";
	QStringList argu;
	argu.append("-R");
	argu.append("-f");
	argu.append("-m");
	argu.append("-c");
	argu.append(ui.ip_lineEdit->text());
	//process->start(program, argu);
}

void WifiTestWidget::slot_readyRead()
{
// 	QByteArray qba = process->readAllStandardOutput();
// 	QTextCodec* pTextCodec = QTextCodec::codecForName("System");
// 	assert(pTextCodec != nullptr);
// 	QString str = pTextCodec->toUnicode(qba);
// 	ui.textBrowser->insertPlainText(str);
	float num;
	std::vector<float> v;
// 	while (!process->atEnd())
// 	{
// 		QString result = QLatin1String(process->readLine());
// 		if(result.contains("/sec")&& result.contains("sender")) {
// 			num = getSpeed(result);
// 			_averageBandwidth = num;
// 		}
// 		else if (result.contains("/sec")) {
// 			num = getSpeed(result);
// 			v.push_back(num);
// 		}
// 	}
	auto res = std::minmax_element(v.begin(), v.end());
	_minBandwidth = *res.first;
	_maxBandwidth = *res.second;
	upToWidget(_minBandwidth, _maxBandwidth, _averageBandwidth);
}

void WifiTestWidget::false_button()
{
	if (!this->isHidden()) {
		this->hide();
	}
	_timer->stop();
// 	if (process != nullptr) {
// 		process->close();
// 		process->waitForFinished();
// 	}	
	emit wifi_signal("wifiscanresults", Result_False);
}

void WifiTestWidget::ok_button()
{
	if (!this->isHidden()) {
		this->hide();
	}
	_timer->stop();
// 	if (process != nullptr) {
// 		process->close();
// 		process->waitForFinished();
// 	}
	emit wifi_signal("wifiscanresults", Result_Pass);
}



float WifiTestWidget::getSpeed(QString str)
{
	QList<float> list;
	float fl;
	QStringList list1 = str.split("  ");
	list1 = list1.filter("/sec");
	int pos = 0;

	QString ss = list1.join(",");
	qDebug() << ss;
	if (ss != "") {
		QRegExp rx("\\d+\\.\\d+|(\\d+)");  // 匹配数字
		while ((pos = rx.indexIn(ss, pos)) != -1) {
			list<< rx.cap(0).toFloat();
			pos += rx.matchedLength();
		}
		fl = list.at(0);
		if(ss.contains("Kbits")){
			fl = fl * 1024;
		}
		else if(ss.contains("Mbits")){
			fl = fl * 1024 * 1024;
		}
	}
	return fl;
}

void WifiTestWidget::upToWidget(float _min, float _max, float _average)
{
	QString min_tail="bits/sec";
	QString max_tail = "bits/sec";
	QString aver_tail = "bits/sec";
	if (_min > 1024){
		_min = _min / 1024;
		min_tail = "Kbits/sec";
	}
	if (_min > 1024) {
		_min = _min / 1024;
		min_tail = "Mbits/sec";
	}
	ui.minNum_label->setText(QString::number(_min) + min_tail);

	if (_max > 1024) {
		_max = _max / 1024;
		max_tail = "Kbits/sec";
	}
	if (_max > 1024) {
		_max = _max / 1024;
		max_tail = "Mbits/sec";
	}
	ui.maxNum_label->setText(QString::number(_max) + max_tail);

	if (_average > 1024) {
		_average = _average / 1024;
		aver_tail = "Kbits/sec";
	}
	if (_average > 1024) {
		_average = _average / 1024;
		aver_tail = "Mbits/sec";
	}
	ui.averageNum_label->setText(QString::number(_average) + aver_tail);
}
