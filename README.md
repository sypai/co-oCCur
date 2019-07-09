<!-- PROJECT LOGO -->

<br />
<p>
  <h1 style="color:#1C9963;">co-oCCur</h1>
  <h3>High-speed subtitle synchronization tool</h3>
  <h4>GSoC 2019 | CCExtractor Development</h4>
</p>

[![GSoC](https://img.shields.io/badge/GSoC-2019-green.svg)](https://summerofcode.withgoogle.com/dashboard/project/6506536917008384/overview/)
[![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg)](http://perso.crans.org/besson/LICENSE.html)

**co-oCCur** is a high-speed subtitle synchronization tool. 
It is being developed under GSoC 2019 with **CCExtractor development**.
<br />
Mentor: @[csfmp3](https://github.com/cfsmp3)

It consists of two tools:<br />
**Tool A** 
Use case: Synchronization of subtitles between two versions (for example, with and without commercials) of the same 
audiovisual content. <br /> 
It will take as input the original audiovisual content, the edited audiovisual content and the 
subtitles document of the original audiovisual content.


**Tool B** 
Use case: Synchronization of subtitles between two versions of the same audiovisual content in the absence of the 
original content. <br />
It will take as input the modified audiovisual content and the subtitle document for the original 
audiovisual content.

---
This project is in it's early stage and is taking baby steps towards the end goal.
Available functionality of the project is going to refactor over time.  
---
 
## Usage

1. Clone the repository from Github:

```bash
git clone https://github.com/sypai/co-oCCur
```

2. Navigate to `install` directory:

```bash
cd install
```

3. Run `build.sh`

```bash
./build.sh
```

5. Sync!

```bash
./co_oCCur -tool [tool options] <tool specific arguments>
```

## Usage Parameters

The parameters to be passed to **co-oCCur**. <br />
[NOTE: This list might change in future]

| **Parameter** 	| **Value** 	| **Description** 	|
|:---------------------:	|:-----------------------------------:	|:------------------------------------------------------------------------------------------------------------:	|
| `-tool` OR `-t` 	| NAME A OR B 	| Select the tool to be used for subtitle synchronization. REQUIREMENT: YES 	|
| `-orgAudio` OR `-org` 	| FILE /path/to/original/audio.wav 	| Original Audio File Restriction: Must be PCM mono sampled at 16000 Hz REQUIREMENT: TOOL A, YES    TOOL B, NO 	|
| `-modAudio` OR `-mod` 	| FILE /path/to/modified/audio.wav 	| Modified Audio File Restriction: Must be PCM mono sampled at 16000 Hz REQUIREMENT: YES 	|
| `-orgSRT` OR `-s` 	| FILE /path/to/original/subtitle.srt 	| Original subtitle file for the the original audio. REQUIREMENT: YES 	|


## Notes

### Requirements

* CMake 
CMake minimum version 3.14 is required.

### Building the blocks
 Running `build.sh` can result in:

```bash
bash: ./build.sh: Permission denied
```

Possible Turnaround:
* Give it execute permission (only possible if the file-system gives RW rights)
```bash
cd co-oCCur/install
chmod +x build.sh
./build.sh
```

* Use CMake to build it
```bash
# Root Directory
cmake ./
make
```

### Argument properly
* Audio Files

Make sure the audio is uncompressed raw PCM (16-bit signed int), mono sampled at 16000 Hz (Enough to cover human speech frequency range).

Using [ffmpeg](https://ffmpeg.org/documentation.html) you can run:

```bash
ffmpeg -i inputVideo.ts -acodec pcm_s16le -ac 1 -ar 16000 -o audioName.wav 
```

* Subtitle Files

The input subtitle file should be a clean and proper SubRip (SRT) file.

## Quick Demo


* IN:
```bash
./co_oCCur
```
  OUT:
  ![Run without arguments](https://raw.githubuser.com/sypai/imageBazar/blob/master/testrun1.png)

* IN:
```bash
./co_oCCur -t A -org ./install/TestFiles/WavAudio/example.wav -mod ./install/TestFiles/WavAudio/example1.wav -s ./install/TestFiles/Subtitles/example.srt
```
What will this trigger?
1. Tool A to be used for synchronization.
2. Read "example.wav" as original audio and extract audio fingerprints from it.
3. Enrich the "example.srt" file with audio fingerprint anchors at corresponding timestamps.
4. Read "example1.wav" as modified audio file. Seek fingerprints at offsets decided by enriched subtitle file, the 
timestamps of fingerprint anchors.
5. Compare the two fingerprints and detect the constant temporal offset.
6. Adjust "example.srt" using delta obtained and created a subtitle file "example_co_oCCur.srt".

## License
GNU General Public License 3.0 (GPL-v3.0)

Check [LICENSE.md](https://github.com/sypai/co-oCCur/blob/master/LICENSE.md)

## Community
You may reach CCExtractor community through the slack channel
where most CCExtractor developers hang out.
* CCExtractor channel on [Slack](https://ccextractor.org/public:general:support?)

We foster a welcoming and respectful community.

## Contributing 
Any contribution to the project would be highly appreciated! 


