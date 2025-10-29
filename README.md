# Email Protocol Stack Simulation (SMTP & POP3) in OMNeT++

This project simulates a minimal email ecosystem with SMTP sending and POP3 retrieval, including a network cloud with delay/drop and a spam filter.

## Modules
- **`UserAgent`**: Generates SMTP sequences and retrieves mail via POP3; displays sent/received counts.
- **`MailTransferAgent`**: Accepts SMTP commands and forwards `EmailMessage` to the SpamFilter/MDA.
- **`SpamFilter`**: Drops emails containing certain keywords.
- **`MailDeliveryAgent`**: Stores per-user mailboxes and serves POP3 `LIST`/`RETR`.
- **`NetworkCloud`**: Adds delay and random drops; broadcasts to all peers.

## Messages
- **`SmtpMessage`**: HELO/MAIL/RCPT/DATA/QUIT.
- **`EmailMessage`**: Email payload.
- **`Pop3Message`**: USER/PASS/LIST/RETR.

## Run
1. Open this project in OMNeT++ IDE.
2. Build (the .msg files will generate *_m files).
3. Run `EmailNetwork` from `simulations/EmailNetwork.ned`.
4. In the GUI, watch packets traverse modules and use the Log/SANs to analyze signals:
   - `emailSent`, `emailReceived`, `emailRelayed`, `emailStored`, `emailDelivered`, `packetDropped`, `packetDelay`, `spamCaught`.

## Tuning
Edit `simulations/omnetpp.ini` to adjust delays, drop probabilities, and intervals.
