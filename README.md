# Сокеты

## Сборка и запуск

1. Необходимо перейти в корневую директорию и выполнить команду для сборки проекта `make`

2. Исполняемые файлы появятся в папке `bin`

3. Для удаления всех объектных и исполняемых файлов необходимо выполнить команду `make clean`

## Задания

### Задание 1. Написать 4 клиент-серверных приложения, где клиент отправляет строку `Hi?`, сервер её получает, изменяет на `Hi!` и возвращает клиенту.

**1. Семейство сокетов `AF_LOCAL`**

1.1 Протокол `TCP` ([Task1/AF_LOCAL/TCP](https://github.com/EltexEmbeddedC/sockets/blob/main/Task1/AF_LOCAL/TCP))

```
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t1_local_tcp_server
I'm server
socket path: /tmp/stream_serv
client sent: Hi?
client will get: Hi!
```

```
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t1_local_tcp_client
I'm client
socket path: /tmp/stream_serv
server will get: Hi?
server sent: Hi!
```
    
1.2 Протокол `UDP` ([Task1/AF_LOCAL/UDP](https://github.com/EltexEmbeddedC/sockets/blob/main/Task1/AF_LOCAL/UDP))

```
I'm server
socket server path: /tmp/stream_server
socket client path: /tmp/stream_client
client sent: Hi?
client will get: Hi!
```

```
I'm client
socket server path: /tmp/stream_server
socket client path: /tmp/stream_client
server will get: Hi?
server sent: Hi!
```
  
**2. Семейство сокетов `AF_INET`**

2.1 Протокол `TCP` ([Task1/AF_INET/TCP](https://github.com/EltexEmbeddedC/sockets/blob/main/Task1/AF_INET/TCP))

```
I'm server
Server listening on port 12345
Client connected from 127.0.0.1:54486
Received from client: Hi?
Sent to client: Hi!
```

```
I'm client
Client started on 127.0.0.1:54486
Sent to server: Hi?
Received from server: Hi!
```
  
2.2 Протокол `UDP` ([Task1/AF_INET/UDP](https://github.com/EltexEmbeddedC/sockets/blob/main/Task1/AF_INET/UDP))

```
I'm server
Server listening on port 12345
Client connected from 127.0.0.1:52289
Received from client: Hi?
Sent to client: Hi!
```

```
I'm client
Client started on 127.0.0.0:52289
Sent to server: Hi?
Received from server: Hi!
```  

### Задание 2. Реализовать 4 схемы сервера.

1. Слушающий сервер получает запрос клиента, порождает новый сервер и отправляет его endpoint клиенту для последующего взаимодействия. При завершении работы с клиентом сервер уничтожается. ([Task2/Type1](https://github.com/EltexEmbeddedC/sockets/blob/main/Task2/Type1))

Сервер:

```
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t2_type1_server 
I'm server
Server listening on port 7777
Creating new server on port 17161
New server listening on port 17161
Received from client: Hi?
Sent to client: Hello from new server! (17161)
Creating new server on port 8664
New server listening on port 8664
Received from client: Hi?
Sent to client: Hello from new server! (8664)
Creating new server on port 10555
New server listening on port 10555
Received from client: Hi?
Sent to client: Hello from new server! (10555)
```

Клиент:

```
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t2_type1_client 
Received new server port: 17161
Sent to new server: Hi?
Received from new server: Hello from new server! (17161)
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t2_type1_client 
Received new server port: 8664
Sent to new server: Hi?
Received from new server: Hello from new server! (8664)
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t2_type1_client 
Received new server port: 10555
Sent to new server: Hi?
Received from new server: Hello from new server! (10555)
```

2. Параллельный сервер с пулом: слушающий сервер при запуске порождает определенное количество серверов. Если при появлении нового клиента все серверы заняты, слушающий сервер просит клиента повторить попытку позже. Является модификацией первой схемы. ([Task2/Type2](https://github.com/EltexEmbeddedC/sockets/blob/main/Task2/Type2))

Сервер:

```
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t2_type2_server 
Created server on port 7778
Server listening on port 7778
Created server on port 7779
Server listening on port 7779
Created server on port 7780
Server listening on port 7780
Created server on port 7781
Server listening on port 7781
Created server on port 7782
Main server listening on port 7777
Server listening on port 7782
Received from client on port 7778: Hi!
Sent to client on port 7778: Hello from server on port 7778!
Received from client on port 7779: Hi!
Sent to client on port 7779: Hello from server on port 7779!
Received from client on port 7778: Hi!
Sent to client on port 7778: Hello from server on port 7778!
```

Клиент:

```
./../Task2/Type2/run_clients_parallel.sh
Received new server port: 7778
Sent to server on port 7778: Hi!
Received from server on port 7778: Hello from server on port 7778!
Received new server port: 7779
Sent to server on port 7779: Hi!
Received from server on port 7779: Hello from server on port 7779!
Received new server port: 7778
Sent to server on port 7778: Hi!
Received from server on port 7778: Hello from server on port 7778!
Все клиенты завершили работу.
```

> .sh скрипт запускает параллельно трех клиентов

3. Схема потребитель-производитель: слушающий сервер создает очередь для заявок и пулл серверов. Свободные серверы извлекают из очереди заявки, обрабатывают их и отсылают ответ клиенту. Является модицикацией второй схемы. ([Task2/Type3](https://github.com/EltexEmbeddedC/sockets/blob/main/Task2/Type3))

Сервер:

```

```

Клиент:

```

```

4. Мультипротокольный сервер, построенный на мультиплексировании: один поток одновременно может принимать TCP и UDP запросы. Реализовать на select, poll, epoll. ([Task2/Type4](https://github.com/EltexEmbeddedC/sockets/blob/main/Task2/Type4))

Сервер:

```

```

Клиент:

```

```

## Задание 3. Реализовать сервер, посылающий multicast сообщения.

Сервер:

```

```

Клиент:

```

```

## Задание 4. Реализовать сервер, посылающий broadcast сообщения.

Сервер:

```

```

Клиент:

```

```
