<h1 align="center">
  <img src="https://i.imgur.com/q92np0W.png" alt="ReverseKit Logo"><br>
  ReverseKit - Dynamic Reverse Engineering Toolkit
</h1>
<p align="center">
  <img src="https://img.shields.io/github/license/zer0condition/reversekit" alt="License">
  <img src="https://img.shields.io/github/stars/zer0condition/reversekit" alt="Stars">
  <img src="https://img.shields.io/github/forks/zer0condition/reversekit" alt="Forks">
</p>
<p>ReverseKit is a comprehensive toolkit designed to aid reverse engineers in the process of dynamic reverse engineering. With a wide range of features and functionalities, it provides an easy-to-use interface and helps you intercept, analyze, and manipulate code and data during runtime.</p>
<p>
  <a href="#features">Features</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#usage">Usage</a> •
  <a href="#contributing">Contributing</a>
  <a href="#youtube-showcase">Video Showcase</a>
</p>

<h2>Features</h2>
<ul>
  <li>Imports information - along withDLL name, function name, and address.</li>
  <li>Intercept all commands executed - system(), ShellExecuteEx(), etc.</li>
  <li>Intercept URLs - APIs like UrlDownloadToFileA and InternetOpenUrlA, etc.</li>
  <li>Bypass common debugger checks - CheckRemoteDebugger() and IsDebuggerPresent().</li>
  <li>Easy-to-use interface powered by ImGui.</li>
  <li>Intercept all thread creations - APIs like std::thread, CreateThread(), etc.</li>
  <li>Hook library with a normal JMP hook and trampoline hook.</li>
</ul>
<h2>Getting Started</h2>
<p>To get started with ReverseKit, you can clone this repository and build the project. Once the build is complete, you can inject the tool and start using it to analyze binaries.</p>
<h2>Usage</h2>
<p>ReverseKit provides a user-friendly interface that enables you to easily intercept, analyze, and manipulate code and data during runtime. With the wide range of features and functionalities, you can:</p>
<ul>
  <li>Monitor and intercept system calls and API functions</li>
  <li>Analyze network traffic and intercept URLs</li>
  <li>Analyze and manipulate thread creations</li>
  <li>Hook into binary code and redirect execution flow</li>
  <li>Bypass common debugger checks</li>
</ul>
<h2>Contributing</h2>
<p>We welcome contributions from the community. If you find a bug or have an idea for a new feature, please open an issue or submit a pull request.</p>
<h2>YouTube Showcase</h2>
<div>
  <a href="https://www.youtube.com/watch?v=3P8ck5U_OXY"><img src="https://i.imgur.com/eEoylM5.png" alt="ReverseKit demo video"></a>
</div>
