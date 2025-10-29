package MyProject.modules;

simple SpamFilter
{
    parameters:
        string spamKeywords = default("lottery,viagra,free money");
        @display("i=block/filter;is=s");
    gates:
        input networkIn[];
        output networkOut[];
}
