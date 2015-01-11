# tolog

## Presentation

`tolog` is a simple tool to make software that logs to standard output play nice with logrotate.

## Usage

Pipe the output of your software to `tolog`, keep a PID file. In practice you will probably do this in an init script.

    myprogram | tolog /var/log/myprogram.log & echo $! > /var/run/myprogram.tolog.pid

Tell logrotate to send `USR1` to `tolog` to reopen the logfile after rotating.

    /var/log/myprogram.log {
        missingok
        compress
        postrotate
            test -r /var/run/myprogram.tolog.pid && kill -USR1 `cat /var/run/myprogram.tolog.pid`
        endscript
    }

## Copyright

Copyright (c) 2015 Pierre Chapuis
