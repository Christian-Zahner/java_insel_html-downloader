# Java ist auch eine Insel HTML Edition Downloader
This program helps to download whole html content from book "Java ist auch eine Insel" from url "http://openbook.rheinwerk-verlag.de/javainsel/".  
It takes all html files, related image, css and js files.

# Build
The software was developed, tested and built on:
- Windows x64 machine
- [Microsoft Visual Studio 2017](https://visualstudio.microsoft.com)
- [Qt 5.11.2 msvc2017 x64 and msvc2015 x86](https://www.qt.io/download)

# Binaries
[Download x86 v1.0.0](https://1drv.ms/u/s!AujRv5FSASnagvQMwtwc5-azU5mOyw)  
[Download x64 v1.0.0](https://1drv.ms/u/s!AujRv5FSASnagvQLa0leAtm2Lbb-YA)

# Future versions of the book
It should work in future versions of this html book.  
Everytime a new book edition is being released, you can use this tool to download the files;

## config.ini file
Change general url in [Url] -> "General", if the directory of the book changes.  
Change version of output folder in [Dir] -> "Output".  
Add new subfolders in [Dir] -> "Key", use a comma to separate.

# Todos (already considered)
- Refactor
- Make it faster
- Use until now unused 404 file comparison to save time
