<h1 align="left">
  ReverseKit - Dynamic Reverse Engineering Toolkit<br>
  <img src="https://raw.githubusercontent.com/zer0condition/ReverseKit/master/ReverseKit.png" width=160px height=160px alt="ReverseKit Logo" align="right">
</h1>
<p align="left">
  <img src="https://img.shields.io/github/license/zer0condition/reversekit?style=flat-square" alt="License">
  <img src="https://img.shields.io/github/stars/zer0condition/reversekit" alt="Stars">
  <img src="https://img.shields.io/github/forks/zer0condition/reversekit" alt="Forks">
</p>
<p>ReverseKit is a comprehensive toolkit designed to aid reverse engineers in the process of dynamic RE. With a wide range of features and functionalities, it provides an easy-to-use interface and helps you intercept, analyze, and manipulate code and data during runtime.</p>
<p>
  <a href="#features">Features</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#usage">Usage</a> <br>
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a> •
  <a href="#images">Images</a>
</p>
<h2>Features</h2>
<ul>
  <li>Imports information - along with DLL name, function name, and address.</li>
  <li>Intercept all system calls via an instrumentation callback.</li>
  <li>Intercept all commands executed - system(), ShellExecuteEx(), etc.</li>
  <li>Intercept all thread creations - APIs like std::thread, CreateThread(), etc.</li>
  <li>Intercept URLs - APIs like UrlDownloadToFileA and InternetOpenUrlA, etc.</li>
  <li>Threads - lists thread ids along with cpu usage, allows you to suspend them with a button.</li>
  <li>Bypass common debugger checks - CheckRemoteDebugger() and IsDebuggerPresent().</li>
  <li>Anti-BSOD for common usermode BSOD methods which abuse RtlAdjustPrivilege.</li>
  <li>Easy-to-use interface powered by ImGui.</li>
  <li>Hook library with a normal JMP hook and trampoline hook.</li>
</ul>
<h2>Getting Started</h2>
<p>To get started with ReverseKit, you can clone this repository and build the project. Once the build is complete, you can inject the tool using ReverseKit Loader, or any prefered injector and start using it to analyze binaries.</p>
<h2>Usage</h2>
<p>ReverseKit provides a user-friendly interface that enables you to easily intercept, analyze, and manipulate code and data during runtime. With the wide range of features and functionalities, you can:</p>
<ul>
  <li>Monitor and intercept system calls and API functions</li>
  <li>Monitor CPU usage by active threads and option to suspend</li>
  <li>Analyze network traffic and intercept URLs</li>
  <li>Analyze and manipulate thread creations</li>
  <li>Hook into binary code and redirect execution flow</li>
  <li>Bypass common debugger checks</li>
</ul>
<h2>Contributing</h2>
<p>We welcome contributions from the community. If you find a bug or have an idea for a new feature, please open an issue or submit a pull request.</p>

 <h2>License</h2>
 <p>This project is licensed under the terms of the MIT license.</p>
 
 <h2>Sponsor</h1>
 
 <img src="https://i.imgur.com/KEOydE7.png" alt="WebSec BV logo">
 <p><b>WebSec BV</b>, a cybersecurity company based in Amsterdam, is recognized for their dedication to helping businesses and individuals protect themselves against online threats. As a valued sponsor, they have contributed significantly to the promotion of cybersecurity and the creation of a safer online world.</p>
 <p>WebSec's team of professionals is committed to staying ahead of the latest threats and developing cutting-edge solutions to keep their clients protected. Their passion for cybersecurity education has made them a trusted and reliable partner in the industry.</p>
 <p>Through their sponsorship and support, WebSec has demonstrated their commitment to promoting cybersecurity awareness and helping people stay safe online. Their contributions are greatly appreciated and have made a significant impact on the work being done in this field, such as making this project 'ReverseKit' a reality.</p>
 <p>Overall, WebSec BV is a trusted and respected leader in the fight against cybercrime, and their sponsorship and support have been instrumental in promoting a safer online world. They are a valued partner and their contributions to this important work are truly appreciated.</p>
<p>
Website: <a href="https://websec.nl/en/">websec.nl</a> <br>
Blog: <a href="https://websec.nl/blog/">websec.nl/blog</a>
</p>


<h2>Images</h2>
<div>
  <img src="https://i.imgur.com/jgGg4tt.jpg" alt="tab">
</div>

<h2>Youtube</h2>
<div>
  <a href="https://www.youtube.com/watch?v=3P8ck5U_OXY">ReverseKit YouTube Demo 1</a> <br>
  <a href="https://www.youtube.com/watch?v=3lAU0GWPQAA">ReverseKit YouTube Demo 2</a> <br>
</div>
