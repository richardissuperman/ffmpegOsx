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
    av_register_all();
    
    AVFormatContext	*pFormatCtx;
    pFormatCtx = avformat_alloc_context();

    
    const char  *filenameSrc = "/Users/zhongqing/Documents/Singing/Mo.mp4";
    
    int result  = avformat_open_input(&pFormatCtx,filenameSrc,NULL,NULL)!=0;
    
    
    if(result<0){
        printf("\n Couldn't open input stream.   error code %d \n",result);
        return -1;
    }
    
    printf("\n Time is %d \n",pFormatCtx->duration);
    
    return 0;
}
