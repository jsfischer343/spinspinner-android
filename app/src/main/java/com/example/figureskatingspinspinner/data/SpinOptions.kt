package com.example.figureskatingspinspinner.data

import androidx.compose.runtime.compositionLocalOf
import com.example.figureskatingspinspinner.data.SpinOptions.Dropdown.SpinDirection
import com.example.figureskatingspinspinner.data.SpinOptions.Dropdown.SpinLevel
import com.example.figureskatingspinspinner.data.SpinOptions.Dropdown.SpinType

sealed class SpinOptions {
    abstract val label: String

    sealed class Dropdown : SpinOptions() {
        abstract val options: Array<String>
        abstract val defaultOption: String

        object SpinType : Dropdown() {
            override val label = "Spin Type"
            override var options: Array<String> = arrayOf("Any","Camel","Sit","Upright","Layback","Combo")
            override val defaultOption = "Any"

            fun updateOptionsForRuleSet(ruleSet: String) {
                if(ruleSet=="Adult Bronze") options = arrayOf("Any","Camel","Sit","Upright","Layback","Combo","2 Foot")
                else options = arrayOf("Any","Camel","Sit","Upright","Layback","Combo")
            }
            fun getValidType(ruleSet: String, currentType: String): String {
                if(ruleSet!="Adult Bronze" && currentType=="2 Foot") return "Any"
                else return currentType
            }
        }

        object SpinLevel : Dropdown() {
            override val label = "Spin Level"
            override var options: Array<String> = arrayOf("Any","Base","1","2","3","4")
            override val defaultOption = "Base"
            fun updateOptionsForRuleSet(ruleSet: String) {
                when(ruleSet) {
                    "Standard" -> options = arrayOf("Any","Base","1","2","3","4")
                    "Adult Junior-Senior" -> options = arrayOf("Any","Base","1","2","3","4")
                    "Adult Intermediate-Novice" -> options = arrayOf("Any","Base","1","2","3","4")
                    "Adult Gold" -> options = arrayOf("Any","Base","1","2","3")
                    "Adult Silver" -> options = arrayOf("Any","Base","1","2")
                    "Adult Bronze" -> options = arrayOf("Any","Base","1")
                }
            }
            fun getValidLevel(ruleSet: String, currentLevel: String): String {
                return when(ruleSet) {
                    "Standard" -> currentLevel
                    "Adult Junior-Senior" -> currentLevel
                    "Adult Intermediate-Novice" -> currentLevel
                    "Adult Gold" -> if(currentLevel !in arrayOf("Any","Base","1","2","3")) "3" else currentLevel
                    "Adult Silver" -> if(currentLevel !in arrayOf("Any","Base","1","2")) "2" else currentLevel
                    "Adult Bronze" -> if(currentLevel !in arrayOf("Any","Base","1")) "1" else currentLevel
                    else -> "Any"
                }
            }
        }

        object SpinDirection : Dropdown() {
            override val label = "Preferred Spin Direction"
            override val options: Array<String> = arrayOf("Counterclockwise","Clockwise")
            override val defaultOption = "Counterclockwise"
        }

        object RuleSet : Dropdown() {
            override val label = "Rule Set"
            override val options: Array<String> = arrayOf("Standard","Adult Junior-Senior", "Adult Intermediate-Novice", "Adult Gold", "Adult Silver", "Adult Bronze")
            override val defaultOption = "Standard"
        }

        private object Initializer { //this is needed because of the problematic way that companion object is initialized before class objects :(
            val allDropdownsList: List<Dropdown> = listOf(SpinType,SpinLevel,SpinDirection,RuleSet)
        }
        companion object {
            val allDropdownsList: List<Dropdown> get() = Initializer.allDropdownsList
        }
    }

    sealed class Toggle : SpinOptions() {
        abstract val state: String

        object Normalize : Toggle() {
            override val label = "Less Weirdness"
            override val state = "true"
        }

        private object Initializer { //this is needed because of the problematic way that companion object is initialized before class objects :(
            val allTogglesList: List<Toggle> = listOf(Normalize)
        }
        companion object {
            val allTogglesList: List<Toggle> get() = Initializer.allTogglesList
        }
    }

    private object Initializer { //this is needed because of the problematic way that companion object is initialized before class objects :(
        val allOptionsList: List<SpinOptions> = listOf(Dropdown.SpinType,Dropdown.SpinLevel,Dropdown.SpinDirection,Dropdown.RuleSet,Toggle.Normalize)
        val defaults: Map<String,String> = mapOf(
            Dropdown.SpinType.label to Dropdown.SpinType.defaultOption,
            Dropdown.SpinLevel.label to Dropdown.SpinLevel.defaultOption,
            Dropdown.SpinDirection.label to Dropdown.SpinDirection.defaultOption,
            Dropdown.RuleSet.label to Dropdown.RuleSet.defaultOption,
            Toggle.Normalize.label to Toggle.Normalize.state
        )
    }
    companion object {
        val allOptionsList: List<SpinOptions> get() = Initializer.allOptionsList
        val defaults: Map<String,String> get() = Initializer.defaults

        //Ran once each time the app starts to update default selection of options or anything dependent on the user's last option selection
        fun initDefaults(currentDataStoreManager: DataStoreManager) {
            val currentRuleSet = currentDataStoreManager.getBlocking(Dropdown.RuleSet.label)
            Dropdown.SpinLevel.updateOptionsForRuleSet(currentRuleSet)
            Dropdown.SpinType.updateOptionsForRuleSet(currentRuleSet)
        }

        //Function used when user selects new option in option menu. This function assures that options are not incompatible (i.e. Adult Silver rule set attempting to spin a level 4 spin)
        suspend fun safeUpdateOptions(dropDown: Dropdown, newOption: String, currentDataStoreManager: DataStoreManager) {
            if(dropDown.label==Dropdown.RuleSet.label) {
                val currentLevel = currentDataStoreManager.get(Dropdown.SpinLevel.label)
                val nextLevel = Dropdown.SpinLevel.getValidLevel(newOption, currentLevel)
                Dropdown.SpinLevel.updateOptionsForRuleSet(newOption)
                if(nextLevel!=currentLevel) currentDataStoreManager.save(Dropdown.SpinLevel.label,nextLevel)

                val currentType = currentDataStoreManager.get(Dropdown.SpinType.label)
                val nextType = Dropdown.SpinType.getValidType(newOption, currentType)
                Dropdown.SpinType.updateOptionsForRuleSet(newOption)
                if(currentType!=nextType) currentDataStoreManager.save(Dropdown.SpinType.label,nextType)
            }
            currentDataStoreManager.save(dropDown.label,newOption)
        }
        //overloading for toggle options (note: currently unused)
        suspend fun safeUpdateOptions(toggle: Toggle, newOption: String, currentDataStoreManager: DataStoreManager) {
        }
    }
}