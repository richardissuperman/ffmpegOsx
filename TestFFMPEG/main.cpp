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
#include <SDL2/SDL.h>
};

int main(int argc, const char * argv[]) {
    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    AVFrame	*pFrame,*pFrameYUV;
    uint8_t *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret, got_picture;
    struct SwsContext *img_convert_ctx;
    // ‰»ÎŒƒº˛¬∑æ∂
    char filepath[]="/Users/zhongqing/Documents/Singing/Mo.mp4";
    
    int frame_cnt;
    
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    
    
    //init SDL
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("error!!!! init SDL");
    }
    else{
        printf("Success!!!! init SDL");
    }
    
    printf("version %s \n",LIBAVCODEC_IDENT);
    printf("AAC codec name %s.\n",avcodec_find_decoder_by_name("aac")->name);

    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return -1;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        return -1;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        printf("Didn't find a video stream.\n");
        return -1;
    }
    
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return -1;
    }
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return -1;
    }
    /*
     * ‘⁄¥À¥¶ÃÌº” ‰≥ˆ ”∆µ–≈œ¢µƒ¥˙¬Î
     * »°◊‘”⁄pFormatCtx£¨ π”√fprintf()
     */
    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    out_buffer=(uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx,0,filepath,0);
    printf("-------------------------------------------------\n");
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    
    frame_cnt=0;
    
    FILE *file = fopen("/Users/zhongqing/Desktop/test.h264", "wb+");
    FILE *yuvfile = fopen("/Users/zhongqing/Desktop/yuv.yuv", "wb+");

    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==videoindex){
            /*
             * ‘⁄¥À¥¶ÃÌº” ‰≥ˆH264¬Î¡˜µƒ¥˙¬Î
             * »°◊‘”⁄packet£¨ π”√fwrite()
             */
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            fwrite(packet->data, 1, packet->size, file);
            if(ret < 0){
                printf("Decode Error.\n");
                return -1;
            }
            if(got_picture){
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                          pFrameYUV->data, pFrameYUV->linesize);
                printf("Decoded frame index: %d\n",frame_cnt);
                fwrite(pFrame->data[0], 1, pCodecCtx->width*pCodecCtx->height, yuvfile);

                /*
                 * ‘⁄¥À¥¶ÃÌº” ‰≥ˆYUVµƒ¥˙¬Î
                 * »°◊‘”⁄pFrameYUV£¨ π”√fwrite()
                 */
                
                frame_cnt++;
                
            }
        }
        av_free_packet(packet);
    }
    
    sws_freeContext(img_convert_ctx);
    fclose(file);
    fclose(yuvfile);
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    
    return 0;
}
