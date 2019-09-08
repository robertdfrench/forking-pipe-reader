# Redirecting stdout to a Pipe
*How to read an arbitrarily long message from a pipe*

This program shows how we can use `printf(3)` and `read(2)` to stream data from
a child process to its parent process. This is roughly analogous to how shell
pipes work under the hood. Since we don't know ahead of time how much data the
child will want to write, we must capture a few bytes at a time until we have
encountered an EOF.

### Running the demo
Just `make`. This will build and run the demo. The output shows how the parent
filled each buffer with data from the child, so we can get a better
understanding of the transfer process. As an example, the child attempts to
print this quote from Doug McIlroy:

> Write programs that do one thing and do it well.
> Write programs that work together.
> Write programs that manipulate text streams, for that is a universal interface.

The parent captures this wisdom 16 bytes at a time, displaying a table to help
us understand how the data is captured:

```console
   [0123456789ABCDEF]
 1 [Write programs t]
 2 [hat do one thing]
 3 [ and do it well.]
 4 [Write programs t]
 5 [hat work togethe]
 6 [r.Write programs]
 7 [ that manipulate]
 8 [ text streams, f]
 9 [or that is a uni]
10 [versal interface]
11 [.]
12 []
```

The empty string on line 12 indicates that `read(2)` encountered an EOF, and as
such we decided to quit reading.

### Creating a pipe between two processes
```console
                                                                             
   ┌──────────────────────────┐                                              
   │                          │                                              
   │                          │                                              
   │                          │                                              
   │      Parent Process      │                                              
   │                          │                                              
   │                          │                                              
   │                          │                                              
   └──────────────────────────┘                                              
                 │                                                           
              pipe()                                                         
                 │                                                           
                 ▼                                                           
   ┌──────────────────────────┐                                              
   │                          │                                              
   │                          │                                              
   │                          │                                              
   │      Parent Process      │                                              
   │                          │                                              
   │   W ─ ─ ─ ─ ─ ─ ─ ─ ─ R  │                                              
   │                          │                                              
   └──────────────────────────┘                                              
                 │                                                           
                 │                                                           
                 ├───────────────fork()────────────────────────┐             
                 │                                             │             
                 ▼                                             ▼             
   ┌──────────────────────────┐                  ┌──────────────────────────┐
   │                          │                  │                          │
   │                          │                  │                          │
   │                          │                  │                          │
   │      Parent Process      │                  │      Child Process       │
   │                          │                  │                          │
   │   W ─ ─ ─ ─ ─ ─ ─ ─ ─ R─ ┼ ─ ─ ─ ─ ─ ─ ─ ─ ─│─ ─W ─ ─ ─ ─ ─ ─ ─ ─ ─ R  │
   │   │                      │                  │                       │  │
   └──────────────────────────┘                  └──────────────────────────┘
       │         │                                             │         │   
                 │                                             │             
       └ ─ ─ ─ ─ ┼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┼ ─ ─ ─ ─ ┘   
                 │                                             │             
                 │                                             │             
             close(W)                                      close(R)          
                 │                                             │             
                 ▼                                             ▼             
   ┌──────────────────────────┐                  ┌──────────────────────────┐
   │                          │                  │                          │
   │                          │                  │                          │
   │                          │                  │                          │
   │      Parent Process      │                  │      Child Process       │
   │                          │                  │                          │
   │                        R ┼ ─ ─ ─ ─ ─ ─ ─ ─ ─│─ W                       │
   │                          │                  │                          │
   └──────────────────────────┘                  └──────────────────────────┘
```

### Credits
* [Social Media icon](https://commons.wikimedia.org/wiki/File:Rivendell_Roadeo_Fork_01.jpg) due to [rperks1](https://www.flickr.com/photos/rperks1/) under [CC BY 2.0](https://creativecommons.org/licenses/by/2.0/deed.en).
