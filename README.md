# Redirecting stdout to a Pipe
*How to read an arbitrarily long message from a pipe*

### Running the demo
Just `make`. 

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
