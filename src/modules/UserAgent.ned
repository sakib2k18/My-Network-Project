package MyProject.modules;

simple UserAgent
{
    parameters:
        string userName;
        string peerSmtp = default("mta");
        string peerPop3 = default("mda");
        bool isSender = default(true);
        string peerUser = default("");
        double sendInterval @unit(s) = default(10s);
        double checkInterval @unit(s) = default(20s);
        @display("i=block/app;is=s");
    gates:
        input networkIn;
        output networkOut;
}