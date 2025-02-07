# Goodies

This forder contains useful stuff.

## Keymap: `italyprog.zip` (**Windows only**)

**NOTE: this is only needed if you have a physical italian keyboard**.

This is an enhanced keyboard map for **italian keyboards** that adds two shortcuts for characters that are often used in programming and on linux: tilda `~` and backtick `.

Those characters are mapped to `AltGR` + `'` and `AltGr` + `ì`, respectively.

To install it, proceed as follows:

1. unzip
2. run the installer `setup.exe`
3. Go to Settings > Time & language > Language & region, then click on the dots beside your language (Italian), then under the section "keyboard" click on "Add a keyboard" and search for "Italian - Programmer keys"
4. Remove the original italian keymap

## MQTT
### Running the broker

The MQTT protocol requires a broker to be running. We are going to use the broker `mosquitto`, which is a very popular and lightweight broker. On Linux, it runs as a service.

Nowadays, the most powerful and safe way for running services is to use Docker containers. So, in the first place, and regardless your operating system, you must install Docker. For Windows, you can download the installer from [here](https://hub.docker.com/editions/community/docker-ce-desktop-windows). For Linux, you can follow the instructions [here](https://docs.docker.com/engine/install/ubuntu/). For MacOS, you can follow the instructions [here](https://docs.docker.com/docker-for-mac/install/).

Once Docker is installed, the MQTT broker can be launched using the Docker container `eclipse-mosquitto`. So, open the terminal **in this folder** and type:

```bash
docker run -it --rm -p1883:1883 -v"${PWD}/mosquitto.conf:/mosquitto/config/mosquitto.conf" eclipse-mosquitto
```

Note that that the `mosquitto.conf` file must be in the current directory.

As a shortcut, you can simply run the script `run_mosquitto.sh` in this folder:

```bash
./run_mosquitto.sh
```

Once you are done, you can stop the broker with the following Docker command:

```bash
docker stop mosquitto
```

### Testing the broker

I suggest to install the free application [MQTT Explorer](http://mqtt-explorer.com/) to test the broker.