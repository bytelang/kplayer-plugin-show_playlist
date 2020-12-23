//
// Created by karaler on 2020/12/23.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2020 Bytelang . All rights reserved.
//

#include "KPVideoShowPlayListPlugin.h"

KPVideoShowPlayListPlugin::KPVideoShowPlayListPlugin(const std::string &identify_name, const std::string &filter_name, const KPFilterType &filter_type, PluginParamsObject plugin_params_object) : KPPluginAdapter(identify_name, filter_name, filter_type, plugin_params_object) {


    if (plugin_params_object.params.find("font_size") != plugin_params_object.params.end()) {
        font_size = plugin_params_object.params["font_size"];
    }
    if (plugin_params_object.params.find("font_color") != plugin_params_object.params.end()) {
        font_color = plugin_params_object.params["font_color"];
    }
    if (plugin_params_object.params.find("x") != plugin_params_object.params.end()) {
        x = plugin_params_object.params["x"];
    }
    if (plugin_params_object.params.find("y") != plugin_params_object.params.end()) {
        y = plugin_params_object.params["y"];
    }
    if (plugin_params_object.params.find("line_height") != plugin_params_object.params.end()) {
        line_height = plugin_params_object.params["line_height"];
    }
    if (plugin_params_object.params.find("show_extension") != plugin_params_object.params.end()) {
        show_extension = plugin_params_object.params["show_extension"];
    }

    // 赋值described
    std::stringstream filter_desc_stream;
    filter_desc_stream << "x=" << x
                       << ":y=" << y << ":fontfile=res/font.ttf:line_spacing=" << line_height << ":fontsize=" << font_size << ":fontcolor=" << font_color << ":text='" << "[无]" << "'";
    filter_desc = filter_desc_stream.str();

    // 查找过滤器
    filter = avfilter_get_by_name("drawtext");
    if (filter == nullptr)
        throw KPFilterException("未知过滤器; filter name: drawtext");
}

KPVideoShowPlayListPlugin::~KPVideoShowPlayListPlugin() {
    global_event_play_list_updated.Unsubscribe(event_id);
}

void KPVideoShowPlayListPlugin::Task() {
    // 监听下一次文件播放更改
    event_id = global_event_play_list_updated.Subscribe([&](const bool &status) {
        logger->debug("接收到事件通知，获取到当前播放列表");
        auto play_list = kplayer_plugin_driver->PluginDriverGetPlayList();
        ChangeTitle(play_list);
    });
}

void KPVideoShowPlayListPlugin::KillTask() {
}

void KPVideoShowPlayListPlugin::InitTask() {
    // 初始化当前文件名
    auto play_list = kplayer_plugin_driver->PluginDriverGetPlayList();
    logger->debug("已获取到当前播放列表");
    ChangeTitle(play_list);
}

int KPVideoShowPlayListPlugin::ChangeTitle(const std::vector<std::string> &play_list) {
    AVDictionary *dict = nullptr;

    void *priv            = GetFilterPriv();
    if (!priv) {
        logger->warn("无法对plugin option进行设置; error: {}", "priv为空");
        return -1;
    }

    // 构建播放列表
    std::stringstream play_list_str;
    int               num = 1;
    for (const auto   &item : play_list) {
        KPlayer::FileInfo file_info(item);
        std::string       title = show_extension == "1" ? file_info.GetFileName() : file_info.GetBaseFileName();

        play_list_str << num << ". ";
        play_list_str << title << std::endl;
        num++;
    }

    av_dict_set(& dict, "text", play_list_str.str().c_str(), 0);
    int ret = av_opt_set_dict(priv, & dict);
    if (ret < 0) {
        logger->error("无法对plugin option进行设置;");
        return ret;
    }
    return 0;
}

KPLAYER_PLUGIN_FUNC(KPVideoShowPlayListPlugin) {
    return new KPVideoShowPlayListPlugin("kplayer", "video_plugin_show_play_list", KP_FILTER_TYPE_VIDEO, std::move(plugin_params));
}

