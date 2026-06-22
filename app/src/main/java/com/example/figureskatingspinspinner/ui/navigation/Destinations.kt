package com.example.figureskatingspinspinner.ui.navigation

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.AddCircle
import androidx.compose.material.icons.filled.Info
import androidx.compose.material.icons.filled.Menu
import androidx.compose.material.icons.outlined.Settings
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.navigation.NavHostController
import com.example.figureskatingspinspinner.R
import com.example.figureskatingspinspinner.ui.screens.MainScreen
import com.example.figureskatingspinspinner.ui.screens.main.frames.OptionsFrame
import com.example.figureskatingspinspinner.ui.screens.main.frames.ResourcesFrame
import com.example.figureskatingspinspinner.ui.screens.main.frames.SpinnerFrame
import com.example.figureskatingspinspinner.ui.screens.settings.SettingsScreen
//import com.example.figureskatingspinspinner.ui.screens.spinhistory.SpinHistoryScreen
import com.example.figureskatingspinspinner.ui.theme.getColorAppropriateHTMLFile
import com.example.figureskatingspinspinner.utils.HTMLViewScreen

sealed class Destination    //Sealed to protect against extension by other classes. No other classes should extend Destination.
 {                          //Also ideal as it creates a clear hierarchy for navigation.
    abstract val route: String //Name of the route. Used by NavHost.
    abstract val label: String //The text the user will visually see in reference to this destination.
    abstract val icon: ImageVector
    abstract val contentDescription: String //For accessibility
    abstract val content: @Composable (navController: NavHostController) -> Unit

    sealed class Main : Destination() {
        object Spinner : Main() { //static and instantiated at first access
            override val route = "main/spinner"
            override val label = "Spinner"
            override val icon = Icons.Filled.AddCircle
            override val contentDescription = "Spinner Frame"
            override val content: @Composable (navController: NavHostController) -> Unit =  {
                navController -> MainScreen(navController,this) {
                    SpinnerFrame()
                }
            }
        }
        object Options : Main() {
            override val route = "main/options"
            override val label = "Options"
            override val icon = Icons.Filled.Menu
            override val contentDescription = "Options Frame"
            override val content: @Composable (navController: NavHostController) -> Unit =  {
                navController -> MainScreen(navController,this) {
                    OptionsFrame()
                }
            }
        }
        object Resources : Main() {
            override val route = "main/resources"
            override val label = "Resources"
            override val icon = Icons.Filled.Info
            override val contentDescription = "Resources Frame"
            override val content: @Composable (navController: NavHostController) -> Unit = {
                navController -> MainScreen(navController,this) {
                    ResourcesFrame(navController)
                }
            }
        }

        private object Initializer { //this is needed because of the problematic way that companion object is initialized before class objects :(
            val allDestinationsList: List<Main> = listOf(Spinner,Options,Resources) //for easy iteration over main screens
        }
        companion object {
            val allDestinationsList: List<Main> get() = Initializer.allDestinationsList
        }
    }
//     object SpinHistory : Destination () {
//         override val route = "spinhistory"
//         override val label = "Spin History"
//         override val icon = Icons.Default.Info
//         val resIcon_light: Int = R.drawable.history_24dp_000000_fill0_wght400_grad0_opsz24
//         val resIcon_dark: Int = R.drawable.history_24dp_ffffff_fill0_wght400_grad0_opsz24
//         override val contentDescription = "Spin History Screen"
//         override val content: @Composable (navController: NavHostController) -> Unit = { navController ->
//             SpinHistoryScreen(navController, this)
//         }
//     }
     object AppGuide : Destination() {
         override val route = "appguide"
         override val label = "Guide"
         override val icon = Icons.Filled.Info
         override val contentDescription = "App Guide"
         override val content: @Composable (navController: NavHostController) -> Unit = { navController ->
             val filePath = getColorAppropriateHTMLFile("file:///android_asset/html/appguide.html","file:///android_asset/html/appguide_dark.html")
             HTMLViewScreen(navController,this,this.label, filePath)
         }
     }
     object About : Destination() {
         override val route = "about"
         override val label = "About"
         override val icon = Icons.Filled.Info
         override val contentDescription = "About App Screen"
         override val content: @Composable (navController: NavHostController) -> Unit = { navController ->
             val filePath = getColorAppropriateHTMLFile("file:///android_asset/html/about.html","file:///android_asset/html/about_dark.html")
             HTMLViewScreen(navController,this,this.label, filePath)
         }
     }
     object Licence : Destination() {
         override val route = "licence"
         override val label = "Licence"
         override val icon = Icons.Filled.Info
         override val contentDescription = "Licence Screen"
         override val content: @Composable (navController: NavHostController) -> Unit = { navController ->
             val filePath = getColorAppropriateHTMLFile("file:///android_asset/html/license.html","file:///android_asset/html/license_dark.html")
             HTMLViewScreen(navController,this,this.label, filePath)
         }
     }
     object Settings : Destination() {
         override val route = "settings"
         override val label = "Settings"
         override val icon = Icons.Outlined.Settings
         override val contentDescription = "Settings Screen"
         override val content: @Composable (navController: NavHostController) -> Unit = {
             navController -> SettingsScreen(navController,this)
         }
     }
}