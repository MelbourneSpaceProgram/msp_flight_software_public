#include <src/payload_processor/uplinks/credits_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/echo_payload.h>
#include <src/util/tirtos_utils.h>

CreditsUplink::CreditsUplink(byte* payload) : Uplink(0) {}

void CreditsUplink::Say(const char* message) {
    Lithium* lithium = Lithium::GetInstance();
    EchoPayload payload(message);
    lithium->Transmit(&payload);
    TirtosUtils::SleepMilli(2000);
}

bool CreditsUplink::ExecuteUplink() {
    Say("This project could not have been completed without the hard work");
    Say("of a group of very dedicated individuals.");
    Say("A special thanks goes out to those who helped bring ACRUX-1 to ");
    Say("completion in the last 6 months of the project");
    Say("Mission Operations/Telecomms\nGabriel Abrahams\nTim Allison-Walker");
    Say("\nAdam Kues\nRyan Crosby\nSilas Wang\nYuxuan (Eric) Lin\n");
    Say("Electronics\nShaun Pye\nAlexander Kuhn\nJiayi (George) Chen");
    Say("\nJohanna Rigby\nLewis Hickey\nManroshan Singh");
    Say("\nMatthew Walker\nMichael Tian\nSamar Kattan\nDa (Karex) Huang\n");
    Say("Software\nBenjamin Ding\nAnthony (Krispy) Kremor\nHugo Umbers");
    Say("\nDaniel Sutherland\nWilhelm Schuetz\nNina Averill\n");
    Say("Flight Systems\nJack McRobbie\nRowan (Satellite Herder) Skewes");
    Say("Skewes\nDeclan Burke\nBeau Colley-Allerton\nOscar Glenister");
    Say("Flight Vehicle\nXi Yuen (William) Ling\nKaleb Foster\nSteven "
        "Despotellis");
    Say("Despotellis\nMatthew Lui\nThomas McCormack");
    Say("Leadership\nBlake Fuller\nAndrew Wetherell\nAnna Bohacova");
    Say("\nMegan Toomey\nChris Morgan\nJake Herd\b");
    Say("Thank you also to everyone else who has been a part of (or helped) "
        "MSP\n");
    Say("\nAditya Ramani\nAdrian Xiao\nAgam Raj\nAkshay Kumar\nAlexander Kuhn");
    Say("\nAlexander Newton\nAlistair Jones\nAndrew Reagan");
    Say("\nAngad Keith\nAzam Khan\nJamie Bialecki\nBraeden Borg\nBrett Hansen");
    Say("\nCheng Lin\nChris Hobbs\nDavesh Patel\nEd Muthiah");
    Say("\nFlorian Chaubeyre\nFreddie Yang\nGabby Nguyen\nHarry Bennett");
    Say("\nHarry Pittock\nHeli Yoon\nJack Zheng");
    Say("\nJoel Mangalasinghe\nJohnny Zheng\nKarin Ericsson\nLachlan Cobiac");
    Say("\nLaila Amal\nLauren Meyer\nMarkos Hasiotis");
    Say("\nMay Mehrebin\nMichael Gu\nMichael Norris\nNadun Udawela");
    Say("\nOwen Hinson\nRami Ali");
    Say("\nRobert Chin\nRob Mearns\nSami Raines\nSam Lindsay\nSarina Moshfegh");
    Say("\nSasika De Silva\nMark Stringer\nSebastian Gawin");
    Say("\nSrinath Kadambi\nTejasvi Hari Krishna\nThomas Graham");
    Say("\nVarun Malik\nVishwesh Sridhar\nYiannis Maxwell\nTroy McCann");
    Say("\nAndrew Rui\nLauren Jessup\nAngela Zhang\nPim Quach-Thanissorn");
    Say("\nTom Garsia\nAnton Tarasenko\nCatie McClelland\nStuart Cooper");
    Say("\nSalina Jaffar\nAhsan Abdullah Saad\nKhalid Beg\nAaron Sim");
    Say("\nLachlan Hamilton\nDanu Damitrias\nAlex Gunner\nKirill Kokorin");
    Say("\nKirill Kokorin\nAbhishekh Shankar\nInez Zheng\nMia Ito");
    Say("\nHamidreza (Will) Borghei\nJordan Moshcovitis\nDiego Aluwihare");
    Say("\nChristopher Franssen\nJoe McKee\nRicardo Garcia Rosas\nStan "
        "Skafidas");
    Say("\nCallum Maltby\nMichael Malek\nLucas Palmer\nNick Mulder\nJack "
        "Lavery");
    Say("\nPeter Shelev\nAlex Ikin");
    Say("\nGrandmas Dumplings\nHouse of Cards\nSARCNET (Joe & Julie)\nTim "
        "Conboy");
    Say("\nMum, Dad and all our families who have supported us through this");
    return true;
}
