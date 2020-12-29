# Summary

[FreeBSD][freebsd] has a feature called **login classes** that allow the system
administrator to:

- Configure resource limits in `/etc/login.conf` that will be applied to each
  new process executed under the respective login class.

- Set up aggregate resource limits for the collection of *all* processes running
  under a login class using `rctl`.

Unfortunately, FreeBSD does not include a simple way to set the login class from
a script or the command line.

The **`setloginclass`** program provided here is a simple command-line tool that
sets the login class, along with the corresponding resource limits and MAC label
from `login.conf`.

- System administrators can create scripts to explicitly set the login class of
  a program being executed.

- The `rc` subsystem can be modified to properly set both the login class and
  the MAC label of a daemon automatically started at boot time.

# Installing

```shell
make && make install
```

This will create `/usr/sbin/setloginclass`. Note that this *isn't* installed in
`/usr/local/sbin` to allow you to modify `/etc/rc.subr` to properly set the
login class for daemons started prior to mounting `/usr/local`.

# Using

This program can only be run as root.

```shell
# Set the "human" login class and its resource limits for just this command.
setloginclass human /command/run/by/human

# Same as above but also set the MAC label.
setloginclass -m human /command/run/by/human
```

# Notes

1. I originally posted this program in [this bug][original].

1. This program will not work in jails.

1. You *cannot* change the login class of a process if you're using `mac_biba`
   *and* the caller cannot use `biba/equal`. (This is an undocumented
   feature of `mac_biba`.)

1. There is a bug in `libutil` that prevents the login class from being set if
   the same operation is also going to set the MAC label *and* `biba/equal` is
   not allowed by that label. (See [this bug][label] for a patch.)

1. Don't forget to run `cap_mkdb` if you update `login.conf`.

[freebsd]: https://freebsd.org
[original]: https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=192900
[label]: https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=177698
