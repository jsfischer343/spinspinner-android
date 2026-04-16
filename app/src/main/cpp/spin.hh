#pragma once
#ifndef SPIN_HH_
#define SPIN_HH_

#include <string>
#include <vector>
#include <utility>
#include "spinsegment.hh"

class Spin
{
    public:
        //base spin qualities
        char baseType; //camel -> 'c'; sit -> 's'; upright -> 'u'; layback -> 'l'; combo -> 'k'
        bool isFlying;
        bool isChangeFoot;
        int level = 0;
        std::vector<SpinSegment> spinSegments; //maximum 2 (for change foot spins)

        //spin features
        typedef struct {
            bool changeFootByJump = false;
            bool difficultEntrance = false;
            bool difficultExit = false;
        } SpinFeatures;
        SpinFeatures features;

        //flags
        bool intermediatePositionFlag = false;
        bool twoVariationsFlag = false; //only the first two difficult variations count towards levels
        bool changeDirectionFlag = false;

        //functions
        bool hasAllPrimaryPositions() const; //for use when validating combo spins
        int getTotalPositions() const;
        bool variationUsed(char positionChar, char variationChar) const; //the same variation can't be used on the same position type (i.e. camel side twice, sit front twice, ...)
        bool featureUsed(char featureChar) const;
        bool hasTwoVariations() const;
        bool hasDifficultChangeOfPosition() const;

        std::string toCode() const;
        std::string prettyPrint() const;
};

#endif
