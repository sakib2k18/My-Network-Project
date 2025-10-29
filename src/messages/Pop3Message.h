#pragma once
#include <omnetpp.h>
#include <string>

namespace MyProject {
class Pop3Message : public omnetpp::cMessage {
  private:
    std::string command;
    std::string username;
    std::string password;
    std::string info;
    int index = 0;
  public:
    Pop3Message(const char *name=nullptr) : omnetpp::cMessage(name) {}
    Pop3Message(const Pop3Message& other) : omnetpp::cMessage(other) { *this = other; }
    virtual Pop3Message *dup() const override { return new Pop3Message(*this); }
    const char *getCommand() const { return command.c_str(); }
    const char *getUsername() const { return username.c_str(); }
    const char *getPassword() const { return password.c_str(); }
    const char *getInfo() const { return info.c_str(); }
    int getIndex() const { return index; }
    void setCommand(const char *v) { command = v ? v : ""; }
    void setUsername(const char *v) { username = v ? v : ""; }
    void setPassword(const char *v) { password = v ? v : ""; }
    void setInfo(const char *v) { info = v ? v : ""; }
    void setIndex(int v) { index = v; }
};
}
