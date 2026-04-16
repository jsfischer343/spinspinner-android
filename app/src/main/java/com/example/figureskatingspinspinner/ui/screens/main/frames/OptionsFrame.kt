package com.example.figureskatingspinspinner.ui.screens.main.frames

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.wrapContentHeight
import androidx.compose.foundation.layout.wrapContentWidth
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.KeyboardArrowDown
import androidx.compose.material3.DropdownMenu
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ElevatedCard
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.getValue
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import com.example.figureskatingspinspinner.data.DataStoreManager
import com.example.figureskatingspinspinner.data.SpinOptions
import com.example.figureskatingspinspinner.data.spinOptionsDataStore
import kotlinx.coroutines.launch

@Composable
fun OptionsFrame() {
    val unused = SpinOptions.Dropdown.SpinType.options //force initialization of SpinOptions.Dropdown to avoid errors with deferred initialization of companion object
    Column(
        modifier = Modifier.fillMaxSize().padding(4.dp).verticalScroll(rememberScrollState()),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        SpinOptions.Dropdown.allDropdownsList.forEach {
            OptionsFrame_DropdownCard(it)
        }
        SpinOptions.Toggle.allTogglesList.forEach {
            OptionsFrame_ToggleCard(it)
        }
    }
}

@Composable
fun OptionsFrame_DropdownCard(dropdown: SpinOptions.Dropdown) {
    val localContext = LocalContext.current
    val spinOptionsDataStoreManager = DataStoreManager(localContext.spinOptionsDataStore)
    val scope = rememberCoroutineScope()
    var dropdownExpanded by remember { mutableStateOf(false) }
    val currentSelectedOption by spinOptionsDataStoreManager.getFlowObj(dropdown.label).collectAsState(initial = "")

    ElevatedCard(
        modifier = Modifier.fillMaxWidth().height(64.dp).clickable {
            dropdownExpanded = true
        }
    ) {
        Row(
            modifier = Modifier.fillMaxWidth(),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.Center
        ) {
            Box(
                modifier = Modifier.weight(3f).padding(start = 8.dp).fillMaxHeight()
            ) {
                Column(
                    modifier = Modifier.padding(4.dp).fillMaxSize()
                        .wrapContentHeight(Alignment.CenterVertically)
                ) {
                    Text(
                        text = dropdown.label,
                        style = MaterialTheme.typography.labelSmall,
                    )
                    Text(
                        modifier = Modifier.padding(start = 8.dp),
                        text = currentSelectedOption,
                        style = MaterialTheme.typography.titleLarge,
                        fontWeight = FontWeight.SemiBold
                    )
                }
            }
            Box(
                modifier = Modifier.weight(1f).padding(end = 8.dp).wrapContentWidth(Alignment.End)
                    .fillMaxHeight()
            ) {
                IconButton(
                    modifier = Modifier.fillMaxSize().wrapContentWidth(Alignment.End),
                    onClick = { dropdownExpanded = true }) {
                    Icon(Icons.Default.KeyboardArrowDown, contentDescription = "dropdown")
                }
            }
        }
        OptionsFrame_DropdownCard_Menu(
            dropdown,
            dropdownExpanded,
            {dropdownExpanded = false},
            { option ->
                scope.launch {
                    spinOptionsDataStoreManager.save(dropdown.label,option)
                }
                dropdownExpanded = false
            }
        )
    }
    Spacer(
        modifier = Modifier.padding(2.dp)
    )
}

@Composable
fun OptionsFrame_DropdownCard_Menu(
    dropdown: SpinOptions.Dropdown,
    expanded: Boolean,
    onDismissRequest: () -> Unit,
    onClick: (String) -> Unit
) {
    DropdownMenu(
        modifier = Modifier.fillMaxWidth(),
        expanded = expanded,
        onDismissRequest = onDismissRequest
    ) {
        dropdown.options.forEach { option ->
            DropdownMenuItem(
                text = {
                    Text(
                        text = option
                    )
                },
                onClick = {onClick(option)}
            )
        }
    }
}

@Composable
fun OptionsFrame_ToggleCard(
    toggle: SpinOptions.Toggle
) {
    val localContext = LocalContext.current
    val spinOptionsDataStoreManager = DataStoreManager(localContext.spinOptionsDataStore)
    val scope = rememberCoroutineScope()
    val toggleState by spinOptionsDataStoreManager.getFlowObj(toggle.label).collectAsState(initial = "true")
    val toggleStateBool: Boolean
    if(toggleState=="true") toggleStateBool = true
    else toggleStateBool = false

    ElevatedCard(
        modifier = Modifier.fillMaxWidth().height(64.dp).clickable {
            scope.launch {
                var nextState: String
                if(toggleState=="true") nextState = "false"
                else nextState = "true"
                spinOptionsDataStoreManager.save(toggle.label,nextState)
            }
        }
    ) {
        Row(
            modifier = Modifier.fillMaxWidth(),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.Center
        ) {
            Box(
                modifier = Modifier.weight(3f).padding(start = 8.dp).fillMaxHeight()
            ) {
                Column(
                    modifier = Modifier.padding(4.dp).fillMaxSize()
                        .wrapContentHeight(Alignment.CenterVertically)
                ) {
                    Text(
                        modifier = Modifier.padding(start = 8.dp),
                        text = toggle.label,
                        style = MaterialTheme.typography.titleLarge,
                        fontWeight = FontWeight.SemiBold
                    )
                }
            }
            Box(
                modifier = Modifier.weight(1f).padding(end = 8.dp).wrapContentWidth(Alignment.End)
                    .fillMaxHeight()
            ) {
                Switch(
                    modifier = Modifier.fillMaxSize().wrapContentWidth(Alignment.End),
                    checked = toggleStateBool,
                    onCheckedChange = {
                        scope.launch {
                            var nextState: String
                            if(toggleState=="true") nextState = "false"
                            else nextState = "true"
                            spinOptionsDataStoreManager.save(toggle.label,nextState)
                        }
                    }
                )
            }
        }
    }
    Spacer(
        modifier = Modifier.padding(2.dp)
    )
}