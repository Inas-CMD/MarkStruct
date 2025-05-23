# 🧱 MarkStruct – A Lightweight Markdown to HTML Compiler in C++

**MarkStruct** is a clean, dependency-free Markdown compiler written entirely in modern C++. It converts `.md`-style text files into styled, standards-compliant HTML with support for multiple themes, tables, lists, blockquotes, footnotes, and more.

## ✨ Features

* ✅ Headings (`#` to `######`)
* ✅ Unordered and ordered lists
* ✅ Nested sublists
* ✅ Blockquotes
* ✅ Code blocks & inline code
* ✅ Footnotes (`[^1]`)
* ✅ Images & hyperlinks
* ✅ Markdown tables
* ✅ Horizontal rules (`---`, `___`, `***`)
* ✅ Text formatting (`*italic*`, `**bold**`, `***both***`)
* ✅ Optional themed HTML output

## 🛠️ Build & Run

### 🔧 Requirements

* C++11 or later
* A C++ compiler like `g++`, `clang++`, or MSVC

### 🔪 Build Example (Linux/macOS)

```bash
g++ main.cpp -o MarkStruct
```

### ▶️ Run

Place your input Markdown text in a file called `FileName.txt`, then run:

```bash
./MarkStruct
```

Output will be saved as `index.html` in the same directory.

## 🎨 Themes

You can choose from 5 built-in HTML/CSS themes:

1. Light (default)
2. Dark
3. Seaside
4. Green Coder
5. Purple Pulse

Theme selection can be modified in the code via the `ConstructSite()` call.

---

## 📊 Example

### Input (`FileName.txt`)

```markdown
# Hello World

This is a *sample* Markdown file.

- Item 1
  - Subitem
- Item 2

> This is a quote.

| Name | Age |
|------|-----|
| John | 25  |

[^1]: This is a footnote.
```

### Output (`index.html`)

```html
<h1>Hello World</h1>
<p>This is a <em>sample</em> Markdown file.</p>
<ul>
  <li>Item 1
    <ul><li>Subitem</li></ul>
  </li>
  <li>Item 2</li>
</ul>
<blockquote>This is a quote.</blockquote>
<table>
  ...
</table>
<ol>
  <li id="fn1">This is a footnote. <a href="#fnref1">↩</a></li>
</ol>
```

---

## 📁 File Structure

```
main.cpp         - Main source file
FileName.txt     - Your Markdown input (required)
index.html       - Generated output file
```

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).
Feel free to use, modify, and distribute with credit.

---

## 🙌 Author

Made by Inas
Indie C++ developer & systems enthusiast

---
