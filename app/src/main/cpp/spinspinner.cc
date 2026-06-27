#include "spinspinner.hh"
#include "easyrandom.hh"

SpinSpinner::SpinSpinner(bool defaultDirection, bool normalize, AdultRuleFlags adultRuleFlags)
{
    this->defaultDirection = defaultDirection;
    this->normalize = normalize;
    this->adultRuleFlags = adultRuleFlags;
}
void SpinSpinner::spin()
{
    targetLevel = easyRandom::range(0,4);
    currentSpin = Spin();

    int spinSelect;
    if(adultRuleFlags.bronze)
        spinSelect = easyRandom::range(0,5);
    else
        spinSelect = easyRandom::range(0,4);
    switch(spinSelect)
    {
        case 0:
            currentSpin.baseType = 'c';
            break;
        case 1:
            currentSpin.baseType = 's';
            break;
        case 2:
            currentSpin.baseType = 'u';
            break;
        case 3:
            currentSpin.baseType = 'l';
            break;
        case 4:
            currentSpin.baseType = 'k';
            break;
        case 5:
            currentSpin.baseType = '2';
            targetLevel = 0;
            break;
        default:
            throw;
    }

    setRandomBaseQualities(); //sets fly and change foot properties
    generateSpin();
}
void SpinSpinner::spin(int level)
{
    targetLevel = level;
    currentSpin = Spin();

    int spinSelect;
    if(adultRuleFlags.bronze && targetLevel==0)
        spinSelect = easyRandom::range(0,5);
    else
        spinSelect = easyRandom::range(0,4);
    switch(spinSelect)
    {
        case 0:
            currentSpin.baseType = 'c';
            break;
        case 1:
            currentSpin.baseType = 's';
            break;
        case 2:
            currentSpin.baseType = 'u';
            break;
        case 3:
            currentSpin.baseType = 'l';
            break;
        case 4:
            currentSpin.baseType = 'k';
            break;
        case 5:
            currentSpin.baseType = '2';
            targetLevel = 0;
            generate2FtUSp();
            return;
            break;
        default:
            throw;
    }

    setRandomBaseQualities(); //sets fly and change foot properties
    generateSpin();
}
void SpinSpinner::spin(char type, int level)
{
    targetLevel = level;
    currentSpin = Spin();

    currentSpin.baseType = type;
    if(currentSpin.baseType=='2')
    {
        targetLevel = 0;
        generate2FtUSp();
        return;
    }

    setRandomBaseQualities(); //sets fly and change foot properties
    generateSpin();
}
void SpinSpinner::generateSpin()
{
    if(currentSpin.baseType=='c'||
        currentSpin.baseType=='s'||
        currentSpin.baseType=='u'||
        currentSpin.baseType=='l')
    {
        generateSpinInOnePosition();
    }
    else if(currentSpin.baseType=='k')
    {
        generateCombo();
    }
    else if(currentSpin.baseType=='2')
        generate2FtUSp();
    else
        throw;
}
void SpinSpinner::generateSpinInOnePosition()
{
    //decide footness
    char startingFootness;
    char otherFootness;
    if(currentSpin.isFlying && currentSpin.baseType=='c' && normalize) //flying forward camel is not a "normal" spin (i.e. if normalize is active then always start with a back-spin)
    {
        startingFootness = 'b';
        otherFootness = 'f';
    }
    else
    {
        if(easyRandom::range(0,1))
        {
            startingFootness = 'f';
            otherFootness = 'b';
        }
        else
        {
            startingFootness = 'b';
            otherFootness = 'f';
        }
    }
    //initialize spin segments
    if(currentSpin.isChangeFoot)
    {
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,startingFootness));
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,otherFootness));


        //adult specific feature
        if(adultRuleFlags.active && targetLevel>0)
        {
            if((currentSpin.baseType=='c'||currentSpin.baseType=='s') && easyRandom::weightedTruth(ADULT_CLEAN_CHANGEFOOT)) //(F)CCSp or (F)CSSp only
            {
                currentSpin.level++;
                currentSpin.features.cleanChangeFootSpin = true;
            }
        }
    }
    else
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,startingFootness));
    //add default spin positions
    for(size_t i=0;i<currentSpin.spinSegments.size();i++)
    {
        SpinSegment* currentSegment = &currentSpin.spinSegments.at(i);
        currentSegment->spinPositions.push_back(SpinPosition(currentSegment,currentSpin.baseType));
    }

    //add levels
    for(int i=0;i<targetLevel-currentSpin.features.cleanChangeFootSpin;i++)
    {
        if(!addLevel())
            throw;
    }

    //record spin
    spinHistory.push_back(currentSpin);
}
void SpinSpinner::generate2FtUSp()
{
    currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,'f')); //used only to differentiate rotational direction
    //record spin
    spinHistory.push_back(currentSpin);
}
void SpinSpinner::generateCombo()
{
    //pick first position
    char randomStartPosition;
    if(targetLevel<1 || (adultRuleFlags.active && targetLevel<2))
        randomStartPosition = 'c';
    else
        randomStartPosition = easyRandom::pickFromVectorWeighted(std::vector<int>{'c','s','u'},std::vector<double>{COMBO_START_CAMEL_PROB,COMBO_START_SIT_PROB,COMBO_START_UPRIGHT_PROB});

    //decide footness
    char startingFootness;
    char otherFootness;
    if(currentSpin.isFlying && randomStartPosition =='c' && normalize) //flying forward camel is decided to be not a normal spin (i.e. if normalize is active then always start with a back-spin)
    {
        startingFootness = 'b';
        otherFootness = 'f';
    }
    else
    {
        if(easyRandom::range(0,1))
        {
            startingFootness = 'f';
            otherFootness = 'b';
        }
        else
        {
            startingFootness = 'b';
            otherFootness = 'f';
        }
    }
    //initialize spin segments
    if(currentSpin.isChangeFoot)
    {
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,startingFootness));
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,otherFootness));
    }
    else
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,startingFootness));

    //push first position
    currentSpin.spinSegments.at(0).spinPositions.push_back(SpinPosition(&currentSpin.spinSegments.at(0),randomStartPosition));

    //Logic to add the rest of the positions
    generateComboPositions();

    int levelOffset = 0;
    //check for difficult change of position
    bool hasDifficultChangeOfPosition = false;
    if(currentSpin.hasDifficultChangeOfPosition())
    {
        currentSpin.level++;
        hasDifficultChangeOfPosition = true;
    }

    //adult specific features
    if(adultRuleFlags.active && targetLevel>0)
    {
        if(adultRuleFlags.gold || adultRuleFlags.silver || adultRuleFlags.bronze)
        {
            if(currentSpin.hasAllPrimaryPositions() && easyRandom::weightedTruth(ADULT_CLEAN_COMBO))
            {
                currentSpin.level++;
                currentSpin.features.allThreeBasicPositionsAnywhere = true;
            }
        }
        else if(currentSpin.isChangeFoot)
        {
            if(currentSpin.spinSegments.at(1).hasAllPrimaryPositions())
            {
                currentSpin.level++;
                currentSpin.spinSegments.at(1).features.allThreeBasicPositionsOnSecondFoot = true;
            }
        }
    }

    if(hasDifficultChangeOfPosition)
        levelOffset++;
    if(currentSpin.features.allThreeBasicPositionsAnywhere)
        levelOffset++;
    if(currentSpin.isChangeFoot)
    {
        if(currentSpin.spinSegments.at(1).features.allThreeBasicPositionsOnSecondFoot)
            levelOffset++;
    }

    //add levels
    for(int i=0;i<targetLevel-levelOffset;i++) //difficult change of position counts as a level, so addLevel is called one less time
    {
        if(!addLevel())
            throw;
    }

    spinHistory.push_back(currentSpin);
}
void SpinSpinner::generateComboPositions()
{
    if(currentSpin.isChangeFoot)
    {
        bool swappedFeet = false;

        while(currentSpin.getTotalPositions()<6)
        {
            if(!swappedFeet)
            {
                //after each position, randomly decide if a change foot will take place early
                if(easyRandom::weightedTruth(0.4)||currentSpin.spinSegments.at(0).spinPositions.size()==3|| //if already at three positions on first segement then must switch to next spin segement (i.e. change feet)
                    ((adultRuleFlags.junior_senior||adultRuleFlags.intermediate_novice)&&easyRandom::weightedTruth(0.4))) //OR  adult specific: increase chance of swapping feet for adult junior-senior and adult intermediate-novice due to additional feature available
                {
                    swappedFeet = true;
                    generateComboPositions_addPosition(swappedFeet);
                }
                else
                {
                    generateComboPositions_addPosition(swappedFeet);
                }
            }
            else
            {
                if(currentSpin.hasAllPrimaryPositions())
                {
                    SpinPosition* currentPosition = &currentSpin.spinSegments.at(1).spinPositions.at(currentSpin.spinSegments.at(1).spinPositions.size()-1);
                    if(currentSpin.spinSegments.at(1).spinPositions.size()==3) //must end if on second foot and done three positions
                    {
                        break;
                    }
                    else if(currentPosition->position == 'u' || currentPosition->position == 'l') //if has all primary positions and current position is upright or layback then there isn't any more that can be added to the spin
                        break;
                    else if(easyRandom::weightedTruth(0.6)) //randomly decide to end second segement early (must have all primary positions)
                    {
                        break;
                    }
                }
                generateComboPositions_addPosition(swappedFeet); //adding to second segment
            }
        }
    }
    else //no change of foot
    {
        generateComboPositions_addPosition(false); //add two last positions on same foot
        generateComboPositions_addPosition(false);
    }
}
void SpinSpinner::generateComboPositions_addPosition(bool swappedFeet)
{
    SpinSegment* currentSegment;
    if(!swappedFeet)
        currentSegment = &currentSpin.spinSegments.at(0);
    else
        currentSegment = &currentSpin.spinSegments.at(1);

    //logic error catch
    if(currentSegment->spinPositions.size()==3 || currentSpin.spinSegments.size()>2) //If this is true then it would imply that we are trying to add a 4th basic position to the same foot OR add a third segment, which wouldn't make sense
        throw;

    char nextPosition  = '\0';
    if(targetLevel>=1 && (!adultRuleFlags.active||targetLevel>=2)) //combo spins level 1 or higher can have a difficult change of position so the logic is more lax (i.e. there are more posibilities)
    {
        std::vector<char> validPositions = {'c','s','u'};
        std::vector<char> usedPositions = currentSegment->getUsedPositions();
        std::vector<char> unusedPositions;

        std::replace(usedPositions.begin(), usedPositions.end(), 'l', 'u'); //if layback is substitute for upright so for the logic to work any layback positions must be treated as uprights
        std::sort(usedPositions.begin(),usedPositions.end());
        std::set_difference(validPositions.begin(),validPositions.end(),usedPositions.begin(),usedPositions.end(),std::back_inserter(unusedPositions));

        std::vector<double> positionWeightsForUnusedPositions;
        for(size_t i=0;i<unusedPositions.size();i++)
        {
            if(unusedPositions.at(i)=='c')
                positionWeightsForUnusedPositions.push_back(COMBO_START_CAMEL_PROB);
            else if(unusedPositions.at(i)=='s')
                positionWeightsForUnusedPositions.push_back(COMBO_START_SIT_PROB);
            else if(unusedPositions.at(i)=='u')
                positionWeightsForUnusedPositions.push_back(COMBO_START_UPRIGHT_PROB);
        }

        nextPosition = easyRandom::pickFromVectorWeighted(unusedPositions,positionWeightsForUnusedPositions);

        if(nextPosition=='u')
        {
            if(easyRandom::weightedTruth(COMBO_SWAP_UPRIGHT_FOR_LAYBACK))
            {
                nextPosition='l';
            }
        }

        //check and update difficult change of position flag
        if(!currentSegment->spinPositions.empty())
        {
            char previousPosition = currentSegment->spinPositions.at(currentSegment->spinPositions.size()-1).position;
            if((previousPosition=='s'||previousPosition=='u'||previousPosition=='l')&&nextPosition=='c')
                currentSegment->features.difficultChangeOfPosition = true;
        }
    }
    else //there is a small amount of base combos that can be rolled so the logic is a bit simpler.
    {
        if(swappedFeet && currentSegment->spinPositions.size()==0) //Is change of foot spin and this is the first position on second side
        {
            SpinSegment* previousSegment = &currentSpin.spinSegments.at(0);
            int previousIndex = previousSegment->spinPositions.size()-1;
            char previousPosition = previousSegment->spinPositions.at(previousIndex).position;
            if(easyRandom::range(0,1)) //50/50 to repeat position on previous foot
            {
                if(previousPosition=='c')
                    nextPosition = 'c';
                else if(previousPosition=='s')
                    nextPosition = 's';
                else if(previousPosition=='u')
                    nextPosition = 'u';
                else if(previousPosition=='l')
                {
                    if(easyRandom::range(0,1))
                        nextPosition = 'u';
                    else
                        nextPosition = 'l';
                }
            }
            else //or change to next position
            {
                if(previousPosition=='c')
                    nextPosition = 's';
                else if(previousPosition=='s')
                    nextPosition = 'u';
                else if(previousPosition=='u')
                    nextPosition = 'u';
                else if(previousPosition=='l')
                {
                    if(easyRandom::range(0,1))
                        nextPosition = 'u';
                    else
                        nextPosition = 'l';
                }
            }
        }
        else //implies that this position is on the same foot as the previous position
        {
            int previousIndex = currentSegment->spinPositions.size()-1;
            char previousPosition = currentSegment->spinPositions.at(previousIndex).position;
            if(previousPosition=='c')
                nextPosition = 's';
            else if(previousPosition=='s')
                nextPosition = 'u';
        }
        if(nextPosition=='u')
        {
            if(easyRandom::weightedTruth(COMBO_SWAP_UPRIGHT_FOR_LAYBACK))
            {
                nextPosition='l';
            }
        }
    }
    if(nextPosition=='\0') //safeguard
        throw;
    currentSegment->spinPositions.push_back(SpinPosition(currentSegment,nextPosition));
}
void SpinSpinner::setRandomBaseQualities()
{
    double changeFootSpinProb;
    double flyingFootSpinProb;
    if(currentSpin.baseType=='c')
    {
        changeFootSpinProb = CAMEL_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = CAMEL_FLYINGSPIN_PROB;
    }
    else if(currentSpin.baseType=='s')
    {
        changeFootSpinProb = SIT_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = SIT_FLYINGSPIN_PROB;
    }
    else if(currentSpin.baseType=='u')
    {
        changeFootSpinProb = UPRIGHT_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = UPRIGHT_FLYINGSPIN_PROB;
    }
    else if(currentSpin.baseType=='l')
    {
        changeFootSpinProb = LAYBACK_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = LAYBACK_FLYINGSPIN_PROB;
    }
    else if(currentSpin.baseType=='k')
    {
        changeFootSpinProb = COMBO_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = COMBO_FLYINGSPIN_PROB;
    }

    if(easyRandom::weightedTruth(changeFootSpinProb))
        currentSpin.isChangeFoot = true;
    else
        currentSpin.isChangeFoot = false;

    if(easyRandom::weightedTruth(flyingFootSpinProb))
        currentSpin.isFlying = true;
    else
        currentSpin.isFlying = false;



}
bool SpinSpinner::addLevel()
{
    if(currentSpin.level>=4)
        return false;
    //adult rules: for bronze "all three basic positions is the only measure for getting level 1 in combo spins"
    else if(adultRuleFlags.bronze && currentSpin.baseType == 'k')
        currentSpin.features.allThreeBasicPositionsAnywhere = true;
    else
    {
        if(currentSpin.level==3 && missingBulletForLevel4() && adultRuleFlags.intermediate_novice==false) //if missing the required bullets for level 4 and the next level is 4 then add from the required level 4 bullets (note: adult levels intermediate-novice doesn't need a required bullet to get level 4)
        {
            addARequiredBulletForLevel4();
        }
        else
        {
            int count = 0;
            while(true) //keep looping until the rolled "spin addition" (variation/features) doesn't conflict with any other additions (lazy implementation)
            {
                if(count>100)
                    return false; //after 100 iterations, assume there is no valid bullet that can be added
                int randomSelect = pickRandomBulletType();
                if(randomSelect==0)
                {
                    if(addVariation())
                        break;
                    else
                        continue;
                }
                else if(randomSelect==1)
                {
                    if(addSpinFeature())
                        break;
                    else
                        continue;
                }
                else if(randomSelect==2)
                {
                    if(addPositionFeature())
                        break;
                    else
                        continue;
                }
                else if(randomSelect==3) //only for combos
                {
                    if(addIntermediatePosition())
                        break;
                    else
                        continue;
                }
                else if(randomSelect==4) //only for change foot spins
                {
                    if(addChangeOfDirection())
                        break;
                    else
                        continue;
                }
                else
                    throw; //implies that randomSelect is outside of possible spin additions (this would be a bug)
                count++;
            }
        }
    }
    currentSpin.level++;
    return true;
}
bool SpinSpinner::addVariation()
{
    SpinPosition* randomPosition = pickNonConflictingPosition();
    char randomVariation = randomPosition->pickRandomVariation();
    if(currentSpin.level==0)
    {
        randomPosition->addVariation(randomVariation,normalize);
        return true;
    }
    else //check for rule conflicts
    {
        if(!easyRandom::weightedTruth(VARIATION_ON_SAME_POSITION_PROB) && !randomPosition->variations.empty()) //reduce the chance of stacking variations on the same position
            return false;
        if(currentSpin.twoVariationsFlag &&
            adultRuleFlags.intermediate_novice==false &&
            adultRuleFlags.gold==false) //adult note: if intermediate-novice or gold then the two variation max is lifted
            return false;
        else
        {
            if(currentSpin.variationUsed(randomPosition->position,randomVariation))
                return false;
            else
            {
                if(randomPosition->addVariation(randomVariation,normalize))
                {
                    //*special exception: used to balance out lack of 8rev features in sit (and combo) spins (due to the requirment that the position have a DV)
                    if(randomPosition->position=='s' && randomPosition->hasAnyFeature())
                    {
                        if((targetLevel==2 && easyRandom::weightedTruth(0.6)) ||
                            (targetLevel==3 && easyRandom::weightedTruth(0.2)))
                        {
                            if(adultRuleFlags.gold || adultRuleFlags.silver || adultRuleFlags.bronze)
                                randomPosition->features.at(0) = '5';
                            else
                                randomPosition->features.at(0) = '8';
                        }
                    }
                    //*

                    if(currentSpin.hasTwoVariations())
                        currentSpin.twoVariationsFlag = true;
                    return true;
                }
                return false;
            }
        }
    }
}
bool SpinSpinner::addSpinFeature()
{
    int randomSelect;
    int tempLowerRandomRange = 1;
    int tempUpperRandomRange = 2;
    if(currentSpin.isChangeFoot)
        tempLowerRandomRange--;


    randomSelect = easyRandom::range(tempLowerRandomRange,tempUpperRandomRange);
    if(randomSelect==0) //changeFootByJump
    {
        if(currentSpin.features.changeFootByJump || shouldAvoidChangeFootByJump())
            return false;
        currentSpin.features.changeFootByJump = true;
        return true;
    }
    else if(randomSelect==1) //difficultEntrance
    {
        if((!currentSpin.features.difficultExit || currentSpin.isFlying) &&
            !currentSpin.features.difficultEntrance)
        {
            currentSpin.features.difficultEntrance = true;
            return true;
        }
    }
    else if(randomSelect==2) //difficultExit
    {
        if(!currentSpin.features.difficultExit &&
            (!currentSpin.features.difficultEntrance || currentSpin.isFlying))
        {
            currentSpin.features.difficultExit = true;
            return true;
        }
    }
    return false;
}
bool SpinSpinner::addPositionFeature()
{
    SpinPosition* randomPosition = pickNonConflictingPosition();
    char randomFeature;
    if(adultRuleFlags.gold || adultRuleFlags.silver || adultRuleFlags.bronze) //tell pickRandomFeature to switch to 5 rev instead of 8
        randomFeature = randomPosition->pickRandomFeature(true);
    else
        randomFeature = randomPosition->pickRandomFeature(false);

    if(currentSpin.featureUsed(randomFeature))
        return false;
    if(!checkFeatureValidity(randomPosition,randomFeature))
        return false;
    if(randomPosition->addFeature(randomFeature,normalize))
        return true;
    return false;
}
bool SpinSpinner::addIntermediatePosition()
{
    if(currentSpin.intermediatePositionFlag)
        return false;
    int randomSegmentIndex = easyRandom::range(0,currentSpin.spinSegments.size()-1);
    SpinSegment* randomSegment = &currentSpin.spinSegments.at(randomSegmentIndex);
    int randomSpinPositionIndex = easyRandom::range(0,randomSegment->spinPositions.size()-1);
    randomSegment->spinPositions.insert(randomSegment->spinPositions.begin()+randomSpinPositionIndex+1,SpinPosition(randomSegment,'i'));
    currentSpin.intermediatePositionFlag = true;
    return true;
}
bool SpinSpinner::addChangeOfDirection()
{
    if(currentSpin.changeDirectionFlag) //implies that change of direction has already been added to this spin
        return false;
    SpinPosition* lastPositionOnFirstSegment = &currentSpin.spinSegments.at(0).spinPositions.at(currentSpin.spinSegments.at(0).spinPositions.size()-1);
    SpinPosition* firstPositionOnSecondSegment = &currentSpin.spinSegments.at(1).spinPositions.at(0);
    if(adultRuleFlags.gold || adultRuleFlags.silver || adultRuleFlags.bronze) //adult note: doesn't need DV for change of direction to count
    {
        if(lastPositionOnFirstSegment->position=='l'||firstPositionOnSecondSegment->position=='l') //change of direction doesn't count for laybacks in adult rules for some reason? (possibily a typo or was overlooked when rule written)
            return false;
    }
    //check for difficult variation included in upright spin (needed for change of direction feature to count)
    else
    {
        if(lastPositionOnFirstSegment->position=='u' &&
            lastPositionOnFirstSegment->hasAnyVariation()==false)
            return false;
        if(firstPositionOnSecondSegment->position=='u' &&
            firstPositionOnSecondSegment->hasAnyVariation()==false)
            return false;
    }
    if(normalize||easyRandom::range(0,1)) //Normalize note: if footness is changed during a change of direction it means you are still spinning on the same foot on the second segment. This raises concerns about if counts as a change foot spin and even just execution seems awkward.
        currentSpin.spinSegments.at(1).swapFootness(); //makes segment 1 footness the same as segment 0
    if(easyRandom::range(0,1))
        currentSpin.spinSegments.at(0).swapDirection();
    else
        currentSpin.spinSegments.at(1).swapDirection();
    currentSpin.changeDirectionFlag = true;
    return true;
}
int SpinSpinner::pickRandomBulletType()
{
    //0: difficult variation, 1: spin feature, 2: position feature, 3: intermediate position 4: change of direction
    int select = -1;
    std::vector<int> selectFromVector;
    std::vector<double> selectVectorWeights;

    /*--TEMP--
    //special exception: used to balance out lack of 8rev features in level 2 sit (and combo) spins (due to the requirment that the position have a DV)
    if((currentSpin.baseType=='s'||currentSpin.baseType=='k') && (targetLevel==2 && currentSpin.level==0)) //if first bullet of level 2 sit or combo
        return 0; //select a DV for first bullet
        */

    if(currentSpin.baseType=='k')
    {
        if(currentSpin.isChangeFoot)
        {
            selectFromVector = std::vector<int>{0,1,2,3,4};
            selectVectorWeights = std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB,ADD_CHANGE_OF_DIRECTION_PROB};
        }
        else
        {
            selectFromVector = std::vector<int>{0,1,2,3};
            selectVectorWeights = std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB};
        }
    }
    else
    {
        if(currentSpin.isChangeFoot)
        {
            selectFromVector = std::vector<int>{0,1,2,4};
            selectVectorWeights = std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_CHANGE_OF_DIRECTION_PROB};
        }
        else
        {
            selectFromVector = std::vector<int>{0,1,2};
            selectVectorWeights = std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB};
        }
    }
    return easyRandom::pickFromVectorWeighted(selectFromVector, selectVectorWeights);
}
SpinPosition* SpinSpinner::pickNonConflictingPosition()
{
    SpinPosition* nonConflictingPosition = nullptr;
    do
    {
        if(!currentSpin.isChangeFoot) //not change foot spin
        {
            int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(0).spinPositions.size()-1);
            nonConflictingPosition = &currentSpin.spinSegments.at(0).spinPositions.at(randomIndex);
        }
        else //is change foot spin (need to check for only 2 bullets on each foot)
        {
            int bulletsOnFirstSegment = currentSpin.spinSegments.at(0).getBulletCount();
            int bulletsOnSecondSegment = currentSpin.spinSegments.at(1).getBulletCount();
            if(adultRuleFlags.active)
            {
                if(currentSpin.features.cleanChangeFootSpin) //awarded to second foot
                    bulletsOnSecondSegment++;
            }

            //special exception: if by accident a difficult change of position occurs on both segments then only count first one
            if(currentSpin.spinSegments.at(0).features.difficultChangeOfPosition && currentSpin.spinSegments.at(1).features.difficultChangeOfPosition)
                bulletsOnSecondSegment--;

            if(bulletsOnFirstSegment==2 && bulletsOnSecondSegment<2) //1. 2 bullets on first side
            {
                int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(1).spinPositions.size()-1);
                nonConflictingPosition = &currentSpin.spinSegments.at(1).spinPositions.at(randomIndex);
            }
            else if(bulletsOnFirstSegment<2 && bulletsOnSecondSegment==2) //2. 2 bullets on second side
            {
                int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(0).spinPositions.size()-1);
                nonConflictingPosition = &currentSpin.spinSegments.at(0).spinPositions.at(randomIndex);
            }
            else if(bulletsOnFirstSegment<2 && bulletsOnSecondSegment<2) //3. less than 2 bullets on both sides
            {
                int randomSegmentIndex = easyRandom::range(0,1);
                int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(randomSegmentIndex).spinPositions.size()-1);
                nonConflictingPosition = &currentSpin.spinSegments.at(randomSegmentIndex).spinPositions.at(randomIndex);
            }
            else
            {
                throw; //when adding levels properly there should never be more than 2 bullets on each foot and 4 bullets total in change foot spins
            }
        }
    } while(nonConflictingPosition->position=='i'); //this while loop ensures that intermediate positions aren't selected for any features (better implementation probably exists)
    return nonConflictingPosition;
}
bool SpinSpinner::missingBulletForLevel4()
{
    if(currentSpin.features.difficultExit||                 //1. difficult exit
        currentSpin.featureUsed('c')||                      //2. change of edge
        currentSpin.changeDirectionFlag||                   //3. opposite directions immediately following each other
        currentSpin.featureUsed('s')||                      //4. clear increase in speed
        (currentSpin.isFlying && currentSpin.features.difficultEntrance)|| //5. difficult variation of flying entry
        currentSpin.featureUsed('w'))                       //6. windmill
    {
        return false;
    }
    return true;
}
void SpinSpinner::addARequiredBulletForLevel4()
{
    while(true) //keep looping until the rolled spin "bullet" (variation/features) doesn't conflict with any other additions (lazy implementation)
    {
        int randomSelect = easyRandom::pickFromVector(std::vector<int>{0,1,2,3,4,5});

        if(randomSelect==0) //difficult exit
        {
            if(!currentSpin.features.difficultEntrance)
                continue;
            currentSpin.features.difficultExit = true;
            break;
        }
        else if(randomSelect==1) //change of edge
        {
            SpinPosition* randomPosition = pickNonConflictingPosition();
            if(!checkFeatureValidity(randomPosition,'c'))
                continue;
            if(randomPosition->addFeature('c',normalize))
                break;
        }
        else if(randomSelect==2) //change of direction
        {
            //check for difficult variation included in upright spin (needed for change of direction feature to count)
            if(currentSpin.baseType=='u')
            {
                if(currentSpin.spinSegments.at(0).getVariationCount()==0)
                    continue;
            }
            if(currentSpin.spinSegments.size()<2)
                continue;
            else
            {
                if(easyRandom::range(0,1))
                    currentSpin.spinSegments.at(0).swapDirection();
                else
                    currentSpin.spinSegments.at(1).swapDirection();
                if(easyRandom::range(0,1))
                    currentSpin.spinSegments.at(1).swapFootness();
                currentSpin.changeDirectionFlag = true;
                break;
            }
        }
        else if(randomSelect==3) //clear increase of speed
        {
            SpinPosition* randomPosition = pickNonConflictingPosition();
            if(!checkFeatureValidity(randomPosition,'s'))
                continue;
            if(randomPosition->addFeature('s',normalize))
                break;
        }
        else if(randomSelect==4) //difficult variation of flying entry
        {
            if(!currentSpin.isFlying)
                continue;
            currentSpin.features.difficultEntrance = true;
            break;
        }
        else if(randomSelect==5) //windmill
        {
            SpinPosition* randomPosition = pickNonConflictingPosition();
            if(!checkFeatureValidity(randomPosition,'w'))
                continue;
            if(randomPosition->addFeature('w',normalize))
                break;
        }
    }
}
bool SpinSpinner::checkFeatureValidity(SpinPosition* spinPosition, char featureInQuestion)
{
    if(adultRuleFlags.active) //if adult rules are being used redirect to below function
        return checkFeatureValidityAdult(spinPosition,featureInQuestion);


    if(spinPosition->position=='u')
    {
        if(spinPosition->variations.empty())
        {
            if(featureInQuestion=='c') //coe on base upright not counted as level
                return false;
            if(featureInQuestion=='s') //speed on base upright not counted as level
                return false;
            if(featureInQuestion=='8') //8rev on base upright not counted as level
                return false;
            if(featureInQuestion=='b') //blade on base upright not counted as level
                return false;
        }
        if(spinPosition->hasVariation('b') && featureInQuestion=='s') //speed on cross foot not counted as level
            return false;
    }
    else if(spinPosition->position=='s')
    {
        if(spinPosition->variations.empty() && featureInQuestion=='8')
            return false;
        if(spinPosition->parent->footness=='b' && featureInQuestion=='c') //backward outside to forward inside not allow for sit position (determined to be "too easy")
            return false;
    }
    if(spinPosition->position!='c' && featureInQuestion=='w')
        return false;
    if(!easyRandom::weightedTruth(FEATURE_ON_SAME_POSITION_PROB) && !spinPosition->features.empty()) //reduce the chance of stacking features on the same position
        return false;
    return true;
}
bool SpinSpinner::checkFeatureValidityAdult(SpinPosition* spinPosition, char featureInQuestion)
{
    if(spinPosition->position=='u')
    {
        if(spinPosition->variations.empty())
        {
            if(featureInQuestion=='c') //coe on base upright not counted as level
                return false;
            if(featureInQuestion=='s') //speed on base upright not counted as level
                return false;
            if(featureInQuestion=='8') //8rev on base upright not counted as level
                return false;
            if(featureInQuestion=='b') //blade on base upright not counted as level
                return false;
        }
        if(spinPosition->hasVariation('b') && featureInQuestion=='s') //speed on cross foot not counted as level
            return false;
    }
    else if(spinPosition->position=='s')
    {
        if(spinPosition->parent->footness=='b' && featureInQuestion=='c') //backward outside to forward inside not allow for sit position (determined to be "too easy")
            return false;
    }
    if(spinPosition->position!='c' && featureInQuestion=='w')
        return false;
    if(!easyRandom::weightedTruth(FEATURE_ON_SAME_POSITION_PROB) && !spinPosition->features.empty()) //reduce the chance of stacking features on the same position
        return false;
    return true;
}
bool SpinSpinner::shouldAvoidChangeFootByJump()
{
    if(!currentSpin.isChangeFoot) //safeguard
        return true;
    else
    {
        SpinSegment* firstSegment = &currentSpin.spinSegments.at(0);
        SpinSegment* secondSegment = &currentSpin.spinSegments.at(1);
        char lastPositionOnFirstSegment = firstSegment->spinPositions.at(firstSegment->spinPositions.size()-1).position;
        char firstPositionOnSecondSegment = secondSegment->spinPositions.at(0).position;

        if(firstPositionOnSecondSegment!='c' && firstPositionOnSecondSegment!='s') //according to updated rules change of foot by jump must go into camel or sit position
            return true;

        if(normalize)
        {
            if(lastPositionOnFirstSegment!='c' && firstPositionOnSecondSegment=='c') //jumping from any other position onto a camel is probably awkward
                return true;
            if(firstSegment->direction!=secondSegment->direction) //no change of foot by jump if changing rotational direction
                return true;
            if(lastPositionOnFirstSegment!='s' && firstSegment->footness=='b') //no change of foot by jump from backspin to forward spin unless sit spin
                return true;
        }
    }
    return false;
}
std::string SpinSpinner::spinHistoryToCode()
{
    std::string historyString = "";
    for(size_t i=0;i<spinHistory.size();i++)
    {
        historyString += spinHistory.at(i).toCode()+"\n";
    }
    return historyString;
}
std::string SpinSpinner::spinHistoryPrettyPrint()
{
    std::string historyString = "";
    for(size_t i=0;i<spinHistory.size();i++)
    {
        historyString += spinHistory.at(i).prettyPrint()+"\n";
    }
    return historyString;
}
