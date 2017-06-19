//
//  main.cpp
//  TestFFMPEG
//
//  Created by ZhongQing on 16/06/17.
//  Copyright © 2017年 ZhongQing. All rights reserved.
//

#include <iostream>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
};

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("%s",avcodec_configuration());
    avcodec_register_all();
    avformat_network_init();
    return 0;
}
