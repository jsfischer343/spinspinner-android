package com.example.figureskatingspinspinner

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.runtime.Composable
import androidx.navigation.compose.rememberNavController
import com.example.figureskatingspinspinner.data.DataStoreManager
import com.example.figureskatingspinspinner.data.SpinOptions
import com.example.figureskatingspinspinner.data.spinOptionsDataStore
import com.example.figureskatingspinspinner.ui.theme.FigureSkatingSpinSpinnerTheme

class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        val spinOptionsDataStoreManager = DataStoreManager(this.spinOptionsDataStore)
        spinOptionsDataStoreManager.initDefaults(SpinOptions.defaults)
        setContent {
            FigureSkatingSpinSpinnerTheme {
                SpinSpinnerApp()
            }
        }
    }
}

@Composable
fun SpinSpinnerApp() {
    val navController = rememberNavController()
    SpinnerNavHost(
        navController = navController
    )
}

