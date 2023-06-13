# THE-LAZY-ALARM
2023 CSE306 system programming team project

# Environement
- Raspbian (Bullseye)
- gcc 9.4.0

# Prerequisite
- NFC Reader ACR1252U
- Button
- Speaker
- Keyboard

# Installation 

```
source install.sh
```

## compile 

```
make
```

# Server

## Pin Map
![pin map](https://github.com/psm7177/THE-LAZY-ALARM/blob/main/img/pinmap.png?raw=true)

## Music

You can enroll the music in `./music` directory. It is must be `.wav` or `.mp3`

## Run Server

```
./bin/server
```

# Client

## Command 

### Create
```
./bin/command create HH:MM
```
The HH indicates hours of range from 0 to 23.

### GET
```
./bin/command get < id | -a | -m >
```
The Get command is required for one option at least. With `id`, the server responds the information of alarm that has the `id`. Using `-a`, the server returns all the alarms in  server. The `-m` option return the list of music in `./music`.

### UPDATE

```
./bin/command update <id> [-v volume] [-d difficulty] [-t HH:MM] [-m music_id]
```
The Update command change the properties of the `id` alarm.

### DELETE
```
./bin/command delete <id>
```

The Delete command delete in the server.
