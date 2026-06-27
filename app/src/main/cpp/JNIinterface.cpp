#include <jni.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include "argparser.hh"
#include "easyrandom.hh"
#include "spinspinner.hh"

bool validateInput(ap::argmap& args)
{
    //adult rules: user can specific only one of the below
    if(std::stoi(args["--adult-junior-senior"])+
        std::stoi(args["--adult-intermediate-novice"])+
        std::stoi(args["--adult-gold"])+
        std::stoi(args["--adult-silver"])+
        std::stoi(args["--adult-bronze"]) > 1)
    {
        return false;
    }
    //spin type
    bool spinTypeValid = false;
    std::vector<std::string> validSpinTypes;
    if(std::stoi(args["--adult-bronze"]))
        validSpinTypes = {"any","camel","sit","upright","layback","combo","2ft"};
    else
        validSpinTypes = {"any","camel","sit","upright","layback","combo"};
    if(args["--type"].empty())
    {
        args["--type"] = "any";
        spinTypeValid = true;
    }
    else
    {
        for(size_t i=0;i<validSpinTypes.size();i++)
        {
            if(args["--type"]==validSpinTypes.at(i))
                spinTypeValid = true;
        }
    }

    //level
    bool levelValid = false;
    std::vector<std::string> validLevels;
    if(args["--type"] == "2ft")
        validLevels = {"0"};
    else if(std::stoi(args["--adult-gold"]))
        validLevels = {"0","1","2","3"};
    else if(std::stoi(args["--adult-silver"]))
        validLevels = {"0","1","2"};
    else if(std::stoi(args["--adult-bronze"]))
        validLevels = {"0","1"};
    else
        validLevels = {"0","1","2","3","4"};
    if(args["--level"].empty() || args["--level"]=="any")
    {
        args["--level"] = easyRandom::pickFromVector(validLevels);
        levelValid = true;
    }
    else
    {
        for(size_t i=0;i<validLevels.size();i++)
        {
            if(args["--level"]==validLevels.at(i))
                levelValid = true;
        }
    }

    //number of spins spun
    bool numberValid = false;
    try
    {
        if(args["--number"].empty())
        {
            args["--number"] = "1";
            numberValid = true;
        }
        else
        {
           int tempCheck = std::stoi(args["--number"]);
           if(tempCheck>0 && tempCheck<=1000)
               numberValid = true;
        }
    }
    catch (std::invalid_argument& e){}; //numberValid = false by default

    if(!spinTypeValid || !levelValid || !numberValid)
    {
        return false;
    }
    return true;
}
AdultRuleFlags setupAdultRuleFlags(ap::argmap& args)
{
    AdultRuleFlags adultRuleFlags = AdultRuleFlags();
    adultRuleFlags.junior_senior = std::stoi(args["--adult-junior-senior"]);
    adultRuleFlags.intermediate_novice = std::stoi(args["--adult-intermediate-novice"]);
    adultRuleFlags.gold = std::stoi(args["--adult-gold"]);
    adultRuleFlags.silver = std::stoi(args["--adult-silver"]);
    adultRuleFlags.bronze = std::stoi(args["--adult-bronze"]);
    if((std::stoi(args["--adult-junior-senior"])+
        std::stoi(args["--adult-intermediate-novice"])+
        std::stoi(args["--adult-gold"])+std::stoi(args["--adult-silver"])+
        std::stoi(args["--adult-bronze"]))>0)
        adultRuleFlags.active = true;
    return adultRuleFlags;
}
std::string runCommand(std::vector<std::string>& commandVector) {

    ap::parser p(commandVector);
    p.add("-l", "--level",                      "Spin level (numeric 0-4)");
    p.add("-t", "--type",                       "Type of spin (any, camel, sit, upright, layback, combo (adult bronze only: 2ft))");
    p.add("-n", "--number",                     "Number spins spun (between 1-1000)");
    p.add("-r", "--reverse",                    "Sets default direction to clockwise instead of counter-clockwise",                             ap::mode::BOOLEAN);
    p.add("-c", "--code",                       "Prints spin as code rather than human readable",                                               ap::mode::BOOLEAN);
    p.add("-b", "--normalize",                  "Reduces strange and awkward transitions, variations, and features",                            ap::mode::BOOLEAN);
    p.add("",   "--adult-junior-senior",        "Modifies the spin logic so spin are in accordance with the adult junior-senior rules.",        ap::mode::BOOLEAN);
    p.add("",   "--adult-intermediate-novice",  "Modifies the spin logic so spin are in accordance with the adult intermediate-novice rules.",  ap::mode::BOOLEAN);
    p.add("",   "--adult-gold",                 "Modifies the spin logic so spin are in accordance with the adult gold rules.",                 ap::mode::BOOLEAN);
    p.add("",   "--adult-silver",               "Modifies the spin logic so spin are in accordance with the adult silver rules.",               ap::mode::BOOLEAN);
    p.add("",   "--adult-bronze",               "Modifies the spin logic so spin are in accordance with the adult bronze rules.",               ap::mode::BOOLEAN);

    ap::argmap args = p.parse();

    if (!args.parsed_successfully()) {
        return "Internal Error";
    }

    if(!validateInput(args))
    {
        return "Internal Error";
    }

    SpinSpinner spinSpinnerObj = SpinSpinner(std::stoi(args["--reverse"]),std::stoi(args["--normalize"]),setupAdultRuleFlags(args));
    int spinLevel = std::stoi(args["--level"]); //if empty it will be set to 0 by validateInput()
    std::string spinType = args["--type"];
    int numberOfSpins = std::stoi(args["--number"]);
    for(int i=0;i<numberOfSpins;i++)
    {
        if(spinType=="any")
        {
            spinSpinnerObj.spin(spinLevel);
        }
        else if(spinType=="combo")
        {
            spinSpinnerObj.spin('k', spinLevel);
        }
        else
        {
            spinSpinnerObj.spin(spinType.c_str()[0], spinLevel);
        }
    }
    if(std::stoi(args["--code"]))
    {
        return spinSpinnerObj.spinHistoryToCode();
    }
    else
    {
        return spinSpinnerObj.spinHistoryPrettyPrint();
    }
    spinSpinnerObj.spinHistory.clear();
}

extern "C"
{
    JNIEXPORT jstring JNICALL
    Java_com_example_figureskatingspinspinner_data_NativeInterface_spinSpinnerCommand(JNIEnv *env, jobject nativeInterfaceInstance, jstring command) {
        const char* commandCharPtr = env->GetStringUTFChars(command, nullptr);
        std::string commandString = commandCharPtr;
        commandString = "placeholder "+commandString;
        env->ReleaseStringUTFChars(command, commandCharPtr);

        std::vector<std::string> commandVector;
        std::stringstream ss(commandString);
        std::string token;
        while(ss >> token)
        {
            commandVector.push_back(token);
        }

        return env->NewStringUTF(runCommand(commandVector).c_str());
    }
}
