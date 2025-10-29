package MyProject.simulations;

network EmailNetwork
{
    @display("bgb=1600,600");  // Very wide, less tall

    submodules:
        // ===== LEFT: CLIENT LAYER =====
        uaSender: MyProject.modules.UserAgent {
            parameters:
                userName = "alice";
                isSender = true;
                peerUser = "bob";
            @display("p=200,200;b=120,70");
        }

        uaReceiver: MyProject.modules.UserAgent {
            parameters:
                userName = "bob";
                isSender = false;
            @display("p=200,400;b=120,70");
        }

        // ===== CENTER: NETWORK LAYER =====
        cloud: MyProject.modules.NetworkCloud {
            @display("p=500,300;b=180,120");
        }

        // ===== RIGHT: SERVER LAYER =====
        mta: MyProject.modules.MailTransferAgent {
            @display("p=800,150;b=140,80");
        }

        spam: MyProject.modules.SpamFilter {
            @display("p=800,300;b=140,80");
        }

        mda: MyProject.modules.MailDeliveryAgent {
            @display("p=800,450;b=140,80");
        }

    connections:
        // Remove all "++" operators for scalar gates
        uaSender.networkOut --> cloud.in++;
        cloud.out++ --> uaSender.networkIn;

        uaReceiver.networkOut --> cloud.in++;
        cloud.out++ --> uaReceiver.networkIn;

        mta.networkOut++ --> cloud.in++;
        cloud.out++ --> mta.networkIn++;

        spam.networkOut++ --> cloud.in++;
        cloud.out++ --> spam.networkIn++;

        mda.networkOut++ --> cloud.in++;
        cloud.out++ --> mda.networkIn++;

        // Direct email flow - scalar to scalar connections
        mta.networkOut++ --> spam.networkIn++;
        spam.networkOut++ --> mda.networkIn++;
}