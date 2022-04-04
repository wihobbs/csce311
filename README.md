# csce311

## multiproc/multiproc.cc
- Thought it'd be nice to run lecture 4 pg 25 on my machine, so I typed it out and put it here.

## ipc/socket_cli_srv.cc
- File can be made with **make socket-cli-srv** from the containing directory
- Run server in terminal with **./socket-cli-srv server** and it will block waiting for client connect
- Run client in separate terminal with **./socket-cli-srv client** and it will block reading STDIN.
  - Text entered into the client's terminal will be transmitted to the server and printed there along with data transmission info
