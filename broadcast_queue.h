//
// Created by wangyaofu on 2018/6/3.
//

#ifndef WEBSOCKET_MASTER_BROADCAST_QUEUE_H
#define WEBSOCKET_MASTER_BROADCAST_QUEUE_H

#include "singleton.h"
#include <memory>
#include <mutex>
#include <queue>

struct BroadcastMsg {
    int type;
    std::string data;
};

class BroadcastMsgQueue {
public:
    BroadcastMsgQueue();
    ~BroadcastMsgQueue();
    void addMsg(BroadcastMsg& msg);
    int getMsg(BroadcastMsg& broadcastMsg);
    size_t queueSize() {
        return msg_queue_.size();
    }
private:
    std::queue<BroadcastMsg> msg_queue_;
    std::mutex msg_mutex_;
};

#define BROAD_QUEUE_PTR Singleton<BroadcastMsgQueue>::Instance()

#endif //WEBSOCKET_MASTER_BROADCAST_QUEUE_H
