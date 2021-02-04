#include "videodecoder.h"

VideoDecoder::VideoDecoder(unsigned int stream_index, AVCodecParameters *avcodec_param)
    :streamIndex(stream_index)
{
    printf("Video Codec: resolution %d x %d\n", avcodec_param->width, avcodec_param->height);
    AVCodec *pLocalCodec = avcodec_find_decoder(avcodec_param->codec_id);
    avCodecContext = avcodec_alloc_context3(pLocalCodec);
    avcodec_parameters_to_context(avCodecContext, avcodec_param);
    avcodec_open2(avCodecContext, pLocalCodec, NULL);
}

VideoDecoder::~VideoDecoder()
{
    avcodec_close(avCodecContext);
    avcodec_free_context(&avCodecContext);
}

AVCodecContext *VideoDecoder::getAvCodecContext() const
{
    return avCodecContext;
}

int VideoDecoder::getStreamIndex() const
{
    return streamIndex;
}

