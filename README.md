# CloudReverb

This is an audio plugin for algorithmic reverb. The algorithm is borrowed from [CloudSeed VST](https://github.com/ValdemarOrn/CloudSeed) 
by Valdemar Erlingsson. I try to rewrite the plugin with JUCE, so it can be also run on Linux and MacOS. For now, the GUI hasn't be functional, 
I temporarily set the plugin preset to [Hyperplane](https://github.com/ValdemarOrn/CloudSeed/blob/master/Factory%20Programs/Hyperplane.json). 
(You can change it from the [source code](https://github.com/xunil-cloud/CloudReverb/blob/master/plugin/PluginProcessor.cpp#L23), though.)


## How to Build
##### 1. clone the repository and change directory into project folder

`git clone --recursive https://github.com/xunil-cloud/CloudReverb.git`

`cd CloudReverb`

##### 2. run cmake command and build the project

`cmake -Bbuild`

(default is debug build, apppend `-DCMAKE_BUILD_TYPE=Release` flag if you want a release build.)

`cd build`

`make`

## Credit

[CloudSeed VST](https://github.com/ValdemarOrn/CloudSeed) by Valdemar Erlingsson
