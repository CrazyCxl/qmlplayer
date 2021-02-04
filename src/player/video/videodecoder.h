#ifndef VIDEODECODER_H
#define VIDEODECODER_H

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class VideoDecoder
{
public:
    VideoDecoder(unsigned int stream_index,AVCodecParameters *avcodec_param);
    ~VideoDecoder();

    AVCodecContext *getAvCodecContext() const;

    int getStreamIndex() const;

private:
    AVCodecContext *avCodecContext;
    unsigned int streamIndex;
};

#endif // VIDEODECODER_H
