# USF4 Rollback

A process-inspection and modification tool for the Steam release of _Ultra Street Fighter 4_.

[TOC]

## Running

### Supported environments
* Windows: Windows 10 or later
* Linux: Fedora 40+, Steam Deck

### Running on Windows

Windows users with a working Steam installation can run USF4 Rollback by extracting a release then double-clicking on `Launcher.exe`. USF4 Rollback will attempt to detect your SF4 installation automatically. Windows users with uncommon or damaged Steam installations may run `Launcher.exe` with the `STEAM_APP_PATH` environment variable to the absolute path of the `Super Street Fighter IV - Arcade Edition` directory installed by Steam. You can navigate to this directory using the Steam library's context menu by right-clicking on Ultra Street Fighter IV's library entry, hovering over "Manage", then selecting "Browse local files", as shown below.

![The Steam right-click context menu, opened on the Ultra Street Fighter 4 library list entry](images/browse-local-files-context-menu.png)

### Running on Linux

The most straighforward way to launch USF4 Rollback on Linux is with [protontricks](https://github.com/Matoking/protontricks). Extract the release, then run `protontricks-launch Launcher.exe` and select SF4 from the popup UI. For convenience, `protontricks-launch --appid 45760 Launcher.exe` can be used to launch USF4 Rollback non-interactively, ex. from shell scripts or program shortcuts.

Linux users who do not install `protontricks` may set the `STEAM_APP_PATH` environment variable to the path of the the `Super Street Fighter IV - Arcade Edition` directory installed by Steam, as demonstrated above. Users should take care to ensure the variable points to a Windows-formatted path accessible from within the Proton container for SF4, and it may be helpful to take advantage of Wine providing the Linux system root as the `Z:` root inside Wine to specify the path. For example, if the local directory is available at `/home/steamdeck/.local/share/Steam/steamapps/common/Super Street Fighter IV - Arcade Edition`, the corresponding path through the Proton container would be `Z:\\home\\steamdeck\\.local\\share\\Steam\\steamapps\\common\\Super Street Fighter IV - Arcade Edition`.

## Building

`USF4 Rollback` is built primarily with [Visual Studio](https://visualstudio.microsoft.com/)
2019 16.10 or later with Visual C++. Other development environments will need
support for installing dependencies, ideally via [vcpkg](https://vcpkg.io/en/index.html)
and build file generation via [CMake](https://cmake.org/).

To build USF4 Rollback with VS2019 16.10+:

1. Follow steps 1 and 2 in [`vcpkg`'s Getting Started guide](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started),
   stopping after `vcpkg` has been bootstrapped.
   - You can stop at step 3- USF4 Rollback already has a manfest file.
2. Set up a local `CMakeUserPresets.json` to describe your environment.
   The following can be used as a quickstart, making sure to provide the
   path to the copy of `vcpkg` checked out in step 1: 
```
{
    "version": 2,
    "configurePresets": [
      {
        "name": "default",
        "inherits": "x86-msvc-ninja-relwithdebinfo",
        "environment": {
          "VCPKG_ROOT": "C:/Users/myuser/path/to/vcpkg"
        }
      }
    ]
  }
  
```
   - Since SF4 is a 32-bit executable, `USF4 Rollback` and its dependencies
     (most importantly Detours) also need to be built targeting a
     32-bit host to properly hook SF4's instructions.
3. Open `CMakeLists.txt` with VS2019's native CMake integration.
   - Ensure [CMakePresets.json integration in Visual Studio](https://learn.microsoft.com/en-us/cpp/build/cmake-presets-vs?view=msvc-170#enable-cmakepresets-json-integration) is enabled.
4. Run `Build All`. Confirm that `Launcher.exe` and `Sidecar.dll` are in
   the build output.
5. Run `Launcher.exe`.

To build USF4 Rollback with the CMake command line:

1. Set up `vcpkg`, as above in step 1.
2. Set up a local `CMakeUserPresets.json` to describe your environment,
   as above in step 2.
3. Using a CLI environment with CMake and a compiler prepared, run
   `cmake --preset default` from the root of the repository.
   - VS users may wish to use either the x86 Native Tools or the
     x64_x86 Cross Tools developer command prompts, as they already
     provide tools like Ninja and Cmake, and have the various environment
     variables used by CMake already prepared.
4. Build USF4 Rollback by running `cmake --build ./path-to-binary-dir/` from the root
   of the repository. Confirm that `Launcher.exe` and `Sidecar.dll` are in
   the build output.
   * If a `CMakeUserPresets.json` file like the one in step 2 is used, the
     the binary dir is `./msvc-build/default`.
5. Run `Launcher.exe`.

Builds generated with CMake that cannot take advantage of `vcpkg` will need to
provide the following dependencies:

* [Detours](https://github.com/microsoft/Detours). Detours is used to install
  custom netplay hooks at runtime.
* [ValveFileVDF](https://github.com/TinyTinni/ValveFileVDF). ValveFileVDF
  is used to parse Steam's configuration files, to automatically detect
  your installation of SF4.
* [Dear Imgui](https://github.com/ocornut/imgui). Dear Imgui is used to
  provide custom overlays for new features and non-durable,
  development-time debugging.
* [spdlog](https://github.com/gabime/spdlog). `spdlog` is used to provide
  durable file logging, both at development time and in release builds.
* [nlohmann/json](https://github.com/nlohmann/json). `json` is used for
  message serialization.
* [GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets/).
  `GamenNetworkingSockets` provides a very helpful high-level API on top
  of message passing, and additionally supports NAT hole punching if
  a signalling server is run.
* [GGPO](https://github.com/pond3r/ggpo), used to provide rollback.




# INSTRUCTIONS (Rollback)

                              HOW TO RUN 
1. Extract the Mod. It does not need to be in any specific folder,
    but it must be in it's own folder. "Launcher.exe" is how you run
    the Mod. DO NOT HAVE THE VANILLA GAME OPEN ALREADY, if it is open already, Close it!
    (If you're Smart, you will add the Launcher as a Non-Steam game, inside Steam, so you get the Steam Overlay)
							
2. BEFORE you try the Rollback!!!! Boot the vanilla game, and goto OFFLINE VERSUS,
     PLAYER VS PLAYER, and set Rounds to 3, time to 99, and EDITION SELECT ON! ALL players involved MUST do this!
							
3. Launch "Launcher.exe"

4. You likely will think that nothing happend, look in your taskbar for SF4, and bring SF4 to the front.
     Sometimes the Command Prompt will open in front, this is normal, just use the Taskbar, or ALT+TAB to swap over to the game. 

5. MASH LIGHT KICK UNTIL YOU GET TO THE MAIN MENU!!!! (Do so on whatever device you intend to play with, this is to avoid a bit of jank later!)
     (IF you don't do this, you may think the game is unresponsive, and can't get to the main menu. If you press Light Kick on another device,
     then when you get into the actual match, your actual device might not work in the match, but will work fine in the menus)

6. When you get to the MAIN MENU, you will notice a very primitive menu that is not there in the vanilla game. (You have a DELAY option, MOST people will set this to "1", if you have consistent desynces and crashing, bump this to "2")
      This is where everything for you will select Character, Costume, Colors, Ultras, Taunts, Stages, choose your EDITION, Handicaps, and so on.
      The first thing you need to do with this Menu is goto the lower right, and expand it out some, make sure to make it taller so you can find your Ultras and your SEND CHAR button.     
      (SEND CHAR acts as your Ready button.)
      DECIDE if you want to host or join.
            6a. IF HOSTING: Click HOST on the Network Menu, BE SURE TO TYPE A NAME, and click HOST. Give anyone joining your IP address.
                (IF you are Port Forwarding, this will be your real IP Address. IF you are using ZeroTier, give them your ZerotTier IP Address.)
            6b. IF JOINING: Get the IP Address of the Host, and paste it into the SESSION SERVER field, followed by ";23456", BE SURE TO TYPE A NAME!
              
7.  Select your character, costume, color, Ultra, and so on from the drop-down box, when you have selected the character you want, and your Ultra and so on, click SEND CHAR.
            NOTE 1: "0" is the default for everything. The Rollback's Menu WILL NOT pull your saved character settings from the vanilla game!
                  So the Value for Chun-Li's Hosenka is "0", her Kikosho is "1", and ULTRA COMBO DOUBLE is "2"
                  Handicap value "0" is NO HANDICAP
            NOTE 2: Character appear in the list in the order they were added to the game, and use the JAPANESE NAMES!
                  This means Dictator, Boxer, and Claw are Vega, M. Bison, and Balrog respectively
            NOTE 3: IF both players do not enter anything into the NAME field, the Rollback mod will throw an error, and the error is unrelated. JUST ENTER A NAME!  

8. If the Menu is annoying, click the Arrow in the upper left corner of the NETWORK MENU, to collapse, and expand. 



# INSTRUCTIONS (Fixing Errors)

If  you get  error 0x0000142  
Install x86 Redistrobutables  
Then install x64  
(In that order!)  
https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170



# INSTRUCTIONS (In-Game Settings)

               In-Game Settings
1. Goto your Settings from the main menu, and goto GRAPHICS.
2. Find the setting labeled VSYNC, and set it to OFF.
3. Find the setting labeled FRAMERATE, and set it to FIXED.
4. Find the setting labeled FILTERING, and set it to OFF, or no more than 4X (This looks nice, but it causes input delay)


             In-Game Settings (Rollback specific)
1. Follow Instructions detailed above.  
2. Goto the Main Menu, and goto VS, then select PLAYER VS PLAYER, and make sure your settings are as follows  
       ROUNDS: 3  
       TIME: 99  
       EDITION SELECT: ON  
       You are welcome to save your replays, just know there is some jank involved with playing them back, best to use OBS-Studio for recording replays.)
                                                                                        
 3. Be sure if you are setting these settings from within the Rollback launcher, you go all the way to the Character selection screen in PLAYER VS PLAYER VERSUS MODE.   
     If you don't get tot he Character Selection screen, it WILL NOT SAVE your settings!                                 
 4. (Semi-optional: Refer to Nvidia Control panel Tutorial)  



# INSTRUCTIONS (NVIDIA Control Panel) 

REDUCING INPUT LAG EVEN OFFLINE  

I'm going to teach you how to minimize input delay in SF4, this method will work with other games, but SF4 gets a huge boost from this. Even offline.  

1. Right-Click your Desktop, and Select NVIDIA CONTROL PANEL.  
                                   
2. Click on MANAGE 3D SETTINGS, and click PROGRAM SETTINGS, then locate SUPER STREET FIGHTER IV: ARCADE EDITION in the list.   
       If it isn't listed, click ADD, it might be in that list, if it isn't in that list, click BROWSE, and find the SSFIV.exe file, and click OK.   
             (For MOST people this will be "C:\Program Files (x86)\Steam\steamapps\common\SUPER STREET FIGHTER IV ARCADE EDITION\SSFIV.EXE)

3. Find the setting labeled LOW LATENCY MODE, and set it to ULTRA.  
Find the setting labeled POWER MANAGEMENT MODE, and set it to PREFER MAXIMUM PERFORMANCE.  
Find the setting labeled THREADED OPTIMIZATION, and set it to ON.  
Find the setting labeled VERTICAL SYNC, and set it to FAST,  

(USF4 players, BE SURE TO REFER TO "_Instructions (In-Game Settings)")



# INSTRUCTIONS (Swapping Between Ultra & SSFIVAE)

To revert the game from ULTRA to SSFIVAE:  
Navigate to USFIV game on Steam, click the GEAR ICON, Select DLC, and search for ULTRA, UNCHECK ULTRA UPGRADE and let the game redownload the gamefiles.  

This will revolt the game to the final update of SSFIVAE, complete with UI differences, wallpapers, Announcer Lines, Arcade Mode Stories & Endings, and even a seperate save file! That's right, you can start over from Zero Points, if you so desire.  

To revert the game BACK TO ULTRA:  
Simply Recheck the ULTRA UPGRADE, and let Steam redownload the remaining 2.25GB DLC.  

If switching between versions, you will maintain your Save Data for each version seperately! It really seems like this is expected of you to do. 



 # INSTRUCTIONS (ZeroTier)

                                WHY I USE ZEROTIER  
I USE ZEROTIER SOLELY BECAUSE I AM RESTRICTED BY COMCAST FROM HOLE PUNCHING, AND PORT FORWARDING!  

ZEROTIER IS NOT REQUIRED TO PLAY THIS MOD!  

YOU MAY HOLE PUNCH, OR PORT FORWARD! I CANNOT, SO I WILL NOT BE GIVING A TUTORIAL ON THESE METHODS!  
IF YOU WISH TO DO WITHOUT ZEROTIER OR SOMETHING SIMILAR, GOOGLE YOUR PREFERED METHOD!  
                      
                                  ! DISCLAIMER !  
IF YOU DO NOT TRUST A PERSON, OR ANY SOFTWARE, DO NOT RUN IT! IF YOU INTEND TO RUN  
SOMETHING LIKE ZEROTIER ONLY RUN THE SOFTWARE WHEN YOU ARE PLAYING!!!!  

                              THINGS YOU SHOULD KNOW  
1. When not playing the Mod, exit ZeroTier.                          
2. ZeroTier, and other like-programs, such as VPNs, and such, are all additional code, and potentionally an addition attack vector!  
   I highly doubt ZeroTier is a problem in itself, as it is free, and open sourced software, and is used for literal space-travel.  
3. IF SOMEONE IS CONNECTED TO ZEROTIER AND GETS INFECTED, THIS COULD PUT OTHERS CONNECTED TO ZEROTIER AT RISK!  
   HOWEVER! SEVERAL PEOPLE, HIGH PROFILE PEOPLE HAVE BEEN CONNECTED TO MY NETWORK FOR OVER A YEAR, AND WE'VE NOT HAD ANY ISSUES!  
   IF YOU ARE CONCERNED ABOUT THIS, PERHAPS ZEROTIER (AND FRANKLY ALL VPN-LIKE SOFTWARE) IS NOT FOR YOU!  

                           How to Install & use ZeroTier  
1. Download and Install ZeroTier, from their Website, or the Github.   
                        
---->    Download Site: https://www.zerotier.com/download/  
Github: https://github.com/zerotier/ZeroTierOne   
(For niche uses, Distros, or those who wish to Build from Source)  
----->   For MOST people,you want the Windows Version.  
DIRECT LINK TO OFFICIAL WINDOWS INSTALLER: https://download.zerotier.com/dist/ZeroTier%20One.msi  

2. Run the Installer, keep clicking NEXT until it finishes, very standard.
        ZeroTier will run in your Taskbar, botton right near your Clock. Often you will need to click the UP ARROW
        to find the Icon. Right click the Icon and select JOIN NEW NETWORK. 
                           
 ----->   Paste in the NETWORK ID, the Network most people are using, and I personally pay for
is NETWORK ID "272f5eae16d120c0" - Whoever you intend to play with all need to be on the same ZeroTier Network  

3. To find your personal ZeroTier IP ADDRESS, right click your ZeroTier icon in the Taskbar, and goto the Network,
    (In this case NETWORK ID "272f5eae16d120c0"), and goto MANAGED ADDRESSES, then left click the IP ADDRESS.

    ----------->   This copies your ZEROTIER IP ADDRESS to the clipboard, you will need this ZeroTier IP ADDRESS to give to your friends 
                   if you want to play, BE SURE to paste the IP ADDRESS followed by ":23456"!

(Refer to Rollback Instructions from this piont forward!)

    NETWORK ID: 272f5eae16d120c0
    TYPE IP ADDRESS FOLLOWED BY 23456
    WITHOUT "23456"  
    YOU WILL NOT MAKE A CONNECTION!    



## License

This project is licensed under the MIT License - see the LICENSE.md file for details.

## External Licenses and Copyright Information

Street Fighter, Street Fighter 4, Ultra Street Fighter 4, and all related software
Copyright © CAPCOM.

Steam
Copyright © Valve Corporation.

Visual Studio, Visual Studio 2019, vcpkg, and Detours
Copyright © Microsoft Corporation.

CMake - Cross Platform Makefile Generator
Copyright © Kitware, Inc. and Contributors.

ValveFileVDF
Copyright © Matthias Möller.

Dear Imgui
Copyright © Omar Cornut

spdlog
Copyright © 2016 Gabi Melman.

nlohmann/json
Copyright © 2013-2022 Niels Lohmann

GameNetworkingSockets
Copyright © 2018, Valve Corporation

GGPO (Good Game Peace Out)
Copyright © GroundStorm Studios, LLC.
