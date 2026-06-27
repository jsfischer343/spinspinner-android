#pragma once
#ifndef RANDOM_HH_
#define RANDOM_HH_

#include <cstdlib>
#include <chrono>
#include <vector>
#include <random>
#include <utility>

namespace easyRandom
{
    inline int range(const int min, const int max)
    {
        srand(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        std::mt19937 mt19937Obj;
        std::uniform_int_distribution<std::mt19937::result_type> udist(min, max);
        std::mt19937::result_type const seedval = rand();
        mt19937Obj.seed(seedval);
        return udist(mt19937Obj);
    }

    inline std::pair<int, int> binaryPair()
    {
        std::pair<int,int> randomPair;
        if(range(0,1))
        {
            randomPair.first = 1;
            randomPair.second = 0;
        }
        else
        {
            randomPair.first = 0;
            randomPair.second = 1;
        }
        return randomPair;
    }

    inline int pickFromVector(const std::vector<int>& vectorToPickFrom)
    {
        if(vectorToPickFrom.size()==0)
            return 0;
        return vectorToPickFrom.at(range(0,vectorToPickFrom.size()-1));
    }

    inline char pickFromVector(const std::vector<char>& vectorToPickFrom)
    {
        if(vectorToPickFrom.size()==0)
            return 0;
        return vectorToPickFrom.at(range(0,vectorToPickFrom.size()-1));
    }

    inline std::string pickFromVector(const std::vector<std::string>& vectorToPickFrom)
    {
        if(vectorToPickFrom.size()==0)
            return 0;
        return vectorToPickFrom.at(range(0,vectorToPickFrom.size()-1));
    }

    inline int pickFromVectorWeighted(const std::vector<int>& vectorToPickFrom, const std::vector<int>& vectorOfWeights)
    {
        if(vectorToPickFrom.size()==0 || vectorOfWeights.size()==0 || (vectorToPickFrom.size()!=vectorToPickFrom.size()))
            throw;

        int sumOfWeights = 0;
        std::vector<double> sequentialVectorOfAbsoluteWeightMarks;
        for(size_t i=0;i<vectorOfWeights.size();i++)
        {
            sumOfWeights += vectorOfWeights.at(i);
        }
        for(size_t i=0;i<vectorOfWeights.size();i++)
        {
            sequentialVectorOfAbsoluteWeightMarks.push_back(((double)vectorOfWeights.at(i)/sumOfWeights));
            if(i!=0)
                sequentialVectorOfAbsoluteWeightMarks.at(i) += sequentialVectorOfAbsoluteWeightMarks.at(i-1);
        }

        double randomLessThanOne = (double)range(0,RAND_MAX)/RAND_MAX;
        if(randomLessThanOne>=0 && randomLessThanOne<=sequentialVectorOfAbsoluteWeightMarks.at(0))
            return vectorToPickFrom.at(0);
        for(size_t i=0;i<sequentialVectorOfAbsoluteWeightMarks.size()-1;i++)
        {
            if(randomLessThanOne>sequentialVectorOfAbsoluteWeightMarks.at(i) && randomLessThanOne<=sequentialVectorOfAbsoluteWeightMarks.at(i+1))
                return vectorToPickFrom.at(i+1);
        }
        return -1;
    }

    inline int pickFromVectorWeighted(const std::vector<int>& vectorToPickFrom, const std::vector<double>& vectorOfWeights)
    {
        if(vectorToPickFrom.size()==0 || vectorOfWeights.size()==0 || (vectorToPickFrom.size()!=vectorToPickFrom.size()))
            throw;

        double sumOfWeights = 0;
        std::vector<double> sequentialVectorOfAbsoluteWeightMarks;
        for(size_t i=0;i<vectorOfWeights.size();i++)
        {
            sumOfWeights += vectorOfWeights.at(i);
        }
        for(size_t i=0;i<vectorOfWeights.size();i++)
        {
            sequentialVectorOfAbsoluteWeightMarks.push_back((vectorOfWeights.at(i)/sumOfWeights));
            if(i!=0)
                sequentialVectorOfAbsoluteWeightMarks.at(i) += sequentialVectorOfAbsoluteWeightMarks.at(i-1);
        }

        double randomLessThanOne = (double)range(0,RAND_MAX)/RAND_MAX;
        if(randomLessThanOne>=0 && randomLessThanOne<=sequentialVectorOfAbsoluteWeightMarks.at(0))
            return vectorToPickFrom.at(0);
        for(size_t i=0;i<sequentialVectorOfAbsoluteWeightMarks.size()-1;i++)
        {
            if(randomLessThanOne>sequentialVectorOfAbsoluteWeightMarks.at(i) && randomLessThanOne<=sequentialVectorOfAbsoluteWeightMarks.at(i+1))
                return vectorToPickFrom.at(i+1);
        }
        return -1;
    }

    inline int pickFromVectorWeighted(const std::vector<char>& vectorToPickFrom, const std::vector<double>& vectorOfWeights)
    {
        if(vectorToPickFrom.size()==0 || vectorOfWeights.size()==0 || (vectorToPickFrom.size()!=vectorToPickFrom.size()))
            throw;

        double sumOfWeights = 0;
        std::vector<double> sequentialVectorOfAbsoluteWeightMarks;
        for(size_t i=0;i<vectorOfWeights.size();i++)
        {
            sumOfWeights += vectorOfWeights.at(i);
        }
        for(size_t i=0;i<vectorOfWeights.size();i++)
        {
            sequentialVectorOfAbsoluteWeightMarks.push_back((vectorOfWeights.at(i)/sumOfWeights));
            if(i!=0)
                sequentialVectorOfAbsoluteWeightMarks.at(i) += sequentialVectorOfAbsoluteWeightMarks.at(i-1);
        }

        double randomLessThanOne = (double)range(0,RAND_MAX)/RAND_MAX;
        if(randomLessThanOne>=0 && randomLessThanOne<=sequentialVectorOfAbsoluteWeightMarks.at(0))
            return vectorToPickFrom.at(0);
        for(size_t i=0;i<sequentialVectorOfAbsoluteWeightMarks.size()-1;i++)
        {
            if(randomLessThanOne>sequentialVectorOfAbsoluteWeightMarks.at(i) && randomLessThanOne<=sequentialVectorOfAbsoluteWeightMarks.at(i+1))
                return vectorToPickFrom.at(i+1);
        }
        return -1;
    }

    inline bool weightedTruth(double weightOfTrue)
    {
        if(weightOfTrue>1 || weightOfTrue<0)
            throw;
        return (bool)pickFromVectorWeighted(std::vector<int>{0,1},std::vector<double>{1-weightOfTrue,weightOfTrue});
    }
}
#endif
