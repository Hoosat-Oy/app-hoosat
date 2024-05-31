
# Hoosat Ledger Integration

Source code for the Hoosat embedded app

# How to Load Hoosat App onto Ledger Device

## Prerequisite

### Install Docker

- Install Docker Desktop from [https://www.docker.com/products/docker-desktop/](https://www.docker.com/products/docker-desktop/)

## Download Ledger app builder Docker image

`docker pull ghcr.io/ledgerhq/ledger-app-builder/ledger-app-builder-lite:latest`

## Clone app-hoosat repository and change directory

`git clone https://github.com/Hoosat-Oy/app-hoosat`
`cd app-hoosat`

## Build and load the app-hoosat 

- Connect Ledger device to computer via USB.
- Make sure the Ledger device is unlocked and at the home screen when the installation starts. 

### Linux Building and loading
- Open app builder on Linux, make sure you are in the `app-hoosat` folder:
`sudo docker run --rm -ti  -v "$(realpath .):/app" --privileged -v "/dev/bus/usb:/dev/bus/usb" --user $(id -u $USER):$(id -g $USER) ghcr.io/ledgerhq/ledger-app-builder/ledger-app-builder-lite:latest` 
- Build and load the application for Ledger Nano S:
`BOLOS_SDK=$NANOS_SDK make load`
- Build and load the application for Ledger Nano S plus:
`BOLOS_SDK=$NANOSP_SDK make load`
- Build and load the application for Ledger Stax:
`BOLOS_SDK=$STAX_SDK make load`
- Build and load the application for Ledger Flex:
`BOLOS_SDK=$FLEX_SDK make load`
- Follow the instructions in your Ledger device to complete the installation.

### OS X Building and loading

- Open app builder on OS X, make sure you are in the `app-hoosat` folder:
`docker run --rm -ti -v "$(realpath .):/app" --privileged -v "/dev/bus/usb:/dev/bus/usb" --user $(id -u $USER):$(id -g $USER) ghcr.io/ledgerhq/ledger-app-builder/ledger-app-builder-lite:latest`
- Build and load the application for Ledger Nano S:
`BOLOS_SDK=$NANOS_SDK make load`
- Build and load the application for Ledger Nano S plus:
`BOLOS_SDK=$NANOSP_SDK make load`
- Build and load the application for Ledger Stax:
`BOLOS_SDK=$STAX_SDK make load`
- Build and load the application for Ledger Flex:
`BOLOS_SDK=$FLEX_SDK make load`
- Follow the instructions in your Ledger device to complete the installation.

## Using Ledger App
  
- Open the app in your ledger device.
- Interact with a front end like the [Hoosat Vault](https://vault.hoosat.fi).
- When prompted to sign, the front end will communicate with the ledger to sign. Verify and sign on your ledger.