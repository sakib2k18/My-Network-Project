#pragma once
#include <omnetpp.h>
#include "../messages/SmtpMessage.h"
#include "../messages/EmailMessage.h"

using namespace omnetpp;
using namespace MyProject;

class MailTransferAgent : public cSimpleModule {
  private:
    int queueLen = 0;
    static simsignal_t sigEmailRelayed;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void updateDisplay();
};
