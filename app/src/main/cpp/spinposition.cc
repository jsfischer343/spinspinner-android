#include "spinposition.hh"

SpinPosition::SpinPosition(SpinSegment* parent, char position)
{
    this->parent = parent;
    this->position = position;
}
bool SpinPosition::addVariation(char variation, bool normalize)
{
    if(variations.empty()||!normalize)
    {
        variations.push_back(variation);
        return true;
    }
    else //normalize the order of variations reduce spin awkwardness (this is subjective to some extent)
    {
        for(size_t i=0;i<variations.size();i++)
        {
            if(variations.at(i)=='u' && variation=='s') //side should come before up
            {
                variations.insert(variations.begin()+i,variation);
                return true;
            }
            if(variations.at(i)=='s' && variation=='b') //behind should come after side
            {
                variations.insert(variations.begin()+i,variation);
                return true;
            }
            if(variations.at(i)=='f' && variation=='t') //straight should come after front (only applies to upright)
            {
                variations.insert(variations.begin()+i,variation);
                return true;
            }
            if(variations.at(i)=='m') //biellmann should always be last
            {
                variations.insert(variations.begin()+i,variation);
                return true;
            }
            if((variations.at(i)=='s' && variation=='t')||(variations.at(i)=='t' && variation=='s')) //don't combine side and straight (only applies to upright)
                return false;
        }
        variations.push_back(variation);
        return true;
    }
}
bool SpinPosition::addFeature(char feature, bool normalize)
{
    if(!normalize)
    {
        features.push_back(feature);
        return true;
    }
    else
    {
        //features that shouldn't be in a given position type
        if(position=='c'){}
        else if(position=='s')
        {
            if(feature=='b')
                return false;
        }
        else if(position=='u'){}
        else if(position=='l')
        {
            if(feature=='b'||feature=='j')
                return false;
        }
        else if(position=='i')
        {
            if(feature=='b'||feature=='j')
                return false;
        }
        //features that shouldn't be combined together (subjective to some extent)
        if(!features.empty())
        {
            for(size_t i=0;i<features.size();i++)
            {
                if((features.at(i)=='s' && feature=='j') || (features.at(i)=='j' && feature=='s'))
                    return false;
                if(features.at(i)=='w' || feature=='w') //windmill shouldn't be combined with any other features
                    return false;
            }
        }
        features.push_back(feature);
        return true;
    }
}
bool SpinPosition::hasVariation(char variation) const
{
    for(size_t i=0;i<variations.size();i++)
    {
        if(variations.at(i)==variation)
            return true;
    }
    return false;
}
bool SpinPosition::hasFeature(char feature) const
{
    for(size_t i=0;i<features.size();i++)
    {
        if(features.at(i)==feature)
            return true;
    }
    return false;
}
bool SpinPosition::hasAnyVariation() const
{
    if(variations.size()>0)
        return true;
    return false;
}
bool SpinPosition::hasAnyFeature() const
{
    if(features.size()>0)
        return true;
    return false;
}
char SpinPosition::pickRandomFeature(bool isRev5) const
{
    std::vector<char> validFeatures;
    if(isRev5)
        validFeatures = {'b','c','j','5','s'};
    else
        validFeatures = {'b','c','j','8','s'};
    if(position=='c')
        validFeatures.push_back('w');
    std::vector<char> usedFeatures = features;
    std::vector<char> unusedFeatures;
    std::sort(validFeatures.begin(),validFeatures.end());
    std::sort(usedFeatures.begin(),usedFeatures.end());
    std::set_difference(validFeatures.begin(),validFeatures.end(),usedFeatures.begin(),usedFeatures.end(),std::back_inserter(unusedFeatures));

    std::vector<double> selectionWeights;
    for(int i=0;i<unusedFeatures.size();i++)
    {
        if(unusedFeatures.at(i)=='b')
            selectionWeights.push_back(POS_FEATURE_BLADE_PROB);
        else if(unusedFeatures.at(i)=='c')
            selectionWeights.push_back(POS_FEATURE_COE_PROB);
        else if(unusedFeatures.at(i)=='j')
            selectionWeights.push_back(POS_FEATURE_JUMP_PROB);
        else if(unusedFeatures.at(i)=='5'||unusedFeatures.at(i)=='8')
            selectionWeights.push_back(POS_FEATURE_REV_PROB);
        else if(unusedFeatures.at(i)=='s')
            selectionWeights.push_back(POS_FEATURE_SPEED_PROB);
    }

    if(unusedFeatures.empty())
        return -1;
    else
        return easyRandom::pickFromVectorWeighted(unusedFeatures,selectionWeights);
}
char SpinPosition::pickRandomVariation() const
{
    std::vector<char> validVariations;
    //for more details see "Singles Spin Difficult Variation Chart.pdf"
    if(position=='c')
    {
        validVariations = {'u','f','s'};
    }
    else if(position=='s')
    {
        validVariations = {'f','b','s'};
    }
    else if(position=='u')
    {
        if(this->hasVariation('s')||this->hasVariation('t')) //note: straight and sideways count as the same DV in upright spins
            validVariations = {'f','m'};
        else
            validVariations = {'s','f','t','m'};
    }
    else if(position=='l')
    {
        validVariations = {'s','b','m'};
    }
    else if(position=='i')
    {
        validVariations = {'u','f','b','s'};
    }
    else
        throw;
    return easyRandom::pickFromVector(validVariations);
}
std::string SpinPosition::toCode() const
{
    std::string resultString = "";
    resultString += getPositionString(true);
    resultString += getVariationString(true);
    if(!this->features.empty())
        resultString += "("+getFeatureString(true)+")";
    return resultString;
}
std::string SpinPosition::prettyPrint() const
{
    std::string resultString = "";
    resultString += getPositionString(false)+" ";
    if(variations.size()>0)
        resultString += getVariationString(false)+" ";
    if(!this->features.empty())
        resultString += "("+getFeatureString(false)+")";
    return resultString;
}
std::string SpinPosition::getPositionString(bool codeFormat) const
{
    if(codeFormat)
    {
        if(position=='c')
            return "C";
        else if(position=='s')
            return "S";
        else if(position=='u')
            return "U";
        else if(position=='l')
            return "L";
        else if(position=='i')
            return "I";
        return "";
    }
    else
    {
        if(position=='c')
            return "camel";
        else if(position=='s')
            return "sit";
        else if(position=='u')
            return "upright";
        else if(position=='l')
            return "layback";
        else if(position=='i')
            return "intermediate";
        return "";
    }
}
std::string SpinPosition::getVariationString(bool codeFormat) const
{
    std::string variationString = "";
    if(codeFormat)
    {
        for(size_t i=0;i<variations.size();i++)
        {
            if(variations.at(i)=='u')
                variationString += "Up";
            else if(variations.at(i)=='s')
                variationString += "Si";
            else if(variations.at(i)=='t')
                variationString += "St";
            else if(variations.at(i)=='f')
                variationString += "Fr";
            else if(variations.at(i)=='b')
                variationString += "Be";
            else if(variations.at(i)=='m')
                variationString += "Bi";
        }
    }
    else
    {
        for(size_t i=0;i<variations.size();i++)
        {
            if(variations.at(i)=='u')
                variationString += "up";
            else if(variations.at(i)=='s')
                variationString += "side";
            else if(variations.at(i)=='t')
                variationString += "straight";
            else if(variations.at(i)=='f')
                variationString += "front";
            else if(variations.at(i)=='b')
                variationString += "behind";
            else if(variations.at(i)=='m')
                variationString += "biellmann";
            if(i!=variations.size()-1)
                variationString += " -> ";
        }
    }
    return variationString;
}
std::string SpinPosition::getFeatureString(bool codeFormat) const
{
    std::string featureString = "";
    if(codeFormat)
    {
        for(size_t i=0;i<features.size();i++)
        {
            if(features.at(i)=='b')
                featureString += "Bl";
            else if(features.at(i)=='c')
                featureString += "Co";
            else if(features.at(i)=='j')
                featureString += "Ju";
            else if(features.at(i)=='8')
                featureString += "8";
            else if(features.at(i)=='s')
                featureString += "Sp";
            else if(features.at(i)=='w')
                featureString += "Wi";
            else if(features.at(i)=='5')
                featureString += "5";
        }
    }
    else
    {
        for(size_t i=0;i<features.size();i++)
        {
            if(features.at(i)=='b')
                featureString += "blade";
            else if(features.at(i)=='c')
                featureString += "coe";
            else if(features.at(i)=='j')
                featureString += "jump";
            else if(features.at(i)=='8')
                featureString += "8revs";
            else if(features.at(i)=='s')
                featureString += "speed";
            else if(features.at(i)=='w')
                featureString += "windmill";
            else if(features.at(i)=='5')
                featureString += "5 revs";
            if(i!=features.size()-1)
                featureString += ", ";
        }
    }
    return featureString;
}
