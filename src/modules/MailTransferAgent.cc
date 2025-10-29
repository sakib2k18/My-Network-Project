#include "MailTransferAgent.h"

Define_Module(MailTransferAgent);

simsignal_t MailTransferAgent::sigEmailRelayed = registerSignal("emailRelayed");

void MailTransferAgent::initialize() {
    updateDisplay();
}

void MailTransferAgent::updateDisplay() {
    char buf[64]; sprintf(buf, "Relayed:%d", queueLen);
    getDisplayString().setTagArg("t", 0, buf);
}

void MailTransferAgent::handleMessage(cMessage *msg) {
    if (auto smtp = dynamic_cast<SmtpMessage*>(msg)) {
        EV_INFO << "[MTA] SMTP cmd: " << smtp->getCommand() << " " << smtp->getArg1() << "\n";
        delete smtp;
        return;
    }
    if (auto em = dynamic_cast<EmailMessage*>(msg)) {
        EV_INFO << "[MTA] forwarding email to SpamFilter/MDA: subject='" << em->getSubject() << "' to " << em->getTo() << "\n";
        queueLen++;
        {
            int n = gateSize("networkOut");
            for (int i = 0; i < n; ++i) {
                cMessage *copy = (i == n - 1 ? em : em->dup());
                send(copy, "networkOut", i);
            }
        }
        emit(sigEmailRelayed, 1L);
        updateDisplay();
        return;
    }
    delete msg;
}
