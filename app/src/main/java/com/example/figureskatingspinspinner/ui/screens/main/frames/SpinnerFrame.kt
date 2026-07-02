package com.example.figureskatingspinspinner.ui.screens.main.frames

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.layout.wrapContentHeight
import androidx.compose.foundation.layout.wrapContentSize
import androidx.compose.foundation.layout.wrapContentWidth
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.ElevatedCard
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import com.example.figureskatingspinspinner.data.SpinOptions
import com.example.figureskatingspinspinner.R
import com.example.figureskatingspinspinner.data.DataStoreManager
import com.example.figureskatingspinspinner.data.NativeInterface
import com.example.figureskatingspinspinner.data.Spin
import com.example.figureskatingspinspinner.data.SpinSegment
import com.example.figureskatingspinspinner.data.spinOptionsDataStore
import com.example.figureskatingspinspinner.ui.theme.getColorAppropriateResource
import kotlinx.coroutines.launch
import kotlin.text.lowercase

const val DEBUG_MODE = true
@Composable
fun SpinnerFrame() {
    val localContext = LocalContext.current
    val spinOptionsDataStoreManager = DataStoreManager(localContext.spinOptionsDataStore)
    var nativeSpinCode by remember { mutableStateOf("") }
    val scope = rememberCoroutineScope()
    val spin = Spin(nativeSpinCode)

    Column(
        modifier = Modifier.fillMaxSize().wrapContentSize(Alignment.Center).padding(8.dp),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        //TODO: Implement Spin History
//        Row(
//            modifier = Modifier.height(64.dp).fillMaxWidth().wrapContentWidth(Alignment.End).clickable(onClick = {
//                navController.navigateSingleTop(Destination.SpinHistory.route)
//            })
//        ) {
//            Image(
//                painter = painterResource(
//                    getColorAppropriateResource(Destination.SpinHistory.resIcon_light,
//                        Destination.SpinHistory.resIcon_dark
//                        )
//                ),
//                contentDescription = Destination.SpinHistory.contentDescription
//            )
//        }
        Row(
            modifier = Modifier.weight(1f).fillMaxWidth().wrapContentSize(Alignment.Center)
        ) {
            SpinSpunOutput(nativeSpinCode, DEBUG_MODE,spin)
        }
        Row(
            modifier = Modifier.height(64.dp).fillMaxWidth(),
            horizontalArrangement = Arrangement.Center
        ) {
            Button(
                modifier = Modifier.padding(8.dp).width(124.dp),
                colors = ButtonDefaults.buttonColors(containerColor = MaterialTheme.colorScheme.primary, contentColor = MaterialTheme.colorScheme.onSurface),
                onClick = {
                scope.launch {
                    val commandString = buildSpinCommandString(spinOptionsDataStoreManager)
                    nativeSpinCode = NativeInterface.spinSpinnerCommand(commandString)
                }
            }
            ) {
                Text(
                    text = "Spin",
                    fontWeight = FontWeight.SemiBold,

                )
            }
        }
    }
}

