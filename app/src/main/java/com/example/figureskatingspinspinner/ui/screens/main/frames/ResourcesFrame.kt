package com.example.figureskatingspinspinner.ui.screens.main.frames

import android.net.Uri
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.wrapContentWidth
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
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
import androidx.compose.ui.unit.dp
import androidx.core.content.FileProvider
import androidx.navigation.NavHostController
import com.example.figureskatingspinspinner.R
import com.example.figureskatingspinspinner.navigateSingleTop
import com.example.figureskatingspinspinner.ui.navigation.Destination
import com.example.figureskatingspinspinner.ui.theme.getColorAppropriateResource
import com.example.figureskatingspinspinner.utils.openPDF
import androidx.core.net.toUri
import com.example.figureskatingspinspinner.data.Spin
import com.example.figureskatingspinspinner.utils.HtmlAssetView
import com.example.figureskatingspinspinner.utils.getPdfUriFromAssets
import java.io.File


@Composable
fun ResourcesFrame(
    navController: NavHostController
) {
    val context = LocalContext.current
    Column(
        modifier = Modifier.fillMaxSize().padding(vertical = 8.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        ResourcesNavigationRow(
            text = Destination.AppGuide.label,
            contentDescription = Destination.AppGuide.contentDescription,
            onClick = { navController.navigateSingleTop(Destination.AppGuide.route) }
        )
        ResourcesNavigationRow(
            text = "2025-26 Levels of Difficulty Singles",
            contentDescription = "2025-26 Levels of Difficulty Singles",
            onClick = {
                val fileUri = getPdfUriFromAssets(context, "pdfs/2025-26 Levels Of Difficulty Singles.pdf")
                openPDF(context, fileUri)
            }
        )
        ResourcesNavigationRow(
            text = "Spin Difficult Variation Chart",
            contentDescription = "Spin Difficult Variation Chart",
            onClick = {
                val fileUri = getPdfUriFromAssets(context, "pdfs/Singles Spin Difficult Variation Chart.pdf")
                openPDF(context, fileUri)
            }
        )
    }
}

@Composable
fun ResourcesNavigationRow(
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