# LGX2 Userspace driver (with LGX support)

This software is based on : [ChrisAJS/lgx2userspace](https://github.com/ChrisAJS/lgx2userspace)

All thanks to them to make it possible.

All I do is minor tweak to meet my needs.

Maybe someone else would find it useful


This project contains a userspace driver for the [AverMedia LGX2 (GC551)](https://avermedia.com/LGX2) as well support for the [AverMedia LGX (GC550)](https://avermedia.com/LGX).

It can be used to display the captured video and audio in a standalone window or
to forward the captured video and audio to a virtual video capture device.

## Building
To build the project, you will need:
* Libusb
* Libpcap
* SDL2

Execute the following commands to build in the root of the project:

```bash
make -j $(nproc)
```


You want to clean it ?

```bash
make clean
```

## Running
Once udev has been configured to grant read and write permission to the device it
will be possible to run the application by executing `lgx2userspace.elf`.

If you are using the LGX GC551, create a file next to `lgx2userspace.elf`, name  that file
`settings.ini` in it place this text :
``[Settings]
LGX2Mode=1``

The application will take a few seconds to run as it streams setup information to the device, 
and it will eventually display a window that will then start to display captured frames.

In my limited testing, certain devices may need to be re-plugged in after the application has 
started. For example, a Nintendo Switch will not recognise the LGX/LGX2 as an output source until
it is undocked and re-docked.

### Options when running
Press the keyboard touch F1 will bring an ImGui menu bar on top, you will be able to change the 
frame size (not window size, this is not implemented yet).
More options will come later feel free to suggest

### Gathering diagnostic information
To help diagnose problems that may be fixed in the future, when submitting an issue
please try to give as much information about your setup as possible and in the case of
inadequate video or audio output, run the userspace driver with the `-v` switch enabled.

With the `-v` switch enabled, the driver will output information regarding time taken to
process frame data and time taken to render both audio and video.

This information could be valuable when identifying issues so please try your best to include it
in any issues you raise, thank you!

### Pulseaudio Setup
Pulseaudio can be configured by issuing the following commands:
```bash
pactl load-module module-null-sink sink_name=lgx2 sink_properties=device.description=LGX2
pactl load-module module-remap-source master=lgx2.monitor source_name=lgx2 source_properties=device.description=LGX2Audio
```
This will create an audio sink called `LGX2 Audio Sink` which can be added to OBS as a Pulseaudio output capture device.

**NOTE: Even if you are using the LGX GC550- leaving the names as lgx2 is required and will not affect behaviour.**
**NOTE: The best way to control the audio volume is to use the gain OBS filter whilst leaving the sink volume at max.**

### Running with configure V4L2 device
Run the userspace driver with the `-d` option to specify which V4L2Loopback device to use:

```bash
./lgx2userspace -d /dev/video99
```

To run using the Pulseaudio sink that you created, use the `-a` switch with the name of the sink:

```bash
./lgx2userspace -a lgx2
```

**NOTE: You may need to unplug and replug in your video source.**

Go to OBS or other streaming software and select the LGX2 V4L2 source. You should now see video from the input device being output.

## Demo
See it in action over at [YouTube](https://www.youtube.com/watch?v=-yzHMbUn-w0).
