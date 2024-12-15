## Exercice 4

1. Code
```
martinav@lulu:~$ telnet 192.168.70.73 smtp
Trying 192.168.70.73...
Connected to 192.168.70.73.
Escape character is '^]'.
220 nivose.informatique.univ-paris-diderot.fr ESMTP Sendmail 8.15.1+Sun/8.14.1; Fri, 27 Jan 2023 18:10:02 +0100 (CET)
HELO lulu
250 nivose.informatique.univ-paris-diderot.fr Hello martinav@lulu.informatique.univ-paris-diderot.fr [192.168.70.236], pleased to meet you
MAIL FROM: martinav
553 5.5.4 martinav... Domain name required for sender address martinav
MAIL FROM: martinav@lulu
250 2.1.0 martinav@lulu... Sender ok
RCPT TO: ripasart@deja-vu
550 5.7.1 ripasart@deja-vu... Relaying denied
RCPT TO: ripasart
250 2.1.5 ripasart... Recipient ok
DATA
354 Enter mail, end with "." on a line by itself
Subject: Pizza time
From: martinav
To: ripasart

Hello Friend,
Do you love pizza ?
Greetings
martinav
.
250 2.0.0 30RHA2om001238 Message accepted for delivery
QUIT
221 2.0.0 nivose.informatique.univ-paris-diderot.fr closing connection
Connection closed by foreign host.
```

2. 
