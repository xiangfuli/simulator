#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include <vector>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>

class VideoEncoder {
  private:
    GstElement *pipeline;
    GstElement *appsink;
    int width;
    int height;
    int fps;
  
  public:
    VideoEncoder(int width, int height, int fps = 30): width(width), height(height), fps(fps) {
        // initialize GStreamer
        gst_init(nullptr, nullptr);

        // pipeline construction
        std::string pipeline_str = 
            "appsrc name=source ! videoconvert ! "
            "x264enc "
            "tune=zerolatency " // 低延迟设置
            "speed-preset=superfast " // 编码速度
            "bitrate=2000 " // 比特率(kbps)
            "key-int-max=30 " // 关键帧间隔
            "bframes=0 " // 禁用B帧，减少延迟
            "ref=1 " // 参考帧数量
            "cabac=false " // 使用CAVLC而不是CABAC
            "threads=4 " // 线程数
            "sliced-threads=true " // 启用切片线程
            "aud=false " // 禁用访问单元分隔符
            "! video/x-h264,profile=baseline,level=(string)3.1,"
            "stream-format=(string)byte-stream,"
            "alignment=(string)au ! "
            "appsink name=sink sync=false";
            
        // 创建管道
        GError *error = nullptr;
        pipeline = gst_parse_launch(pipeline_str.c_str(), &error);
        if (error) {
            std::string error_msg = error->message;
            g_error_free(error);
            throw std::runtime_error("Failed to create pipeline: " + error_msg);
        }

        // 获取appsink元素
        appsink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

        // 设置appsrc属性
        GstElement *appsrc = gst_bin_get_by_name(GST_BIN(pipeline), "source");
        gst_app_src_set_caps(GST_APP_SRC(appsrc),
            gst_caps_new_simple("video/x-raw",
                "format", G_TYPE_STRING, "BGR",
                "width", G_TYPE_INT, width,
                "height", G_TYPE_INT, height,
                "framerate", GST_TYPE_FRACTION, fps, 1,
                nullptr));
        gst_object_unref(appsrc);

        // 启动管道
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }   

    ~VideoEncoder() {
        if (pipeline) {
            gst_element_set_state(pipeline, GST_STATE_NULL);
            gst_object_unref(pipeline);
        }
        if (appsink) {
            gst_object_unref(appsink);
        }
    }
  
    bool write(std::vector<uint8_t>& frame, std::vector<uint8_t>& encoded_data) {
        if (frame.empty()) return false;

        // 将帧推送到管道
        GstBuffer *buffer = gst_buffer_new_allocate(nullptr, 
            frame.size(), nullptr);
        GstMapInfo map;
        gst_buffer_map(buffer, &map, GST_MAP_WRITE);
        memcpy(map.data, frame.data(), frame.size());
        gst_buffer_unmap(buffer, &map);

        GstElement *appsrc = gst_bin_get_by_name(GST_BIN(pipeline), "source");
        gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer);
        gst_object_unref(appsrc);

        // 从appsink获取编码后的数据
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
        if (sample) {
            GstBuffer *buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                encoded_data.resize(map.size);
                memcpy(encoded_data.data(), map.data, map.size);
                gst_buffer_unmap(buffer, &map);
            }
            gst_sample_unref(sample);
            return true;
        }

        return false;
    }
  
};

#endif /* VIDEOENCODER_H */