suspend fun buildSpinCommandString(spinOptionsDataStoreManager: DataStoreManager): String {
    var commandString: String = "-c"
    val spinLevel = spinOptionsDataStoreManager.get(SpinOptions.Dropdown.SpinLevel.label)
    val spinType = spinOptionsDataStoreManager.get(SpinOptions.Dropdown.SpinType.label)?.lowercase()
    val spinDirection = spinOptionsDataStoreManager.get(SpinOptions.Dropdown.SpinDirection.label)?.lowercase()
    val normalize = spinOptionsDataStoreManager.get(SpinOptions.Toggle.Normalize.label)
    val ruleSet = spinOptionsDataStoreManager.get(SpinOptions.Dropdown.RuleSet.label)

    if(spinLevel=="Any") commandString += " -l any"
    else if(spinLevel=="Base") commandString += " -l 0"
    else commandString += " -l $spinLevel"
    if(spinDirection=="clockwise")  commandString += " -r"
    commandString += " -t $spinType"
    if(normalize=="true") commandString += " -b"

    if(ruleSet=="Adult Junior-Senior") commandString += " --adult-junior-senior"
    else if(ruleSet=="Adult Intermediate-Novice") commandString += " --adult-intermediate-novice"
    else if(ruleSet=="Adult Gold") commandString += " --adult-gold"
    else if(ruleSet=="Adult Silver") commandString += " --adult-silver"
    else if(ruleSet=="Adult Bronze") commandString += " --adult-bronze"
    return commandString
}
@Composable
fun SpinSpunOutput(
    debugSpinCode: String,
    showDebugSpinCode: Boolean,
    spin: Spin
) {
    val scrollState = rememberScrollState()
    Column(
        modifier = Modifier.verticalScroll(scrollState),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center
    ) {
        if(showDebugSpinCode) Text(text = "Debug: $debugSpinCode")
        ISUSpinCode(spin.getISUSpinCodeAsText())
        Spacer(modifier = Modifier.padding(16.dp))
        if(spin.baseType=='2') {
            //2FtUSp
            TwoFtUSpCard(spin.spinSegments[0].direction)
        }
        else {
            if(spin.isFlying) {
                FlyCard(spin.spinFeatures.difficultEntrance)
                DownArrowSeparator()
            }
            else if(spin.spinFeatures.difficultEntrance) {
                DifficultEntranceCard()
                DownArrowSeparator()
            }
            for(i in 0 until spin.spinSegments.size) {
                SpinSegmentCard(spin.spinSegments[i])
                if(i<spin.spinSegments.size-1) {
                    if(spin.spinFeatures.changeFootByJump) Text(text = "- Jump -")
                    else DownArrowSeparator()
                }
            }
            if(spin.spinFeatures.difficultExit) {
                DownArrowSeparator()
                DifficultExitCard()
            }
            if(spin.spinFeatures.cleanChangeFootSpin) {
                Spacer(modifier = Modifier.fillMaxWidth().height(8.dp))
                AdultFeatureCard("Both side must achieve clear basic positions.")
            }
            if(spin.spinFeatures.allThreeBasicPositionsAnywhere) {
                Spacer(modifier = Modifier.fillMaxWidth().height(8.dp))
                AdultFeatureCard("All three basic positions must be achieved.")
            }
            if(spin.spinFeatures.allThreeBasicPositionOnSecondFoot) {
                Spacer(modifier = Modifier.fillMaxWidth().height(8.dp))
                AdultFeatureCard("All three basic positions must be achieved on the second foot.")
            }
        }
    }
}

@Composable
fun SpinSegmentCard(
    spinSegment: SpinSegment
) {
    //direction
    var directionContentDesc: String
    var directionColor: Color
    var directionResLight: Int
    var directionResDark: Int
    if(spinSegment.direction=='r') {
        directionContentDesc = "Counterclockwise"
        directionColor = MaterialTheme.colorScheme.secondary
        directionResLight = R.drawable.rotate_left_24dp_000000_fill0_wght400_grad0_opsz24
        directionResDark = R.drawable.rotate_left_24dp_ffffff_fill0_wght400_grad0_opsz24
    }
    else {
        directionContentDesc = "Clockwise"
        directionColor = MaterialTheme.colorScheme.tertiary
        directionResLight = R.drawable.rotate_right_24dp_000000_fill0_wght400_grad0_opsz24
        directionResDark = R.drawable.rotate_right_24dp_ffffff_fill0_wght400_grad0_opsz24
    }

    ElevatedCard(
        modifier = Modifier.padding(horizontal = 12.dp, vertical = 4.dp).fillMaxWidth().wrapContentHeight(Alignment.CenterVertically)
    ) {
        Column(
            modifier = Modifier.padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Box(
                modifier = Modifier.fillMaxWidth(),
            ) {
                Box(
                    modifier = Modifier.background(directionColor, shape = CircleShape).padding(4.dp)
                ) {
                    //direction indicator
                    Image(
                        painter = painterResource(
                            getColorAppropriateResource(
                                directionResLight,
                                directionResDark)
                        ),
                        contentDescription = directionContentDesc
                    )
                }
                //footness
                Box(
                    modifier = Modifier.fillMaxWidth(),
                    contentAlignment = Alignment.Center
                ) {
                    Text(text = spinSegment.footnessToText())
                }
            }
            spinSegment.spinPositions.forEachIndexed { index, position ->
                //position
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    verticalAlignment = Alignment.CenterVertically,
                    horizontalArrangement = Arrangement.Center
                ) {
                    Text(text = position.positionToText())
                }
                //'+' separator
                if(index<spinSegment.spinPositions.size-1)
                {
                    Row(
                        modifier = Modifier.fillMaxWidth(),
                        verticalAlignment = Alignment.CenterVertically,
                        horizontalArrangement = Arrangement.Center
                    ) {
                        Text(text = "+")
                    }
                }
            }
        }
    }
}

