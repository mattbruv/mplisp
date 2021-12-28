## mplisp

A C++ interpreter for a simple LISP dialect, made for educational purposes.

I wrote about what I learned with this project [here](https://mattbruv.github.io/mplisp/).

### Features

The following language features are supported:

- Data types: `Number`, `Symbol`, `Boolean`, `List`
- Lexical scope
- Arithmetic functions `+`, `-`, `*`, `/`
- Number comparison functions `>`, `<`, `>=`, `<=`, `=`
- `lambda` functions
- `define`
- `quote` or `'` to quote data
- `if` for conditional evaluation
- `car`, `cdr`, and `cons` for list manipulation
- boolean function `empty?` to check if a list is empty

#### Standard Library:

- `filter`, `map`, `max`, and `sum`.
- boolean functions `and` and `or`.

### Non-Features

- Garbage collection

  - When I designed this interpreter, I wrote it as a recursive tree-walking AST interpreter with each new expression being `new` on the heap.
    I have tried messing around with implementing garbage collection but it has proven to be a lot harder than I imagined and quite difficult to debug.

- Strings and characters
