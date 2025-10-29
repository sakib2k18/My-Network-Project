#pragma once
#include <omnetpp.h>
#include <string>

namespace MyProject {
class SmtpMessage : public omnetpp::cMessage {
  private:
    std::string command;
    std::string arg1;
  public:
    SmtpMessage(const char *name=nullptr) : omnetpp::cMessage(name) {}
    SmtpMessage(const SmtpMessage& other) : omnetpp::cMessage(other) { *this = other; }
    virtual SmtpMessage *dup() const override { return new SmtpMessage(*this); }
    const char *getCommand() const { return command.c_str(); }
    const char *getArg1() const { return arg1.c_str(); }
    void setCommand(const char *v) { command = v ? v : ""; }
    void setArg1(const char *v) { arg1 = v ? v : ""; }
};
}
