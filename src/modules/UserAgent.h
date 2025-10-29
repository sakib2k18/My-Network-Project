#pragma once
#include <omnetpp.h>
#include "../messages/SmtpMessage.h"
#include "../messages/Pop3Message.h"
#include "../messages/EmailMessage.h"

using namespace omnetpp;
using namespace MyProject;

class UserAgent : public cSimpleModule {
  private:
    std::string userName;
    bool isSender = true;
    cMessage *popOnce = nullptr;
    cMessage *doneTimer = nullptr;

    int sentCount = 0;
    int recvCount = 0;

    static simsignal_t sigEmailSent;
    static simsignal_t sigEmailReceived;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void updateDisplay();
};
