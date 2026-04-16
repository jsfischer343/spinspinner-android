package com.example.figureskatingspinspinner.data

class Spin(spinCode: String) {
    var baseType: Char = 'e'
    var isFlying: Boolean = false
    var isChangeFoot: Boolean = false
    var level: Int = -1
    var spinSegments: MutableList<SpinSegment> = mutableListOf()
    data class SpinFeatures(
        var changeFootByJump: Boolean = false,
        var difficultEntrance: Boolean = false,
        var difficultExit: Boolean = false
    )
    var spinFeatures = SpinFeatures()

    init { //the spinCode needs to be parsed into the spin object
        var tokens = spinCode.split(Regex("(?<=:|]|(DX)|(DE))|(?=[c\\[])")).toMutableList()
        var i = 1
        while(i<tokens.size) {
            if(tokens[i-1]=="c" && tokens[i-1]==tokens[i]) {
                tokens[i-1] = "cc"
                tokens.removeAt(i)
            }
            else i++
        }
        if(tokens.indexOf("")!=-1) tokens.removeAt(tokens.indexOf(""))

        if(tokens.isNotEmpty()) {
            if(tokens[0][0]=='L')
            {
                //Parse Level
                level = tokens[0][1].digitToInt();
                tokens.removeAt(0)
                //isFlying
                if(tokens[0]=="F") { isFlying = true; tokens.removeAt(0) }
                //Segments
                tokens = parseSegments(tokens)
                if(tokens.isNotEmpty()) if(tokens[0]=="DE") {
                    spinFeatures.difficultEntrance = true
                    tokens.removeAt(0)
                }
                if(tokens.isNotEmpty()) if(tokens[0]=="DX") { spinFeatures.difficultExit = true }
            }
        }
        if(isComboSpin())
            baseType = 'k'
        else {
            if(spinSegments.isNotEmpty()) {
                if(spinSegments[0].spinPositions.isNotEmpty())
                {
                    baseType = spinSegments[0].spinPositions[0].position
                }
            }
        }
    }
    private fun isComboSpin(): Boolean {
        var camelPositionCount: Int = 0
        var sitPositionCount: Int = 0
        var uprightPositionCount: Int = 0
        for(segment in spinSegments) {
            for(position in segment.spinPositions) {
                when(position.position) {
                    'c' -> camelPositionCount++
                    's' -> sitPositionCount++
                    'u' -> uprightPositionCount++
                    'i' -> return true
                    'l' -> return false
                }
            }
        }
        if(camelPositionCount>0 && sitPositionCount>0 && uprightPositionCount>0) return true
        else return false
    }
    private fun parseSegments(tokens: MutableList<String>): MutableList<String> {
        var tokens = tokens.toMutableList()
        var direction: Char = 'e'
        if(tokens[0]=="c") { direction = 'l' }
        else if(tokens[0]=="cc") { direction = 'r' }
        tokens.removeAt(0)
        spinSegments.add(SpinSegment(tokens[0],direction))
        tokens.removeAt(0)
        if(tokens.isNotEmpty())
        {
            if(tokens[0]=="-j-") {
                isChangeFoot = true
                spinFeatures.changeFootByJump = true
                tokens.removeAt(0)
                tokens = parseSegments(tokens)
            }
            else if(tokens[0]=="+") {
                isChangeFoot = true
                tokens.removeAt(0)
                tokens = parseSegments(tokens)
            }
        }
        return tokens
    }
    fun getISUSpinCodeAsText(): String {
        var returnString = ""
        if(baseType=='e') return ""
        if(isFlying) returnString += "F"
        if(isChangeFoot) returnString += "C"
        if(baseType=='k') returnString += "Co"
        else returnString += baseType.uppercaseChar()
        returnString += "Sp"
        if(level==0) returnString += "B"
        else returnString += level.toString()
        return returnString
    }
}

class SpinSegment(segmentCode: String, direction: Char) {
    var direction: Char = direction
    var footness: Char = 'e'
    var spinPositions: MutableList<SpinPosition> = mutableListOf()

    init {
        var segmentCode = segmentCode
        segmentCode = segmentCode.removeRange(0,1)
        segmentCode = segmentCode.removeRange(segmentCode.length - 1, segmentCode.length)
        var tokens = segmentCode.split(Regex("\\+")).toMutableList()
        var footnessIndex: Int
        footnessIndex = tokens[0].indexOf("Fw")
        if(footnessIndex != -1) {
            footness = 'f'
            tokens[0] = tokens[0].removeRange(0,2)
        }
        else
        {
            footnessIndex = tokens[0].indexOf("Bw")
            if(footnessIndex != -1) {
                footness = 'b'
                tokens[0] = tokens[0].removeRange(0,2)
            }
        }
        for(i in 0 until tokens.size) {
            if(tokens[i]!="+") spinPositions.add(SpinPosition(tokens[i]))
        }
    }

