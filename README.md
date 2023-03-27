# ReverseKit

<b>ReverseKit - Reverse Engineering Aid</b>

### Features

Hooking library - Normal JMP instruction hook and trampoline hook.

1. <b>Imports information</b> - Prints all the imports with the origin dll name, function name and addresses respectively.
2. <b>Intercept all system/shell/all commands</b> - Hooks CreateProcessInternalW to intercept all commands, even hidden ones.
3. <b>Intercept URLs sent through URLDownloadToFileA and InternetOpenUrlW </b> - Hooks these functions to intercept all URL links.
4. <b>Debugger check bypass </b> - Hooks IsDebuggerPresent and CheckRemoteDebuggerPresent and returns FALSE.

<b>Work in progress, adding more necessary features.</b>

Some images:
![Imgur](https://i.imgur.com/ExrfWGQ.png)
![Imgur](https://i.imgur.com/PWuwnI6.png)
![Imgur](https://i.imgur.com/a7uPFmH.png)
