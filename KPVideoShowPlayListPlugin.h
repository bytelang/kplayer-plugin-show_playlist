//
// Created by karaler on 2020/12/23.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2020 Bytelang . All rights reserved.
//

#ifndef KPLAYER_KPVideoShowPlayListPlugin_H
#define KPLAYER_KPVideoShowPlayListPlugin_H

#include "adapter/KPPluginAdapter.h"
#include "util/KPHelper.h"
#include "config.h"

#include <utility>

extern std::shared_ptr<KPPluginDriver> kplayer_plugin_driver;
extern KPEvent<bool>                   global_event_play_list_updated;

class KPVideoShowPlayListPlugin : public KPPluginAdapter {
protected:
    std::string font_size      = "17";
    std::string font_color     = "white";
    std::string x              = "10";
    std::string y              = "10";
    std::string line_height    = "3";
    std::string show_extension = "0";
    std::string count          = "10";
    uint64_t    event_id       = -1;
    void Task() override;
    bool        stop           = false;
protected:
    int ChangeTitle(const std::vector<std::string> &);
public:
    explicit KPVideoShowPlayListPlugin(const std::string &identify_name, const std::string &filter_name, const KPFilterType &filter_type, PluginParamsObject plugin_params_object);
    ~KPVideoShowPlayListPlugin() override;
    void KillTask() override;
    void InitTask() override;
};

// slot
extern "C" {
KPLAYER_PLUGIN_FUNC(KPVideoShowPlayListPlugin);
}

#endif //KPLAYER_KPVideoShowPlayListPlugin_H
