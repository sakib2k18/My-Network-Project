#pragma once
#include <omnetpp.h>
#include <vector>
#include <string>
#include <sstream>
#include "../messages/EmailMessage.h"

using namespace omnetpp;
using namespace MyProject;

class SpamFilter : public cSimpleModule {
  private:
    std::vector<std::string> keywords;
    int caught = 0;
    static simsignal_t sigSpamCaught;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void updateDisplay();
};
