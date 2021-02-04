#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <functional>
#include <share.h>
#include "video/videodecoder.h"
#include "../utils/logger.h"

using namespace std;

class PlayerCallBack
{
public:
    virtual void rowVideoData(unsigned char *data,int width,int height) = 0;
};

class Player
{
public:
    Player();
    int open(const std::string &path);

    void setCallBack(PlayerCallBack *value);

private:
    void readframe();

private:
    unique_ptr<VideoDecoder> videoDecoder;
    PlayerCallBack *callBack;
    AVFormatContext *pFormatContext;
};

#endif // PLAYER_H
