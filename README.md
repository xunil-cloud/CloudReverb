# CloudReverb

This is an audio plugin for algorithmic reverb. The algorithm is borrowed from [CloudSeed VST](https://github.com/ValdemarOrn/CloudSeed) 
by Valdemar Erlingsson. I try to rewrite the plugin with JUCE, so it can be also run on Linux and MacOS.

For now, there is no graphical user interface for plugin. Though, You can now control the plugin parameters from the plugin host or DAW. I might add gui (maybe using [foleys_gui_magic](https://github.com/ffAudio/foleys_gui_magic)) in the future.

Since I write this plugin on Linux, the plugin is mainly tested on Linux. It should run on MacOS and Windows, but need more test. If you find any issue, feel free to create new issue.

## How to Build

### Linux

clone the repository:

```sh
git clone --recursive https://github.com/xunil-cloud/CloudReverb.git
```

run cmake command and build the project:
```sh
cd CloudReverb
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Credit

[CloudSeed VST](https://github.com/ValdemarOrn/CloudSeed) by Valdemar Erlingsson