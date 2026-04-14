## About
SpinSpinner is a simple app that "spins" random figure skating spins that follow ISU (international skating union) rules. It is intended to assist skaters in coming up with spins for their programs by generating spins that adhere to the required guidelines for acquiring spin "levels".

## Installation

### App Store
TBD

### ADB/Streamed Install
To install, download the apk from [releases](https://github.com/jsfischer343/skating-spin-spinner-android/releases) or compile from source using [AndroidStudio](https://developer.android.com/studio). Then use [ADB Tools](https://developer.android.com/tools/releases/platform-tools) ```adb install path_to_apk/spinspinner.apk``` after connecting to the device using USB or wireless debugging.

## Guide

### Terminology
* Direction: Refers to the rotational direction of the spin, either clockwise or counterclockwise.
* Footness: Refers to foot that is spun on, either dominant (counterclockwise on right foot, clockwise on left foot) or non-dominant (clockwise on right foot, counterclockwise on left foot).
* Position: Camel, sit, upright, or layback that act as a base components of spins. There is also a "non-basic" or intermediate position that can appear in combo spins.
* Segment: A series of positions all on the same foot. Change foot spins will have two segments, all other spins will only have one.
* (Difficult) Variation: A change of core or body position inside of a spin position that changes the quality of the spin in a significant way.
* Feature: A special modification to the nature of a spin that makes it more difficult. In some contexts a difficult variation is also considered a feature.

### Spinner UI Layout
Spins are broken down into segments: entrance, first segment, second segment (change foot only), and exit.
* Entrance: This card will specify if the spin has a fly or a difficult entrance otherwise it is omitted.
* Segment(s): Contains the actual spinning portions of the spin. Every segment will have a direction specified by a rotation symbol in the top left and the "footness" at the top of the card. Then each position will appear in descending rows separated by a "+". Qualities of each position will be mentioned after the position. First, variations separated by "->", then features contained in parenthesis.
* Exit: This card will only appear if there is a difficult exit otherwise it is omitted. It is up to the skater to decide how to make the exit difficult.

## Disclaimers
The content in this repository and application are completely independent and are not directly or indirectly endorsed by ISU. Although a good effort is made to ensure that spins spun using this program will follow guidelines there is no guarantee. Make sure to check each spin against the most up to date rules provided by ISU.

## Credits
Argparser: [argparser.hh](app/src/main/cpp/argparser.hh) from https://github.com/fmenozzi/argparser

## AI Disclaimer
Most code written in this repository is done using existing documentation with minimal AI used to generate code in the development to create atomic functionalities and validate syntax.