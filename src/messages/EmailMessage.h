#pragma once
#include <omnetpp.h>
#include <string>

namespace MyProject {
class EmailMessage : public omnetpp::cMessage {
  private:
    std::string from;
    std::string to;
    std::string subject;
    std::string body;
    omnetpp::SimTime createdAt;
  public:
    EmailMessage(const char *name=nullptr) : omnetpp::cMessage(name) {}
    EmailMessage(const EmailMessage& other) : omnetpp::cMessage(other) { *this = other; }
    virtual EmailMessage *dup() const override { return new EmailMessage(*this); }
    // getters
    const char *getFrom() const { return from.c_str(); }
    const char *getTo() const { return to.c_str(); }
    const char *getSubject() const { return subject.c_str(); }
    const char *getBody() const { return body.c_str(); }
    omnetpp::SimTime getCreatedAt() const { return createdAt; }
    // setters
    void setFrom(const char *v) { from = v ? v : ""; }
    void setTo(const char *v) { to = v ? v : ""; }
    void setSubject(const char *v) { subject = v ? v : ""; }
    void setBody(const char *v) { body = v ? v : ""; }
    void setCreatedAt(const omnetpp::SimTime& t) { createdAt = t; }
};
}
