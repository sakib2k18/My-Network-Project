#pragma once
#include <omnetpp.h>

using namespace omnetpp;

class NetworkCloud : public cSimpleModule {
  private:
    double dropProbability;
    double delayMean;
    double delayJitter;
    static simsignal_t sigDropped;
    static simsignal_t sigTransitDelay;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
