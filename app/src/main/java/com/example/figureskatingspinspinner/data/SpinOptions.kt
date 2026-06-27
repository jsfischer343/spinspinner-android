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
            override val options: Array<String> = arrayOf("Any","Camel","Sit","Upright","Layback","Combo")
            override val defaultOption = "Any"
        }

        object SpinLevel : Dropdown() {
            override val label = "Spin Level"
            override val options: Array<String> = arrayOf("Any","Base","1","2","3","4")
            override val defaultOption = "Base"
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
            override val label = "Normalize"
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
    }
}