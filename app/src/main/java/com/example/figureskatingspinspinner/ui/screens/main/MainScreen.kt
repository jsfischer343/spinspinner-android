package com.example.figureskatingspinspinner.ui.screens

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.NavigationBar
import androidx.compose.material3.NavigationBarItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.foundation.layout.Row
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import com.example.figureskatingspinspinner.navigateSingleTop
import com.example.figureskatingspinspinner.navigateSingleTopAndClearNavStack
import com.example.figureskatingspinspinner.ui.navigation.Destination

@Composable
fun MainScreen(
    navController: NavHostController,
    currentDestination: Destination,
    content: @Composable () -> Unit //current frame composable function is passed here selected from Destinations.Main.allDestinationsList
) {
    Scaffold(
        topBar = { MainScreen_TopBar(
            navController = navController
        ) },
        bottomBar = { MainScreen_NavBar(
            navController = navController,
            currentDestination = currentDestination
        ) }
    ) { innerPadding ->
        Surface( //acts as the background for the center frame on the main screen
            modifier = Modifier.fillMaxSize()
                .padding(innerPadding)
        ) {
            content() //look at definitions in SpinnerNavHost.kt and Destinations.kt
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MainScreen_TopBar(
    navController: NavHostController
) {
    CenterAlignedTopAppBar(
        title = {
            Row(

            ) {
                Text(
                    text = "SpinSpinner"
                )
                Text(
                    text = "beta",
                    style = MaterialTheme.typography.labelSmall
                )
            }
        },
        actions = {
            IconButton(
                onClick = { navController.navigateSingleTop(Destination.Settings.route) }
            ) {
                Icon(
                    imageVector = Destination.Settings.icon,
                    contentDescription = "Settings"
                )
            }
        }
    )
}

@Composable
fun MainScreen_NavBar(
    navController: NavHostController,
    currentDestination: Destination
) {
    NavigationBar(
    ) {
        Destination.Main.allDestinationsList.forEach { destination ->
            NavigationBarItem(
                label = { Text(destination.label) },
                icon = { Icon(
                    destination.icon,
                    contentDescription = destination.contentDescription
                ) },
                selected = checkDestinationsMatch(currentDestination, destination),
                onClick = { navController.navigateSingleTop(destination.route) }
            )
        }
    }
}

private fun checkDestinationsMatch(destinationOne: Destination, destinationTwo: Destination): Boolean {
    return destinationOne==destinationTwo
}