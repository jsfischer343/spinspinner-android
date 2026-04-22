package com.example.figureskatingspinspinner

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.NavGraph.Companion.findStartDestination
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import com.example.figureskatingspinspinner.ui.navigation.Destination

@Composable
fun SpinnerNavHost(
    navController: NavHostController,
    modifier: Modifier = Modifier
) {
    Surface(modifier = Modifier.fillMaxSize()) {} //default background (used to avoid "white" flashing during transitions in dark mode)
    NavHost(
        modifier = modifier,
        navController = navController,
        startDestination = Destination.Main.Spinner.route
    ) {
        Destination.Main.allDestinationsList.forEach { destination ->
            composable(route = destination.route) {
                destination.content(navController)
            }
        }
        composable(route = Destination.SpinHistory.route) {
            Destination.SpinHistory.content(navController)
        }
        composable(route = Destination.AppGuide.route) {
            Destination.AppGuide.content(navController)
        }
        composable(route = Destination.About.route) {
            Destination.About.content(navController)
        }
        composable(route = Destination.Licence.route) {
            Destination.Licence.content(navController)
        }
        composable(route = Destination.Settings.route) {
            Destination.Settings.content(navController)
        }
    }
}

fun NavHostController.navigateSingleTop(route: String) {
    this.navigate(route = route) {
        launchSingleTop = true
        restoreState = true
        anim {
            enter = 0
            exit = 0
            popEnter = 0
            popExit = 0
        }
    }
}

fun NavHostController.navigateSingleTopAndClearNavStack(route: String) {
    this.navigate(route) {
        popUpTo (
            this@navigateSingleTopAndClearNavStack.graph.findStartDestination().id
        ) {
            saveState = true
        }
        launchSingleTop = true
        restoreState = true
        anim {
            enter = 0
            exit = 0
            popEnter = 0
            popExit = 0
        }
    }
}