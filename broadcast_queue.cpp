//
// Created by wangyaofu on 2018/6/3.
//

#include "broadcast_queue.h"

BroadcastMsgQueue::BroadcastMsgQueue() {

}

BroadcastMsgQueue::~BroadcastMsgQueue() {

}

void BroadcastMsgQueue::addMsg(BroadcastMsg& msg) {
    std::lock_guard<std::mutex> lk(msg_mutex_);
    msg_queue_.push(msg);
}

int BroadcastMsgQueue::getMsg(BroadcastMsg& broadcastMsg) {
    std::lock_guard<std::mutex> lk(msg_mutex_);
    if (msg_queue_.size() <= 0) {
        return 0;
    }
    broadcastMsg = msg_queue_.front();
    msg_queue_.pop();
    return 1;
}