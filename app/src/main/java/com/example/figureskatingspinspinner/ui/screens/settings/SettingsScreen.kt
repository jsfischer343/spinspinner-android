package com.example.figureskatingspinspinner.ui.screens.settings

import android.content.Intent
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.wrapContentWidth
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.core.content.ContextCompat.getString
import androidx.core.net.toUri
import androidx.navigation.NavHostController
import com.example.figureskatingspinspinner.R
import com.example.figureskatingspinspinner.navigateSingleTop
import com.example.figureskatingspinspinner.popBackStackSafe
import com.example.figureskatingspinspinner.ui.navigation.Destination
import com.example.figureskatingspinspinner.ui.theme.getColorAppropriateResource


@Composable
fun SettingsScreen(
    navController: NavHostController,
    currentDestination: Destination
) {
    val context = LocalContext.current
    Scaffold(
        topBar = { SettingsScreen_TopBar(
            navController = navController
        ) }
    ) { innerPadding ->
        Surface( //acts as the background for the center frame on the main screen
            modifier = Modifier.fillMaxSize()
                .padding(innerPadding)
        ) {
            Column(
                modifier = Modifier.fillMaxSize().padding(vertical = 8.dp),
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                SettingsNavigationRow(
                    text = Destination.About.label,
                    contentDescription = Destination.About.contentDescription,
                    onClick = { navController.navigateSingleTop(Destination.About.route) }
                )
                SettingsNavigationRow(
                    text = "Github",
                    contentDescription = "Github",
                    onClick = {
                        val intent = Intent(Intent.ACTION_VIEW,getString(context, R.string.uri_github_repo).toUri())
                        context.startActivity(intent)
                    }
                )
                SettingsNavigationRow(
                    text = "Report Bug or Issue",
                    contentDescription = "Report Bug or Issue",
                    onClick = {
                        val intent = Intent(Intent.ACTION_VIEW,getString(context, R.string.uri_reportbug).toUri())
                        context.startActivity(intent)
                    }
                )
                SettingsNavigationRow(
                    text = "Beta Survey",
                    contentDescription = "Beta Survey",
                    onClick = {
                        val intent = Intent(Intent.ACTION_VIEW,getString(context, R.string.uri_betasurvey).toUri())
                        context.startActivity(intent)
                    }
                )
                SettingsNavigationRow(
                    text = Destination.Licence.label,
                    contentDescription = Destination.Licence.contentDescription,
                    onClick = { navController.navigateSingleTop(Destination.Licence.route) }
                )
                Spacer(modifier = Modifier.weight(1f))
                Row() {
                    Text(text = "SpinSpinner", style = MaterialTheme.typography.bodySmall, color = MaterialTheme.colorScheme.secondary)
                }
                Row() {
                    val version = stringResource(R.string.app_version)
                    Text(text = "Version $version", style = MaterialTheme.typography.bodySmall, color = MaterialTheme.colorScheme.secondary)
                }
            }
        }
    }
}

@Composable
fun SettingsNavigationRow(
    text: String,
    contentDescription: String,
    onClick: () -> Unit
) {
    Row(
        modifier = Modifier.fillMaxWidth().clickable(onClick = onClick).height(64.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Box(
            modifier = Modifier.weight(3f).padding(start = 8.dp)
        ) {
            Text(modifier = Modifier.padding(start = 24.dp), text = text, style = MaterialTheme.typography.titleMedium)
        }
        Box(
            modifier = Modifier.weight(1f).padding(end = 12.dp).wrapContentWidth(Alignment.End)
        ) {
            Image(
                painter = painterResource(
                    getColorAppropriateResource(
                        R.drawable.open_in_new_24dp_000000_fill0_wght400_grad0_opsz24,
                        R.drawable.open_in_new_24dp_ffffff_fill0_wght400_grad0_opsz24
                    )
                ),
                contentDescription = contentDescription
            )
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SettingsScreen_TopBar(
    navController: NavHostController
) {
    CenterAlignedTopAppBar(
        title = { Text(
            text = "Settings"
        ) },
        navigationIcon = {
            IconButton(
                onClick = { navController.popBackStackSafe() }
            ) {
                Icon(
                    imageVector = Icons.AutoMirrored.Filled.ArrowBack,
                    contentDescription = "Back arrow"
                )
            }
        }
    )
}