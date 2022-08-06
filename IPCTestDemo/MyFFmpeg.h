#pragma once
#include <QString>
#include <QObject>
#include <QImage>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
}
class MyFFmpeg:public QObject
{
	Q_OBJECT
public:
	explicit MyFFmpeg(QObject *parent = nullptr);
	~MyFFmpeg();

	void MyFFmpegSetUrl(QString rtspUrl);
	void MyFFmpegInfo();

	int MyFFmpegInit();
	void MyFFmpegDestroy();
	int MyFFmpepReadFrame();
signals:
	void MyFFmpegSigGetOneFrame(QImage img);
private:
	int              m_videoIndex;
	QString          m_rtspUrl;
	//AVPicture        m_AVPicture;
	AVCodec         *m_AVCodec;
	AVFormatContext *m_AVFormatContext;
	AVCodecContext  *m_AVCodecContext;
	AVFrame         *m_AVFrame;
	AVFrame         *m_AVFrameRGB;
	AVPacket        *m_AVPacket;
	SwsContext      *m_SwsContext;
	uint8_t         *m_OutBuffer;
	int frameIndex;
};


