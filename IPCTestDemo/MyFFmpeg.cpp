#include "MyFFmpeg.h"

#pragma comment(lib, "./ffmpeg/lib/avcodec.lib")
#pragma comment(lib, "./ffmpeg/lib/avdevice.lib")
#pragma comment(lib, "./ffmpeg/lib/avfilter.lib")
#pragma comment(lib, "./ffmpeg/lib/avformat.lib")
#pragma comment(lib, "./ffmpeg/lib/avutil.lib")
#pragma comment(lib, "./ffmpeg/lib/swscale.lib")
#include "Cpclog.h"

MyFFmpeg::MyFFmpeg(QObject *parent /*= nullptr*/)
{
	//MyFFmpegInfo();
	timeout_ = 3000;
}

MyFFmpeg::~MyFFmpeg()
{
	MyFFmpegDestroy();
}

void MyFFmpeg::MyFFmpegInfo()
{
	qDebug("------------------------------------------------------------------");
	qDebug("%s", avcodec_configuration());
	qDebug("version: %d", avcodec_version());
	qDebug("------------------------------------------------------------------");
}

int MyFFmpeg::MyFFmpegInit()
{
	int i;
	int ret = -1;

	// 获取视频播放URL
	QByteArray byteRtspUrl = m_rtspUrl.toLocal8Bit();
	char *pRtspUrl = byteRtspUrl.data();

	// 初始化所有组件，调用该函数后，才能使用复用器和编解码器
	//av_register_all();

	// 初始化网络库
	avformat_network_init();

	// 分配AVFormatContext，它是FFMPEG解封装（flv，mp4，rmvb，avi）功能的结构体，
	m_AVFormatContext = avformat_alloc_context();
	if (m_AVFormatContext == nullptr)
	{
		return -1;
	}
	m_AVFormatContext->interrupt_callback.callback = decode_interrupt_cb;
	m_AVFormatContext->interrupt_callback.opaque = this;
	// 设置参数
	AVDictionary *options = NULL;
// 	// 设置传输协议为TCP协议
// 	av_dict_set(&options, "rtsp_transport", "tcp", 0);
// 
// 	// 设置TCP连接最大延时时间
// 	av_dict_set(&options, "max_delay", "500000", 0);
// 
// 	// 设置“buffer_size”缓存容量
// 	av_dict_set(&options, "buffer_size", "1024000", 0);
// 
// 	// 设置avformat_open_input超时时间为3秒
// 	av_dict_set(&options, "stimeout", "3000000", 0);

//下面参数设置avformat_find_stream_info函数阻塞时间过长问题
	av_dict_set(&options, "probesize", "2048", 0);
	av_dict_set(&options, "max_analyze_duration", "1000", 0);

	RestTiemout();
	// 打开网络流或文件流
	ret = avformat_open_input(&m_AVFormatContext, pRtspUrl, NULL, &options);
	if (ret != 0)
	{
		qDebug("Couldn't open input stream, ret=%d\n", ret);
		return -1;
	}
	
	// 读取流数据包并获取流的相关信息
	if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0)
	{
		qDebug("Couldn't find stream information.\n");
		return -1;
	}
	

	// 确定流格式是否为视频
	for (i = 0; i < m_AVFormatContext->nb_streams; i++)
	{
		if (m_AVFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_videoIndex = i;
			break;
		}
	}

	if (m_videoIndex == -1)
	{
		qDebug("Didn't find a video stream.\n");
		return -1;
	}
	//m_AVCodecContext = m_AVFormatContext->streams[m_videoIndex]->codec;
	m_AVCodecContext = avcodec_alloc_context3(NULL);

	if (m_AVCodecContext == NULL)
	{
		printf("Could not allocate AVCodecContext \n");
		return -1;
	}
	if (m_AVFormatContext->streams[m_videoIndex]->codecpar->codec_type != AVMEDIA_TYPE_VIDEO) 
	{
		printf("Couldn't find audio stream information \n");
		return -1;
	}
	ret = avcodec_parameters_to_context(m_AVCodecContext, m_AVFormatContext->streams[m_videoIndex]->codecpar);
	if (ret < 0) {
		return -1;
	}
	m_AVCodecContext->pkt_timebase = m_AVFormatContext->streams[m_videoIndex]->time_base;
	// 根据编码器的ID号查找对应的解码器
	m_AVCodec = const_cast<AVCodec *>(avcodec_find_decoder(m_AVCodecContext->codec_id));
	//m_AVCodec = avcodec_find_decoder(m_AVCodecContext->codec_id);
	if (NULL == m_AVCodec)
	{
		qDebug("avcodec_find_decoder AV_CODEC_ID_H264 fail!\n");
		return -1;
	}

	// 配置编码器上下文的参数
