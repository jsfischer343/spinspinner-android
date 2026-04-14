#include "spinsegment.hh"

SpinSegment::SpinSegment(bool defaultDirection, char footness)
{
    this->footness = footness;
    if(defaultDirection)
        this->direction = 'l';
    else
        this->direction = 'r';
}
SpinSegment::SpinSegment(bool defaultDirection, char footness, std::vector<SpinPosition> spinPositions)
{
    this->footness = footness;
    this->spinPositions = spinPositions;
    if(defaultDirection)
        this->direction = 'l';
    else
        this->direction = 'r';
}
void SpinSegment::swapDirection()
{
    if(direction=='r')
        direction='l';
    else if(direction=='l')
        direction='r';
}
void SpinSegment::swapFootness()
{
    if(footness=='b')
        footness='f';
    else if(footness=='f')
        footness='b';
}
std::vector<char> SpinSegment::getUsedPositions() const
{
    std::vector<char> usedPositions;
    for(size_t i=0;i<spinPositions.size();i++)
    {
        usedPositions.push_back(spinPositions.at(i).position);
    }
    return usedPositions;
}
int SpinSegment::getBullets() const
{
    int sumOfBullets = 0;

    //count features and variations on spin positions
    for(size_t i=0;i<spinPositions.size();i++)
    {
        sumOfBullets += spinPositions.at(i).variations.size();
        sumOfBullets += spinPositions.at(i).features.size();
    }

    //count segement specific features
    if(features.difficultChangeOfPosition)
        sumOfBullets++;
    if(features.biellmannAfterLayback)
        sumOfBullets++;

    return sumOfBullets;
}
bool SpinSegment::hasDifficultChangeOfPosition() const
{
    for(size_t i=1;i<this->spinPositions.size();i++)
    {
        char previousPosition = this->spinPositions.at(i-1).position;
        char currentPosition = this->spinPositions.at(i).position;
        if(previousPosition=='s'||previousPosition=='u')
        {
            if(currentPosition=='c')
            {
                return true;
            }
        }
    }
    return false;
}
std::string SpinSegment::getDirectionString() const
{
    if(direction=='r')
        return "ccw";
    else if(direction=='l')
        return "cw";
    return "";
}
std::string SpinSegment::getFootnessString() const
{
    if(footness=='b')
        return "back";
    else if(footness=='f')
        return "forward";
    return "";
}
std::string SpinSegment::toCode() const
{
    std::string resultString = "";

    //direction
    if(this->direction=='r')
        resultString += "cc[";
    else if(this->direction=='l')
        resultString += "c[";

    //footness
    if(this->footness=='b')
        resultString += "Bw";
    else if(this->footness=='f')
        resultString += "Fw";

    for(size_t i=0;i<spinPositions.size();i++)
    {
        resultString += spinPositions.at(i).toCode();
        if(i!=spinPositions.size()-1)
            resultString += "+";
    }
    if(this->features.biellmannAfterLayback)
        resultString += "Bi";
    resultString += "]";

    return resultString;
}
std::string SpinSegment::prettyPrint() const
{
    std::string resultString = "";

    //direction
    if(this->direction=='r')
        resultString += "(ccw)[ ";
    else if(this->direction=='l')
        resultString += "(cw)[ ";

    //footness
    if(this->footness=='b')
        resultString += "back ";
    else if(this->footness=='f')
        resultString += "forward ";

    for(size_t i=0;i<spinPositions.size();i++)
    {
        resultString += spinPositions.at(i).prettyPrint();
        if(i!=spinPositions.size()-1)
            resultString += " + ";
    }
    if(this->features.biellmannAfterLayback)
        resultString += " -> biellmann";
    resultString += " ]";

    return resultString;
}
