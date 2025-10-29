#include "NetworkCloud.h"

Define_Module(NetworkCloud);

simsignal_t NetworkCloud::sigDropped = registerSignal("packetDropped");
simsignal_t NetworkCloud::sigTransitDelay = registerSignal("packetDelay");

void NetworkCloud::initialize() {
    dropProbability = par("dropProbability");
    delayMean = par("delayMean");
    delayJitter = par("delayJitter");
}

void NetworkCloud::handleMessage(cMessage *msg) {
    // Random drop
    if (uniform(0,1) < dropProbability) {
        emit(sigDropped, 1L);
        EV_WARN << "[Cloud] PACKET DROPPED!\n";
        bubble("PACKET DROPPED!");
        delete msg;
        return;
    }

    // Delay and broadcast to all outputs except the incoming index
    int inIndex = msg->getArrivalGate()->getIndex();
    simtime_t d = delayMean + uniform(-delayJitter, delayJitter);
    if (d < 0) d = 0;
    emit(sigTransitDelay, d);

    int n = gateSize("out");
    for (int i = 0; i < n; ++i) {
        if (i == inIndex) continue;
        cMessage *copy = (i == n - 1 ? msg : msg->dup());
        sendDelayed(copy, d, "out", i);
    }
}
