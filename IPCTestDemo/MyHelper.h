#ifndef MYHELPER_H
#define MYHELPER_H

#endif // MYHELPER_H

#include <QObject>
#include <QListWidget>
#include <QSettings>
#include <QWidget>
#include <QDesktopWidget>
#include <QFile>
#include <QFileIconProvider>
#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QCoreApplication>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QCursor>
#include <QTimer>
//#include <QSound>
#include <QApplication>
#include <QStyleFactory>
#include <QTextStream>
#include <QDebug>

class MyHelper : public QObject
{
public:

    //设置全局为plastique样式
    static void SetPlastiqueStyle()
    {
        QApplication::setStyle(QStyleFactory::create("Plastique"));
        QApplication::setPalette(QApplication::style()->standardPalette());//替换应用程序的颜色组合
    }

#if 1
    //设置编码为GB2312
    static void SetGB2312Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("GB2312");
        QTextCodec::setCodecForLocale(codec);
        //QTextCodec::setCodecForCStrings(codec);       // 这两个函数在QT中已经不再支持了
        //QTextCodec::setCodecForTr(codec);
    }

    //设置编码为UTF8
    static void SetUTF8Code()
    {
        QTextCodec *codec=QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        //QTextCodec::setCodecForCStrings(codec);       // 这两个函数在QT中已经不再支持了
        //QTextCodec::setCodecForTr(codec);
    }
