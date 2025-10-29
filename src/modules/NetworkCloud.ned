package MyProject.modules;

simple NetworkCloud
{
    parameters:
        double delayMean @unit(s) = default(0.2s);
        double delayJitter @unit(s) = default(0.1s);
        double dropProbability = default(0.05);
        @display("i=cloud");
    gates:
        input in[5];  // 5 input gates for uaSender, uaReceiver, mta, spam, mda
        output out[5]; // 5 output gates for uaSender, uaReceiver, mta, spam, mda
}
