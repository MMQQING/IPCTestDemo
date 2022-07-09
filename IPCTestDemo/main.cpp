#include "IPCTestDemo.h"
#include "Login.h"
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QFontDatabase>
#include <QTextCodec>
#include <QMessageBox>
#include <QtWidgets/QApplication>
#include "AddTestItemDialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QFontDatabase::addApplicationFont(":/IPCTestDemo/Resources/fontawesome-webfont.ttf");
	QString fontName = QFontDatabase::applicationFontFamilies(0).at(0);
	QFont font(fontName);
	font.setPointSize(9);
	font.setFamily(QStringLiteral("Î¢ÈíÑÅºÚ"));
	QApplication::setFont(font);

	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QSystemSemaphore sema("a", 1, QSystemSemaphore::Open);
	sema.acquire();
	QSharedMemory mem("b");
	if (!mem.create(1))
	{
		QMessageBox::information(0, QObject::tr("Tip"), QObject::tr("Program has been running!"));
		sema.release();
		return 0;
	}
	sema.release();

	Login w;
    //IPCTestDemo w;
	//AddTestItemDialog w;
    w.show();
    return a.exec();
}
