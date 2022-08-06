#ifndef RTSP_PLAYER_H
#define RTSP_PLAYER_H

#include "MyFFmpeg.h"
#include <QObject>


class RTSP_Player : public QObject
{
	Q_OBJECT
public:
	enum PlayerState {
		FFmpegInitFail = 0,
		FFmpegInitSucc,
		FFmpegStoped
	};

	explicit RTSP_Player(MyFFmpeg *ffmpeg = nullptr, QObject *parent = nullptr);
	void SetPlayerUrl(QString playerUrl);

signals:
	void SigOpenUrlResult(int result);

public slots:
	void PlayerStart();
	void PlayerStop();

private:
	volatile bool   m_stopped;				// ֹͣ���ű�ʶ��Ϊtrueʱֹͣ���ţ��˳�����ѭ��
	QString         m_playerUrl;			// ����url
	MyFFmpeg        *m_ffmpeg;				// MyFFmepgָ��
};

#endif // RTSP_PLAYER_H
