# ft_IRC
The IRC project for 42 school involves developing an IRC server in C++98. IRC, or Internet Relay Chat, is a text-based communication protocol on the Internet that offers real-time messaging. Users can engage in public or private conversations by exchanging direct messages or joining group channels.

The IRC server must be capable of handling multiple clients simultaneously without hanging. It is not allowed to use forking, so all I/O operations must be non-blocking. The server should utilize a single poll() or equivalent function to handle various operations such as reading, writing, and listening.

The project requires choosing a reference client, which will be used during the evaluation process. The reference client must be able to connect to the server without encountering any errors. Communication between the client and server should be done via TCP/IP (v4 or v6).

The implemented features should mimic the functionality of an official IRC server. This includes the ability to authenticate, set a nickname and username, join channels, send and receive private messages, and forward channel messages to all clients in the channel. The server should distinguish between operators and regular users, and implement operator-specific commands.

# A Brief History of IRC: 
## Connecting Online Communities and Empowering Collaboration

IRC, or Internet Relay Chat, has a rich history that dates back to the late 1980s. It was created by Jarkko Oikarinen, a Finnish computer scientist, in 1988. Oikarinen developed IRC as a means of facilitating real-time group communication over the Internet. The protocol quickly gained popularity and became one of the earliest forms of online chat.

IRC became famous for its role in connecting people from around the world and fostering vibrant online communities. It provided a platform for individuals to engage in discussions, share information, and collaborate on various topics. IRC's text-based nature allowed for efficient communication, making it accessible to a wide range of users.

One significant political event that contributed to IRC's fame was its use during the Gulf War in the early 1990s. As mainstream media coverage of the war was limited, people turned to IRC as a source of real-time updates and discussions. IRC channels became hubs of information exchange, enabling individuals to share news, analysis, and personal experiences from the conflict zone.

IRC's decentralized and open nature also made it attractive to political activists and dissidents. It provided a platform for organizing and coordinating grassroots movements, protests, and advocacy efforts. Activists could communicate and collaborate anonymously, making IRC a valuable tool for political dissent and free expression.

Throughout the years, IRC has continued to evolve and adapt to changing communication technologies. While its popularity has declined with the rise of social media and instant messaging platforms, IRC remains a significant part of internet history and continues to be used by various communities, including open-source software projects, tech enthusiasts, and niche interest groups.