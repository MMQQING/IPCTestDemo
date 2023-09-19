#include "RTSP_Player.h"
#include <QThread>

RTSP_Player::RTSP_Player(MyFFmpeg *ffmpeg, QObject *parent)
	: QObject(parent)
	, m_ffmpeg(ffmpeg)
{
	connstate = true;
	bindSinalSlot();
}

void RTSP_Player::SetPlayerUrl(QString playerUrl)
{
	this->m_playerUrl = playerUrl;
}

void RTSP_Player::bindSinalSlot()
{
	bool bl = connect(m_ffmpeg, SIGNAL(GetFramIsTimeOut()), this, SLOT(Slots_FramTimeout()));
}

void RTSP_Player::PlayerStart()
{
	if (m_ffmpeg == NULL) {
		emit SigOpenUrlResult(RTSP_Player::FFmpegInitFail);
		return;
	}

	m_ffmpeg->MyFFmpegSetUrl(this->m_playerUrl);
	int ret = m_ffmpeg->MyFFmpegInit();
	if (ret != 0) {
		emit SigOpenUrlResult(RTSP_Player::FFmpegInitFail);
		return;
	}

	emit SigOpenUrlResult(RTSP_Player::FFmpegInitSucc);

	m_stopped = false;

	while (1) {
		if (m_stopped) {
			qDebug("--------------- stop ----------------");
			break;
		}

		if (!m_ffmpeg) {
			break;
		}

		if (m_ffmpeg->MyFFmpepReadFrame() < 0) {
			qDebug("--------------- get frame fail, stop -----------");
			continue;
		}
		QThread::msleep(10);
	}

	m_ffmpeg->MyFFmpegDestroy();
	emit SigOpenUrlResult(RTSP_Player::FFmpegStoped);
	qDebug("---------------- quit -----------------");

	return;
}

void RTSP_Player::PlayerStop()
{
	m_stopped = true;
}

void RTSP_Player::Slots_FramTimeout()
{
	emit Sig_FramTimeOut();
}
