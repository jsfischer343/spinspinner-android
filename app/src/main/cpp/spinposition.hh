#pragma once
#ifndef SPINPOSITION_HH_
#define SPINPOSITION_HH_

#include <algorithm>
#include <string>
#include <vector>
#include "easyrandom.hh"

//Position feature probabilities
#define POS_FEATURE_BLADE_PROB 0.175
#define POS_FEATURE_COE_PROB 0.2
#define POS_FEATURE_JUMP_PROB 0.2
#define POS_FEATURE_REV_PROB 0.25
#define POS_FEATURE_SPEED_PROB 0.175

class SpinSegment; //forward declartion for creating valid reference

class SpinPosition
{
    public:
        /* --Valid Settings--
         * position: camel -> 'c'; sit -> 's'; upright -> 'u'; layback -> 'l'; intermediate -> 'i'
         * variation: up -> 'u'; front -> 'f'; behind -> 'b'; side -> 's'; biellmann -> 'm'; straight -> 't'
         * features: blade -> 'b'; coe -> 'c'; jump -> 'j'; 8 revs -> '8'; speed -> 's'; windmill -> 'w'
         * adult specific features: 5 revs -> '5';
         */
        SpinSegment* parent;
        char position = -1; //basic position: camel, sit, upright, layback, intermediate
        std::vector<char> variations = {}; //change of body making spin position more difficult
        std::vector<char> features = {}; //features that are associated with a specific position (see spin features)


        //functions
        SpinPosition(SpinSegment* parent, char position);

        bool addVariation(char variation, bool normalize);
        bool addFeature(char feature, bool normalize);
        bool hasVariation(char variation) const;
        bool hasFeature(char feature) const;
        bool hasAnyVariation() const;
        bool hasAnyFeature() const;
        char pickRandomFeature(bool isRev5) const;
        char pickRandomVariation() const;

        std::string toCode() const;
        std::string prettyPrint() const;
        std::string getPositionString(bool codeFormat) const;
        std::string getVariationString(bool codeFormat) const;
        std::string getFeatureString(bool codeFormat) const;
};

#endif
