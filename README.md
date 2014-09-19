# setsid for macosx

A MacOS X port of the util-linux `setsid` command.

## Synopsis

`setsid program [arg...]`

## Description

`setsid` runs a program in a new session.

## Options

| Option     | Description                                                                                                      |
| ---------- | ---------------------------------------------------------------------------------------------------------------- |
| -w, --wait | Wait the execution of the program to end, and return the exit value of the child as return value of the `setsid` |
| -c, --ctty | Ignored, added for compatibility                                                                                 |

## See also

[setsid(2)](http://linux.die.net/man/2/setsid)

## Authors

- Rick Sladkey <jrs[nospam]world.std.com> - writing the original setsid utility
- Latchezar Tzvetkoff <latchezar[nospam]tzvetkoff.net> - porting the code
