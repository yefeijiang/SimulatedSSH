#!/usr/bin/expect
set timeout 30
spawn su - root -c "ifconfig"
expect "Password:"
send "root123\r"
expect eof
exit
