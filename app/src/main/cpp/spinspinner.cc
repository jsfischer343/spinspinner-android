#include "spinspinner.hh"
#include "easyrandom.hh"

SpinSpinner::SpinSpinner(bool defaultDirection, bool normalize)
{
    this->defaultDirection = defaultDirection;
    this->normalize = normalize;
}
void SpinSpinner::spin()
{
    targetLevel = easyRandom::range(0,4);
    currentSpin = Spin();

    int spinSelect = easyRandom::range(0,4);
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
        default:
            currentSpin.baseType = 'e';
    }

    setRandomBaseQualities(); //sets fly and change foot properties
    initializeBaseStructure();
    generateSpin();
}
void SpinSpinner::spin(int level)
{
    targetLevel = level;
    currentSpin = Spin();

    int spinSelect = easyRandom::range(0,4);
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
        default:
            currentSpin.baseType = 'e';
    }

    setRandomBaseQualities(); //sets fly and change foot properties
    initializeBaseStructure();
    generateSpin();
}
void SpinSpinner::spin(char type, int level)
{
    targetLevel = level;
    currentSpin = Spin();

    currentSpin.baseType = type;

    setRandomBaseQualities(); //sets fly and change foot properties
    initializeBaseStructure();
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
}
void SpinSpinner::generateSpinInOnePosition()
{
    //add levels
    for(int i=0;i<targetLevel;i++)
    {
        addLevel();
    }

    //record spin
    spinHistory.push_back(currentSpin);
}
void SpinSpinner::generateCombo()
{
    //pick first position
    char randomStartPosition;
    if(targetLevel<1)
        randomStartPosition = 'c';
    else
        randomStartPosition = easyRandom::pickFromVectorWeighted(std::vector<int>{'c','s','u'},std::vector<double>{COMBO_START_CAMEL_PROB,COMBO_START_SIT_PROB,COMBO_START_UPRIGHT_PROB});

    //push first segment and position
    currentSpin.spinSegments.at(0).spinPositions.push_back(SpinPosition(&currentSpin.spinSegments.at(0),randomStartPosition));

    //Logic to add the rest of the positions
    generateComboPositions();

    //check for difficult change of position
    bool hasDifficultChangeOfPosition = false;
    if(targetLevel>=1 && currentSpin.hasDifficultChangeOfPosition())
    {
        currentSpin.level++;
        hasDifficultChangeOfPosition = true;
    }
    //add levels
    for(int i=0;i<targetLevel-hasDifficultChangeOfPosition;i++) //difficult change of position counts as a level, so addLevel is called one less time
    {
        addLevel();
    }

    spinHistory.push_back(currentSpin);
}
void SpinSpinner::generateComboPositions()
{
    if(currentSpin.isChangeFoot)
    {
        bool endSpinBool = false; //randomly turned true to end the spin early (i.e. less than 6 positions)
        bool swappedFeet = false;

        while(currentSpin.getTotalPositions()<6 && !endSpinBool)
        {
            if(!swappedFeet)
            {
                //after each position, randomly decide if a change foot will take place early
                if(easyRandom::weightedTruth(0.4) || currentSpin.spinSegments.at(0).spinPositions.size()==3) //if already at three positions on first segement then must switch to next spin segement (i.e. change feet)
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
                generateComboPositions_addPosition(swappedFeet); //adding to second segment
                if(currentSpin.hasAllPrimaryPositions())
                {
                    if(currentSpin.spinSegments.at(1).spinPositions.size()==3) //must end if on second foot and done three positions
                    {
                        endSpinBool=true;
                    }
                    else if(easyRandom::weightedTruth(0.6)) //randomly decide to end second segement early (must have all primary positions)
                    {
                        endSpinBool=true;
                    }
                }
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

    if(currentSegment->spinPositions.size()==3 || currentSpin.spinSegments.size()>2) //If this is true then it would imply that we are trying to add a 4th basic position to the same foot OR add a third segment, which wouldn't make sense
    {
        return;
    }

    char nextPosition;
    if(targetLevel>=1) //combo spins level 1 or higher can have a difficult change of position so the logic is more lax (i.e. there are more posibilities)
    {
        std::vector<char> validPositions = {'c','s','u'};
        std::vector<char> usedPositions = currentSegment->getUsedPositions();
        std::vector<char> unusedPositions;

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

        //check and update difficult change of position flag
        if(!currentSegment->spinPositions.empty())
        {
            char previousPosition = currentSegment->spinPositions.at(currentSegment->spinPositions.size()-1).position;
            if((previousPosition=='s'||previousPosition=='u')&&nextPosition=='c')
                currentSegment->features.difficultChangeOfPosition = true;
        }
    }
    else //there is a small amount of base combos that can be rolled so the logic is a bit simpler. Note: intermediate positions aren't valid for a base spin
    {
        if(currentSpin.spinSegments.size()==2 && currentSegment->spinPositions.size()==0) //Is change of foot spin and this is the first position
        {
            SpinSegment* previousSegment = &currentSpin.spinSegments.at(currentSpin.spinSegments.size()-2);
            int previousIndex = previousSegment->spinPositions.size()-1;
            char previousPosition = previousSegment->spinPositions.at(previousIndex).position;
            if(easyRandom::range(0,1)) //50/50 to repeat position on previous foot or change
            {
                if(previousPosition=='c')
                    nextPosition = 'c';
                else if(previousPosition=='s')
                    nextPosition = 's';
                else if(previousPosition=='u')
                    nextPosition = 'u';
            }
            else //
            {
                if(previousPosition=='c')
                    nextPosition = 's';
                else if(previousPosition=='s')
                    nextPosition = 'u';
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
    }
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
void SpinSpinner::initializeBaseStructure()
{
    char startingFootness;
    char otherFootness;

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

    if(currentSpin.isChangeFoot)
    {
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,startingFootness));
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,otherFootness));
    }
    else
        currentSpin.spinSegments.push_back(SpinSegment(defaultDirection,startingFootness));

    if(currentSpin.baseType!='k')
    {
        for(size_t i=0;i<currentSpin.spinSegments.size();i++)
        {
            SpinSegment* currentSegment = &currentSpin.spinSegments.at(i);
            currentSegment->spinPositions.push_back(SpinPosition(currentSegment,currentSpin.baseType));
        }
    }
}
void SpinSpinner::addLevel()
{
    if(currentSpin.level==0)
    {
        int randomSelect = pickRandomBulletType();
        if(randomSelect==0)
        {
            addVariation();
        }
        else if(randomSelect==1) //add spin/segment feature
        {
            addSpinFeature();
        }
        else if(randomSelect==2)
        {
            addPositionFeature();
        }
        else if(randomSelect==3) //(only for combos)
        {
            addIntermediatePosition();
        }
        else if(randomSelect==4) //(only for change foot spins)
        {
            addChangeOfDirection();
        }
        currentSpin.level++;
        return;
    }
    else //need to check for conflicts with features already in the spin
    {
        if(currentSpin.level==3 && missingBulletForLevel4()) //if missing the required bullets for level 4 and the next level is 4 then add from the required level 4 bullets
        {
            addARequiredBulletForLevel4();
        }
        else
        {
            while(true) //keep looping until the rolled "spin addition" (variation/features) doesn't conflict with any other additions (lazy implementation)
            {
                int randomSelect = pickRandomBulletType(); //of the categories spin feature, position feature, intermediate position, change of direction
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
                else if(randomSelect==3) //only occurs in combo spins (see pickRandomAddition sub-function for logic)
                {
                    if(addIntermediatePosition())
                        break;
                    else
                        continue;
                }
                else if(randomSelect==4) //change of direction (only for change foot spins)
                {
                    if(addChangeOfDirection())
                        break;
                    else
                        continue;
                }
            }
        }
        currentSpin.level++;
    }
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
        if(currentSpin.twoVariationsFlag)
            return false;
        if(!easyRandom::weightedTruth(VARIATION_ON_SAME_POSITION_PROB) && !randomPosition->variations.empty()) //reduce the chance of stacking variations on the same position
            return false;
        else
        {
            if(currentSpin.variationUsed(randomPosition->position,randomVariation))
                return false;
            else
            {
                if(randomPosition->addVariation(randomVariation,normalize))
                {
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
    if(currentSpin.baseType=='l')
        tempUpperRandomRange = 3;

    randomSelect = easyRandom::range(tempLowerRandomRange,tempUpperRandomRange);
    if(randomSelect==0 && !currentSpin.features.changeFootByJump) //changeFootByJump
    {
        if(shouldAvoidChangeFootByJump()) //avoid change foot by jump if there is change of direction or awkward positions
            return false;
        currentSpin.features.changeFootByJump = true;
        return true;
    }
    else if(randomSelect==1 &&
    (!currentSpin.features.difficultExit || currentSpin.isFlying) &&
    !currentSpin.features.difficultEntrance) //difficultEntrance
    {
        currentSpin.features.difficultEntrance = true;
        return true;
    }
    else if(randomSelect==2 &&
    !currentSpin.features.difficultExit &&
    (!currentSpin.features.difficultEntrance || currentSpin.isFlying)) //difficultExit
    {
        currentSpin.features.difficultExit = true;
        return true;
    }
    return false;
}
bool SpinSpinner::addPositionFeature()
{
    SpinPosition* randomPosition = pickNonConflictingPosition();
    char randomFeature = randomPosition->pickRandomFeature();
    if(randomFeature=='w' && !easyRandom::weightedTruth(WINDMILL_PROB))
        return false;
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
    if(currentSpin.changeDirectionFlag)
        return false;
    if(easyRandom::range(0,1))
        currentSpin.spinSegments.at(0).swapDirection();
    else
        currentSpin.spinSegments.at(1).swapDirection();
    if(easyRandom::range(0,1))
        currentSpin.spinSegments.at(1).swapFootness();
    currentSpin.changeDirectionFlag = true;
    return true;
}
int SpinSpinner::pickRandomBulletType()
{
    //0: difficult variation, 1: spin feature, 2: position feature, 3: intermediate position 4: change of direction
    if(currentSpin.baseType=='k')
    {
        if(currentSpin.isChangeFoot)
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,3,4},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB,ADD_CHANGE_OF_DIRECTION_PROB});
        else
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,3},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB});
    }
    else
    {
        if(currentSpin.isChangeFoot)
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,4},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_CHANGE_OF_DIRECTION_PROB});
        else
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB});
    }
}
SpinPosition* SpinSpinner::pickNonConflictingPosition()
{
    if(!currentSpin.isChangeFoot) //not change foot spin
    {
        int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(0).spinPositions.size()-1);
        return &currentSpin.spinSegments.at(0).spinPositions.at(randomIndex);
    }
    else //is change foot spin (need to check for only 2 bullets on each foot)
    {
        int bulletsOnFirstSegment = currentSpin.spinSegments.at(0).getBullets();
        int bulletsOnSecondSegment = currentSpin.spinSegments.at(1).getBullets();

        if(bulletsOnFirstSegment==2 && bulletsOnSecondSegment<2) //1. 2 bullets on first side
        {
            int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(1).spinPositions.size()-1);
            return &currentSpin.spinSegments.at(1).spinPositions.at(randomIndex);
        }
        else if(bulletsOnFirstSegment<2 && bulletsOnSecondSegment==2) //2. 2 bullets on second side
        {
            int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(0).spinPositions.size()-1);
            return &currentSpin.spinSegments.at(0).spinPositions.at(randomIndex);
        }
        else if(bulletsOnFirstSegment<2 && bulletsOnSecondSegment<2) //3. less than 2 bullets on both sides
        {
            int randomSegmentIndex = easyRandom::range(0,1);
            int randomIndex = easyRandom::range(0,currentSpin.spinSegments.at(randomSegmentIndex).spinPositions.size()-1);
            return &currentSpin.spinSegments.at(randomSegmentIndex).spinPositions.at(randomIndex);
        }
    }
}
bool SpinSpinner::missingBulletForLevel4()
{
    if(currentSpin.hasDifficultChangeOfPosition()||         //1. difficult change of position
        currentSpin.features.difficultExit||                //2. difficult exit
        currentSpin.featureUsed('c')||                      //3. change of edge
        currentSpin.changeDirectionFlag||                   //4. opposite directions immediately following each other
        currentSpin.featureUsed('s')||                      //5. clear increase in speed
        (currentSpin.isFlying && currentSpin.features.difficultEntrance)) //6. difficult variation of flying entry
    {
        return false;
    }
    return true;
}
void SpinSpinner::addARequiredBulletForLevel4()
{
    while(true) //keep looping until the rolled spin "bullet" (variation/features) doesn't conflict with any other additions (lazy implementation)
    {
        int randomSelect = easyRandom::pickFromVector(std::vector<int>{0,1,2,3,4});

        if(randomSelect==0)
        {
            if(currentSpin.features.difficultEntrance && !currentSpin.isFlying)
                continue;
            currentSpin.features.difficultExit = true;
            break;
        }
        else if(randomSelect==1)
        {
            SpinPosition* randomPosition = pickNonConflictingPosition();
            if(!checkFeatureValidity(randomPosition,'c'))
                continue;
            randomPosition->features.push_back('c');
            break;
        }
        else if(randomSelect==2)
        {
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
        else if(randomSelect==3)
        {
            SpinPosition* randomPosition = pickNonConflictingPosition();
            if(!checkFeatureValidity(randomPosition,'s'))
                continue;
            randomPosition->features.push_back('s');
            break;
        }
        else if(randomSelect==4)
        {
            if(!currentSpin.isFlying)
                continue;
            currentSpin.features.difficultEntrance = true;
            break;
        }
    }
}
bool SpinSpinner::checkFeatureValidity(SpinPosition* spinPosition, char featureInQuestion)
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
    if(spinPosition->position=='s')
    {
        if(spinPosition->variations.empty() && featureInQuestion=='8')
            return false;
        if(spinPosition->parent->footness=='b' && featureInQuestion=='b') //backward outside to forward inside not allow for sit position (determined to be "too easy")
            return false;
    }
    if(!easyRandom::weightedTruth(FEATURE_ON_SAME_POSITION_PROB) && !spinPosition->features.empty()) //reduce the chance of stacking features on the same position
        return false;
    return true;
}
bool SpinSpinner::shouldAvoidChangeFootByJump()
{
    if(!currentSpin.isChangeFoot) //safeguard
        return true;
    else if(normalize)
    {
        SpinSegment* firstSegment = &currentSpin.spinSegments.at(0);
        SpinSegment* secondSegment = &currentSpin.spinSegments.at(1);
        char lastPositionOnFirstSegment = firstSegment->spinPositions.at(firstSegment->spinPositions.size()-1).position;
        if(firstSegment->direction!=secondSegment->direction) //no change of foot by jump if changing rotational direction
            return true;
        if((lastPositionOnFirstSegment!='s'||lastPositionOnFirstSegment!='u') && currentSpin.spinSegments.at(0).footness=='b') //no change of foot by jump from backspin to forward spin (unless sit or upright?)
            return true;
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
