# Hoosat Ledger Integration

Source code for the Hoosat embedded app.

# How to Load App onto Ledger Device

## Prerequisite

### Install Docker, VSCode and Ledger Tools Extension

- Install VSCode from [https://code.visualstudio.com/](https://code.visualstudio.com/)

- Install Ledger Dev Tools extension from [https://marketplace.visualstudio.com/items?itemName=LedgerHQ.ledger-dev-tools](https://marketplace.visualstudio.com/items?itemName=LedgerHQ.ledger-dev-tools)

- Install Docker Desktop from [https://www.docker.com/products/docker-desktop/](https://www.docker.com/products/docker-desktop/)


## Build Source Code

- Open Docker Desktop and make sure that it is running.
- Open the repository folder with VSCode.
- Open the Ledger Dev Tools extension from VSCode sidemenu.
  - Click on 'Docker Container > Update container' to update the Docker container. Wait for it to update.
  - Click on 'Build > Build' to build the source code.
 
## Load onto Device

- Connect ledger device to computer via USB.
- Make sure device is unlocked and at the home screen.
- In VSCode, click 'Device Operations > Load app on device' to load it onto your ledger.
- Click accept to the prompts asking if you would like to install.

## Using Ledger App

- Open the app in your ledger device.
- Interact with a front end like the Hoosat Vault.
- When prompted to sign, the front end will communicate with the ledger to sign. Verify and sign on your ledger.