#endif

    // 设置界面全局字体
    static void SetFont(QApplication &app)
    {
        //设置全局字体
        QFont font = app.font();
        font.setPointSize(9);
        font.setFamily("微软雅黑");
        app.setFont(font);
    }

    //显示信息框，仅确定按钮
    static void ShowMessageBoxInfo(QString info)
    {
        QMessageBox msg;
        //msg.setStyleSheet("font:10pt '宋体'");
        msg.setStyleSheet("font:10pt '微软雅黑'");
        msg.setWindowTitle(tr("提示"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Information);
        msg.addButton(tr("确定"),QMessageBox::ActionRole);
        msg.exec();
    }

    //显示错误框，仅确定按钮
    static void ShowMessageBoxError(QString info)
    {
        QMessageBox msg;
        //msg.setStyleSheet("font:10pt '宋体'");
        msg.setStyleSheet("font:10pt '微软雅黑'");
        msg.setWindowTitle(tr("错误"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Critical);
        msg.addButton(tr("确定"),QMessageBox::ActionRole);
        msg.exec();
    }

    //显示询问框，确定和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        QMessageBox msg;
        //msg.setStyleSheet("font:10pt '宋体'");
        msg.setStyleSheet("font:10pt '微软雅黑'");
        msg.setWindowTitle(tr("询问"));
        msg.setText(info);
        msg.setIcon(QMessageBox::Question);
        msg.addButton(tr("确定"),QMessageBox::ActionRole);
        msg.addButton(tr("取消"),QMessageBox::RejectRole);

        return msg.exec();
    }

    //16进制字符串转字节数组
    static QByteArray HexStrToByteArray(QString str)
    {
        QByteArray senddata;
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; )
        {
            //hstr=str[i].toAscii();            // QT4 支持接口，qt5不再支持，需要使用toLatin1()接口
            hstr = str[i].toLatin1();
            if(hstr == ' ')
            {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
            // lstr = str[i].toAscii();
            lstr = str[i].toLatin1();
            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch)
    {
        if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
    }

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data)
    {
        QString temp="";
        QString hex=data.toHex();
        for (int i=0;i<hex.length();i=i+2)
        {
            temp+=hex.mid(i,2)+" ";
        }
        return temp.trimmed().toUpper();
    }

    // 字符串转16进制QString
    static QString CharArrayToHexStr(unsigned char *data, int len)
    {
        QString temp="";
        QString hex;
        for (int i=0;i<len;i++)
        {
            hex.sprintf("%02X", data[i]);
            temp+=hex+" ";
        }
        return temp.trimmed().toUpper();
    }

    //16进制字符串转10进制
    static uchar StrHexToDecimal(QString strHex)
    {
        bool ok;
        return strHex.toInt(&ok,16);
    }

    //10进制字符串转10进制
    static uchar StrDecimalToDecimal(QString strDecimal)
    {
        bool ok;
        return strDecimal.toInt(&ok,10);
    }

    //16进制字符串转2进制字符串
    static QString StrHexToStrBin(QString strHex)
    {
        uchar decimal=StrHexToDecimal(strHex);
        QString bin=QString::number(decimal,2);

        uchar len=bin.length();
        if (len<8)
        {
            for (int i=0;i<8-len;i++)
            {
                bin="0"+bin;
            }
        }

        return bin;
    }

    //计算校验码
    static uchar GetCheckCode(uchar data[],uchar len)
    {
        uchar temp=0;

        for (uchar i=0;i<len;i++)
        {
            temp+=data[i];
        }

        return temp%256;
    }

    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    //获取当前路径
    static QString GetCurrentPath()
    {
        return QString(QCoreApplication::applicationDirPath()+"/");
    }

#if 0
    //播放声音
    static void PlaySound(QString soundName)
    {
        QSound::play(soundName);
    }
#endif

    //获取当前日期时间星期
    static QString GetLongDateTime()
    {
        QDateTime time=QDateTime::currentDateTime();
        return time.toString("yyyy年MM月dd日 hh时mm分ss秒 dddd");
    }

    //获取当前时期时间值
    static QString GetCurrentDateTime()
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
    }

    //获取当前年月日
    static QString GetCurrnetDate()
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }

    // 获取当前时间戳
    static uint GetCurrentTimestamp()
    {
        QDateTime time = QDateTime::currentDateTime();   //获取当前时间
        return time.toTime_t();   //将当前时间转为时间戳
    }

    //打开网址
    static void OpenUrlOrExe(QString url)
    {
        QDesktopServices::openUrl(QUrl(url));
    }

    //判断是否闰年
    static bool IsLeapYear(QString strDate)
    {
        int year=strDate.left(4).toInt();
        if(((year % 4 ==0) && (year % 100 !=0 ))|| (year % 400 ==0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class FormHelper:public QObject
{
public:    

    //窗体居中显示
    static void FormInCenter(QWidget *frm)
    {
        QDesktopWidget desktop;
        int screenX=desktop.availableGeometry().width();
        int screenY=desktop.availableGeometry().height();
        int wndX=frm->width();
        int wndY=frm->height();
        QPoint movePoint(screenX/2-wndX/2,screenY/2-wndY/2);
        frm->move(movePoint);
    }

    //窗体没有最大化按钮
    static void FormNoMaxButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinimizeButtonHint);
    }

    //窗体没有最大化和最小化按钮
    static void FormOnlyCloseButton(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowMinMaxButtonsHint);
        frm->setWindowFlags(Qt::WindowCloseButtonHint);
    }

    //设置窗体透明度
    static void FormOpacity(QWidget *frm,double value)
    {
        frm->setWindowOpacity(value);
    }

    //窗体不能改变大小
    static void FormNotResize(QWidget *frm)
    {
        frm->setFixedSize(frm->width(),frm->height());
    }

    //窗体前端显示
    static void FormOnTop(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowStaysOnTopHint);
    }

    //窗体后端显示
    static void FormOnBottom(QWidget *frm)
    {
        frm->setWindowFlags(Qt::WindowStaysOnBottomHint);
    }

    //获取桌面大小
    static QSize GetDesktopSize()
    {
        QDesktopWidget desktop;
        return QSize(desktop.availableGeometry().width(),desktop.availableGeometry().height());
    }

    //设置指定窗体为plastique样式
    static void SetPlastiqueStyle(QWidget *frm)
    {
        frm->setStyle(QStyleFactory::create("Plastique"));
    }

    static void SetCustomStyle(QWidget *frm)
    {
        QString myStyle1="QPushButton {";
        myStyle1.append("border: 2px solid #8f8f91; ");
        myStyle1.append("border-radius: 6px;");
        myStyle1.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #f6f7fa, stop: 1 #dadbde); ");
        myStyle1.append("min-width: 80px;}");
        myStyle1.append("QPushButton:pressed {");
        myStyle1.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #dadbde, stop: 1 #f6f7fa); }");
        myStyle1.append("QPushButton:default  {");
        myStyle1.append("border-color: navy; /* 使得按钮显示变得更加突出 */}");

        QString myStyle2="QSlider::groove:horizontal {";
        myStyle2.append("border: 1px solid #999999;");
        myStyle2.append("height: 8px;");
        myStyle2.append("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);");
        myStyle2.append("margin: 2px 0;}");
        myStyle2.append("QSlider::handle:horizontal {");
        myStyle2.append("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);");
        myStyle2.append("border: 1px solid #5c5c5c;");
        myStyle2.append("width: 18px; margin: -2px 0;");
        myStyle2.append("border-radius: 3px;}");

        QString myStyle3="QGroupBox {";
        myStyle3.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,  stop: 0 #E0E0E0, stop: 1 #FFFFFF);");
        myStyle3.append("border: 2px solid gray;");
        myStyle3.append("border-radius: 5px;");
        myStyle3.append("margin-top: 1ex;}");
        myStyle3.append("QGroupBox::title {");
        myStyle3.append("subcontrol-origin: margin; ");
        myStyle3.append("subcontrol-position: top center;");
        myStyle3.append("padding: 0 3px;}");

        frm->setStyleSheet(myStyle1+myStyle2+myStyle3);
    }

    //改变样式
    static bool InstallStyle(QWidget* window,QString styleFile)
    {
        QFile file(styleFile);
        QString strStyleSheet;
        if (!file.open(QIODevice::WriteOnly))
        {
            strStyleSheet= "";
        }
        else
        {
            strStyleSheet=file.readAll();
        }
        file.close();

        if(strStyleSheet.isEmpty())
        {
            return false;
        }
        window->setStyleSheet(strStyleSheet);
        return true;
    }
};


