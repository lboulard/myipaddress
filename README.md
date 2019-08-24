
# Find host IP addresses

- ‟`myipaddress`” shall be same as ‟`hostname -i`” and return one IP.
  - Usually on Debian and Ubuntu, printed value is `127.0.1.1` due to GNOME
    defect requiring always a resolvable host name.
- ‟`allipaddr`” shall be same as ‟`hostname -I`” and list all non-local IP
  addresses.
