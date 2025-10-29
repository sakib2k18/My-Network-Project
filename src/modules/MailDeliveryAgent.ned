package MyProject.modules;

simple MailDeliveryAgent
{
    parameters:
        @display("i=block/server2;is=s");
    gates:
        input networkIn[];
        output networkOut[];
}