    fun footnessToText(): String {
        if(footness=='b') return "Back"
        else if(footness=='f') return "Forward"
        else return "Error"
    }
}

class SpinPosition(positionCode: String) {
    var position: Char = 'e'
    var variations: MutableList<Char> = mutableListOf()
    var features: MutableList<Char> = mutableListOf()

    init {
        var positionCode = positionCode
        if (positionCode[0] == 'C') {
            position = 'c'
        } else if (positionCode[0] == 'S') {
            position = 's'
        } else if (positionCode[0] == 'U') {
            position = 'u'
        } else if (positionCode[0] == 'L') {
            position = 'l'
        } else if (positionCode[0] == 'I') {
            position = 'i'
        }
        positionCode = positionCode.removeRange(0, 1)
        if (positionCode.isNotEmpty()) {
            var tempIndex = -1
            tempIndex = positionCode.indexOf("Fr")
            if (tempIndex != -1) {
                variations.add('f'); positionCode =
                    positionCode.removeRange(tempIndex, tempIndex + 2)
            }
            tempIndex = positionCode.indexOf("Be")
            if (tempIndex != -1) {
                variations.add('b'); positionCode =
                    positionCode.removeRange(tempIndex, tempIndex + 2)
            }
            tempIndex = positionCode.indexOf("Bi")
            if (tempIndex != -1) {
                variations.add('m'); positionCode =
                    positionCode.removeRange(tempIndex, tempIndex + 2)
            }
            tempIndex = positionCode.indexOf("Si")
            if (tempIndex != -1) {
                variations.add('s'); positionCode =
                    positionCode.removeRange(tempIndex, tempIndex + 2)
            }
            tempIndex = positionCode.indexOf("St")
            if (tempIndex != -1) {
                variations.add('t'); positionCode =
                    positionCode.removeRange(tempIndex, tempIndex + 2)
            }
            tempIndex = positionCode.indexOf("Up")
            if (tempIndex != -1) {
                variations.add('u'); positionCode =
                    positionCode.removeRange(tempIndex, tempIndex + 2)
            }
            tempIndex = positionCode.indexOf("(")
            if (tempIndex != -1) {
                parseFeatures(positionCode.substring(tempIndex, positionCode.length))
            }
        }
    }

    private fun parseFeatures(featureCode: String) {
        var featureCode = featureCode
        featureCode = featureCode.removeRange(0, 1)
        featureCode = featureCode.removeRange(featureCode.length - 1, featureCode.length)
        var tokens = featureCode.split(Regex("(?<=(Co)|(Bl)|(Ju)|(8)|(Sp))")).toMutableList()
        val tempIndex = tokens.indexOf("")
        if (tempIndex != -1) tokens.removeAt(tempIndex)
        for (i in 0 until tokens.size) {
            if (tokens[i] == "Bl") {
                features.add('b')
            }
            if (tokens[i] == "Co") {
                features.add('c')
            }
            if (tokens[i] == "Ju") {
                features.add('j')
            }
            if (tokens[i] == "8") {
                features.add('8')
            }
            if (tokens[i] == "Sp") {
                features.add('s')
            }
            if (tokens[i] == "Wi") {
                features.add('w')
            }
        }
    }

    fun positionToText(): String {
        var returnString = ""
        returnString += getPositionString()+" "
        returnString += getVariationString()+" "
        if(features.isNotEmpty()) returnString += "("+getFeatureString()+")"
        return returnString
    }

    private fun getPositionString(): String {
        return when(position) {
            'c' -> "Camel"
            's' -> "Sit"
            'u' -> "Upright"
            'l' -> "Layback"
            'i' -> "Intermediate"
            else -> "error"
        }
    }
    private fun getVariationString(): String {
        var returnString = ""
        for(i in 0 until variations.size) {
            returnString += when (variations[i]) {
                'u' -> "Up"
                's' -> "Side"
                'f' -> "Front"
                'b' -> "Behind"
                't' -> "Straight"
                'm' -> "Biellmann"
                else -> "error"
            }
            if(i<variations.size-1) returnString += " -> "
        }
        return returnString
    }
    private fun getFeatureString(): String {
        var returnString = ""
        for(i in 0 until features.size)
        {
            returnString += when(features[i]) {
                'b' -> "Blade"
                'c' -> "COE"
                'j' -> "Jump"
                '8' -> "8 Revs"
                's' -> "Speed"
                'w' -> "Windmill"
                else -> "error"
            }
            if(i<features.size-1) returnString += ", "
        }
        return returnString
    }
}
