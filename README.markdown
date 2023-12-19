# Levels

Basic level meter. Useful for testing and debugging plug-ins that don't have a built-in level meter.

![](screenshot.png)

For more info on how this plug-in works, [check out the blog post](https://audiodev.blog/levels-plugin/).

## How to build

This plug-in uses [JUCE](https://juce.com) and is built using CMake. Make sure [JUCE's CMake support](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md) has been installed on your system.

### macOS

```text
$ git clone https://github.com/hollance/levels.git
$ cd levels
$ cmake -B build -G Xcode
$ cmake --build build -j --config Release
```

The resulting AU and VST3 will be installed in `~/Library/Audio/Plug-Ins/`.

I have only tried it on an Intel Mac running macOS 12 (Monterey) with Xcode 14.2 and JUCE 7.0.8.

### Windows

```text
$ git clone https://github.com/hollance/levels.git
$ cd levels
$ cmake -B build -G "Visual Studio 17 2022"
$ cmake --build build -j --config Release
```

The resulting VST will be installed in `C:\Program Files\Common Files\VST3` (you may need to change permissions on this folder).

I have only tried it on Windows 10 with Visual Studio 2022 and JUCE 7.0.9.

## License

The source code in this repo is licensed under the terms of the [MIT license](LICENSE).
