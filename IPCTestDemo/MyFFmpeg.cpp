#include "MyFFmpeg.h"

#pragma comment(lib, "./ffmpeg/lib/avcodec.lib")
#pragma comment(lib, "./ffmpeg/lib/avdevice.lib")
#pragma comment(lib, "./ffmpeg/lib/avfilter.lib")
#pragma comment(lib, "./ffmpeg/lib/avformat.lib")
#pragma comment(lib, "./ffmpeg/lib/avutil.lib")
#pragma comment(lib, "./ffmpeg/lib/swscale.lib")

MyFFmpeg::MyFFmpeg(QObject *parent /*= nullptr*/)
{
	MyFFmpegInfo();
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

	// ��ȡ��Ƶ����URL
	QByteArray byteRtspUrl = m_rtspUrl.toLocal8Bit();
	char *pRtspUrl = byteRtspUrl.data();

	// ��ʼ��������������øú����󣬲���ʹ�ø������ͱ������
	//av_register_all();

	// ��ʼ�������
	avformat_network_init();

	// ����AVFormatContext������FFMPEG���װ��flv��mp4��rmvb��avi�����ܵĽṹ�壬
	// ����ɲο�https://blog.csdn.net/leixiaohua1020/article/details/14214705
	m_AVFormatContext = avformat_alloc_context();

	// ���ò���
	AVDictionary *options = NULL;
	// ���ô���Э��ΪTCPЭ��
	av_dict_set(&options, "rtsp_transport", "udp", 0);

	// ����TCP���������ʱʱ��
	av_dict_set(&options, "max_delay", "100", 0);

	// ���á�buffer_size����������
	av_dict_set(&options, "buffer_size", "1024000", 0);

	// ����avformat_open_input��ʱʱ��Ϊ3��
	av_dict_set(&options, "stimeout", "3000000", 0);

	// �����������ļ���
	ret = avformat_open_input(&m_AVFormatContext, pRtspUrl, NULL, &options);
	if (ret != 0)
	{
		qDebug("Couldn't open input stream, ret=%d\n", ret);
		return -1;
	}

	// ��ȡ�����ݰ�����ȡ���������Ϣ
	if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0)
	{
		qDebug("Couldn't find stream information.\n");
		return -1;
	}

	// ȷ������ʽ�Ƿ�Ϊ��Ƶ
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
	if (m_AVFormatContext->streams[m_videoIndex]->codecpar->codec_type != AVMEDIA_TYPE_VIDEO) {
		printf("Couldn't find audio stream information \n");
		return -1;
	}
	ret = avcodec_parameters_to_context(m_AVCodecContext, m_AVFormatContext->streams[m_videoIndex]->codecpar);
	if (ret < 0) {
		return -1;
	}
	m_AVCodecContext->pkt_timebase = m_AVFormatContext->streams[m_videoIndex]->time_base;
	// ���ݱ�������ID�Ų��Ҷ�Ӧ�Ľ�����
	m_AVCodec = const_cast<AVCodec *>(avcodec_find_decoder(m_AVCodecContext->codec_id));
	//m_AVCodec = avcodec_find_decoder(m_AVCodecContext->codec_id);
	if (NULL == m_AVCodec)
	{
		qDebug("avcodec_find_decoder AV_CODEC_ID_H264 fail!\n");
		return -1;
	}

	// ���ñ����������ĵĲ���
	m_AVCodecContext->bit_rate = 0;         //����
	m_AVCodecContext->time_base.den = 30;   // ����2������֡�ʣ�ÿ��/30֡
	m_AVCodecContext->time_base.num = 1;
	m_AVCodecContext->frame_number = 1;     //ÿ��һ����Ƶ֡

	// Initialize the AVCodecContext to use the given AVCodec
	if (avcodec_open2(m_AVCodecContext, m_AVCodec, NULL) < 0)
	{
		qDebug("avcodec_open2 fail");
		return -1;
	}

	// alloc AVFrame
	m_AVFrame = av_frame_alloc();
	m_AVFrameRGB = av_frame_alloc();

	// ͼ��ɫ�ʿռ�ת�����ֱ������š�ǰ��ͼ���˲�����
	m_SwsContext = sws_getContext(m_AVCodecContext->width, m_AVCodecContext->height,
		m_AVCodecContext->pix_fmt, m_AVCodecContext->width, m_AVCodecContext->height,
		AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

	int bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB32, m_AVCodecContext->width, m_AVCodecContext->height, 1);
	m_OutBuffer = (uint8_t *)av_malloc(bytes * sizeof(uint8_t));

	// ��������ڴ�ռ��m_AVFrameRGBʹ��
	av_image_fill_arrays(m_AVFrameRGB->data, m_AVFrameRGB->linesize, m_OutBuffer, AV_PIX_FMT_RGB32, m_AVCodecContext->width, m_AVCodecContext->height, 1);

	// ΪAVPacket�ֱ��ڴ�ռ�
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

	// ��ȡ��һ֡����
	ret = av_read_frame(m_AVFormatContext, m_AVPacket);
	if (ret < 0)
	{
		qDebug("av_read_frame fail!\n");
		return -1;
	}

	if (m_AVPacket->stream_index != m_videoIndex)
	{
		av_packet_unref(m_AVPacket);
		return 0;
	}


	//  ����m_AVPacket��Decode the video frame of size avpkt->size from avpkt->data into picture
	//ret = avcodec_decode_video2(m_AVCodecContext, m_AVFrame, &getPicture, m_AVPacket);
	if (avcodec_send_packet(m_AVCodecContext, m_AVPacket) < 0 || (getPicture = avcodec_receive_frame(m_AVCodecContext, m_AVFrame)) < 0) {
		av_packet_unref(m_AVPacket);
		return -1; 
	}
// 	if (ret < 0)
// 	{
// 		qDebug("avcodec_decode_video2 fail!\n");
// 		av_free_packet(m_AVPacket);
// 		return 0;
// 	}

	// got_picture_ptr Zero if no frame could be decompressed, otherwise, it is nonzero.
	// �ж��Ƿ�������Ƶ֡��������
	if (getPicture == 0)
	{
#if 0
		qDebug("# get frame: width=%d,height=%d,format=%d,key_frame=%d", m_AVFrame->width,
			m_AVFrame->height, m_AVFrame->format, m_AVFrame->key_frame);
#endif
		// �Խ�����Ƶ֡�������š���ʽת���Ȳ���
		sws_scale(m_SwsContext, (uint8_t const * const *)m_AVFrame->data,
			m_AVFrame->linesize, 0, m_AVCodecContext->height,
			m_AVFrameRGB->data, m_AVFrameRGB->linesize);

		// ת����QImage
		QImage tmmImage((uchar *)m_OutBuffer, m_AVCodecContext->width, m_AVCodecContext->height, QImage::Format_RGB32);
		QImage image = tmmImage.copy();
		//qDebug() << "numbytes" << image.size();
		//image.save("./" + QString("%1.jpg").arg(frameIndex++));
		// ����QImage
		emit MyFFmpegSigGetOneFrame(image);
	}

	// �ͷ���Դ
	av_packet_unref(m_AVPacket);

	return 0;
}

void MyFFmpeg::MyFFmpegSetUrl(QString rtspUrl)
{
	m_rtspUrl = rtspUrl;
}