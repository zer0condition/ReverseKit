# ReverseKit

<b>ReverseKit - Reverse Engineering Aid</b>

### Features

1. <b>Imports information</b> - Prints all the imports with the origin dll name, function name and addresses respectively.
![Imgur](https://i.imgur.com/ExrfWGQ.png)


2. <b>Intercept all system/shell/all commands</b> - Hooks CreateProcessInternalW to intercept all commands, even hidden ones.
![Imgur](https://i.imgur.com/PWuwnI6.png)


2. <b>Intercept all thread creations</b> - Hooks NtCreateThreadEx to intercept all thread creations (CreateThread, std::thread, etc).
![Imgur](https://i.imgur.com/a7uPFmH.png)


<b>Work in progress, adding more necessary features.</b>
