# Сокеты

## Сборка и запуск

1. Необходимо перейти в корневую директорию и выполнить команду для сборки проекта `make`

2. Исполняемые файлы появятся в папке `bin`

3. Для удаления всех объектных и исполняемых файлов необходимо выполнить команду `make clean`

## Задания

### Задание 1. Написать 4 клиент-серверных приложения, где клиент отправляет строку `Hi?`, сервер её получает, изменяет на `Hi!` и возвращает клиенту.

**1. Семейство сокетов `AF_LOCAL`**

1.1 Протокол `TCP`

```bash
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t1_local_tcp_server
I'm server
socket path: /tmp/stream_serv
client sent: Hi?
client will get: Hi!
```

```bash
alexey@alexey-HVY-WXX9:~/Projects/Eltex/HW/sockets/bin$ ./t1_local_tcp_client
I'm client
socket path: /tmp/stream_serv
server will get: Hi?
server sent: Hi!
```
    
1.2 Протокол `UDP`

```bash
I'm server
socket server path: /tmp/stream_server
socket client path: /tmp/stream_client
client sent: Hi?
client will get: Hi!
```

```bash
I'm client
socket server path: /tmp/stream_server
socket client path: /tmp/stream_client
server will get: Hi?
server sent: Hi!
```
  
**2. Семейство сокетов `AF_INET`**

2.1 Протокол `TCP`

```bash
I'm server
Server listening on port 12345
Client connected from 127.0.0.1:54486
Received from client: Hi?
Sent to client: Hi!
```

```bash
I'm client
Client started on 127.0.0.1:54486
Sent to server: Hi?
Received from server: Hi!
```
  
2.2 Протокол `UDP`

```bash
I'm server
Server listening on port 12345
Client connected from 127.0.0.1:52289
Received from client: Hi?
Sent to client: Hi!
```

```bash
I'm client
Client started on 127.0.0.0:52289
Sent to server: Hi?
Received from server: Hi!
```  
