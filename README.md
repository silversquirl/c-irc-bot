# Hot reloading in C

**THIS IS A BAD IDEA**

Don't ever use this code. It's not a good example of how to write an IRC
bot, and it's not a good example of how to write C. However, it is a cool
example of what C can do if you really want it to.

I basically wrote this to prove a point. If you want hot reloading, you
should really use an interpreted language rather than a compiled one.

## Why is this a bad idea?

First off, don't write IRC bots in C. IRC is text, and C is not a good
language for processing text. A scripting language like Python, Ruby or
Perl is a much better choice.

Secondly, `dlopen` is almost *never* a good idea. There are many things
that can break at a moment's notice because someone moved or deleted
something they shouldn't, and there's usually a better way of doing things
than dynamic loading.
