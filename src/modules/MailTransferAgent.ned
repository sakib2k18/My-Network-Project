package MyProject.modules;

simple MailTransferAgent
{
    parameters:
        @display("i=block/server;is=s");
    gates:
        input networkIn[];
        output networkOut[];
}
