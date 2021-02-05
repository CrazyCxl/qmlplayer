#include "player.h"
#include "../utils/logger.h"
#include <thread>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

Player::Player()
{
    //    open( "C:\\Users\\cxl\\Videos\\VID_20200416_211528.mp4");
}

Player::~Player()
{
}

int Player::open(const std::string &path)
{
    pFormatContext = avformat_alloc_context();
    int ret = 0;

    if((ret = avformat_open_input(&pFormatContext,path.c_str(), NULL, NULL))){
        logger()<<"avformat_open_input failed "<<ret;
        return ret;
    }

    printf("Format %s, duration %lld us", pFormatContext->iformat->long_name, pFormatContext->duration);
    long secs = pFormatContext->duration/AV_TIME_BASE;
    long mins = secs/60;
    logger()<<mins<<"分"<< secs%60<<"秒";

    av_dump_format(pFormatContext,0,path.c_str(),0);

    //To access the streams
    if((ret = avformat_find_stream_info(pFormatContext,  NULL))){
        logger()<<"avformat_open_input failed "<<ret;
        return ret;
    }

    for (unsigned int i = 0; i < pFormatContext->nb_streams ; i++)
    {
        AVCodecParameters *pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoDecoder = make_unique<VideoDecoder>(i,pLocalCodecParameters);
        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            printf("Audio Codec: %d channels, sample rate %d", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
            audioDecoder = make_unique<VideoDecoder>(i,pLocalCodecParameters);
        }
    }

    readFThread = make_unique<thread>(&Player::readframe, this);
    readFThread->detach();
    return 0;
}

void Player::setCallBack(PlayerCallBack *value)
{
    callBack = value;
}

void Player::readframe()
{
    AVPacket *pPacket = av_packet_alloc();
    AVFrame *pFrame = av_frame_alloc();

    int width = videoDecoder->getAvCodecContext()->width;
    int height = videoDecoder->getAvCodecContext()->height;

    SwsContext *sws_conotext =  sws_getContext(width,height,
                                               videoDecoder->getAvCodecContext()->pix_fmt,
                                               width,height,AV_PIX_FMT_RGB24,SWS_BICUBIC,NULL,NULL,NULL);
    AVFrame *yuvFrame = av_frame_alloc();

    int size = av_image_get_buffer_size(AV_PIX_FMT_RGB24,width,height,1);
    uint8_t *buff = (uint8_t *)av_malloc(size);
    av_image_fill_arrays(yuvFrame->data,yuvFrame->linesize,buff,AV_PIX_FMT_RGB24,width,height,1);
    yuvFrame->width = width;
    yuvFrame->height = height;
    int16_t* outputBuffer = NULL;
    while (av_read_frame(pFormatContext, pPacket) == 0) {
        if(pPacket->stream_index == videoDecoder->getStreamIndex()){
            if(avcodec_send_packet(videoDecoder->getAvCodecContext(), pPacket)){
                logger()<<"error send packet!";
                continue;
            }
            if(!avcodec_receive_frame(videoDecoder->getAvCodecContext(), pFrame)){
                sws_scale(sws_conotext,pFrame->data,pFrame->linesize,0,height,yuvFrame->data,yuvFrame->linesize);
                if(callBack)
                    callBack->rowVideoData(yuvFrame->data[0],yuvFrame->width,yuvFrame->height);
            }
        }else if(pPacket->stream_index == audioDecoder->getStreamIndex()){
            if(avcodec_send_packet(audioDecoder->getAvCodecContext(), pPacket)){
                logger()<<"error send packet!";
                continue;
            }
            if(!avcodec_receive_frame(audioDecoder->getAvCodecContext(), pFrame)){
                int in_samples = pFrame->nb_samples;
                int i=0;
                float* inputChannel0 = (float*)pFrame->extended_data[0];
                if(!outputBuffer)
                    outputBuffer = (int16_t*)av_malloc(pFrame->nb_samples * pFrame->channels*2);
                // Mono
                if (pFrame->channels==1) {
                    for (i=0 ; i<in_samples ; i++) {
                        float sample = *inputChannel0++;
                        if (sample<-1.0f) sample=-1.0f; else if (sample>1.0f) sample=1.0f;
                        outputBuffer[i] = (unsigned char) (sample * 32767.0f);
                    }
                }
                // Stereo
                else {
                    float* inputChannel1 = (float*)pFrame->extended_data[1];
                    for (i=0 ; i<in_samples ; i++) {
                         outputBuffer[i*2] = (int16_t) ((*inputChannel0++) * 32767.0f);
                         outputBuffer[i*2+1] = (int16_t) ((*inputChannel1++) * 32767.0f);
                    }
                }
                if(callBack)
                    callBack->rowAudioData((unsigned char*)outputBuffer,pFrame->nb_samples * pFrame->channels*2);
            }
        }
        av_packet_unref(pPacket);
    }

    sws_freeContext(sws_conotext);
    av_frame_free(&yuvFrame);
    avformat_close_input(&pFormatContext);
}
