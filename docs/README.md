# HK_TDM_M4_App
This is the repository for HK_TDM_V1 board ARM-M4 applications. The app runs on the V1 release of the HW.

## ARM-M4 on HK_TDM_V1: SW Documentation
This document describes the procedure to compile the ARM-M4 application of the HK_TDM_V1.

The SW is intended to be compiled in the mbed-os environment.
The project tree consists of the following directories:

    1. the mbed-os project directory, which is the root of the project;
    2. the application repo, `hk_tdm_m4_app`, which contains the program main;
    3. the `redlibs` repo, with all the custom libraries eventually needed by the application.

In the mbed-utils folder of the project repo there are some useful files for mbed-os project initialization contained.

The files that must be copied, as explained in point 8, can be automatically copied running the script file setup_prj.sh from the Git bash shell, which is likely to be already installed for git. Otherwise, they can be copied one by one and edited and modified as needed.

1. Install mbed-os in the [Mbed OS release page](https://os.mbed.com/mbed-os/releases/)

1. In case mbed-cli is not installed yet, use the following link and complete the installation of all the packages:  
https://os.mbed.com/docs/mbed-os/v5.11/quick-start/offline-with-mbed-cli.html
NB: the version this project has been compiled by is the Mebd-OS 5. For later versions, check compatibility.

1. create a new Mbed OS project:  

    ```
    mbed new --program project_name
    ```

1. from bitbucket, clone ‘redlibs’ repo into ‘project_name’ folder; ‘user’ must be changed with your account name on bitbucket:

    ```
    $ git clone https://user@bitbucket.org/fabrizio_ameli/redlibs.git
    ```

1. from gitlab.cern.ch, clone ‘hk_tdm_m4_app’ repo into ‘project_name’ folder; use your account to access gitlab

    ```
    $ git clone ssh://git@gitlab.cern.ch:7999/fameli/hk_tdm_m4_app.git
    ```

    ```
    $ git clone https://gitlab.cern.ch/fameli/hk_tdm_m4_app.git
    ```

1. In the `mbed-utils` folder, there some files and a folder that need to be copied in the project root directory. This task can be accomplished by running a script file located in the `mbed_utils` directory: open a bash shell there and invoke the following command:

    ```
    $ source ./setup_prj.sh
    ```

    1. Copy file mbed-utils/custom_targets.json in the mbed-os project root directory to add the target board to the mbed-os compiler. More information on this process, called `target porting`, can be found at this [mbed-os page](https://os.mbed.com/docs/mbed-os/v5.15/porting/index.html). Instructions on how a target can be configured can be found in the [project configuration page](https://os.mbed.com/docs/mbed-os/v5.15/reference/adding-and-configuring-targets.html). Here is where you can configure the preprocessor macro, using the syntax `macros:["NAME=VAL"]`; for example `macros:["CLOCK_SOURCE=USE_PLL_HSI"].

	1. Copy folder mbed-utils/TARGET_HK_TDM_F446RE, contained in the mbed-utils directory, in the  mbed-os project root directory:
        The files in the folder are dedicated to processor pins configuration:
        
        `PeripheralNames.h`  
        `PeripeheralPins.c`  
        `PinNames.h`  
        `system_clock.c`  

	1. Copy file mbed-utils/.mbed in the mbed-os project root directory to setup mbed compiler. The file contains the following lines:  

        ```
        ROOT=.
        TOOLCHAIN=GCC_ARM
        TARGET=HK_TDM_F446RE
        ```

	1. Copy file mbed-utils/.mbedignore in the mbed-os project root directory to instruct mbed compiler to ignore some files. Should contain at least the following lines:  
        ```
        mbed-os/components/*
        redlibs\common\boards\*
        redlibs\targets\m4\boards\*
        ```

The `mbed_app.json` macro configuration is now embedded inside `custom_target.json` file and is not necessary anymore to copy it in the project directory.

Everything is setup now! Run compilation invoking the following command from shell:  
```
$ mbed compile
```
