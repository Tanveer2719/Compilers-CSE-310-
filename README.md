# CSE-301: Compilers Sessional

Welcome to the **CSE-301: Compilers Sessional** repository! This course is designed to guide us through the process of constructing a simple compiler. Each offline assignment builds a critical component of the compiler, from the symbol table to intermediate code generation.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Assignments](#assignments)
   - [Offline 1: Symbol Table](#offline-1-symbol-table)
   - [Offline 2: Lexical Analyzer](#offline-2-lexical-analyzer)
   - [Offline 3: Syntax and Semantic Analysis](#offline-3-syntax-and-semantic-analysis)
   - [Offline 4: Intermediate Code Generation](#offline-4-intermediate-code-generation)

---

## Introduction

The primary objective of this sessional is to construct a simple compiler step by step. After completing the assignments, I have gained hands-on experience in developing critical components of a compiler, including symbol tables, lexical analysis, parsing, semantic analysis, and intermediate code generation.

---

## Assignments

### Offline 1: Symbol Table
Path: [Offline 1](./Offline%201)

The first step in constructing a compiler is implementing a **Symbol Table**. A Symbol Table is a data structure that stores information about various entities in the source program, such as:

- Identifiers
- Objects
- Function names

Each entry in the Symbol Table may contain information about the entity's type, value, and scope. In this assignment, the Symbol Table is implemented as a list of hash tables, where each hash table corresponds to a specific scope of the program.

### Offline 2: Lexical Analyzer
Path: [Offline 2](./Offline%202)

In the second assignment, a **Lexical Analyzer** (also known as a tokenizer or scanner) is constructed. The Lexical Analyzer performs the following tasks:

- Scans the source program as a sequence of characters.
- Converts the character sequence into a sequence of tokens.

This is a critical step in the compilation process as it provides the parser with meaningful tokens to process.

### Offline 3: Syntax and Semantic Analysis
Path: [Offline 3](./Offline%203)

In this assignment, the **Syntax Analyzer** (parser) and **Semantic Analyzer** are implemented to complete the front end of the compiler. The focus areas include:

- **Grammar Rules**: Parsing a subset of the C programming language, specifically function implementations.
- **Lex and Yacc**: Using Lex (Flex) for token generation and Yacc (Bison) for parsing.
- **Semantic Analysis**: Ensuring the source code adheres to type rules and other constraints.

### Offline 4: Intermediate Code Generation
Path: [Offline 4](./Offline%204)

In the final assignment, the compiler generates **Intermediate Code** for error-free source programs. The intermediate representation is based on the **8086 assembly language**, which serves as the target for this assignment. Tasks include:

- Translating high-level constructs to intermediate assembly code.
- Ensuring correctness based on previous syntax and semantic analysis.

---


Feel free to explore and adapt the content. For questions or suggestions, don't hesitate to reach out!