@Composable
fun TwoFtUSpCard(
    spinDirection: Char
) {
    //direction
    var directionContentDesc: String
    var directionColor: Color
    var directionResLight: Int
    var directionResDark: Int
    if(spinDirection=='r') {
        directionContentDesc = "Counterclockwise"
        directionColor = MaterialTheme.colorScheme.secondary
        directionResLight = R.drawable.rotate_left_24dp_000000_fill0_wght400_grad0_opsz24
        directionResDark = R.drawable.rotate_left_24dp_ffffff_fill0_wght400_grad0_opsz24
    }
    else {
        directionContentDesc = "Clockwise"
        directionColor = MaterialTheme.colorScheme.tertiary
        directionResLight = R.drawable.rotate_right_24dp_000000_fill0_wght400_grad0_opsz24
        directionResDark = R.drawable.rotate_right_24dp_ffffff_fill0_wght400_grad0_opsz24
    }

    ElevatedCard(
        modifier = Modifier.padding(horizontal = 12.dp, vertical = 4.dp).fillMaxWidth().wrapContentHeight(Alignment.CenterVertically)
    ) {
        Column(
            modifier = Modifier.padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Box(
                modifier = Modifier.fillMaxWidth(),
            ) {
                Box(
                    modifier = Modifier.background(directionColor, shape = CircleShape).padding(4.dp)
                ) {
                    //direction indicator
                    Image(
                        painter = painterResource(
                            getColorAppropriateResource(
                                directionResLight,
                                directionResDark)
                        ),
                        contentDescription = directionContentDesc
                    )
                }
                //footness
                Box(
                    modifier = Modifier.fillMaxWidth(),
                    contentAlignment = Alignment.Center
                ) {}
            }
            //position
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.Center
            ) {
                Text(text = "2 Foot Upright")
            }
        }
    }
}

@Composable
fun ISUSpinCode(
    codeString: String
) {
    if(codeString!="")
    {
        Text(
            modifier = Modifier.padding(start = 8.dp).fillMaxWidth().wrapContentWidth(Alignment.CenterHorizontally),
            text = codeString,
            style = MaterialTheme.typography.titleLarge,
            fontWeight = FontWeight.SemiBold
        )
    }
}
@Composable
fun FlyCard(
    difficult: Boolean
) {
    var flyCardText = ""
    if(difficult) flyCardText = "Difficult Fly"
    else flyCardText = "Fly"
    ElevatedCard(
        modifier = Modifier.padding(horizontal = 12.dp, vertical = 4.dp).fillMaxWidth().wrapContentHeight(Alignment.CenterVertically)
    ) {
        Column(
            modifier = Modifier.padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.Center
            ) {
                Text(text = flyCardText)
            }
        }
    }
}

@Composable
fun DifficultEntranceCard() {
    ElevatedCard(
        modifier = Modifier.padding(horizontal = 12.dp, vertical = 4.dp).fillMaxWidth().wrapContentHeight(Alignment.CenterVertically)
    ) {
        Column(
            modifier = Modifier.padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.Center
            ) {
                Text(text = "Difficult Entrance")
            }
        }
    }
}

@Composable
fun DifficultExitCard() {
    ElevatedCard(
        modifier = Modifier.padding(horizontal = 12.dp, vertical = 4.dp).fillMaxWidth().wrapContentHeight(Alignment.CenterVertically)
    ) {
        Column(
            modifier = Modifier.padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.Center
            ) {
                Text(text = "Difficult Exit")
            }
        }
    }
}

@Composable
fun DownArrowSeparator() {
    Image(
        painter = painterResource(
            getColorAppropriateResource(
                R.drawable.arrow_cool_down_24dp_000000_fill0_wght400_grad0_opsz24,
                R.drawable.arrow_cool_down_24dp_ffffff_fill0_wght400_grad0_opsz24)
        ),
        contentDescription = "Arrow cool down"
    )
}

@Composable
fun AdultFeatureCard(
    text: String
) {
    ElevatedCard(
        modifier = Modifier.padding(horizontal = 12.dp, vertical = 4.dp).fillMaxWidth().wrapContentHeight(Alignment.CenterVertically)
    ) {
        Column(
            modifier = Modifier.padding(8.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.Center
            ) {
                Text(text = "*Adult Feature: $text")
            }
        }
    }
}