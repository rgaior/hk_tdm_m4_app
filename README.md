# hk_tdm_m4_app

## Setup

```
git submodule update --init
```

## Build instructions

```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DMBED_TARGET=TARGET_HK_TDM_F446RE

make -j -C build
```
