#include "SpamFilter.h"

Define_Module(SpamFilter);

simsignal_t SpamFilter::sigSpamCaught = registerSignal("spamCaught");

void SpamFilter::initialize() {
    const char *list = par("spamKeywords");
    std::stringstream ss(list);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // trim spaces
        size_t b = item.find_first_not_of(' ');
        size_t e = item.find_last_not_of(' ');
        if (b!=std::string::npos)
            keywords.push_back(item.substr(b, e-b+1));
    }
    updateDisplay();
}

void SpamFilter::updateDisplay() {
    char buf[64]; sprintf(buf, "Spam:%d", caught);
    getDisplayString().setTagArg("t", 0, buf);
}

void SpamFilter::handleMessage(cMessage *msg) {
    if (auto em = dynamic_cast<MyProject::EmailMessage*>(msg)) {
        std::string text = em->getSubject();
        text += " ";
        text += em->getBody();
        bool isSpam = false;
        for (auto &k: keywords) {
            if (text.find(k) != std::string::npos) { isSpam = true; break; }
        }
        if (isSpam) {
            caught++;
            emit(sigSpamCaught, 1L);
            EV_WARN << "[SpamFilter] SPAM detected: '" << em->getSubject() << "'\n";
            bubble("SPAM BLOCKED");
            delete em; // drop
            updateDisplay();
        } else {
            int n = gateSize("networkOut");
            for (int i = 0; i < n; ++i) {
                cMessage *copy = (i == n - 1 ? em : em->dup());
                send(copy, "networkOut", i);
            }
        }
        return;
    }
    // pass through any other control messages
    {
        int n = gateSize("networkOut");
        for (int i = 0; i < n; ++i) {
            cMessage *copy = (i == n - 1 ? msg : msg->dup());
            send(copy, "networkOut", i);
        }
    }
}
