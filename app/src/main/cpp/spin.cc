#include "spin.hh"

bool Spin::hasAllPrimaryPositions() const
{
    bool hasCamel = false;
    bool hasSit = false;
    bool hasUpright = false;
    bool hasLayback = false;
    for(size_t i=0;i<spinSegments.size();i++)
    {
        for(size_t j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            if(spinSegments.at(i).spinPositions.at(j).position=='c')
                hasCamel = true;
            else if(spinSegments.at(i).spinPositions.at(j).position=='s')
                hasSit = true;
            else if(spinSegments.at(i).spinPositions.at(j).position=='u')
                hasUpright = true;
            else if(spinSegments.at(i).spinPositions.at(j).position=='l')
                hasLayback = true;
        }
    }
    if(hasCamel && hasSit && (hasUpright||hasLayback))
        return true;
    return false;
}
int Spin::getTotalPositions() const
{
    int sum = 0;
    for(size_t i=0;i<spinSegments.size();i++)
    {
        sum += spinSegments.at(i).spinPositions.size();
    }
    return sum;
}
bool Spin::variationUsed(char positionChar, char variationChar) const
{
    for(size_t i=0;i<spinSegments.size();i++)
    {
        for(size_t j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            for(size_t k=0;k<spinSegments.at(i).spinPositions.at(j).variations.size();k++)
            {
                if(spinSegments.at(i).spinPositions.at(j).position==positionChar && spinSegments.at(i).spinPositions.at(j).variations.at(k)==variationChar)
                    return true;
            }
        }
    }
    return false;
}
bool Spin::featureUsed(char featureChar) const
{
    for(size_t i=0;i<spinSegments.size();i++)
    {
        for(size_t j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            for(size_t k=0;k<spinSegments.at(i).spinPositions.at(j).features.size();k++)
            {
                if(spinSegments.at(i).spinPositions.at(j).features.at(k)==featureChar)
                    return true;
            }
        }
    }
    return false;
}
bool Spin::hasTwoVariations() const
{
    int variationCount = 0;
    for(size_t i=0;i<spinSegments.size();i++)
    {
        for(size_t j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            variationCount += spinSegments.at(i).spinPositions.at(j).variations.size();
        }
    }
    if(variationCount>=2)
        return true;
    return false;
}
bool Spin::hasDifficultChangeOfPosition() const
{
    for(size_t i=0;i<this->spinSegments.size();i++)
    {
        if(this->spinSegments.at(i).hasDifficultChangeOfPosition())
            return true;
    }
    return false;
}
std::string Spin::toCode() const
{
    std::string resultString = "";
    //level
    resultString += "L"+std::to_string(this->level)+":";
    if(this->baseType=='2')
    {
        if(this->spinSegments.at(0).direction=='r')
            resultString+="cc";
        else
            resultString+="c";
        resultString+="[2FtUSp]";
    }
    else
    {
        //flying modifier
        if(this->isFlying)
            resultString += "F";

        //first segment
        resultString += this->spinSegments.at(0).toCode();

        if(this->spinSegments.size()>1)
        {
            //transition
            if(this->features.changeFootByJump)
                resultString += "-j-";
            else
                resultString += "+";

            //second segment
            resultString += this->spinSegments.at(1).toCode();

            //any remaining segments?
            for(size_t i=2;i<spinSegments.size();i++)
            {
                resultString += this->spinSegments.at(i).toCode();
            }
        }

        //final modifiers
        if(this->features.difficultEntrance)
            resultString += "DE";
        if(this->features.difficultExit)
            resultString += "DX";

        //adult specific features
        if(this->features.cleanChangeFootSpin||this->features.allThreeBasicPositionsAnywhere)
            resultString += "CBP";
        else if(this->isChangeFoot)
        {
            if(this->spinSegments.at(1).features.allThreeBasicPositionsOnSecondFoot)
                resultString += "CBP2F";
        }
    }

    return resultString;
}
std::string Spin::prettyPrint() const
{
    std::string resultString = "";
    //level
    resultString += "Level "+std::to_string(this->level)+": ";
    if(this->baseType=='2')
    {
        if(this->spinSegments.at(0).direction=='r')
            resultString+="(ccw)";
        else
            resultString+="(cc)";
        resultString+="[ 2 foot upright ]";
    }
    else
    {
        //flying modifier
        if(this->isFlying)
            resultString += "flying ";

        //first segment
        resultString += this->spinSegments.at(0).prettyPrint();

        if(this->spinSegments.size()>1)
        {
            //transition
            if(this->features.changeFootByJump)
                resultString += " --jump-- ";
            else
                resultString += " + ";

            //second segment
            resultString += this->spinSegments.at(1).prettyPrint();

            //any remaining segments?
            for(size_t i=2;i<spinSegments.size();i++)
            {
                resultString += this->spinSegments.at(i).prettyPrint();
            }
        }

        //final modifiers
        if(this->features.difficultEntrance && this->features.difficultExit)
            resultString += " with difficult entrance & exit";
        else
        {
            if(this->features.difficultEntrance)
                resultString += " with difficult entrance";
            if(this->features.difficultExit)
                resultString += " with difficult exit";
        }

        //adult specific features
        if(this->features.cleanChangeFootSpin||this->features.allThreeBasicPositionsAnywhere)
            resultString += " with clean basic positions";
        else if(this->isChangeFoot)
        {
            if(this->spinSegments.at(1).features.allThreeBasicPositionsOnSecondFoot)
                resultString += " with clean basic positions all on second foot";
        }
    }

    return resultString;
}
