# ft_IRC
The IRC project for 42 school involves developing an IRC server in C++98. IRC, or Internet Relay Chat, is a text-based communication protocol on the Internet that offers real-time messaging. Users can engage in public or private conversations by exchanging direct messages or joining group channels.

The IRC server must be capable of handling multiple clients simultaneously without hanging. It is not allowed to use forking, so all I/O operations must be non-blocking. The server should utilize a single poll() or equivalent function to handle various operations such as reading, writing, and listening.

The project requires choosing a reference client, which will be used during the evaluation process. The reference client must be able to connect to the server without encountering any errors. Communication between the client and server should be done via TCP/IP (v4 or v6).

The implemented features should mimic the functionality of an official IRC server. This includes the ability to authenticate, set a nickname and username, join channels, send and receive private messages, and forward channel messages to all clients in the channel. The server should distinguish between operators and regular users, and implement operator-specific commands.

Writing clean code is an essential expectation for this project.