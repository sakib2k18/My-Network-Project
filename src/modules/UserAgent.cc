#include "UserAgent.h"

Define_Module(UserAgent);

simsignal_t UserAgent::sigEmailSent = registerSignal("emailSent");
simsignal_t UserAgent::sigEmailReceived = registerSignal("emailReceived");

void UserAgent::initialize() {
    userName = par("userName").stringValue();
    isSender = par("isSender");
    if (isSender) {
        // Send to peer user
        std::string toUser = par("peerUser").stdstringValue();
        std::string fromAddr = userName + std::string("@example.com");
        std::string toAddr = (toUser.empty()? userName : toUser) + std::string("@example.com");

        // Minimal SMTP sequence
        SmtpMessage *h = new SmtpMessage("SMTP:HELO"); h->setCommand("HELO"); h->setArg1((std::string("client.")+userName).c_str()); send(h, "networkOut");
        SmtpMessage *m = new SmtpMessage("SMTP:MAIL"); m->setCommand("MAIL"); m->setArg1((std::string("<")+fromAddr+">").c_str()); send(m, "networkOut");
        SmtpMessage *r = new SmtpMessage("SMTP:RCPT"); r->setCommand("RCPT"); r->setArg1((std::string("<")+toAddr+">").c_str()); send(r, "networkOut");
        SmtpMessage *d = new SmtpMessage("SMTP:DATA"); d->setCommand("DATA"); send(d, "networkOut");

        // Email payload
        EmailMessage *em = new EmailMessage("EMAIL");
        em->setFrom(fromAddr.c_str());
        em->setTo(toAddr.c_str());
        em->setSubject((std::string("Hello from ") + userName).c_str());
        em->setBody("Minimal email simulation");
        em->setCreatedAt(simTime());
        send(em, "networkOut");
        SmtpMessage *q = new SmtpMessage("SMTP:QUIT"); q->setCommand("QUIT"); send(q, "networkOut");
        sentCount++;
        emit(sigEmailSent, 1L);
    } else {
        // Receiver: perform minimal POP3 retrieval
        Pop3Message *u = new Pop3Message("POP3:USER"); u->setCommand("USER"); u->setUsername(userName.c_str()); send(u, "networkOut");
        Pop3Message *p = new Pop3Message("POP3:PASS"); p->setCommand("PASS"); p->setPassword("password"); send(p, "networkOut");
        Pop3Message *l = new Pop3Message("POP3:LIST"); l->setCommand("LIST"); send(l, "networkOut");
        Pop3Message *t = new Pop3Message("POP3:RETR"); t->setCommand("RETR"); t->setIndex(0); send(t, "networkOut");
    }
    // End simulation shortly after actions to ensure one-shot run
    doneTimer = new cMessage("doneTimer");
    scheduleAt(simTime() + 0.2, doneTimer);
    updateDisplay();
}

void UserAgent::updateDisplay() {
    char buf[128];
    sprintf(buf, "Sent:%d\nRecv:%d", sentCount, recvCount);
    getDisplayString().setTagArg("t", 0, buf);
}

void UserAgent::handleMessage(cMessage *msg) {
    if (msg == doneTimer) {
        delete msg;
        endSimulation();
        return;
    }
    // Incoming network message (only EmailMessage is used in minimal setup)
    if (auto em = dynamic_cast<EmailMessage*>(msg)) {
        recvCount++;
        emit(sigEmailReceived, 1L);
        EV_INFO << "[UA:" << getFullName() << "] received email: subject='" << em->getSubject() << "' from " << em->getFrom() << "\n";
        bubble("EMAIL RECEIVED");
        delete em;
        updateDisplay();
        // Stop immediately after successful receive
        endSimulation();
        return;
    }
    delete msg;
}
