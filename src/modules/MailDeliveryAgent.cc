#include "MailDeliveryAgent.h"

Define_Module(MailDeliveryAgent);

simsignal_t MailDeliveryAgent::sigStored = registerSignal("emailStored");
simsignal_t MailDeliveryAgent::sigDelivered = registerSignal("emailDelivered");

void MailDeliveryAgent::initialize() {
    updateDisplay();
}

void MailDeliveryAgent::updateDisplay() {
    int total = 0; for (auto &p: mailboxes) total += (int)p.second.size();
    char buf[64]; sprintf(buf, "Mailboxes:%d", total);
    getDisplayString().setTagArg("t", 0, buf);
}

void MailDeliveryAgent::handleMessage(cMessage *msg) {
    if (auto em = dynamic_cast<EmailMessage*>(msg)) {
        std::string user;
        std::string to = em->getTo();
        auto at = to.find('@');
        user = (at!=std::string::npos) ? to.substr(0, at) : to;
        mailboxes[user].push_back(em);
        emit(sigStored, 1L);
        EV_INFO << "[MDA] stored email for user=" << user << ", subject='" << em->getSubject() << "'\n";
        updateDisplay();
        return;
    }
    if (auto pop = dynamic_cast<Pop3Message*>(msg)) {
        std::string cmd = pop->getCommand();
        std::string user = pop->getUsername();
        if (cmd == "USER") {
            // accept any user
            Pop3Message *ok = new Pop3Message("+OK USER"); ok->setCommand("+OK"); ok->setInfo("USER accepted");
            {
                int n = gateSize("networkOut");
                for (int i = 0; i < n; ++i) {
                    cMessage *copy = (i == n - 1 ? ok : ok->dup());
                    send(copy, "networkOut", i);
                }
            }
        } else if (cmd == "PASS") {
            Pop3Message *ok = new Pop3Message("+OK PASS"); ok->setCommand("+OK"); ok->setInfo("PASS accepted");
            {
                int n = gateSize("networkOut");
                for (int i = 0; i < n; ++i) {
                    cMessage *copy = (i == n - 1 ? ok : ok->dup());
                    send(copy, "networkOut", i);
                }
            }
        } else if (cmd == "LIST") {
            int cnt = (int)mailboxes[user].size();
            Pop3Message *ok = new Pop3Message("+OK LIST"); ok->setCommand("+OK"); ok->setInfo(std::to_string(cnt).c_str());
            {
                int n = gateSize("networkOut");
                for (int i = 0; i < n; ++i) {
                    cMessage *copy = (i == n - 1 ? ok : ok->dup());
                    send(copy, "networkOut", i);
                }
            }
        } else if (cmd == "RETR") {
            int idx = pop->getIndex();
            auto &box = mailboxes[user];
            if (!box.empty() && idx >= 0 && idx < (int)box.size()) {
                EmailMessage *em = box[idx]->dup();
                {
                    int n = gateSize("networkOut");
                    for (int i = 0; i < n; ++i) {
                        cMessage *copy = (i == n - 1 ? em : em->dup());
                        send(copy, "networkOut", i);
                    }
                }
                emit(sigDelivered, 1L);
                Pop3Message *ok = new Pop3Message("+OK RETR"); ok->setCommand("+OK"); ok->setInfo("Email sent");
                {
                    int n = gateSize("networkOut");
                    for (int i = 0; i < n; ++i) {
                        cMessage *copy = (i == n - 1 ? ok : ok->dup());
                        send(copy, "networkOut", i);
                    }
                }
            } else {
                Pop3Message *err = new Pop3Message("-ERR RETR"); err->setCommand("-ERR"); err->setInfo("No such message");
                {
                    int n = gateSize("networkOut");
                    for (int i = 0; i < n; ++i) {
                        cMessage *copy = (i == n - 1 ? err : err->dup());
                        send(copy, "networkOut", i);
                    }
                }
            }
        }
        delete pop;
        return;
    }
    delete msg;
}
