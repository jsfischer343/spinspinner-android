#pragma once
#ifndef SPINSPINNER_HH_
#define SPINSPINNER_HH_

//Probabilities for base spin qualities (think spin code)
#define CAMEL_CHANGEFOOTSPIN_PROB 0.5
#define CAMEL_FLYINGSPIN_PROB 0.5
#define SIT_CHANGEFOOTSPIN_PROB 0.5
#define SIT_FLYINGSPIN_PROB 0.5
#define UPRIGHT_CHANGEFOOTSPIN_PROB 0.5
#define UPRIGHT_FLYINGSPIN_PROB 0.01
#define LAYBACK_CHANGEFOOTSPIN_PROB 0.3
#define LAYBACK_FLYINGSPIN_PROB 0.01
#define COMBO_CHANGEFOOTSPIN_PROB 0.5
#define COMBO_FLYINGSPIN_PROB 0.5

//Weights for spin order in combo spins (sorta)
#define COMBO_START_CAMEL_PROB 0.6
#define COMBO_START_SIT_PROB 0.36
#define COMBO_START_UPRIGHT_PROB 0.04
#define COMBO_SWAP_UPRIGHT_FOR_LAYBACK 0.15

//Probabilities for spin bullets
#define ADD_VARIATION_PROB 0.32
#define ADD_SPIN_FEATURE_PROB 0.19
#define ADD_POSITION_FEATURE_PROB 0.23
#define ADD_INTERMEDIATE_POSITION_PROB 0.13 //only affects combo spins
#define ADD_CHANGE_OF_DIRECTION_PROB 0.13

//Probability of allowing stacking features or variations on same position
#define FEATURE_ON_SAME_POSITION_PROB 0.05
#define VARIATION_ON_SAME_POSITION_PROB 0.8

//Adult specific probabilities (note:
#define ADULT_CLEAN_CHANGEFOOT 0.5 //if the spin is (F)CCSp or (F)CSSp this is the chance that it will add to the level (basic position on both sides it expected to be achieved by skater)
#define ADULT_CLEAN_COMBO 0.5 //if the spin is (F)CoSp or (F)CCoSp this is the chance that it will add a level (basic position on all three primary positions is expected of the skater)

#include <algorithm>
#include <string>
#include <vector>
#include "spin.hh"
#include "easyrandom.hh"

typedef struct AdultRuleFlags
{
    bool active = false; //should be set true if any of the below are true
    bool junior_senior = false;
    bool intermediate_novice = false;
    bool gold = false;
    bool silver = false;
    bool bronze = false;
} AdultRuleFlags;

class SpinSpinner
{
    public:
        bool defaultDirection = false; //false: counter-clockwise, true: clockwise
        bool normalize = true;
        AdultRuleFlags adultRuleFlags;
        std::vector<Spin> spinHistory = {}; //every time one of the spin functions is called it will push to this list

        SpinSpinner(bool defaultDirection, bool normalize, AdultRuleFlags adultRuleFlags);

        void spin(); //clarification: as in the act of spinning a wheel
        void spin(int level);
        void spin(char spinType, int level);
    private:
        int targetLevel = 0; //generateSpin will keep adding levels until it reaches this target
        Spin currentSpin;
        void generateSpin();
            void generateSpinInOnePosition();
            void generate2FtUSp();
            void generateCombo();
                void generateComboPositions();
                    void generateComboPositions_addPosition(bool swappedFeet);

        void setRandomBaseQualities(); //selects base qualities for the spin (think spin code)
        //main logic for adding levels
        bool addLevel();
            bool addVariation();
            bool addSpinFeature();
            bool addPositionFeature();
            bool addIntermediatePosition();
            bool addChangeOfDirection();
            int pickRandomBulletType();
            SpinPosition* pickNonConflictingPosition();
            bool missingBulletForLevel4();
            void addARequiredBulletForLevel4();
            bool checkFeatureValidity(SpinPosition* spinPosition, char featureInQuestion);
            bool checkFeatureValidityAdult(SpinPosition* spinPosition, char featureInQuestion);
            bool shouldAvoidChangeFootByJump();


    public:
        std::string spinHistoryToCode();
        std::string spinHistoryPrettyPrint();

};

#endif