// 	m_AVCodecContext->bit_rate = 20;         //码率
// 	m_AVCodecContext->time_base.den = 20;   // 下面2行设置帧率，每秒/30帧
// 	m_AVCodecContext->time_base.num = 1;
// 	m_AVCodecContext->frame_number = 1;     //每包一个视频帧

	// Initialize the AVCodecContext to use the given AVCodec
	if (avcodec_open2(m_AVCodecContext, m_AVCodec, NULL) < 0)
	{
		qDebug("avcodec_open2 fail");
		return -1;
	}

	// alloc AVFrame
	m_AVFrame = av_frame_alloc();
	m_AVFrameRGB = av_frame_alloc();
	// 图像色彩空间转换、分辨率缩放、前后图像滤波处理
	m_SwsContext = sws_getContext(m_AVCodecContext->width, m_AVCodecContext->height,
		m_AVCodecContext->pix_fmt, m_AVCodecContext->width, m_AVCodecContext->height,
		AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
	
	int bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32, m_AVCodecContext->width, m_AVCodecContext->height, 1);
	m_OutBuffer = (uint8_t *)av_malloc(bytes * sizeof(uint8_t));
	
	// 将分配的内存空间给m_AVFrameRGB使用
	av_image_fill_arrays(m_AVFrameRGB->data, m_AVFrameRGB->linesize, m_OutBuffer, AV_PIX_FMT_RGB32, m_AVCodecContext->width, m_AVCodecContext->height, 1);
	
	// 为AVPacket分别内存空间
	int packSize = m_AVCodecContext->width * m_AVCodecContext->height;
	m_AVPacket = (AVPacket *)malloc(sizeof(AVPacket));
	av_new_packet(m_AVPacket, packSize);

	qDebug("============== MyFFmpegInit ok! ====================== ");
	return 0;
}


void MyFFmpeg::MyFFmpegDestroy()
{
	av_free(m_OutBuffer);
	av_free(m_AVFrameRGB);
	av_frame_free(&m_AVFrame);
//	av_frame_free(&m_AVFrameRGB);
	sws_freeContext(m_SwsContext);
	av_packet_unref(m_AVPacket);
	free(m_AVPacket);
	avcodec_close(m_AVCodecContext);
	avformat_close_input(&m_AVFormatContext);
	avformat_free_context(m_AVFormatContext);
	avformat_network_deinit();
}

int MyFFmpeg::MyFFmpepReadFrame()
{
	int ret = -1;
	int getPicture = 0;
	// 获取下一帧数据
	RestTiemout();
	ret = av_read_frame(m_AVFormatContext, m_AVPacket);
	if (ret < 0)
	{
//		qDebug("av_read_frame fail!\n");
		return -1;
	}
	if (m_AVPacket->stream_index != m_videoIndex)
	{
		int m = m_AVPacket->stream_index;
		//av_packet_unref(m_AVPacket);
		return -1;
	}
	RestTiemout();

	//  解码m_AVPacket，Decode the video frame of size avpkt->size from avpkt->data into picture

// 	if (avcodec_send_packet(m_AVCodecContext, m_AVPacket) < 0 || (getPicture = avcodec_receive_frame(m_AVCodecContext, m_AVFrame)) < 0) {
// 		av_packet_unref(m_AVPacket);
// 		return -1; 
// 	}
	int re = avcodec_send_packet(m_AVCodecContext, m_AVPacket);
	av_packet_unref(m_AVPacket);
	if (re != 0) {
		return -1;
}
	re = avcodec_receive_frame(m_AVCodecContext, m_AVFrame);
	if (re != 0) {
		return -1;
	}
	// 判断是否已有视频帧被解码了
	if (getPicture == 0)
	{
#ifdef _DEBUG
		qDebug("# get frame: width=%d,height=%d,format=%d,key_frame=%d", m_AVFrame->width,
			m_AVFrame->height, m_AVFrame->format, m_AVFrame->key_frame);
#endif
		// 对解码视频帧进行缩放、格式转换等操作
		sws_scale(	m_SwsContext, 
					(uint8_t const * const *)m_AVFrame->data,
					m_AVFrame->linesize, 
					0,
					m_AVCodecContext->height,
					m_AVFrameRGB->data, 
					m_AVFrameRGB->linesize);

		// 转换到QImage
		QImage tmmImage(	(uchar *)m_OutBuffer, 
							m_AVCodecContext->width, 
							m_AVCodecContext->height, 
							QImage::Format_RGB32);
		QImage image = tmmImage.copy();
		//qDebug() << "numbytes" << image.size();
		//image.save("./" + QString("%1.jpg").arg(frameIndex++));
		// 发送QImage
		emit MyFFmpegSigGetOneFrame(image);
	}

	// 释放资源
	av_packet_unref(m_AVPacket);
	
	return 0;
}

int MyFFmpeg::decode_interrupt_cb(void *ctx)
{
	MyFFmpeg *rtsp_puser = (MyFFmpeg *)ctx;
	if (rtsp_puser->IsTimeout()) {
		CPCLOG_ERROR << "timeout:" << rtsp_puser->GetTimeout() << "ms";
		emit rtsp_puser->GetFramIsTimeOut();
		return 1;
	}
#ifdef _DEBUG
	qDebug("block time:%lld", rtsp_puser->GetBlockTime());
#endif
	return 0;
}

void MyFFmpeg::MyFFmpegSetUrl(QString rtspUrl)
{
	m_rtspUrl = rtspUrl;
}

bool MyFFmpeg::IsTimeout()
{
	DWORD SS = GetTickCount();
	DWORD MM = SS - pre_time_;
	if (MM > timeout_) {
		return true;    // 超时
	}
	return false;
}

void MyFFmpeg::SetTimeout(int time)
{
	timeout_ = time;
}

void MyFFmpeg::RestTiemout()
{
	pre_time_ = GetTickCount();        // 重置为当前时间
}

int MyFFmpeg::GetTimeout()
{
	return timeout_;
}

int64_t MyFFmpeg::GetBlockTime()
{
	return GetTickCount() - pre_time_;
}