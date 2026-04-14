package com.example.figureskatingspinspinner.data

object NativeInterface {
    init {
        System.loadLibrary("figureskatingspinspinner")
    }
    external fun spinSpinnerCommand(command: String): String
}