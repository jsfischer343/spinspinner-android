package com.example.figureskatingspinspinner.ui.screens.spinhistory

//import androidx.compose.foundation.layout.fillMaxSize
//import androidx.compose.foundation.layout.padding
//import androidx.compose.material.icons.Icons
//import androidx.compose.material.icons.automirrored.filled.ArrowBack
//import androidx.compose.material3.CenterAlignedTopAppBar
//import androidx.compose.material3.ExperimentalMaterial3Api
//import androidx.compose.material3.Icon
//import androidx.compose.material3.IconButton
//import androidx.compose.material3.Scaffold
//import androidx.compose.material3.Surface
//import androidx.compose.material3.Text
//import androidx.compose.runtime.Composable
//import androidx.compose.ui.Modifier
//import androidx.compose.ui.platform.LocalContext
//import androidx.navigation.NavHostController
//import com.example.figureskatingspinspinner.ui.navigation.Destination


//@Composable
//fun SpinHistoryScreen(
//    navController: NavHostController,
//    currentDestination: Destination
//) {
//    val context = LocalContext.current
//    Scaffold(
//        topBar = {
//            SpinHistoryScreen_TopBar(
//                navController = navController
//            )
//        }
//    ) { innerPadding ->
//        Surface( //acts as the background for the center frame on the main screen
//            modifier = Modifier.fillMaxSize()
//                .padding(innerPadding)
//        ) {
//
//        }
//    }
//}
//
//@OptIn(ExperimentalMaterial3Api::class)
//@Composable
//fun SpinHistoryScreen_TopBar(
//    navController: NavHostController
//) {
//    CenterAlignedTopAppBar(
//        title = { Text(
//            text = "Spin History"
//        ) },
//        navigationIcon = {
//            IconButton(
//                onClick = { navController.popBackStack() }
//            ) {
//                Icon(
//                    imageVector = Icons.AutoMirrored.Filled.ArrowBack,
//                    contentDescription = "Back arrow"
//                )
//            }
//        }
//    )
//}