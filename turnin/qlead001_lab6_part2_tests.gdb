# Test file for "Lab6_TimerSynchSMs"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

test "Wait => PORTB: 1"
setPINA 0xFF
continue 2
expectPORTB 1
checkResult

test "Wait => PORTB: 2"
continue 31
expectPORTB 2
checkResult

test "Wait => PORTB: 4"
continue 31
expectPORTB 4
checkResult

test "Wait => PORTB: 1"
continue 31
expectPORTB 1
checkResult

test "Wait => PORTB: 2"
continue 31
expectPORTB 2
checkResult

test "PINA0: 0 => PORTB: 2, state: Stop"
setPINA 0xFE
continue 31
expectPORTB 2
expect state Stop
checkResult

test "PINA0: 1 => PORTB: 2, state: WaitButton"
setPINA 0xFF
continue 31
expectPORTB 2
expect state WaitButton
checkResult

test "PINA0: 0 => PORTB: 1"
setPINA 0xFE
continue 1
setPINA 0xFF
continue 2
expectPORTB 1
checkResult

test "Wait => PORTB: 2"
continue 31
expectPORTB 2
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