class FileHelper : public QObject
{
public:    

    //获取选择文件
    static QString GetFileName(QString filter)
    {
        return QFileDialog::getOpenFileName(NULL,tr("选择文件"),QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择文件集合
    static QStringList GetFileNames(QString filter)
    {
        return QFileDialog::getOpenFileNames(NULL,tr("选择文件"),QCoreApplication::applicationDirPath(),filter);
    }

    //获取选择文件夹中的文件
    static QStringList GetFolderFileNames(QStringList filter)
    {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (!strFolder.length()==0)
        {
            QDir myFolder(strFolder);

            if (myFolder.exists())
            {
                fileList= myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //文件是否存在
    static bool FileExists(QString strFile)
    {
        QFile tempFile(strFile);
        if (tempFile.exists())
        {
            return true;
        }
        return false;
    }

    //文件夹是否存在
    static bool FolderExists(QString strFolder)
    {
        QDir tempFolder(strFolder);
        if(tempFolder.exists())
        {
            return true;
        }
        return false;
    }

    //移除文件
    static bool RemoveFile(QString strFile)
    {
        return QFile::remove(strFile);
    }

    //重命名文件
    static bool RenameFile(QString oldName,QString newName)
    {
        return QFile::rename(oldName,newName);
    }

    //获取文件目录
    static QString GetBasePath(QString strFullPath)
    {
        QFileInfo fileInfo(strFullPath);
        return fileInfo.path();
    }

    //获取拓展名
    static QString GetExtension(QString strFile)
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.suffix();
    }

    //获取文件夹名称
    static QString GetFolderName(QString strFullPath)
    {
        QString strPath(strFullPath);
        strPath.replace('\\','/');
        int pos=strPath.lastIndexOf('/');
        return strPath.mid(pos+1);
    }

    //复制文件
    static bool CopyFile(QString sourceFile, QString targetFile)
    {
        if (FileExists(targetFile))
        {
            int ret=QMessageBox::information(NULL,tr("提示"),tr("文件已经存在，是否替换？"),QMessageBox::Yes | QMessageBox::No);
            if (ret!=QMessageBox::Yes)
            {
                return false;
            }
        }
        return QFile::copy(sourceFile,targetFile);
    }

    //判断是否是文件
    static bool IsFile(QString strFile)
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.isFile();
    }

    //判断是否是文件夹
    static bool IsFolder(QString strFolder)
    {
        QFileInfo fileInfo(strFolder);
        return fileInfo.isDir();
    }

    //追加文本
    static bool AppendText(QString strFile, QString strContent )
    {
        QFile inFile(strFile);
        if (!inFile.open(QIODevice::Append | QIODevice::Text))
        {
            QMessageBox::information(NULL,tr("提示"),tr("打开文件出错！"));
            return false;
        }
        QTextStream inStream(&inFile);
        inStream<<strContent<<"\r\n";
        inFile.close();
        return true;
    }

    //创建文件夹
    static bool CreateFolder(QString strFolder)
    {
        QDir dir;
        return dir.mkdir(strFolder);
    }

    //创建文件
    static bool CreateFile(QString strFile)
    {
        QFile file(strFile);
        if (!file.open(QIODevice::WriteOnly))
        {
            return false;
        }
        file.close();
        return true;
    }

    //读取文件内容
    static QString GetFileContent(QString strFile)
    {
        QFile file(strFile);
        QString txt;
        if (!file.open(QIODevice::WriteOnly))
        {
            txt= "";
        }
        else
        {
            txt=file.readAll();
        }
        file.close();
        return txt;
    }

    //获取文件行数
    static int GetFileLineNum(QString &strFile)
    {
        int lineNum = 0;
        QFile file(strFile);
        if (file.open(QFile::ReadOnly | QIODevice::Text))
        {
            QTextStream data(&file);
            QString line;
            while (!data.atEnd())
            {
                line = data.readLine();
                if (line.length() > 0) {
                    lineNum++;
                }
            }

            file.close();
        }

        return lineNum;
    }

    //获取文件的创建时间
    static int GetFileCreateTime(QString &strFile, QDateTime &dateTime)
    {
        int retValue = 0;
        QFileInfo info(strFile);
        if (info.exists()) {
            dateTime = info.created();
        } else {
            retValue = -1;
        }

        return retValue;
    }

};

typedef unsigned short int uint16;
typedef unsigned long int uint32;
// 短整型大小端互换
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | (((uint16)(A) & 0x00ff) << 8))
// 长整型大小端互换
#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | (((uint32)(A) & 0x00ff0000) >> 8) | (((uint32)(A) & 0x0000ff00) << 8) | (((uint32)(A) & 0x000000ff) << 24))

class NetHelper : public QObject
{
public:
    static int checkCPUendian()
    {
          union{
                 unsigned long int i;
                 unsigned char s[4];
          }c;

          c.i = 0x12345678;
          return (0x12 == c.s[0]);
    }

    static unsigned long Htonl(unsigned long h)
    {
        return checkCPUendian() ? h : BigLittleSwap32(h);
    }

    static unsigned long Ntohl(unsigned long n)
    {
        return checkCPUendian() ? n : BigLittleSwap32(n);
    }

    static unsigned short Htons(unsigned short h)
    {
        return checkCPUendian() ? h : BigLittleSwap16(h);
    }

    static unsigned short Ntohs(unsigned short n)
    {
        return checkCPUendian() ? n : BigLittleSwap16(n);
    }
};
