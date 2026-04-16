#pragma once
#ifndef SPINSPINNER_HH_
#define SPINSPINNER_HH_

//Probablities for base spin qualities (think spin code)
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

//Probablities for spin bullets
#define ADD_VARIATION_PROB 0.32
#define ADD_SPIN_FEATURE_PROB 0.19
#define ADD_POSITION_FEATURE_PROB 0.23
#define ADD_INTERMEDIATE_POSITION_PROB 0.13 //only affects combo spins
#define ADD_CHANGE_OF_DIRECTION_PROB 0.13

//Probablitiy of allowing stacking features or variations on same position
#define FEATURE_ON_SAME_POSITION_PROB 0.05
#define VARIATION_ON_SAME_POSITION_PROB 0.8

//Misc probabilities
#define WINDMILL_PROB 0.3

#include <algorithm>
#include <string>
#include <vector>
#include "spin.hh"
#include "easyrandom.hh"

class SpinSpinner
{
    public:
        bool defaultDirection = false; //false: counter-clockwise, true: clockwise
        bool normalize = true;
        std::vector<Spin> spinHistory = {}; //every time one of the spin functions is called it will push to this list

        SpinSpinner(bool defaultDirection, bool normalize);

        void spin(); //as in the act of spinning a wheel
        void spin(int level);
        void spin(char spinType, int level);
    private:
        int targetLevel = 0; //the target level for the currentSpin being generated
        Spin currentSpin;
        void generateSpin();
            void generateSpinInOnePosition();
            void generateCombo();
                void generateComboPositions();
                    void generateComboPositions_addPosition(bool swappedFeet);

        void setRandomBaseQualities(); //selects base qualities for the spin (think spin code)
        void initializeBaseStructure(); //adds spin segements and starting positions for some spins
        //main logic for adding levels
        void addLevel();
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
            bool shouldAvoidChangeFootByJump();


    public:
        std::string spinHistoryToCode();
        std::string spinHistoryPrettyPrint();

};

#endif
