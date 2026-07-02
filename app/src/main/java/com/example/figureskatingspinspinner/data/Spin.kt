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
        var difficultExit: Boolean = false,
        //adult specific:
        var cleanChangeFootSpin: Boolean = false, //change foot spins
        var allThreeBasicPositionsAnywhere: Boolean = false, //adult-silver and lower for combo spins
        var allThreeBasicPositionOnSecondFoot: Boolean = false //adult-gold and higher for combo spins
    )
    var spinFeatures = SpinFeatures()

    init { //the spinCode needs to be parsed into the spin object
        if(spinCode.contains("2FtUSp")) {
            baseType = '2'
            level = 0

            //the below is simply for the purpose of storing the direction of the spin (i.e. clockwise, counterclockwise) everything else is irrelevant for 2 foot spin
            if(spinCode.contains("cc")) spinSegments.add(SpinSegment("cc[FwC]"))
            else spinSegments.add(SpinSegment("c[FwC]"))
        }
        else if(spinCode!="") {
            var spinCode = spinCode
            spinCode = processLevel(spinCode)
            spinCode = processFlyState(spinCode)
            spinCode = processNextSpinSegment(spinCode) //first segment
            if(anotherSegmentExists(spinCode)) {
                isChangeFoot = true
                spinCode = processTransition(spinCode)
                spinCode = processNextSpinSegment(spinCode)
            }
            spinCode = processFinalTags(spinCode)
            baseType = getSpinType()
        }
    }

    private fun processLevel(spinCode: String): String {
        level = spinCode.substring(1,2).toInt()
        return spinCode.removeRange(0,3)
    }
    private fun processFlyState(spinCode: String): String {
        if(spinCode[0] == 'F') {
            isFlying = true
            return spinCode.removeRange(0,1)
        }
        else {
            isFlying = false
            return spinCode
        }
    }
    private fun anotherSegmentExists(spinCode: String): Boolean {
        val index = spinCode.indexOf(']',0)
        if(index == -1) return false
        return true
    }
    private fun processNextSpinSegment(spinCode: String): String {
        var spinCode = spinCode
        val startIndex = spinCode.indexOf('c',0)
        val endIndex = spinCode.indexOf(']',0)+1
        if(startIndex == -1 && endIndex == 0) return spinCode
        if(startIndex != 0) spinCode = spinCode.removeRange(0,startIndex)
        val segmentCode = spinCode.substring(0,endIndex)
        spinSegments.add(SpinSegment(segmentCode))
        return spinCode.removeRange(0,endIndex)
    }
    private fun processTransition(spinCode: String): String {
        if(spinCode[0]=='+') return spinCode.removeRange(0,1) //remove the plus (next segment should be right after it)
        if(spinCode.substring(0,3)=="-j-") {
            spinFeatures.changeFootByJump = true
            return spinCode.removeRange(0,3)
        }
        return ""
    }
    private fun processFinalTags(spinCode: String): String {
        if(spinCode.isEmpty()) return spinCode
        var spinCode = spinCode
        val validFinalTags: List<String> = listOf("DE","DX","CBP2F","CBP")
        var startIndex: Int
        for(i in validFinalTags.indices) {
            startIndex = spinCode.indexOf(validFinalTags[i])
            if(startIndex != -1) {
                when(validFinalTags[i]) {
                    "DE" -> spinFeatures.difficultEntrance = true
                    "DX" -> spinFeatures.difficultExit = true
                    "CBP" -> {
                        if(baseType!='k') {
                            spinFeatures.cleanChangeFootSpin = true
                        }
                        else {
                            spinFeatures.allThreeBasicPositionsAnywhere = true
                        }
                    }
                    "CBP2F" -> spinFeatures.allThreeBasicPositionOnSecondFoot = true
                }
                spinCode = spinCode.removeRange(startIndex,startIndex+validFinalTags[i].length)
            }
        }
        return spinCode
    }
    fun getSpinType(): Char {
        var hasCamel = false
        var hasSit = false
        var hasUpright = false
        var hasLayback = false
        var hasIntermediate = false
        for(i in spinSegments.indices) {
            for(j in spinSegments[i].spinPositions.indices) {
                when(spinSegments[i].spinPositions[j].position) {
                    'c' -> hasCamel = true
                    's' -> hasSit = true
                    'u' -> hasUpright = true
                    'l' -> hasLayback = true
                    'i' -> hasIntermediate = true
                }
            }
        }
        if(hasCamel&&hasSit&&(hasUpright||hasLayback)) {
            return 'k'
        }
        else if(hasCamel) {
            return 'c'
        }
        else if(hasSit) {
            return 's'
        }
        else if(hasUpright) {
            return 'u'
        }
        else if(hasLayback) {
            return 'l'
        }
        return 'e'
    }
    fun getISUSpinCodeAsText(): String {
        var returnString = ""
        if(baseType=='e') return ""
        if(baseType=='2') return "2FtUSp"
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

class SpinSegment(segmentCode: String) {
    var direction: Char = 'e'
    var footness: Char = 'e'
    var spinPositions: MutableList<SpinPosition> = mutableListOf()
    data class SpinSegmentFeatures(
        var difficultChangeOfPosition: Boolean = false,
    )
    var spinSegmentFeatures = SpinSegmentFeatures()

    init {
        var segmentCode = segmentCode
        segmentCode = processDirection(segmentCode)
        segmentCode = processFootness(segmentCode)
        segmentCode = processNextPosition(segmentCode)
        while(anotherPositionExists(segmentCode)) {
            segmentCode = processNextPosition(segmentCode)
        }
        if(checkForDifficultChangeOfPosition()) spinSegmentFeatures.difficultChangeOfPosition = true
    }
    private fun processDirection(segmentCode: String): String {
        if(segmentCode.substring(0,2)=="cc") {
            direction = 'r'
            return segmentCode.removeRange(0,2)
        }
        else if(segmentCode.substring(0,1)=="c") {
            direction = 'l'
            return segmentCode.removeRange(0,1)
        }
        return "" //Error
    }
    private fun processFootness(segmentCode: String): String {
        var startIndex: Int
        startIndex = segmentCode.indexOf("Fw",0)
        footness = 'f'
        if(startIndex == -1) {
            startIndex = segmentCode.indexOf("Bw",0)
            footness = 'b'
            if(startIndex == -1) footness = 'e'
        }
        return segmentCode.removeRange(startIndex,startIndex+2)
    }
    private fun processNextPosition(segmentCode: String): String {
        var startIndex = segmentCode.indexOf('[',0)
        var endIndex: Int
        if(startIndex == -1) startIndex = segmentCode.indexOf('+',0)
        endIndex = segmentCode.indexOf('+',startIndex+1)
        if(endIndex == -1) endIndex = segmentCode.indexOf(']',startIndex+1)
        spinPositions.add(SpinPosition(segmentCode.substring(startIndex+1,endIndex)))
        if(segmentCode[endIndex]==']')
            return ""
        return segmentCode.removeRange(startIndex,endIndex)
    }
    private fun anotherPositionExists(segmentCode: String): Boolean {
        return segmentCode.isNotEmpty()
    }
    private fun checkForDifficultChangeOfPosition(): Boolean {
        if(spinPositions.size < 2) return false
        for(i in 1..spinPositions.lastIndex) {
            var previousPosition = spinPositions[i-1].position
            var currentPosition = spinPositions[i].position
            if(previousPosition in listOf('s','u','l')) {
                if(currentPosition == 'c')
                    return true
            }
        }
        return false
    }

    fun footnessToText(): String {
        if(footness=='f') return "Forward"
        else if(footness=='b') return "Backward"
        return "Error"
    }
}

class SpinPosition(positionCode: String) {
    var position: Char = 'e'
    var variations: MutableList<Char> = mutableListOf()
    var features: MutableList<Char> = mutableListOf()
    val validVariationCodes: List<String> = listOf("Fr","Be","Si","Up","St","Bi")
    val validVariations: List<Char> =       listOf('f','b','s','u','t','m')
    val validFeatureCodes: List<String> =   listOf("Bl","Co","Ju","Sp","Wi","8","5")
    val validFeatures: List<Char> =         listOf('b','c','j','s','w','8','5')

    init {
        var positionCode = positionCode
        positionCode = processPosition(positionCode)
        positionCode = processVariations(positionCode)
        positionCode = processFeatures(positionCode)
    }
    private fun processPosition(positionCode: String): String {
        var positionCode = positionCode
        if(positionCode[0]=='C') { position = 'c' }
        else if(positionCode[0]=='S') { position = 's' }
        else if(positionCode[0]=='U') { position = 'u' }
        else if(positionCode[0]=='L') { position = 'l' }
        else if(positionCode[0]=='I') { position = 'i' }
        return positionCode.removeRange(0,1)
    }
    private fun processVariations(positionCode: String): String {
        if(positionCode.isEmpty()) return positionCode
        var positionCode = positionCode
        var startIndex: Int
        for(i in validVariationCodes.indices) {
            startIndex = positionCode.indexOf(validVariationCodes[i])
            if(startIndex != -1) {
                variations.add(validVariations[i])
                positionCode = positionCode.removeRange(startIndex,startIndex+validVariationCodes[i].length)
            }
        }
        return positionCode
    }
    private fun processFeatures(positionCode: String): String {
        if(positionCode.isEmpty()) return positionCode
        var positionCode = positionCode
        var startIndex = positionCode.indexOf("(",0)
        if(startIndex != -1) positionCode = positionCode.removeRange(startIndex,startIndex+1) //remove parenthesis
        for(i in validFeatureCodes.indices) {
            startIndex = positionCode.indexOf(validFeatureCodes[i])
            if(startIndex != -1) {
                features.add(validFeatures[i])
                positionCode = positionCode.removeRange(startIndex,startIndex+validFeatureCodes[i].length)
            }
        }
        if(positionCode[0]==')') positionCode = positionCode.removeRange(0,1)
        return positionCode
    }

    fun positionToText(): String {
        var returnText = when(position) {
            'c' -> "Camel"
            's' -> "Sit"
            'u' -> "Upright"
            'l' -> "Layback"
            'i' -> "Non-Basic"
            else -> "Error"
        }
        for(i in variations.indices) {
            returnText += when(variations[i]) {
                'f' -> " Front"
                'b' -> " Behind"
                's' -> " Side"
                'u' -> " Up"
                't' -> " Straight"
                'm' -> " Biellmann"
                else -> " Error"
            }
            if(i!=variations.size-1) returnText += " ->"
        }
        if(features.isNotEmpty()) returnText += " ("
        for(i in features.indices) {
            returnText += when(features[i]) {
                'b' -> "Blade"
                'c' -> "COE"
                'j' -> "Jump"
                's' -> "Speed"
                'w' -> "Windmill"
                '8' -> "8 Revs"
                '5' -> "5 Revs"
                else -> "Error"
            }
            if(i!=features.size-1) returnText += ", "
        }
        if(features.isNotEmpty()) returnText += ")"
        return returnText
    }
}
