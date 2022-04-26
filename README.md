# term-do

Hybrid of [GNOME Do][1] and Emacs [ido mode][2] for a standard
[VT100][3] terminal.

![Demo of term-do in action](img/demo.gif)

## History

While working on my dissertation--which involved a lot of
research-caliber rather than production-caliber C and C++--I went
through an earnest phase of using `vim` rather than `emacs` with the
goal of having fairly evaluated both. In the process, I began sorely
missing Emacs's [`ido mode`][2]. At the same time, I was commonly
working on Ubuntu systems where the [GNOME Do][1] application launcher
was having a bit of a moment. This all led to my interest in creating
a extensible, terminal-based `ido-mode`-like prompt.

## Objectives

`term-do` was largely duct-taped together as a means of
procrastination in between doing other research tasks. I did, however,
have a few things things in mind while working on it:

1. Show completions as each character is typed.

2. Allow "rotating" through the completions with Ctrl+S and Ctrl+R,
   `ido-mode`-style.

3. Support completions from a variety of (pluggable) sources.

4. Fast startup time: Should be near-instantaneous to launch from a
   terminal; any delay would disincentivize use in favor of plain bash
   completion.

5. Portable with a few dependencies as possible.

## Design

These loose requirements spiraled into a series of elements that went
into the (overengineered) design:

- Plugin system implemented as dynamic libraries, providing a
  mechanism to yield custom completions.

- Hand-rolled ternary search tree implementation for quickly
  identifying completions from a prefix.

- Bash history file parser for weighting commands such that common
  completions can be sorted in use-order.

- Optional client/server mode for even faster startup time on older
  hardware

- A persistent mode where the process does not exit after the
  completed command is run, essentially functioning as a lightweight
  shell

## Packaging

Though I do not get as much use out of `term-do` these days, I've
consistently packaged it for whatever OS I'm using so it's
available. Notably, there is an AUR package and a NixOS derivation
available. See [this commit][4] for an example of how to overlay the
Nix derivation.

## Usage

I found that having to type the `term-do` command before using it was
cumbersome enough to discourage using it. So instead, I've mapped it
to the Emacs-esque `M-x` shortcut. This is done in `bash` using
`.inputrc` with:

    $if Bash
    "\M-x": "term-do\C-M"
    $endif

and for `zsh` using `.zshrc` with:

    bindkey -s "^[x" 'term-do^M'

If you want `term-do`'s `cd` functionality to bubble up to the current
terminal, then you can wrap it in a shell function:

    term-do() {command term-do "$*" && builtin cd $(cat ~/.term-do.d/pwd)}

The rest of the keyboard shortcuts are likely familiar to users of Emacs:

- `C-s`: rotate completions forward

![C-s](img/rotate-forward.gif)

- `C-r`: rotate completions backward

![C-r](img/rotate-backward.gif)

- `tab`: add current completion

![tab](img/tab.gif)

- `enter`: run command
- `C-f`: remove current token and rebuild completions
- `C-c`, `C-d`, `C-g`: quit

## Future

There are better completion options these days, particularly in `zsh`,
and `term-do`'s code has bit-rotted significantly. But I still haven't
found a terminal completion that matches `ido-mode` closely enough for
my tastes. Hopefully one day I can resurrect `term-do`, possibly in
the form of a `zsh` plugin or other similar form to fill this niche.

[1]: https://en.wikipedia.org/wiki/GNOME_Do
[2]: https://www.emacswiki.org/emacs/InteractivelyDoThings
[3]: https://en.wikipedia.org/wiki/VT100
[4]: https://github.com/malloc47/config/commit/50710ddc70d893afc727f27c2d273d3b2fc049ce
