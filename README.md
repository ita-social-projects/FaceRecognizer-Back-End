<a href="https://softserve.academy/"><img src="https://s.057.ua/section/newsInternalIcon/upload/images/news/icon/000/050/792/vnutr_5ce4f980ef15f.jpg" title="SoftServe IT Academy" alt="SoftServe IT Academy"></a>

# Real-Time Face Mask Detector 

> The application recognizes if the person is wearing a mask or not.

> Input data is taken from a real-time webcam stream


[![Coverage Status](https://img.shields.io/gitlab/coverage/ita-social-projects/GreenCity/master?style=flat-square)](https://coveralls.io)
[![Pending Pull-Requests](https://img.shields.io/github/issues-pr/ita-social-projects/Real-Time-Face-Mask-Detector-Server)](https://github.com/ita-social-projects/Real-Time-Face-Mask-Detector-Server/pulls)
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

---

## Table of Contents
- [Setup](#setup)
- [Installation](#installation)
- [Documentation](#Documentation)
- [Contributing](#contributing)
  - [git flow](#git-flow)
- [License](#license)

---

## Setup

- Install **[SQLAPI](https://www.sqlapi.com/Download/)** on **C** partition
- If you don't have intalled python and pip, please install one. **[Link](https://www.python.org/downloads/)** to the download page.
- Install **conan** using this command in the command prompt:
  ```
    pip install conan
  ```
---

## Installation

- Project is using mvcs compiler, so you need to have intalled **Visual Studio** or **MSBuild tool**
- Microsoft Visual Studio Installer Projects extension for Visual Studio must be installed 
- In the repository's root directory create a folder (for instance **"conanfiles"**)
- Open command prompt and enter to this directory
- In the command prompt run the following command if you want to build your project in debug mod:
  ```
  conan install .. --settings build_type=Debug --settings arch=x86_64 --options shared=True
  ```
  In the way, you want to build it in Release:

  ```
  conan install .. --settings build_type=Release --settings arch=x86_64 --options shared=True
  ```
- Install **Qt extension** in Visual Studio, after that, in the Extensions->Qt VS Tools->Qt Options, add "**C:\Qt_vc**" in the Path field and press "OK" button

### Required to install
* [SQLAPI](https://www.sqlapi.com/Download/)
* [SQLServer](https://www.microsoft.com/en-us/sql-server/sql-server-downloads)
* [Conan](https://conan.io/downloads.html)

### Clone

- Clone this repo to your local machine using `https://github.com/ita-social-projects/Real-Time-Face-Mask-Detector-Server.git`

---

## Documentation

---

## Contributing

### Git flow
> This project is using **[Git Feature Branch Workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow)**. In our case new branch should be named as **type of task/id of ticket in your task tracking system**. For instance, if you are implementing a new feature and it's number "Lv-128", branch should be named as **feature/Lv-128**.
#### Step 1

- **Option 1**
    - 🍴 Fork this repo!

- **Option 2**
    - 👯 Clone this repo to your local machine using `https://github.com/ita-social-projects/SOMEREPO.git`

#### Step 2

- **HACK AWAY!** 🔨🔨🔨

#### Step 3

- 🔃 Create a new pull request using <a href="https://github.com/ita-social-projects/Real-Time-Face-Mask-Detector-Server/compare" target="_blank">github.com/ita-social-projects/Real-Time-Face-Mask-Detector-Server/compare</a>.

### Issue flow

---

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
- Copyright 2020 © <a href="https://softserve.academy/" target="_blank"> SoftServe IT Academy</a>.
