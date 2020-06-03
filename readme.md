### VPE description
```
VeriSilicon Platform Engine(VPE for short) consists of VeriSilicon Platform
Interfaces (VPI for short) and SDK. It is the control software for VeriSilicon
SoC platforms and separate IPs.
Here, VPI provides the APIs for VeriSilicon Video encoding and decoding functions.
The details are below:
    * Transcoding
          The input is HEVC,H264,VP9 bitstream up to 4K@60 FPS. The output is
          HEVC,H264,VP9 up to four channels with downscaling.
    * Encoding
          The input is raw frame.
          The output supports below:
              H264: Main Profile, levels 1 - 5.2
                    High Profile, levels 1 - 5.2
                    High 10 Profile, levels 1 - 5.2
              HEVC: Main Profile, Level 5.1, High Tier
                    Main10 profile, Level 5.1, High Tier
                    Main Still Profile
              VP9:  Profile 0 (8-bit)
                    Profile 2 (10-bit)
    * Decoding
          The input supports below:
              H264: Baseline Profile, Levels 1 – 5.1
                    Main Profile, Levels 1 - 5.1
                    High Profile, Levels 1 – 5.1
              HEVC: Main Profile, up to Level 5.2, High Tier
                    Main10 profile, up to Level 5.2, High Tier
                    Main Still Profile
              VP9:  Profile 0 (8-bit)
                    Profile 2 (10-bit)
          The output is YUV NV12.
    * Video Processing
          Download/Downscale/Format conversion
```

### VPE Plugin description
```
VPE Plugin is for the multimedia frameworks to enable video transcoding, encoding,
decoding and processing with VeriSilicon Platform Engine.
Here VPE Plugin for FFmpeg is supported and to be extended to GStreamer and others.
```

### The overall architecture:
                +----------------------------------------+
                |              |              |          |
                |  FFmpeg      |  GStreamer   |   APP    |
                |              |              |          |
                |  +-----------|  +-----------|          |
                |  | VPE Plugin|  | VPE Plugin|          |
                +----------------------------------------+
                +----------------------------------------+
                |  VPE                                   |
                |   +--------------------------------+   |
                |   |                                |   |
                |   |             VPI                |   |
                |   |                                |   |
                |   +--------------------------------+   |
                |   +--------------------------------+   |
                |   |                                |   |
                |   |            VPE Codecs          |   |
                |   |      (Dynamic Link Libraries)  |   |
                |   |                                |   |
                |   +--------------------------------+   |
                |   +--------------------------------+   |
                |   |                                |   |
                |   |         OS Wrapper Layer       |   |
                |   |                                |   |
                |   +--------------------------------+   |
                +----------------------------------------+
                +----------------------------------------+
                |                                        |
                |              Linux  kernel             |
                |                                        |
                +----------------------------------------+

### VPI directory
```
├── build                         Configure files for building
├── doc                           Documentations
│   ├── enc_params_h26x.md        The enc_params parameter description for h26x encoder
│   └── enc_params_vp9.md         The enc_params parameter description for vp9 encoder
├── drivers                       The linux driver
│   ├── build_drivers.sh          The script to build the driver
│   ├── load_drivers.sh           The script to load the driver
│   ├── README.md                 Readme
│   └── transcoder-pcie           Driver source code
├── firmware                      The firmware for VeriSilicon Platform
│   └── ZSP_FW_RP_V017.bin
├── Makefile                      Makefile
├── readme.md                     Readme
├── sdk_inc                       VeriSilicon Platform Codec SDK header files
├── sdk_libs                      VeriSilicon Platform Codec SDK libraries
└── vpi                           VeriSilicon Platform Interfaces source code
    ├── inc                       VPI header files
    ├── Makefile                  VPI Makefile
    ├── src                       VPI source code
    └── utils                     VPI log code

```

### How to build, install and use VPI module

* make help           - help info
* make                - make all with VPI library and transcoder-pcie driver
* make vpi            - make only VPI library
* make drivers        - make only transcoder-pcie driver
* make install        - copy the vpe sdk libs to default search path
                          install the pcie driver
                          run "sudo make install" if permission denied
* make clean          - make clean VPI and drivers

### How to make FFmpeg
* How to make FFmpeg
  ```
  cd ffmpeg
  ./configure --enable-vpe  --extra-ldflags="-L/lib/vpe" --extra-libs="-lvpi"
  make
  ```
