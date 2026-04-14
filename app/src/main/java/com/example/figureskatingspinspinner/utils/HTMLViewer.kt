package com.example.figureskatingspinspinner.utils

import android.webkit.WebView
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.navigation.NavHostController
import com.example.figureskatingspinspinner.ui.navigation.Destination

@Composable
fun HTMLViewScreen(
    navController: NavHostController,
    currentDestination: Destination,
    header: String,
    filePath: String
) {
    val scrollState = rememberScrollState()
    Scaffold(
        topBar = { HTMLViewScreen_TopBar(
            navController = navController,
            header = header
        ) }
    ) { innerPadding ->
        Surface(
            modifier = Modifier.fillMaxSize()
                .padding(innerPadding)
        ) {
            Column(
                modifier = Modifier.padding(8.dp).verticalScroll(scrollState),
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                HtmlAssetView(Modifier.fillMaxWidth(),filePath)
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HTMLViewScreen_TopBar(
    navController: NavHostController,
    header: String
) {
    CenterAlignedTopAppBar(
        title = { Text(
            text = header
        ) },
        navigationIcon = {
            IconButton(
                onClick = { navController.popBackStack() }
            ) {
                Icon(
                    imageVector = Icons.AutoMirrored.Filled.ArrowBack,
                    contentDescription = "Back arrow"
                )
            }
        }
    )
}

@Composable
fun HtmlAssetView(
    modifier: Modifier,
    filePath: String
) {
    AndroidView(
        modifier = modifier,
        factory = { context ->
            WebView(context).apply {
                loadUrl(filePath)
            }
        }
    )
}