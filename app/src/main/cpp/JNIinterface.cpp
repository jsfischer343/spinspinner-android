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
    //spin type
    bool spinTypeValid = false;
    std::vector<std::string> validSpinTypes = {"any","camel","sit","upright","layback","combo"};
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
    std::vector<std::string> validLevels = {"0","1","2","3","4"};
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
            if(tempCheck>0 && tempCheck<=100)
                numberValid = true;
        }
    }
    catch (std::invalid_argument& e) {};

    if(!spinTypeValid || !levelValid || !numberValid)
    {
        return false;
    }
    return true;
}

std::string runCommand(std::vector<std::string>& commandVector) {

    ap::parser p(commandVector);
    p.add("-l", "--level",  "Spin level (numeric 0-4)");
    p.add("-t", "--type",   "Type of spin (any, camel, sit, upright, layback, combo)");
    p.add("-n", "--number", "Number spins spun (between 1-100)");
    p.add("-r", "--reverse","Sets default direction to clockwise instead of counter-clockwise", ap::mode::BOOLEAN);
    p.add("-c", "--code",   "Prints spin as code rather than human readable", ap::mode::BOOLEAN);
    p.add("-b", "--normalize",   "Reduces strange and awkward transitions, variations, and features", ap::mode::BOOLEAN);

    ap::argmap args = p.parse();

    if (!args.parsed_successfully()) {
        return "Internal Error";
    }

    if(!validateInput(args))
    {
        return "Internal Error";
    }

    SpinSpinner spinSpinnerObj = SpinSpinner(std::stoi(args["--reverse"]),std::stoi(args["--normalize"]));
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