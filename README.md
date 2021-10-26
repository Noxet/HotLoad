# HotLoad

HotLoad - the hot-reload for C++ libraries

## Notes
For hot reloading to work, we need to replace the DLL file in use.
On Windows, this is usually not possible. However, there is a trick:
1. Rename the current DLL file in use (without moving it to another folder). This will keep the file handle valid.
1. Add the new DLL file to the folder, with the original name
1. Now, when we try to reopen the DLL, we will open the new one, profit.