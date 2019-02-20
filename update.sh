#!/bin/sh
LOCK_NAME="/tmp/tbox_upgrade.lock"
if ( set -o noclobber; echo "$$" > "$LOCK_NAME") 2> /dev/null;
then
trap 'rm -f "$LOCK_NAME"; exit $?' INT TERM EXIT

### start                                   
kill -9 $(pidof tbox)                                 
sleep 1                               
mv tbox old_tbox
mv tbox_decrypt tbox                             
chmod 777 tbox                               
sync                                          
echo upgrade success                          
reboot                                       
### end

### Removing lock
rm -f $LOCK_NAME
trap - INT TERM EXIT
else
echo "Failed to acquire lockfile: $LOCK_NAME." 
echo "Held by $(cat $LOCK_NAME)"
exit 1
fi
