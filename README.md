<p align="center"><img src="assets/image.png" alt="board"></p>
<p align="center"><h1 align="center">Word Hunt Solver</h1></p>
<p align="center">
	<em><code>❯ Beat your friends at Word Hunt</code></em>
</p>
<p align="center">
	<img src="https://img.shields.io/github/license/twilkhoo/WordHuntSolver?style=default&logo=opensourceinitiative&logoColor=white&color=0080ff" alt="license">
	<img src="https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white" alt="c++-badge">
	<img src="https://img.shields.io/github/languages/count/twilkhoo/WordHuntSolver?style=default&color=0080ff" alt="repo-language-count">
</p>
<p align="center"><!-- default option, no dependency badges. -->
</p>
<p align="center">
	<!-- default option, no dependency badges. -->
</p>
<br>

##  Table of Contents

- [ Overview](#overview)
- [ Features](#features)
- [ Demo](#demo)
- [ Project Structure](#project-structure)
- [ Getting Started](#getting-started)
  - [ Prerequisites](#prerequisites)
  - [ Installation](#installation)
  - [ Usage](#usage)
    - [ Client](#client)
    - [ Server](#server)
- [ Future Improvements](#future-improvements)
- [ License](#license)


##  Overview

Word Hunt is an iOS game pigeon game, similar to Boggle. I never understood the actual strategy behind this, so I made this project to find all words on a given board and display them neatly.

## Technical Process

The game board is entered in as a 4x4 grid on the frontend. After pressing submit, the board, represented as a comma-separated string, is sent in a `HTTP GET` request to a `C++` server listening for connections. When the server begins, it parses a dictionary (in the form of a line-separated .txt file) into a trie, to allow easy lookup of possible words in the board. After creating the trie, a TCP socket listening for clients, and once a client receives a request, it backtracks through all possible words, and checks if they are dictionary words in the trie. Once all words have been determined, the letters (including letter cells) are sent back to the client as a response. The frontend client then presents all the possible words that are >3 charcters long, in a graphical grid form. 

##  Features

Backend built with:
- C++ 20
- Multithreading to support multiple concurrent clients, and for word finding using a backtracking algorithm
- TCP raw sockets for reliable message transfer
- HTTP request/response format

Frontend built with:
- Next.js (with React)
- Shadcn for UI components
- TailwindCSS for styling

## Demo

Below is a working demo of the frontend, and backend.

https://github.com/user-attachments/assets/4a1b527e-ba37-4dd0-ab96-c5251570a8c0


##  Project Structure

```sh
└── WordHuntSolver/
    ├── README.md
    ├── client
    │   ├── .eslintrc.json
    │   ├── .gitignore
    │   ├── README.md
    │   ├── app
    │   │   ├── favicon.ico
    │   │   ├── fonts
    │   │   │   ├── GeistMonoVF.woff
    │   │   │   └── GeistVF.woff
    │   │   ├── globals.css
    │   │   ├── layout.tsx
    │   │   └── page.tsx
    │   ├── components
    │   │   ├── Grid.tsx
    │   │   ├── PathBoard.tsx
    │   │   ├── PathBoardCard.tsx
    │   │   └── ui
    │   │       ├── button.tsx
    │   │       ├── card.tsx
    │   │       ├── input.tsx
    │   │       └── tooltip.tsx
    │   ├── components.json
    │   ├── lib
    │   │   └── utils.ts
    │   ├── next.config.mjs
    │   ├── package-lock.json
    │   ├── package.json
    │   ├── postcss.config.mjs
    │   ├── tailwind.config.ts
    │   └── tsconfig.json
    └── server
        ├── CMakeLists.txt
        ├── assets
        │   └── words_alpha.txt
        ├── main.cpp
        ├── notes.txt
        ├── server.cpp
        ├── solver
        ├── solver.cpp
        ├── solver.h
        ├── util.cpp
        └── util.h
```

##  Getting Started

###  Prerequisites

Before getting started with WordHuntSolver, ensure your runtime environment meets the following requirements:

- **Programming Language:** TypeScript, C++ 20, g++ 9.4.0
- **Package Manager:** npm, Cmake


###  Installation

Install WordHuntSolver frontend (client) and backend (server) seperately:

**Build from source:**

1. Clone the WordHuntSolver repository:
```sh
git clone https://github.com/twilkhoo/WordHuntSolver
```

2. Navigate to the project directory:

```sh
cd WordHuntSolver
```

3. Install the project dependencies:

```sh
npm install
```

###  Usage

#### Client 
Run WordHuntSolver client using the following command, invoking Next.js:

```sh
npm start
```

Or, if developing, run:
```sh
npm run dev
```

#### Server
To invoke the server, first build using cmake:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

From the build folder just made, run one of: ./server to use port 5000 for the server, or specify a server port: ./server <portNo>

##  Future Improvements

- Add a custom STL, just for fun.
- Support different board types beyond the standard 4x4.

##  License

This project is protected under the [SELECT-A-LICENSE](https://choosealicense.com/licenses) License. For more details, refer to the [LICENSE](https://choosealicense.com/licenses/) file.
