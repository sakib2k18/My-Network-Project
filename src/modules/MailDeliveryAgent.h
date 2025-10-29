#pragma once
#include <omnetpp.h>
#include <map>
#include <vector>
#include <string>
#include "../messages/EmailMessage.h"
#include "../messages/Pop3Message.h"

using namespace omnetpp;
using namespace MyProject;

class MailDeliveryAgent : public cSimpleModule {
  private:
    std::map<std::string, std::vector<EmailMessage*>> mailboxes;
    static simsignal_t sigStored;
    static simsignal_t sigDelivered;
    void updateDisplay();
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
